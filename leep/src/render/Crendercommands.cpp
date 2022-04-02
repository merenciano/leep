#include "Crendercommands.h"
#include "Crenderer.h" // TODO Move frame allocator to rendercommands and remove this include 
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
	int32_t channels;
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
			THE_CommandData texcd;
			texcd.createtex.tex = fb.color();
			texcd.createtex.release_ram = 0;
			THE_CreateTextureExecute(&texcd);
		}
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, r.textures_[fb.color().handle()].internal_id_, 0);
		ifb.width_ = (float)r.textures_[fb.color().handle()].width_;
		ifb.height_ = (float)r.textures_[fb.color().handle()].height_;
	}
	if (ifb.depth_) {
		if (r.textures_[fb.depth().handle()].gpu_version_ == 0) {
			THE_CommandData texcd;
			texcd.createtex.tex = fb.depth();
			texcd.createtex.release_ram = 0;
			THE_CreateTextureExecute(&texcd);
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
		config.channels = 1;
		break;

	case TexType::LUT:
		config.format = GL_RG;
		config.internal_format = GL_RG16F;
		config.type = GL_FLOAT;
		config.filter = GL_LINEAR;
		config.wrap = GL_CLAMP_TO_EDGE;
		config.channels = 2;
		break;

	case TexType::RGB: 
		config.format = GL_RGB;
		config.internal_format = GL_RGB;
		config.type = GL_UNSIGNED_BYTE;
		config.filter = GL_LINEAR;
		config.wrap = GL_REPEAT;
		config.channels = 3;
		break;

	case TexType::SRGB: 
		config.format = GL_RGB;
		config.internal_format = GL_SRGB;
		config.type = GL_UNSIGNED_BYTE;
		config.filter = GL_LINEAR;
		config.wrap = GL_REPEAT;
		config.channels = 3;
		break;

	case TexType::RGBA_F16:
		config.format = GL_RGBA;
		config.internal_format = GL_RGBA16F;
		config.type = GL_FLOAT;
		config.filter = GL_LINEAR;
		config.wrap = GL_CLAMP_TO_EDGE;
		config.channels = 4;
		break;

	case TexType::DEPTH:
		config.format = GL_DEPTH_COMPONENT;
		config.internal_format = GL_DEPTH_COMPONENT;
		config.type = GL_FLOAT;
		config.filter = GL_LINEAR;
		config.wrap = GL_CLAMP_TO_BORDER;
		config.channels = 1;
		break;

	case TexType::RGB_F16:
		config.format = GL_RGB;
		config.internal_format = GL_RGB16F;
		config.type = GL_FLOAT;
		config.filter = GL_LINEAR;
		config.wrap = GL_CLAMP_TO_EDGE;
		config.channels = 3;
		break;

	default: 
		LEEP_CORE_ERROR("Invalid format");
		config.format = GL_INVALID_ENUM;
		config.internal_format = GL_INVALID_ENUM;
		config.type = GL_INVALID_ENUM;
		config.filter = GL_INVALID_ENUM;
		config.wrap = GL_INVALID_ENUM;
		config.channels = 0;
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
				itex.path_, &width, &height, &nchannels, config.channels);
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
	leep::Material *mat = (Material*)THE_AllocateFrameResource(sizeof(Material));
	mat = (new (mat) Material());
	mat->set_type(MT_SKYBOX);
	mat->set_data((float*)&view, 16);
	mat->set_tex((Texture*)&(data->skybox.cubemap), 1, 0);

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

	LEEP_CORE_ASSERT(mat->type() != MaterialType::MT_NONE,
		"Material type not setted");

	// Set the uniforms
	r.materials_[mat->type()].useMaterialData(*mat);

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
		r.materials_[mat->type()].internal_id(), "a_position");
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
	leep::Material &mat = *(data->draw.mat);

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
		THE_CommandData cbcd;
		cbcd.createbuff.buffer = geo.vertex_buffer();
		THE_CreateBufferExecute(&cbcd);
	} else {
	    glBindBuffer(GL_ARRAY_BUFFER,
	        r.buffers_[geo.vertex_buffer().handle()].internal_id_);
	}

	// Create the OpenGL index buffer if it has not been created yet
	if (r.buffers_[index_handle].gpu_version_ == 0) {
		THE_CommandData cbcd;
		cbcd.createbuff.buffer = geo.index_buffer();
		THE_CreateBufferExecute(&cbcd);
	} else {
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
	        r.buffers_[geo.index_buffer().handle()].internal_id_);
	}

	switch(geo.vertex_buffer().type()) {
	case BufferType::VERTEX_BUFFER_3P_3N: {
		// POSITION
		GLint attrib_pos = glGetAttribLocation(
			r.materials_[mat.type()].internal_id(),"a_position");
		glEnableVertexAttribArray(attrib_pos);
		glVertexAttribPointer(attrib_pos, 3, GL_FLOAT, GL_FALSE,
			6 * sizeof(float), (void*)0);
		glVertexAttribDivisor(attrib_pos, 0);

		// NORMAL
		attrib_pos = glGetAttribLocation(
			r.materials_[mat.type()].internal_id(), "a_normal");
		glEnableVertexAttribArray(attrib_pos);
		glVertexAttribPointer(attrib_pos, 3, GL_FLOAT, GL_FALSE,
			6 * sizeof(float), (void*)(3 * sizeof(float)));
		glVertexAttribDivisor(attrib_pos, 0);
		break;
	}

	case BufferType::VERTEX_BUFFER_3P_2UV: {
		// POSITION
		GLint attrib_pos = glGetAttribLocation(
			r.materials_[mat.type()].internal_id(),"a_position");
		glEnableVertexAttribArray(attrib_pos);
		glVertexAttribPointer(attrib_pos, 3, GL_FLOAT, GL_FALSE,
			5 * sizeof(float), (void*)0);
		glVertexAttribDivisor(attrib_pos, 0);

		// UV
		attrib_pos = glGetAttribLocation(
			r.materials_[mat.type()].internal_id(), "a_uv");
		glEnableVertexAttribArray(attrib_pos);
		glVertexAttribPointer(attrib_pos, 2, GL_FLOAT, GL_FALSE,
			5 * sizeof(float), (void*)(3 * sizeof(float)));
		glVertexAttribDivisor(attrib_pos, 0);
		break;
	}
	case BufferType::VERTEX_BUFFER_3P_3N_2UV: {
		// POSITION
		GLint attrib_pos = glGetAttribLocation(
			r.materials_[mat.type()].internal_id(),"a_position");
		glEnableVertexAttribArray(attrib_pos);
		glVertexAttribPointer(attrib_pos, 3, GL_FLOAT, GL_FALSE,
			8 * sizeof(float), (void*)0);
		glVertexAttribDivisor(attrib_pos, 0);

		// NORMAL
		attrib_pos = glGetAttribLocation(
			r.materials_[mat.type()].internal_id(), "a_normal"); 
		glEnableVertexAttribArray(attrib_pos);
		glVertexAttribPointer(attrib_pos, 3, GL_FLOAT, GL_FALSE,
			8 * sizeof(float), (void*)(3 * sizeof(float)));
		glVertexAttribDivisor(attrib_pos, 0);

		// UV
		attrib_pos = glGetAttribLocation(
			r.materials_[mat.type()].internal_id(), "a_uv");
		glEnableVertexAttribArray(attrib_pos);
		glVertexAttribPointer(attrib_pos, 2, GL_FLOAT, GL_FALSE,
			8 * sizeof(float), (void*)(6 * sizeof(float)));
		glVertexAttribDivisor(attrib_pos, 0);
		break;
	}

	case BufferType::VERTEX_BUFFER_3P_3N_3T_3B_2UV: {
		// POSITION
		GLint attrib_pos = glGetAttribLocation(
			r.materials_[mat.type()].internal_id(),"a_position");
		glEnableVertexAttribArray(attrib_pos);
		glVertexAttribPointer(attrib_pos, 3, GL_FLOAT, GL_FALSE,
			14 * sizeof(float), (void*)0);
		glVertexAttribDivisor(attrib_pos, 0);

		// NORMAL
		attrib_pos = glGetAttribLocation(
			r.materials_[mat.type()].internal_id(), "a_normal"); 
		glEnableVertexAttribArray(attrib_pos);
		glVertexAttribPointer(attrib_pos, 3, GL_FLOAT, GL_FALSE,
			14 * sizeof(float), (void*)(3 * sizeof(float)));
		glVertexAttribDivisor(attrib_pos, 0);

		// TANGENT
		attrib_pos = glGetAttribLocation(
			r.materials_[mat.type()].internal_id(),"a_tangent"); 
		glEnableVertexAttribArray(attrib_pos);
		glVertexAttribPointer(attrib_pos, 3, GL_FLOAT, GL_FALSE,
			14 * sizeof(float), (void*)(6 * sizeof(float)));
		glVertexAttribDivisor(attrib_pos, 0);
	
		// BITANGENT
		attrib_pos = glGetAttribLocation(
			r.materials_[mat.type()].internal_id(),"a_bitangent"); 
		glEnableVertexAttribArray(attrib_pos);
		glVertexAttribPointer(attrib_pos, 3, GL_FLOAT, GL_FALSE,
			14 * sizeof(float), (void*)(9 * sizeof(float)));
		glVertexAttribDivisor(attrib_pos, 0);

		// UV
		attrib_pos = glGetAttribLocation(
			r.materials_[mat.type()].internal_id(), "a_uv");
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
			THE_CommandData cbcd;
			cbcd.createbuff.buffer = attr;
			THE_CreateBufferExecute(&cbcd);
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

	uint32_t index_count = (uint32_t)r.buffers_[geo.index_buffer().handle()].count_;
	glDrawElementsInstanced(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, 0, data->draw.inst_count);
}

void THE_EquirectToCubeExecute(THE_CommandData *data)
{
	Renderer &r = GM.renderer();
	leep::Texture o_cube = data->eqr_cube.out_cube;
	leep::Texture o_pref = data->eqr_cube.out_prefilt;
	leep::Texture o_lut = data->eqr_cube.out_lut;
	const char *path = data->eqr_cube.in_path;
	InternalTexture &icu = r.textures_[o_cube.handle()];

	if (icu.cpu_version_ > icu.gpu_version_) {
		THE_CommandData cccd;
		cccd.createcubemap.texture = o_cube;
		THE_CreateCubemapExecute(&cccd);
	}

	glm::mat4 proj = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	glm::mat4 views[] = {
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
	};

	GLuint fb;
	glGenFramebuffers(1, &fb);
	glBindFramebuffer(GL_FRAMEBUFFER, fb);
	glViewport(0, 0, icu.width_, icu.height_);
	// Manually sync framebuffer here after changing textures
	Texture equirec;
	equirec.create(path, TexType::RGB_F16);
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
	dcd.draw.geometry = Renderer::s_cube;
	for (int32_t i = 0; i < 6; ++i) {
		glm::mat4 vp = proj * views[i];
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, icu.internal_id_, 0);
		glClear(GL_COLOR_BUFFER_BIT);
		dcd.draw.mat = (Material*)THE_AllocateFrameResource(sizeof(Material));
		dcd.draw.mat = (new (dcd.draw.mat) Material());
		dcd.draw.mat->set_tex(&equirec, 1);
		dcd.draw.mat->set_type(MT_EQUIREC_TO_CUBE);
		dcd.draw.mat->set_data((float*)&vp, 16);
		THE_DrawExecute(&dcd);
	}

	if (o_pref.handle() != CommonDefs::UNINIT_HANDLE) {
		InternalTexture &ipref = r.textures_[o_pref.handle()];
		if (ipref.cpu_version_ > ipref.gpu_version_) {
			THE_CommandData cccd;
			cccd.createcubemap.texture = o_pref;
			THE_CreateCubemapExecute(&cccd);
		}

		PrefilterEnvData pref_data;
		for (int32_t i = 0; i < 5; ++i) {
			// mip size
			uint32_t s = (uint32_t)((float)ipref.width_ * powf(0.5f, (float)i));
			glViewport(0, 0, s, s);
			pref_data.roughness_ = (float)i / 4.0f;  // mip / max mip levels - 1

			THE_CommandData dcd;
			dcd.draw.inst_count = 1U;
			dcd.draw.geometry = Renderer::s_cube;
			for (int32_t j = 0; j < 6; ++j) {
				pref_data.vp_ = proj * views[j];
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
				    GL_TEXTURE_CUBE_MAP_POSITIVE_X + j, ipref.internal_id_, i);
				glClear(GL_COLOR_BUFFER_BIT);
				dcd.draw.mat = (Material*)THE_AllocateFrameResource(sizeof(Material));
				dcd.draw.mat = (new (dcd.draw.mat) Material());
				dcd.draw.mat->set_type(MT_PREFILTER_ENV);
				dcd.draw.mat->set_tex((Texture*)&o_cube, 1, 0);
				dcd.draw.mat->set_data((float*)&pref_data, sizeof(PrefilterEnvData) / 4);
				THE_DrawExecute(&dcd);
			}
		}
	}

	if (o_lut.handle() != CommonDefs::UNINIT_HANDLE) {
		InternalTexture &ilut = r.textures_[o_lut.handle()];
		if (ilut.cpu_version_ > ilut.gpu_version_) {
			THE_CommandData ctcd;
			ctcd.createtex.tex = o_lut;
			ctcd.createtex.release_ram = 0;
			THE_CreateTextureExecute(&ctcd);
		}
		glViewport(0, 0, ilut.width_, ilut.height_);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ilut.internal_id_, 0);
		glClear(GL_COLOR_BUFFER_BIT);
		THE_CommandData dcd;
		dcd.draw.geometry = Renderer::s_quad;
		dcd.draw.inst_count = 1U;
		dcd.draw.mat = (Material*)THE_AllocateFrameResource(sizeof(Material));
		dcd.draw.mat = (new (dcd.draw.mat) Material());
		dcd.draw.mat->set_type(MT_LUT_GEN);
		THE_DrawExecute(&dcd);
	}

	glDeleteFramebuffers(1, &fb);
	equirec.release();
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
			LEEP_CORE_ASSERT(false, "RenderOption invalid BlendCommand S value");
			break;
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
			LEEP_CORE_ASSERT(false, "RenderOption invalid BlendCommand D value");
			break;
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
			LEEP_CORE_ASSERT(false, "RenderOption invalid CullFace value");
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

	Renderer &r = GM.renderer();
	InternalFramebuffer &ifb = r.framebuffers_[data->usefb.fb.handle()];
	GLsizei width;
	GLsizei height;

	if (ifb.gpu_version_ == 0) {
		LEEP_CORE_ASSERT(data->usefb.fb.handle() >= 0 && ifb.cpu_version_ > 0, "Framebuffer not created");
		glGenFramebuffers(1, (GLuint*)&(ifb.internal_id_));
		THE_CommandData ctcd;
		ctcd.createtex.release_ram = 0;
		if (ifb.color_) {
			ctcd.createtex.tex = data->usefb.fb.color();
			THE_CreateTextureExecute(&ctcd);
		}
		if (ifb.depth_) {
			ctcd.createtex.tex = data->usefb.fb.depth();
			THE_CreateTextureExecute(&ctcd);
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, ifb.internal_id_);

	// Set viewport
	if (ifb.color_) {
		width = r.textures_[data->usefb.fb.color().handle()].width_;
		height = r.textures_[data->usefb.fb.color().handle()].height_;
		glViewport(0, 0, width, height);
	}

	if (ifb.depth_) {
		if (ifb.color_) {
			LEEP_CORE_ASSERT(width == (GLsizei)r.textures_[data->usefb.fb.depth().handle()].width_ &&
		        	height == (GLsizei)r.textures_[data->usefb.fb.depth().handle()].height_,
				"Color and depth texture sizes of framebuffer not matching");
		} else {
			width = r.textures_[data->usefb.fb.depth().handle()].width_;
			height = r.textures_[data->usefb.fb.depth().handle()].height_;
			glViewport(0, 0, width, height);
		}
	}

	// Update framebuffer if the textures have been changed
	if (ifb.gpu_version_ < ifb.cpu_version_) {
		if (ifb.color_) {
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
				r.textures_[data->usefb.fb.color().handle()].internal_id_, 0); 
		}
		if (ifb.depth_) {
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
				r.textures_[data->usefb.fb.depth().handle()].internal_id_, 0);
		}
		ifb.gpu_version_ = ifb.cpu_version_;
	}
}

void THE_UseMaterialExecute(THE_CommandData *data)
{
	GLint *tu = (GLint*)malloc(data->usemat.mat.tcount_ * sizeof(GLint));
	for (int i = 0; i < data->usemat.mat.tcount_; ++i) {
		tu[i] = data->usemat.mat.tex_[i].handle() + 1;
	}
	Renderer &r = GM.renderer();
	GLuint program = r.materials_[data->usemat.mat.type_].internal_id();
	glUseProgram(program);
	uint32_t uniform_pos = glGetUniformLocation(program, "u_scene_data");
	glUniform4fv(uniform_pos, data->usemat.mat.dcount_ / 4, data->usemat.mat.data_);
	uniform_pos = glGetUniformLocation(program, "u_scene_tex");
	glUniform1iv(uniform_pos, data->usemat.mat.cube_start_, tu);
	uniform_pos = glGetUniformLocation(program, "u_scene_cube");
	glUniform1iv(uniform_pos, data->usemat.mat.tcount_ - data->usemat.mat.cube_start_,
		tu + data->usemat.mat.cube_start_);
}

#endif // THE_OPENGL