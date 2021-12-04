#include "Crendercommands.h"

#include "render/commands/create-texture.h"
#include "render/commands/create-buffer.h"
#include "render/camera.h"


#ifdef THE_OPENGL
// TODO: Cambiar esto por SDL y Opengl
//#include "SDL2/SDL.h"
//#include "SDL2/SDL_opengl.h"
//#include <GL/gl.h>
#include "glad/glad.h"
#include "stb_image.h"

struct THE_CubeConfig
{
	GLenum internal_format;
	GLenum format;
	GLenum type;
	GLenum wrap;
	GLenum min_filter;
	GLenum mag_filter;
};

struct THE_TextureConfig
{
	GLenum internal_format;
	GLenum format;
	GLenum type;
	GLenum wrap;
	GLenum filter;
};

void THE_ClearExecute(THE_CommandData *data)
{
	uint32_t mask = 0U;
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
	leep::Renderer &r = leep::Manager::instance().renderer();
	leep::Buffer buff = data->createbuff.buffer;

	LEEP_CORE_ASSERT(r.buffers_[buff.handle()].cpu_version_ > 0,
		"This buffer hasn't got any data yet");

	glGenBuffers(1, &(r.buffers_[buff.handle()].internal_id_));

	if (buff.type() == leep::BufferType::INDEX_BUFFER) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, r.buffers_[buff.handle()].internal_id_);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			r.buffers_[buff.handle()].count_ * sizeof(uint32_t),
			(const void*)r.buffers_[buff.handle()].data_.indices_,
			GL_STATIC_DRAW);
	} else if (buff.type() != leep::BufferType::NONE) {
		glBindBuffer(GL_ARRAY_BUFFER, r.buffers_[buff.handle()].internal_id_);
		glBufferData(GL_ARRAY_BUFFER,
			r.buffers_[buff.handle()].count_ * sizeof(float),
			(const void*)r.buffers_[buff.handle()].data_.vertices_,
			GL_STATIC_DRAW);
	}

	r.buffers_[buff.handle()].gpu_version_ = r.buffers_[buff.handle()].cpu_version_;

	// Delete ram data now that has been copied into the vram
	buff.freeSystemRamData();
}

void THE_CreateCubemapExecute(THE_CommandData *data)
{
	THE_CubeConfig config;
	leep::Renderer &r = leep::GM.renderer();
	int32_t id = data->createcubemap.texture.handle();

	LEEP_CORE_ASSERT(r.textures_[id].cpu_version_ > r.textures_[id].gpu_version_,
		"Texture created before (CPU)");
	LEEP_CORE_ASSERT(id < 62,
		"Start thinking about the max textures");
	LEEP_CORE_ASSERT(id >= 0, "Texture not initialized");
	LEEP_CORE_ASSERT(r.textures_[id].internal_id_ == CommonDefs::UNINIT_INTERNAL_ID,
		"Texture already created in the gpu");

	switch (r.textures_[id].type_) {
	case leep::TexType::SKYBOX:
		config.format = GL_RGB;
		config.internal_format = GL_SRGB;
		config.type = GL_UNSIGNED_BYTE;
		config.min_filter = GL_LINEAR;
		config.mag_filter = GL_LINEAR;
		config.wrap = GL_CLAMP_TO_EDGE;
		break;

	case leep::TexType::ENVIRONMENT:
		config.format = GL_RGB;
		config.internal_format = GL_RGB16F;
		config.type = GL_FLOAT;
		config.min_filter = GL_LINEAR;
		config.mag_filter = GL_LINEAR;
		config.wrap = GL_CLAMP_TO_EDGE;
		break;

	case leep::TexType::PREFILTER_ENVIRONMENT:
		config.format = GL_RGB;
		config.internal_format = GL_RGB16F;
		config.type = GL_FLOAT;
		config.min_filter = GL_LINEAR_MIPMAP_LINEAR;
		config.mag_filter = GL_LINEAR;
		config.wrap = GL_CLAMP_TO_EDGE;
		break;

	default:
		LEEP_CORE_ERROR("Trying to create a cubemap with an invalid format");
		return;
	}

	glGenTextures(1, (GLuint*)&(r.textures_[id].internal_id_));
	r.textures_[id].texture_unit_ = id + 1;
	glActiveTexture(GL_TEXTURE0 + r.textures_[id].texture_unit_);
	glBindTexture(GL_TEXTURE_CUBE_MAP, r.textures_[id].internal_id_);

	// The path for cubemaps will be the directory where the skyboxfaces are
	// and inside the directory the faces must have these names
	if (r.textures_[id].type_ == leep::TexType::SKYBOX) {
		leep::String faces[6] = {
			// MEGA TODO: Fix this soon
			/*r.textures_[id].path_ + "/right.jpg",
			r.textures_[id].path_ + "/left.jpg",
			r.textures_[id].path_ + "/up.jpg",
			r.textures_[id].path_ + "/down.jpg",
			r.textures_[id].path_ + "/front.jpg",
			r.textures_[id].path_ + "/back.jpg",*/
	
		};
	
		int width, height, nchannels;
		stbi_set_flip_vertically_on_load(0);
		for (uint32_t i = 0; i < 6; ++i) {
			uint8_t* img_data = stbi_load(
				faces[i].c_str(), &width, &height, &nchannels, 0);
			LEEP_CORE_ASSERT(img_data,"Couldn't load the image to the cubemap");
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
				config.internal_format, width, height, 0,
				config.format, config.type, img_data);
			stbi_image_free(img_data);
		}
		r.textures_[id].width_ = width;
		r.textures_[id].height_ = height;
	} else {
		LEEP_CORE_ASSERT(r.textures_[id].width_ > 0 && r.textures_[id].height_ > 0,
			"The texture have to have size for the empty environment");
		for (uint32_t i = 0; i < 6; ++i) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
				config.internal_format, r.textures_[id].width_,
				r.textures_[id].height_, 0, config.format, config.type, 0);
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, config.wrap);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, config.wrap);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, config.wrap);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, config.min_filter);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, config.mag_filter);
	if (r.textures_[id].type_ == TexType::PREFILTER_ENVIRONMENT) {
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	}
	r.textures_[id].gpu_version_ = r.textures_[id].cpu_version_;
}

void THE_CreateFramebufferExecute(THE_CommandData *data)
{
	Renderer &r = GM.renderer();
	Framebuffer fb = data->createfb.fb;
	int32_t id = fb.handle();
	InternalFramebuffer &ifb = r.framebuffers_[id];

	LEEP_CORE_ASSERT(!((ifb.color_ && ifb.depth_) && (
		(r.textures_[ifb.color_texture_.handle()].width_ != 
		 r.textures_[ifb.depth_texture_.handle()].width_) || 
		(r.textures_[ifb.color_texture_.handle()].height_ !=
		 r.textures_[ifb.depth_texture_.handle()].height_)
		)), "The size of the color and depth buffer has to be the same");

	if (ifb.internal_id_ == CommonDefs::UNINIT_INTERNAL_ID) {
		glGenFramebuffers(1, (GLuint*)&(ifb.internal_id_));
	}
	glBindFramebuffer(GL_FRAMEBUFFER, ifb.internal_id_);

	if (ifb.color_) {
		if (r.textures_[fb.color().handle()].gpu_version_ == 0) {
			CreateTexture().set_texture(fb.color()).executeCommand();
		}
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, r.textures_[fb.color().handle()].internal_id_, 0);
		ifb.width_ = (float)r.textures_[fb.color().handle()].width_;
		ifb.height_ = (float)r.textures_[fb.color().handle()].height_;
	}
	if (ifb.depth_) {
		if (r.textures_[fb.depth().handle()].gpu_version_ == 0) {
			CreateTexture().set_texture(fb.depth()).executeCommand();
		}
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
			GL_TEXTURE_2D, r.textures_[fb.depth().handle()].internal_id_, 0);
		ifb.width_ = (float)r.textures_[fb.depth().handle()].width_;
		ifb.height_ = (float)r.textures_[fb.depth().handle()].height_;
	}
	ifb.gpu_version_ = ifb.cpu_version_;
}

void THE_CreateTextureExecute(THE_CommandData *data)
{
	THE_TextureConfig config;
	int32_t id = data->createtex.tex.handle();
	InternalTexture &itex = GM.renderer().textures_[id];

	LEEP_CORE_ASSERT(itex.cpu_version_ == 1, "Texture created before?");
	LEEP_CORE_ASSERT(id < 62, "Max texture units"); // Tex unit is id + 1
	LEEP_CORE_ASSERT(id >= 0, "Texture not initialized");
	LEEP_CORE_ASSERT(itex.internal_id_ == CommonDefs::UNINIT_INTERNAL_ID,
		"Texture already created on GPU");

	switch(itex.type_) {
	case TexType::R:
		config.format = GL_RED;
		config.internal_format = GL_R8;
		config.type = GL_UNSIGNED_BYTE;
		config.filter = GL_LINEAR;
		config.wrap = GL_REPEAT;
		break;

	case TexType::LUT:
		config.format = GL_RG;
		config.internal_format = GL_RG16F;
		config.type = GL_FLOAT;
		config.filter = GL_LINEAR;
		config.wrap = GL_CLAMP_TO_EDGE;
		break;

	case TexType::RGB: 
		config.format = GL_RGB;
		config.internal_format = GL_RGB;
		config.type = GL_UNSIGNED_BYTE;
		config.filter = GL_LINEAR;
		config.wrap = GL_REPEAT;
		break;

	case TexType::SRGB: 
		config.format = GL_RGB;
		config.internal_format = GL_SRGB;
		config.type = GL_UNSIGNED_BYTE;
		config.filter = GL_LINEAR;
		config.wrap = GL_REPEAT;
		break;

	case TexType::RGBA_F16:
		config.format = GL_RGBA;
		config.internal_format = GL_RGBA16F;
		config.type = GL_FLOAT;
		config.filter = GL_LINEAR;
		config.wrap = GL_CLAMP_TO_EDGE;
		break;

	case TexType::DEPTH:
		config.format = GL_DEPTH_COMPONENT;
		config.internal_format = GL_DEPTH_COMPONENT;
		config.type = GL_FLOAT;
		config.filter = GL_LINEAR;
		config.wrap = GL_CLAMP_TO_BORDER;
		break;

	case TexType::RGB_F16:
		config.format = GL_RGB;
		config.internal_format = GL_RGB16F;
		config.type = GL_FLOAT;
		config.filter = GL_LINEAR;
		config.wrap = GL_CLAMP_TO_EDGE;
		break;

	default: 
		LEEP_CORE_ERROR("Invalid format");
		config.format = GL_INVALID_ENUM;
		config.internal_format = GL_INVALID_ENUM;
		config.type = GL_INVALID_ENUM;
		config.filter = GL_INVALID_ENUM;
		config.wrap = GL_INVALID_ENUM;
		break;
	}

	glGenTextures(1, (GLuint*)&(itex.internal_id_));
	itex.texture_unit_ = id + 1;
	glActiveTexture(GL_TEXTURE0 + itex.texture_unit_);
	glBindTexture(GL_TEXTURE_2D, itex.internal_id_);

	if (itex.type_ == TexType::RGB_F16) {
		if (!itex.pix_) {
			stbi_set_flip_vertically_on_load(1);
			int width, height, nchannels;
			itex.pix_ = stbi_loadf(
				itex.path_, &width, &height, &nchannels, 0);
			itex.width_ = width;
			itex.height_ = height;
		}
		LEEP_CORE_ASSERT(itex.pix_, "The image couldn't be loaded");
		glTexImage2D(GL_TEXTURE_2D, 0, config.internal_format, itex.width_,
			itex.height_, 0, config.format, config.type, itex.pix_);
		if (data->createtex.release_ram) {
			stbi_image_free(itex.pix_);
			itex.pix_ = nullptr;
		}
	} else if (itex.path_[0] != '\0') {
		if (!itex.pix_) {
			stbi_set_flip_vertically_on_load(1);
			int width, height, nchannels;
			itex.pix_ = stbi_load(
				itex.path_, &width, &height, &nchannels, STBI_rgb);
			itex.width_ = width;
			itex.height_ = height;
		}
		LEEP_CORE_ASSERT(itex.pix_, "The image couldn't be loaded");
		glTexImage2D(GL_TEXTURE_2D, 0, config.internal_format, itex.width_,
			itex.height_, 0, config.format, config.type, itex.pix_);
		glGenerateMipmap(GL_TEXTURE_2D);
		if (data->createtex.release_ram) {
			stbi_image_free(itex.pix_);
			itex.pix_ = nullptr;
		}
	} else {
		glTexImage2D(GL_TEXTURE_2D, 0, config.internal_format, itex.width_,
			itex.height_, 0, config.format, config.type, NULL);
	}
	// No shadows outside shadow maps
	float border_color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, config.filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, config.filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, config.wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, config.wrap);
	itex.gpu_version_ = itex.cpu_version_;
}

void THE_SkyboxExecute(THE_CommandData *data)
{
        Renderer &r = GM.renderer();
	glm::mat4 view = leep::GM.camera().static_view_projection();
	leep::Material mat;
	mat.set_type(MT_SKYBOX);
	mat.set_data((float*)&view, 16);
	mat.set_tex((Texture*)&(data->skybox.cubemap), 1, 0);

	int32_t vertex_handler = Renderer::s_cube.vertex_buffer().handle();
	int32_t index_handler = Renderer::s_cube.index_buffer().handle();

	LEEP_CORE_ASSERT(vertex_handler != CommonDefs::UNINIT_HANDLE,
		"You are trying to draw with an uninitialized vertex buffer");

	LEEP_CORE_ASSERT(index_handler != CommonDefs::UNINIT_HANDLE,
		"You are trying to draw with an uninitialized index buffer");

	LEEP_CORE_ASSERT(r.buffers_[vertex_handler].cpu_version_ > 0,
		"Vertex buffer without data");

	LEEP_CORE_ASSERT(r.buffers_[index_handler].cpu_version_ > 0, 
		"Index buffer without data");

	LEEP_CORE_ASSERT(mat.type() != MaterialType::MT_NONE,
		"Material type not setted");

	// Set the uniforms
	r.materials_[mat.type()].useMaterialData(mat);

	// Create the OpenGL vertex buffer if it has not been created yet
	if (r.buffers_[vertex_handler].gpu_version_ == 0) {
		glGenBuffers(1, &(r.buffers_[vertex_handler].internal_id_));
		glBindBuffer(GL_ARRAY_BUFFER, r.buffers_[vertex_handler].internal_id_);
		glBufferData(GL_ARRAY_BUFFER,
			r.buffers_[vertex_handler].count_ * sizeof(float),
			(const void*)r.buffers_[vertex_handler].data_.vertices_,
			GL_STATIC_DRAW);
		r.buffers_[vertex_handler].gpu_version_ = r.buffers_[vertex_handler].cpu_version_; 
	} else {
		glBindBuffer(GL_ARRAY_BUFFER,
			r.buffers_[Renderer::s_cube.vertex_buffer().handle()].internal_id_);
	}

	// Create the OpenGL index buffer if it has not been created yet
	if (r.buffers_[index_handler].gpu_version_ == 0) {
		glGenBuffers(1, &(r.buffers_[index_handler].internal_id_));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, r.buffers_[index_handler].internal_id_);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			r.buffers_[index_handler].count_ * sizeof(uint32_t),
			(const void*)r.buffers_[index_handler].data_.indices_,
			GL_STATIC_DRAW);
		r.buffers_[index_handler].gpu_version_ = r.buffers_[index_handler].cpu_version_; 
	} else {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
			r.buffers_[Renderer::s_cube.index_buffer().handle()].internal_id_);
	}

	GLint attrib_pos = glGetAttribLocation(
		r.materials_[mat.type()].internal_id(), "a_position");
	glVertexAttribPointer(attrib_pos, 3, GL_FLOAT, GL_FALSE,
		8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(attrib_pos);

	uint32_t index_count = (uint32_t)r.buffers_[Renderer::s_cube.index_buffer().handle()].count_;

	glDepthFunc(GL_LEQUAL);
	glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, 0);
	glDepthFunc(GL_LESS);

	// TODO: Remove this
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void THE_DrawExecute(THE_CommandData *data)
{
	LEEP_ASSERT(data->draw.inst_count > 0, "Set inst count");
	leep::Geometry geo = data->draw.geometry;
	leep::Material mat = data->draw.mat;

	Renderer& r = GM.renderer();
	int32_t vertex_handle = geo.vertex_buffer().handle();
	int32_t index_handle = geo.index_buffer().handle();

	LEEP_CORE_ASSERT(vertex_handle !=CommonDefs::UNINIT_HANDLE,
		"You are trying to draw with an uninitialized vertex buffer");

	LEEP_CORE_ASSERT(index_handle != CommonDefs::UNINIT_HANDLE,
		"You are trying to draw with an uninitialized index buffer");

	LEEP_CORE_ASSERT(r.buffers_[vertex_handle].cpu_version_ > 0,
		"Vertex buffer without data");

	LEEP_CORE_ASSERT(r.buffers_[index_handle].cpu_version_ > 0,
		"Index buffer without data");

	LEEP_CORE_ASSERT(mat.type() != MaterialType::MT_NONE,
		"Material type not setted");

	// Set the uniforms
	r.materials_[mat.type()].useMaterialData(mat);

	// Create the OpenGL vertex buffer if it has not been created yet
	if (r.buffers_[vertex_handle].gpu_version_ == 0) {
		CreateBuffer().set_buffer(geo.vertex_buffer()).executeCommand();
	} else {
	    glBindBuffer(GL_ARRAY_BUFFER,
	        r.buffers_[geo.vertex_buffer().handle()].internal_id_);
	}

	// Create the OpenGL index buffer if it has not been created yet
	if (r.buffers_[index_handle].gpu_version_ == 0) {
	    CreateBuffer().set_buffer(geo.index_buffer()).executeCommand();
	} else {
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
	        r.buffers_[geo.index_buffer().handle()].internal_id_);
	}

	switch(geo.vertex_buffer().type()) {
	case BufferType::VERTEX_BUFFER_3P_3N: {
		// POSITION
		GLint attrib_pos = glGetAttribLocation(
			r.materials_[material_.type()].internal_id(),"a_position");
		glEnableVertexAttribArray(attrib_pos);
		glVertexAttribPointer(attrib_pos, 3, GL_FLOAT, GL_FALSE,
			6 * sizeof(float), (void*)0);
		glVertexAttribDivisor(attrib_pos, 0);

		// NORMAL
		attrib_pos = glGetAttribLocation(
			r.materials_[material_.type()].internal_id(), "a_normal");
		glEnableVertexAttribArray(attrib_pos);
		glVertexAttribPointer(attrib_pos, 3, GL_FLOAT, GL_FALSE,
			6 * sizeof(float), (void*)(3 * sizeof(float)));
		glVertexAttribDivisor(attrib_pos, 0);
		break;
	}

	case BufferType::VERTEX_BUFFER_3P_2UV: {
		// POSITION
		GLint attrib_pos = glGetAttribLocation(
			r.materials_[material_.type()].internal_id(),"a_position");
		glEnableVertexAttribArray(attrib_pos);
		glVertexAttribPointer(attrib_pos, 3, GL_FLOAT, GL_FALSE,
			5 * sizeof(float), (void*)0);
		glVertexAttribDivisor(attrib_pos, 0);

		// UV
		attrib_pos = glGetAttribLocation(
			r.materials_[material_.type()].internal_id(), "a_uv");
		glEnableVertexAttribArray(attrib_pos);
		glVertexAttribPointer(attrib_pos, 2, GL_FLOAT, GL_FALSE,
			5 * sizeof(float), (void*)(3 * sizeof(float)));
		glVertexAttribDivisor(attrib_pos, 0);
		break;
	}
	case BufferType::VERTEX_BUFFER_3P_3N_2UV: {
		// POSITION
		GLint attrib_pos = glGetAttribLocation(
			r.materials_[material_.type()].internal_id(),"a_position");
		glEnableVertexAttribArray(attrib_pos);
		glVertexAttribPointer(attrib_pos, 3, GL_FLOAT, GL_FALSE,
			8 * sizeof(float), (void*)0);
		glVertexAttribDivisor(attrib_pos, 0);

		// NORMAL
		attrib_pos = glGetAttribLocation(
			r.materials_[material_.type()].internal_id(), "a_normal"); 
		glEnableVertexAttribArray(attrib_pos);
		glVertexAttribPointer(attrib_pos, 3, GL_FLOAT, GL_FALSE,
			8 * sizeof(float), (void*)(3 * sizeof(float)));
		glVertexAttribDivisor(attrib_pos, 0);

		// UV
		attrib_pos = glGetAttribLocation(
			r.materials_[material_.type()].internal_id(), "a_uv");
		glEnableVertexAttribArray(attrib_pos);
		glVertexAttribPointer(attrib_pos, 2, GL_FLOAT, GL_FALSE,
			8 * sizeof(float), (void*)(6 * sizeof(float)));
		glVertexAttribDivisor(attrib_pos, 0);
		break;
	}

	case BufferType::VERTEX_BUFFER_3P_3N_3T_3B_2UV: {
		// POSITION
		GLint attrib_pos = glGetAttribLocation(
			r.materials_[material_.type()].internal_id(),"a_position");
		glEnableVertexAttribArray(attrib_pos);
		glVertexAttribPointer(attrib_pos, 3, GL_FLOAT, GL_FALSE,
			14 * sizeof(float), (void*)0);
		glVertexAttribDivisor(attrib_pos, 0);

		// NORMAL
		attrib_pos = glGetAttribLocation(
			r.materials_[material_.type()].internal_id(), "a_normal"); 
		glEnableVertexAttribArray(attrib_pos);
		glVertexAttribPointer(attrib_pos, 3, GL_FLOAT, GL_FALSE,
			14 * sizeof(float), (void*)(3 * sizeof(float)));
		glVertexAttribDivisor(attrib_pos, 0);

		// TANGENT
		attrib_pos = glGetAttribLocation(
			r.materials_[material_.type()].internal_id(),"a_tangent"); 
		glEnableVertexAttribArray(attrib_pos);
		glVertexAttribPointer(attrib_pos, 3, GL_FLOAT, GL_FALSE,
			14 * sizeof(float), (void*)(6 * sizeof(float)));
		glVertexAttribDivisor(attrib_pos, 0);
	
		// BITANGENT
		attrib_pos = glGetAttribLocation(
			r.materials_[material_.type()].internal_id(),"a_bitangent"); 
		glEnableVertexAttribArray(attrib_pos);
		glVertexAttribPointer(attrib_pos, 3, GL_FLOAT, GL_FALSE,
			14 * sizeof(float), (void*)(9 * sizeof(float)));
		glVertexAttribDivisor(attrib_pos, 0);

		// UV
		attrib_pos = glGetAttribLocation(
			r.materials_[material_.type()].internal_id(), "a_uv");
		glEnableVertexAttribArray(attrib_pos);
		glVertexAttribPointer(attrib_pos, 2, GL_FLOAT, GL_FALSE,
			14 * sizeof(float), (void*)(12 * sizeof(float)));
		glVertexAttribDivisor(attrib_pos, 0);
		break;
	}
	default:
		LEEP_CORE_ERROR("Wrong vertex buffer for draw command.");
		break;
	}

	if (data->draw.inst_count > 1U) {
		leep::Buffer attr = data->draw.inst_attr;
		LEEP_CORE_ASSERT(attr.handle() != CommonDefs::UNINIT_HANDLE, "Instance count must be greater than one.");
		LEEP_CORE_ASSERT(attr.type() == BufferType::VERTEX_BUFFER_3P,
			"The instance attributes buffer has the wrong type.");

		if (r.buffers_[attr.handle()].gpu_version_ == 0) {
			CreateBuffer().set_buffer(attr).executeCommand();
		} else {
			glBindBuffer(GL_ARRAY_BUFFER,
				r.buffers_[attr.handle()].internal_id_);
		}

		GLint attrib_pos = glGetAttribLocation(
			r.materials_[mat.type()].internal_id(),"a_offset");
		glEnableVertexAttribArray(attrib_pos);
		glVertexAttribPointer(attrib_pos, 3, GL_FLOAT, GL_FALSE,
			3 * sizeof(float), (void*)0);
		glVertexAttribDivisor(attrib_pos, 1);
	}

	uint32_t index_count = (uint32_t)r.buffers_[geometry_.index_buffer().handle()].count_;
	glDrawElementsInstanced(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, 0, inst_count_);
}

#endif // THE_OPENGL