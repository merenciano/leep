#include "rendercommands.h"
#include "renderer.h" // TODO Move frame allocator to rendercommands and remove this include 
#include "internalresources.h"
#include "camera.h"

#define THE_OPENGL // Hardcoded define until another render backend is implemented.
#ifdef THE_OPENGL
#include "glad/glad.h"
#include "stb_image.h"

typedef struct {
	GLenum internal_format;
	GLenum format;
	GLenum type;
	GLenum wrap;
	GLenum min_filter;
	GLenum mag_filter;
} THE_CubeConfig;

typedef struct {
	GLenum internal_format;
	GLenum format;
	GLenum type;
	GLenum wrap;
	GLenum filter;
	s32 channels;
} THE_TextureConfig;

void THE_ClearExecute(THE_CommandData *data)
{
	u32 mask = 0;
	if (data->clear.bcolor)
		mask |= GL_COLOR_BUFFER_BIT;
	if (data->clear.bdepth)
		mask |= GL_DEPTH_BUFFER_BIT;
	if (data->clear.bstencil)
		mask |= GL_STENCIL_BUFFER_BIT;
	glClearColor(data->clear.color[0], data->clear.color[1],
		data->clear.color[2], data->clear.color[3]);
	glClear(mask);
}

void THE_CreateBufferExecute(THE_CommandData *data)
{
	THE_InternalBuffer *b = buffers + (data->createbuff.buffer);
	THE_ASSERT(b->cpu_version > 0, "This buffer hasn't got any data yet");
	THE_ASSERT(b->internal_id == (u32)THE_UNINIT, "Initialized buffer");

	glGenBuffers(1, &b->internal_id);

	if (b->type == THE_BUFFER_INDEX) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, b->internal_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, b->count * sizeof(u32),
			(const void*)b->indices, GL_STATIC_DRAW);
	} else if (b->type != THE_BUFFER_NONE) {
		glBindBuffer(GL_ARRAY_BUFFER, b->internal_id);
		glBufferData(GL_ARRAY_BUFFER, b->count * sizeof(float),
			(const void*)b->vertices, GL_STATIC_DRAW);
	}

	b->gpu_version = b->cpu_version;

	// Delete ram data now that has been copied into the vram
	// TODO: Uncomment this... Im just testing
	//THE_FreeBufferData(data->createbuff.buffer);
}

void THE_CreateCubemapExecute(THE_CommandData *data)
{
	THE_CubeConfig config;
	THE_Texture tex = data->createcubemap.texture;
	THE_InternalTexture *t = textures + tex;

	THE_ASSERT(IsValidTexture(tex), "Invalid texture");
	THE_ASSERT(t->cpu_version > t->gpu_version, "Texture not created on CPU or already created on GPU");
	THE_ASSERT(tex < 62, "Start thinking about the max textures");
	THE_ASSERT(t->internal_id == THE_UNINIT, "Texture already created in the gpu");

	switch (t->type) {
	case THE_TEX_SKYBOX:
		config.format = GL_RGB;
		config.internal_format = GL_SRGB;
		config.type = GL_UNSIGNED_BYTE;
		config.min_filter = GL_LINEAR;
		config.mag_filter = GL_LINEAR;
		config.wrap = GL_CLAMP_TO_EDGE;
		break;

	case THE_TEX_ENVIRONMENT:
		config.format = GL_RGB;
		config.internal_format = GL_RGB16F;
		config.type = GL_FLOAT;
		config.min_filter = GL_LINEAR;
		config.mag_filter = GL_LINEAR;
		config.wrap = GL_CLAMP_TO_EDGE;
		break;

	case THE_TEX_PREFILTER_ENVIRONMENT:
		config.format = GL_RGB;
		config.internal_format = GL_RGB16F;
		config.type = GL_FLOAT;
		config.min_filter = GL_LINEAR_MIPMAP_LINEAR;
		config.mag_filter = GL_LINEAR;
		config.wrap = GL_CLAMP_TO_EDGE;
		break;

	default:
		THE_SLOG_ERROR("Trying to create a cubemap with an invalid format");
		return;
	}

	glGenTextures(1, (GLuint*)&(t->internal_id));
	t->texture_unit = tex + 1;
	glActiveTexture(GL_TEXTURE0 + t->texture_unit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, t->internal_id);

	// The path for cubemaps will be the directory where the skyboxfaces are
	// and inside the directory the faces must have these names
	if (t->type == THE_TEX_SKYBOX) {
		char *faces[6] = {
			// MEGA TODO: Fix this soon
			/*
			strcat(t.path, "/right.jpg"),
			strcat(t.path, "/left.jpg"),
			strcat(t.path, "/up.jpg"),
			strcat(t.path, "/down.jpg"),
			strcat(t.path, "/front.jpg"),
			strcat(t.path, "/back.jpg"),*/
	
		};
	
		int width, height, nchannels;
		stbi_set_flip_vertically_on_load(0);
		for (u32 i = 0; i < 6; ++i) {
			u8 *img_data = stbi_load(faces[i], &width, &height, &nchannels, 0);
			THE_ASSERT(img_data, "Couldn't load the image to the cubemap");
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
				config.internal_format, width, height, 0,
				config.format, config.type, img_data);
			stbi_image_free(img_data);
		}
		t->width = width;
		t->height = height;
	} else {
		THE_ASSERT(t->width > 0 && t->height > 0,
			"The texture have to have size for the empty environment");
		for (u32 i = 0; i < 6; ++i) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
				config.internal_format, t->width,
				t->height, 0, config.format, config.type, 0);
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, config.wrap);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, config.wrap);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, config.wrap);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, config.min_filter);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, config.mag_filter);
	if (t->type == THE_TEX_PREFILTER_ENVIRONMENT) {
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	}
	t->gpu_version = t->cpu_version;
}

void THE_CreateFramebufferExecute(THE_CommandData *data)
{
	THE_Framebuffer fb = data->createfb.fb;
	THE_InternalFramebuffer *ifb = framebuffers + fb;

	THE_ASSERT(!((ifb->color && ifb->depth) && (
		(textures[ifb->color_tex].width !=
		 textures[ifb->depth_tex].width) ||
		(textures[ifb->color_tex].height !=
		 textures[ifb->depth_tex].height)
		)), "The size of the color and depth buffer has to be the same");

	if (ifb->internal_id == THE_UNINIT) {
		glGenFramebuffers(1, (GLuint*)&(ifb->internal_id));
	}

	glBindFramebuffer(GL_FRAMEBUFFER, ifb->internal_id);

	if (ifb->color) {
		if (textures[ifb->color_tex].gpu_version == 0) {
			THE_CommandData texcd;
			texcd.createtex.tex = ifb->color;
			texcd.createtex.release_ram = 0;
			THE_CreateTextureExecute(&texcd);
		}

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, textures[ifb->color_tex].internal_id, 0);
		ifb->width = textures[ifb->color_tex].width;
		ifb->height = textures[ifb->color_tex].height;
	}

	if (ifb->depth) {
		if (textures[ifb->depth_tex].gpu_version == 0) {
			THE_CommandData texcd;
			texcd.createtex.tex = ifb->depth_tex;
			texcd.createtex.release_ram = 0;
			THE_CreateTextureExecute(&texcd);
		}

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
			GL_TEXTURE_2D, textures[ifb->depth_tex].internal_id, 0);
		ifb->width = textures[ifb->depth_tex].width;
		ifb->height = textures[ifb->depth_tex].height;
	}

	ifb->gpu_version = ifb->cpu_version;
}

void THE_CreateTextureExecute(THE_CommandData *data)
{
	THE_TextureConfig config;
	THE_Texture tex = data->createtex.tex;
	THE_InternalTexture *t = textures + tex;

	THE_ASSERT(IsValidTexture(tex), "Invalid texture");
	THE_ASSERT(t->cpu_version == 1, "Texture created before?");
	THE_ASSERT(tex < 62, "Max texture units"); // Tex unit is id + 1
	THE_ASSERT(t->internal_id == THE_UNINIT, "Texture already created on GPU");

	switch(t->type) {
	case THE_TEX_R:
		config.format = GL_RED;
		config.internal_format = GL_R8;
		config.type = GL_UNSIGNED_BYTE;
		config.filter = GL_LINEAR;
		config.wrap = GL_REPEAT;
		config.channels = 1;
		break;

	case THE_TEX_LUT:
		config.format = GL_RG;
		config.internal_format = GL_RG16F;
		config.type = GL_FLOAT;
		config.filter = GL_LINEAR;
		config.wrap = GL_CLAMP_TO_EDGE;
		config.channels = 2;
		break;

	case THE_TEX_RGB: 
		config.format = GL_RGB;
		config.internal_format = GL_RGB;
		config.type = GL_UNSIGNED_BYTE;
		config.filter = GL_LINEAR;
		config.wrap = GL_REPEAT;
		config.channels = 3;
		break;

	case THE_TEX_SRGB: 
		config.format = GL_RGB;
		config.internal_format = GL_SRGB;
		config.type = GL_UNSIGNED_BYTE;
		config.filter = GL_LINEAR;
		config.wrap = GL_REPEAT;
		config.channels = 3;
		break;

	case THE_TEX_RGBA_F16:
		config.format = GL_RGBA;
		config.internal_format = GL_RGBA16F;
		config.type = GL_FLOAT;
		config.filter = GL_LINEAR;
		config.wrap = GL_CLAMP_TO_EDGE;
		config.channels = 4;
		break;

	case THE_TEX_DEPTH:
		config.format = GL_DEPTH_COMPONENT;
		config.internal_format = GL_DEPTH_COMPONENT;
		config.type = GL_FLOAT;
		config.filter = GL_LINEAR;
		config.wrap = GL_CLAMP_TO_BORDER;
		config.channels = 1;
		break;

	case THE_TEX_RGB_F16:
		config.format = GL_RGB;
		config.internal_format = GL_RGB16F;
		config.type = GL_FLOAT;
		config.filter = GL_LINEAR;
		config.wrap = GL_CLAMP_TO_EDGE;
		config.channels = 3;
		break;

	default: 
		THE_SLOG_ERROR("Invalid format");
		config.format = GL_INVALID_ENUM;
		config.internal_format = GL_INVALID_ENUM;
		config.type = GL_INVALID_ENUM;
		config.filter = GL_INVALID_ENUM;
		config.wrap = GL_INVALID_ENUM;
		config.channels = 0;
		break;
	}

	glGenTextures(1, (GLuint*)&(t->internal_id));
	t->texture_unit = tex + 1;
	glActiveTexture(GL_TEXTURE0 + t->texture_unit);
	glBindTexture(GL_TEXTURE_2D, t->internal_id);

	if (t->type == THE_TEX_RGB_F16) {
		if (!t->pix) {
			stbi_set_flip_vertically_on_load(1);
			int width, height, nchannels;
			t->pix = stbi_loadf(
				t->path, &width, &height, &nchannels, 0);
			t->width = width;
			t->height = height;
		}
		THE_ASSERT(t->pix, "The image couldn't be loaded");
		glTexImage2D(GL_TEXTURE_2D, 0, config.internal_format, t->width, t->height, 0,
			config.format, config.type, t->pix);
		if (data->createtex.release_ram) {
			stbi_image_free(t->pix);
			t->pix = NULL;
		}
	} else if (*(t->path) != '\0') {
		if (!t->pix) {
			stbi_set_flip_vertically_on_load(1);
			int width, height, nchannels;
			t->pix = stbi_load(t->path, &width, &height, &nchannels, config.channels);
			t->width = width;
			t->height = height;
		}

		THE_ASSERT(t->pix, "The image couldn't be loaded");
		glTexImage2D(GL_TEXTURE_2D, 0, config.internal_format, t->width, t->height, 0,
			config.format, config.type, t->pix);
		glGenerateMipmap(GL_TEXTURE_2D);
		if (data->createtex.release_ram) {
			stbi_image_free(t->pix);
			t->pix = NULL;
		}
	} else {
		glTexImage2D(GL_TEXTURE_2D, 0, config.internal_format, t->width, t->height, 0,
			config.format, config.type, NULL);
	}
	// No shadows outside shadow maps
	float border_color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, config.filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, config.filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, config.wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, config.wrap);
	t->gpu_version = t->cpu_version;
}

void THE_SkyboxExecute(THE_CommandData *data)
{
	THE_Material *mat = (THE_Material*)THE_AllocateFrameResource(sizeof(THE_Material));
	struct mat4 static_vp = THE_CameraStaticViewProjection(&camera);
	mat->type = THE_MT_SKYBOX;
	mat->data = (float*)THE_AllocateFrameResource(16 * sizeof(float));
	mat4_assign(mat->data, (float*)&static_vp);
	mat->dcount = 16;
	mat->tex = (THE_Texture*)THE_AllocateFrameResource(sizeof(THE_Texture));
	*(mat->tex) = data->skybox.cubemap;
	mat->tcount = 1;
	mat->cube_start = 0;

	THE_ASSERT(CUBE_MESH.vertex != THE_UNINIT,
		"You are trying to draw with an uninitialized vertex buffer");

	THE_ASSERT(CUBE_MESH.index != THE_UNINIT,
		"You are trying to draw with an uninitialized index buffer");

	THE_ASSERT(buffers[CUBE_MESH.vertex].cpu_version > 0, "Vertex buffer without data");
	THE_ASSERT(buffers[CUBE_MESH.index].cpu_version > 0, "Index buffer without data");

	// Set the uniforms
	UseMaterial(mat);

	// Create the OpenGL vertex buffer if it has not been created yet
	if (buffers[CUBE_MESH.vertex].gpu_version == 0) {
		glGenBuffers(1, &(buffers[CUBE_MESH.vertex].internal_id));
		glBindBuffer(GL_ARRAY_BUFFER, buffers[CUBE_MESH.vertex].internal_id);
		glBufferData(GL_ARRAY_BUFFER,
			buffers[CUBE_MESH.vertex].count * sizeof(float),
			(const void*)buffers[CUBE_MESH.vertex].vertices, GL_STATIC_DRAW);
		buffers[CUBE_MESH.vertex].gpu_version = buffers[CUBE_MESH.vertex].cpu_version;
	} else {
		glBindBuffer(GL_ARRAY_BUFFER, buffers[CUBE_MESH.vertex].internal_id);
	}

	// Create the OpenGL index buffer if it has not been created yet
	if (buffers[CUBE_MESH.index].gpu_version == 0) {
		glGenBuffers(1, &(buffers[CUBE_MESH.index].internal_id));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[CUBE_MESH.index].internal_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			buffers[CUBE_MESH.index].count * sizeof(uint32_t),
			(const void*)buffers[CUBE_MESH.index].indices, GL_STATIC_DRAW);
		buffers[CUBE_MESH.index].gpu_version = buffers[CUBE_MESH.index].cpu_version;
	} else {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[CUBE_MESH.index].internal_id);
	}

	GLint attrib_pos = glGetAttribLocation(materials[mat->type], "a_position");
	glVertexAttribPointer(attrib_pos, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(attrib_pos);

	u32 index_count = buffers[CUBE_MESH.index].count; // Implicit cast to unsigned

	glDepthFunc(GL_LEQUAL);
	glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, 0);
	glDepthFunc(GL_LESS);

	// TODO: Remove this
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void THE_DrawExecute(THE_CommandData *data)
{
	THE_ASSERT(data->draw.inst_count > 0, "Set inst count");
	THE_Mesh mesh = data->draw.mesh;
	THE_Material *mat = data->draw.mat;

	//int32_t vertex_handle = geo.vertex_buffer().handle();
	//int32_t index_handle = geo.index_buffer().handle();

	THE_ASSERT(mesh.vertex != THE_UNINIT,
		"You are trying to draw with an uninitialized vertex buffer");

	THE_ASSERT(mesh.index != THE_UNINIT,
		"You are trying to draw with an uninitialized index buffer");

	THE_ASSERT(buffers[mesh.vertex].cpu_version > 0, "Vertex buffer without data");
	THE_ASSERT(buffers[mesh.index].cpu_version > 0, "Index buffer without data");
	THE_ASSERT(mat->type != THE_MT_NONE, "Material type not setted");

	// Set the uniforms
	UseMaterial(mat);

	// Create the OpenGL vertex buffer if it has not been created yet
	if (buffers[mesh.vertex].gpu_version == 0) {
		THE_CommandData cbcd;
		cbcd.createbuff.buffer = mesh.vertex;
		THE_CreateBufferExecute(&cbcd);
	} else {
	    glBindBuffer(GL_ARRAY_BUFFER, buffers[mesh.vertex].internal_id);
	}

	// Create the OpenGL index buffer if it has not been created yet
	if (buffers[mesh.index].gpu_version == 0) {
		THE_CommandData cbcd;
		cbcd.createbuff.buffer = mesh.index;
		THE_CreateBufferExecute(&cbcd);
	} else {
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
	        buffers[mesh.index].internal_id);
	}

	switch(buffers[mesh.vertex].type) {
	case THE_BUFFER_VERTEX_3P_3N: {
		// POSITION
		GLint attrib_pos = glGetAttribLocation(materials[mat->type],"a_position");
		glEnableVertexAttribArray(attrib_pos);
		glVertexAttribPointer(attrib_pos, 3, GL_FLOAT, GL_FALSE,
			6 * sizeof(float), (void*)0);
		glVertexAttribDivisor(attrib_pos, 0);

		// NORMAL
		attrib_pos = glGetAttribLocation(materials[mat->type], "a_normal");
		glEnableVertexAttribArray(attrib_pos);
		glVertexAttribPointer(attrib_pos, 3, GL_FLOAT, GL_FALSE,
			6 * sizeof(float), (void*)(3 * sizeof(float)));
		glVertexAttribDivisor(attrib_pos, 0);
		break;
	}

	case THE_BUFFER_VERTEX_3P_2UV: {
		// POSITION
		GLint attrib_pos = glGetAttribLocation(materials[mat->type],"a_position");
		glEnableVertexAttribArray(attrib_pos);
		glVertexAttribPointer(attrib_pos, 3, GL_FLOAT, GL_FALSE,
			5 * sizeof(float), (void*)0);
		glVertexAttribDivisor(attrib_pos, 0);

		// UV
		attrib_pos = glGetAttribLocation(materials[mat->type], "a_uv");
		glEnableVertexAttribArray(attrib_pos);
		glVertexAttribPointer(attrib_pos, 2, GL_FLOAT, GL_FALSE,
			5 * sizeof(float), (void*)(3 * sizeof(float)));
		glVertexAttribDivisor(attrib_pos, 0);
		break;
	}
	case THE_BUFFER_VERTEX_3P_3N_2UV: {
		// POSITION
		GLint attrib_pos = glGetAttribLocation(materials[mat->type],"a_position");
		glEnableVertexAttribArray(attrib_pos);
		glVertexAttribPointer(attrib_pos, 3, GL_FLOAT, GL_FALSE,
			8 * sizeof(float), (void*)0);
		glVertexAttribDivisor(attrib_pos, 0);

		// NORMAL
		attrib_pos = glGetAttribLocation(materials[mat->type], "a_normal"); 
		glEnableVertexAttribArray(attrib_pos);
		glVertexAttribPointer(attrib_pos, 3, GL_FLOAT, GL_FALSE,
			8 * sizeof(float), (void*)(3 * sizeof(float)));
		glVertexAttribDivisor(attrib_pos, 0);

		// UV
		attrib_pos = glGetAttribLocation(materials[mat->type], "a_uv");
		glEnableVertexAttribArray(attrib_pos);
		glVertexAttribPointer(attrib_pos, 2, GL_FLOAT, GL_FALSE,
			8 * sizeof(float), (void*)(6 * sizeof(float)));
		glVertexAttribDivisor(attrib_pos, 0);
		break;
	}

	case THE_BUFFER_VERTEX_3P_3N_3T_3B_2UV: {
		// POSITION
		GLint attrib_pos = glGetAttribLocation(materials[mat->type],"a_position");
		glEnableVertexAttribArray(attrib_pos);
		glVertexAttribPointer(attrib_pos, 3, GL_FLOAT, GL_FALSE,
			14 * sizeof(float), (void*)0);
		glVertexAttribDivisor(attrib_pos, 0);

		// NORMAL
		attrib_pos = glGetAttribLocation(materials[mat->type], "a_normal"); 
		glEnableVertexAttribArray(attrib_pos);
		glVertexAttribPointer(attrib_pos, 3, GL_FLOAT, GL_FALSE,
			14 * sizeof(float), (void*)(3 * sizeof(float)));
		glVertexAttribDivisor(attrib_pos, 0);

		// TANGENT
		attrib_pos = glGetAttribLocation(materials[mat->type],"a_tangent"); 
		glEnableVertexAttribArray(attrib_pos);
		glVertexAttribPointer(attrib_pos, 3, GL_FLOAT, GL_FALSE,
			14 * sizeof(float), (void*)(6 * sizeof(float)));
		glVertexAttribDivisor(attrib_pos, 0);
	
		// BITANGENT
		attrib_pos = glGetAttribLocation(materials[mat->type],"a_bitangent"); 
		glEnableVertexAttribArray(attrib_pos);
		glVertexAttribPointer(attrib_pos, 3, GL_FLOAT, GL_FALSE,
			14 * sizeof(float), (void*)(9 * sizeof(float)));
		glVertexAttribDivisor(attrib_pos, 0);

		// UV
		attrib_pos = glGetAttribLocation(materials[mat->type], "a_uv");
		glEnableVertexAttribArray(attrib_pos);
		glVertexAttribPointer(attrib_pos, 2, GL_FLOAT, GL_FALSE,
			14 * sizeof(float), (void*)(12 * sizeof(float)));
		glVertexAttribDivisor(attrib_pos, 0);
		break;
	}
	default:
		THE_SLOG_ERROR("Wrong vertex buffer for draw command.");
		break;
	}

	if (data->draw.inst_count > 1U) {
		THE_Buffer attr = data->draw.inst_attr;
		THE_ASSERT(attr != THE_UNINIT, "Instance count must be greater than one.");
		THE_ASSERT(buffers[attr].type == THE_BUFFER_VERTEX_3P,
			"The instance attributes buffer has the wrong type.");

		if (buffers[attr].gpu_version == 0) {
			THE_CommandData cbcd;
			cbcd.createbuff.buffer = attr;
			THE_CreateBufferExecute(&cbcd);
		} else {
			glBindBuffer(GL_ARRAY_BUFFER, buffers[attr].internal_id);
		}

		GLint attrib_pos = glGetAttribLocation(materials[mat->type],"a_offset");
		glEnableVertexAttribArray(attrib_pos);
		glVertexAttribPointer(attrib_pos, 3, GL_FLOAT, GL_FALSE,
			3 * sizeof(float), (void*)0);
		glVertexAttribDivisor(attrib_pos, 1);
	}

	u32 index_count = buffers[mesh.index].count; // Implicit cast to unsigned
	glDrawElementsInstanced(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, 0, data->draw.inst_count);
}

void THE_EquirectToCubeExecute(THE_CommandData *data)
{
	THE_Texture o_cube = data->eqr_cube.out_cube;
	THE_Texture o_pref = data->eqr_cube.out_prefilt;
	THE_Texture o_lut = data->eqr_cube.out_lut;
	const char *path = data->eqr_cube.in_path;
	THE_InternalTexture *icu = textures + o_cube;

	if (icu->cpu_version > icu->gpu_version) {
		THE_CommandData cccd;
		cccd.createcubemap.texture = o_cube;
		THE_CreateCubemapExecute(&cccd);
	}

	struct mat4 proj = smat4_perspective(to_radians(90.0f), 1.0f, 0.1f, 10.0f);
	struct mat4 views[] = {
		smat4_look_at(svec3(0.0f, 0.0f, 0.0f), svec3(1.0f, 0.0f, 0.0f), svec3(0.0f, -1.0f, 0.0f)),
		smat4_look_at(svec3(0.0f, 0.0f, 0.0f), svec3(-1.0f, 0.0f, 0.0f), svec3(0.0f, -1.0f, 0.0f)),
		smat4_look_at(svec3(0.0f, 0.0f, 0.0f), svec3(0.0f, 1.0f, 0.0f), svec3(0.0f, 0.0f, 1.0f)),
		smat4_look_at(svec3(0.0f, 0.0f, 0.0f), svec3(0.0f, -1.0f, 0.0f), svec3(0.0f, 0.0f, -1.0f)),
		smat4_look_at(svec3(0.0f, 0.0f, 0.0f), svec3(0.0f, 0.0f, 1.0f), svec3(0.0f, -1.0f, 0.0f)),
		smat4_look_at(svec3(0.0f, 0.0f, 0.0f), svec3(0.0f, 0.0f, -1.0f), svec3(0.0f, -1.0f, 0.0f)),
	};

	GLuint fb;
	glGenFramebuffers(1, &fb);
	glBindFramebuffer(GL_FRAMEBUFFER, fb);
	glViewport(0, 0, icu->width, icu->height);
	// Manually sync framebuffer here after changing textures
	THE_Texture equirec = THE_CreateTexture(path, THE_TEX_RGB_F16);
	THE_CommandData ctcd;
	ctcd.createtex.release_ram = 0;
	ctcd.createtex.tex = equirec;
	THE_CreateTextureExecute(&ctcd);
	THE_CommandData ro;
	ro.renderops.cull_face = THE_CULLFACE_DISABLED;
	ro.renderops.changed_mask = THE_CULL_FACE_BIT;
	THE_RenderOptionsExecute(&ro);

	THE_CommandData dcd;
	dcd.draw.inst_count = 1U;
	dcd.draw.mesh = CUBE_MESH;
	for (s32 i = 0; i < 6; ++i) {
		struct mat4 vp = smat4_multiply(proj, views[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, icu->internal_id, 0);
		glClear(GL_COLOR_BUFFER_BIT);
		dcd.draw.mat = (THE_Material*)THE_AllocateFrameResource(sizeof(THE_Material));
		THE_MaterialSetFrameTexture(dcd.draw.mat, &equirec, 1, -1);
		dcd.draw.mat->type = THE_MT_EQUIREC_TO_CUBE;
		THE_MaterialSetFrameData(dcd.draw.mat, (float*)&vp, 16);
		THE_DrawExecute(&dcd);
	}

	if (o_pref != THE_UNINIT) {
		THE_InternalTexture *ipref = textures + o_pref;
		if (ipref->cpu_version > ipref->gpu_version) {
			THE_CommandData cccd;
			cccd.createcubemap.texture = o_pref;
			THE_CreateCubemapExecute(&cccd);
		}

		THE_PrefilterEnvData pref_data;
		for (s32 i = 0; i < 5; ++i) {
			// mip size
			s32 s = (s32)((float)ipref->width * powf(0.5f, (float)i));
			glViewport(0, 0, s, s);
			pref_data.roughness = (float)i / 4.0f;  // mip / max mip levels - 1

			THE_CommandData draw_cd;
			draw_cd.draw.inst_count = 1U;
			draw_cd.draw.mesh = CUBE_MESH;
			for (s32 j = 0; j < 6; ++j) {
                pref_data.vp = smat4_multiply(proj, views[j]);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
				    GL_TEXTURE_CUBE_MAP_POSITIVE_X + j, ipref->internal_id, i);
				glClear(GL_COLOR_BUFFER_BIT);
				draw_cd.draw.mat = (THE_Material*)THE_AllocateFrameResource(sizeof(THE_Material));
				draw_cd.draw.mat->type = THE_MT_PREFILTER_ENV;
                THE_MaterialSetFrameTexture(draw_cd.draw.mat, &o_cube, 1, 0);
                THE_MaterialSetFrameData(draw_cd.draw.mat, (float*)&pref_data, sizeof(THE_PrefilterEnvData) / 4);
				THE_DrawExecute(&draw_cd);
			}
		}
	}

	if (o_lut != THE_UNINIT) {
		THE_InternalTexture *ilut = textures + o_lut;
		if (ilut->cpu_version > ilut->gpu_version) {
			THE_CommandData createtex_cd;
			createtex_cd.createtex.tex = o_lut;
			createtex_cd.createtex.release_ram = 0;
			THE_CreateTextureExecute(&createtex_cd);
		}
		glViewport(0, 0, ilut->width, ilut->height);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ilut->internal_id, 0);
		glClear(GL_COLOR_BUFFER_BIT);
		THE_CommandData draw_cd;
		draw_cd.draw.mesh = QUAD_MESH;
		draw_cd.draw.inst_count = 1U;
		draw_cd.draw.mat = (THE_Material*)THE_AllocateFrameResource(sizeof(THE_Material));
		draw_cd.draw.mat->type = THE_MT_LUT_GEN;
		THE_DrawExecute(&draw_cd);
	}

	glDeleteFramebuffers(1, &fb);
	THE_ReleaseTexture(equirec);
}

void THE_RenderOptionsExecute(THE_CommandData *data)
{
	// Blend options
	if (data->renderops.changed_mask & THE_BLEND_FUNC_BIT) {
		GLuint sfac, dfac;
		switch(data->renderops.sfactor) {
		case THE_BLENDFUNC_ONE:
			sfac = GL_ONE;
			break;

		case THE_BLENDFUNC_SRC_ALPHA:
			sfac = GL_SRC_ALPHA;
			break;

		case THE_BLENDFUNC_ONE_MINUS_SRC_ALPHA:
			sfac = GL_ONE_MINUS_SRC_ALPHA;
			break;

		case THE_BLENDFUNC_ZERO:
			sfac = GL_ZERO;
			break;

		default:
			THE_ASSERT(false, "RenderOption invalid BlendCommand S value");
			return;
		}

		switch(data->renderops.dfactor) {
		case THE_BLENDFUNC_ONE:
			dfac = GL_ONE;
			break;

		case THE_BLENDFUNC_SRC_ALPHA:
			dfac = GL_SRC_ALPHA;
			break;

		case THE_BLENDFUNC_ONE_MINUS_SRC_ALPHA:
			dfac = GL_ONE_MINUS_SRC_ALPHA;
			break;

		case THE_BLENDFUNC_ZERO:
			dfac = GL_ZERO;
			break;

		default:
			THE_ASSERT(false, "RenderOption invalid BlendCommand D value");
			return;
		}

		glBlendFunc(sfac, dfac);
	}

	if (data->renderops.changed_mask & THE_ENABLE_BLEND_BIT) {
		if (data->renderops.blend)
			glEnable(GL_BLEND);
		else
			glDisable(GL_BLEND);
	}

	// Cull options
	if (data->renderops.changed_mask & THE_CULL_FACE_BIT) {
		switch(data->renderops.cull_face) {
		case THE_CULLFACE_DISABLED:
			glDisable(GL_CULL_FACE);
			break;

		case THE_CULLFACE_FRONT:
			glEnable(GL_CULL_FACE);
			glCullFace(GL_FRONT);
			break;

		case THE_CULLFACE_BACK:
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			break;

		case THE_CULLFACE_FRONT_AND_BACK:
			glEnable(GL_CULL_FACE);
			glCullFace(GL_FRONT_AND_BACK);
			break;

		default:
			THE_ASSERT(false, "RenderOption invalid CullFace value");
			break;
		}
	}

	// Depth options
	if (data->renderops.changed_mask & THE_WRITE_DEPTH_BIT) {
		// GL_TRUE is 1 and GL_FALSE 0 so this should work...
		glDepthMask(data->renderops.write_depth);
	}
	if (data->renderops.changed_mask & THE_DEPTH_TEST_BIT) {
		if (data->renderops.depth_test)
			glEnable(GL_DEPTH_TEST);
		else
			glDisable(GL_DEPTH_TEST);
	}
}

void THE_UseFramebufferExecute(THE_CommandData *data)
{
	if (data->usefb.def) {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		return;
	}

	THE_InternalFramebuffer *ifb = framebuffers + data->usefb.fb;
	GLsizei width;
	GLsizei height;

	if (ifb->gpu_version == 0) {
		THE_ASSERT(data->usefb.fb >= 0 && ifb->cpu_version > 0, "Framebuffer not created");
		glGenFramebuffers(1, (GLuint*)&(ifb->internal_id));
		THE_CommandData ctcd;
		ctcd.createtex.release_ram = 0;
		if (ifb->color) {
			ctcd.createtex.tex = ifb->color_tex;
			THE_CreateTextureExecute(&ctcd);
		}
		if (ifb->depth) {
			ctcd.createtex.tex = ifb->depth_tex;
			THE_CreateTextureExecute(&ctcd);
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, ifb->internal_id);

	// Set viewport
	if (ifb->color) {
		width = (s32)textures[ifb->color_tex].width;
		height = (s32)textures[ifb->color_tex].height;
		glViewport(0, 0, width, height);
	}

	if (ifb->depth) {
		if (ifb->color) {
			THE_ASSERT(width == (GLsizei)textures[ifb->depth_tex].width &&
                height == (GLsizei)textures[ifb->depth_tex].height,
				"Color and depth texture sizes of framebuffer not matching");
		} else {
			width = (s32)textures[ifb->depth_tex].width;
			height = (s32)textures[ifb->depth_tex].height;
			glViewport(0, 0, width, height);
		}
	}

	// Update framebuffer if the textures have been changed
	if (ifb->gpu_version < ifb->cpu_version) {
		if (ifb->color) {
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
				textures[ifb->color_tex].internal_id, 0);
		}
		if (ifb->depth) {
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
				textures[ifb->depth_tex].internal_id, 0);
		}
		ifb->gpu_version = ifb->cpu_version;
	}
}

void THE_UseMaterialExecute(THE_CommandData *data)
{
	GLint *tu = (GLint*)malloc(data->usemat.mat->tcount * sizeof(GLint));
	for (s32 i = 0; i < data->usemat.mat->tcount; ++i) {
		tu[i] = data->usemat.mat->tex[i] + 1;
	}

	GLuint program = materials[data->usemat.mat->type];
	glUseProgram(program);
	s32 uniform_pos = glGetUniformLocation(program, "u_scene_data");
	glUniform4fv(uniform_pos, data->usemat.mat->dcount / 4, data->usemat.mat->data);
	uniform_pos = glGetUniformLocation(program, "u_scene_tex");
	glUniform1iv(uniform_pos, data->usemat.mat->cube_start, tu);
	uniform_pos = glGetUniformLocation(program, "u_scene_cube");
	glUniform1iv(uniform_pos, data->usemat.mat->tcount - data->usemat.mat->cube_start,
		tu + data->usemat.mat->cube_start);
}

#endif // THE_OPENGL