#include "renderer.h"
#include "rendercommands.h"
#include "internalresources.h"
#include "camera.h"
#include "core/io.h"
#include "core/mem.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STBI_MALLOC(sz)           THE_Alloc(sz)
#define STBI_REALLOC(p,newsz)     THE_Realloc(p,newsz)
#define STBI_FREE(p)              THE_Free(p)
#endif
#include "stb_image.h"

#ifndef TINYOBJ_LOADER_C_IMPLEMENTATION
#define TINYOBJ_LOADER_C_IMPLEMENTATION
#define TINYOBJ_MALLOC THE_Alloc
#define TINYOBJ_REALLOC THE_Realloc 
#define TINYOBJ_CALLOC THE_Calloc 
#define TINYOBJ_FREE THE_Free 
#endif
#include "tinyobj_loader_c.h"

THE_Mesh SPHERE_MESH;
THE_Mesh CUBE_MESH;
THE_Mesh QUAD_MESH;

THE_RenderQueue render_queue;
THE_Camera camera;
struct vec4 sun_dir_intensity;

typedef struct THE_AvailableNode {
	struct THE_AvailableNode *next;
	int32_t value;
} THE_AvailableNode;

static THE_AvailableNode *available_buffer;
static THE_AvailableNode *available_tex;
static THE_AvailableNode *available_fb;

static THE_RenderCommand *curr_pool;
static THE_RenderCommand *curr_pool_last;
static THE_RenderCommand *next_pool;
static THE_RenderCommand *next_pool_last;

static int8_t *frame_pool[2];
static int8_t *frame_pool_last;
static int8_t frame_switch;

static THE_Buffer AddBuffer()
{
	THE_ASSERT(buffer_count < THE_MAX_BUFFERS, "Max buffers reached");
	return buffer_count++;
}

static THE_Texture AddTexture()
{
	THE_ASSERT(texture_count < THE_MAX_TEXTURES, "Max textures reached");
	return texture_count++;
}

static THE_Buffer AddFramebuffer()
{
	THE_ASSERT(framebuffer_count < THE_MAX_FRAMEBUFFERS, "Max framebuffers reached");
	return framebuffer_count++;
}

void THE_InitRender()
{
	curr_pool = THE_PersistentAlloc(THE_RENDER_QUEUE_CAPACITY * sizeof(THE_RenderCommand), 0);
	next_pool = THE_PersistentAlloc(THE_RENDER_QUEUE_CAPACITY * sizeof(THE_RenderCommand), 0);
	curr_pool_last = curr_pool;
	next_pool_last = next_pool;

	buffers = THE_PersistentAlloc(sizeof(THE_InternalBuffer) * THE_MAX_BUFFERS, 0);
	textures = THE_PersistentAlloc(sizeof(THE_InternalTexture) * THE_MAX_TEXTURES, 0);
	framebuffers = THE_PersistentAlloc(sizeof(THE_InternalFramebuffer) * THE_MAX_FRAMEBUFFERS, 0);
	buffer_count = 0;
	texture_count = 0;
	framebuffer_count = 0;

	available_buffer = NULL;
	available_tex = NULL;
	available_fb = NULL;

	// InternalMaterial initialization
	materials = THE_PersistentAlloc(sizeof(THE_InternalMaterial) * THE_MT_MAX, 0);
	materials[THE_MT_FULL_SCREEN_IMAGE] = InitInternalMaterial("fullscreen-img");
	materials[THE_MT_SKYBOX] = InitInternalMaterial("skybox");
	materials[THE_MT_EQUIREC_TO_CUBE] = InitInternalMaterial("eqr-to-cube");
	materials[THE_MT_PREFILTER_ENV] = InitInternalMaterial("prefilter-env");
	materials[THE_MT_LUT_GEN] = InitInternalMaterial("lut-gen");

	/* 
	2 Frame allocator (2 frame since is the lifetime of render resources)
	first frame making the render queue and second frame for the actual render
	Frame_pool[0] is the entire buffer and frame_pool[1] is a ptr to the half of it
	that way we can alternate freeing only one half each frame so it is synced with
	the render queues
	*/
	frame_pool[0] = THE_PersistentAlloc(THE_FRAME_POOL_SIZE, 0);
	frame_pool[1] = frame_pool[0] + THE_FRAME_POOL_SIZE / 2;
	frame_pool_last = frame_pool[0];
	frame_switch = 0;


	THE_CameraInit(&camera, 70.0f, 300.0f, THE_WindowGetWidth(), THE_WindowGetHeight(), 0);
	sun_dir_intensity = svec4(1.0f, -1.0f, 0.0f, 1.0f);

	SPHERE_MESH = THE_CreateSphereMesh(32, 32);
	CUBE_MESH = THE_CreateCubeMesh();
	QUAD_MESH = THE_CreateQuadMesh();
}

/*
 * Concatenates a list of commands to the render queue.
 */
void THE_AddCommands(THE_RenderCommand *rc)
{
	if (render_queue.next_last) {
		render_queue.next_last->next = rc;
	} else {
		render_queue.next = rc;
	}

	THE_RenderCommand *c = NULL;
	for (c = rc; c->next != NULL; c = c->next);
	render_queue.next_last = c;
}

void THE_RenderFrame()
{
	THE_RenderCommand* i = render_queue.curr;
	if (!i) {
		return;
	}

	i->execute(&(i->data));
	while (i != render_queue.curr_last) {
		i = i->next;
		i->execute(&(i->data));
	}
	render_queue.curr = NULL;
	render_queue.curr_last = NULL;
	// TODO delete resources marked for release
}

void THE_SubmitFrame()
{
	render_queue.curr = render_queue.next;
	render_queue.curr_last = render_queue.next_last;
	render_queue.next = NULL;
	render_queue.next_last = NULL;

	THE_RenderCommand *tmp = curr_pool;
	curr_pool = next_pool;
	curr_pool_last = next_pool_last;
	next_pool = tmp;
	next_pool_last = tmp; /* Free rendered commands */
	memset(next_pool, '\0', THE_RENDER_QUEUE_CAPACITY * sizeof(THE_RenderCommand));

	
	frame_switch = !frame_switch;
	frame_pool_last = frame_pool[frame_switch];
	memset(frame_pool_last, '\0', THE_FRAME_POOL_SIZE / 2);
}

THE_RenderCommand *THE_AllocateCommand()
{
	THE_ASSERT((next_pool_last - next_pool) < THE_RENDER_QUEUE_CAPACITY - 1,
		"Not enough memory in the RenderQueue pool");
	return next_pool_last++;
}

void *THE_AllocateFrameResource(size_t size)
{
	THE_ASSERT(((frame_pool_last + size) - frame_pool[frame_switch]) < THE_FRAME_POOL_SIZE / 2,
		"Not enough memory in the frame pool");
	void *ret = frame_pool_last;
	frame_pool_last += size;
	return ret;
}

int32_t THE_IsInsideFramePool(void *address)
{
	return address > (void*)*frame_pool && address < (void*)*frame_pool + THE_FRAME_POOL_SIZE;
}

size_t THE_RenderQueueUsed()
{
	return curr_pool_last - curr_pool;
}

// BUFFER FUNCTIONS
s32 IsValidBuffer(THE_Buffer buff)
{
	for (THE_AvailableNode *node = available_buffer; node != NULL; node = node->next) {
		if (node->value == buff) {
			return 0;
		}
	}
	return buff < buffer_count;
}

THE_Buffer THE_CreateBuffer()
{
	THE_Buffer ret;
	if (available_buffer != NULL) {
		THE_AvailableNode *node = available_buffer;
		available_buffer = available_buffer->next;
		ret = node->value;
		THE_Free(node);
	} else {
		ret = AddBuffer();
	}

	buffers[ret].vertices = NULL;
	buffers[ret].count = 0;
	buffers[ret].cpu_version = 0;
	buffers[ret].gpu_version = 0;
	buffers[ret].type = THE_BUFFER_NONE;
	buffers[ret].internal_id = THE_UNINIT;
	return ret;
}

void THE_SetBufferData(THE_Buffer buff, void *data, uint32_t count, THE_BufferType t)
{
	THE_ASSERT(IsValidBuffer(buff), "Invalid buffer");
	THE_ASSERT(t != THE_BUFFER_NONE , "Invalid buffer type");
	THE_ASSERT(data != NULL, "Data already points to something");
	THE_ASSERT(buffers[buff].vertices == NULL, "There is data to be freed before setting new one");
	buffers[buff].type = t;
	buffers[buff].count = count;
	if (t == THE_BUFFER_INDEX) {
		buffers[buff].indices = (u32*)data;
	} else {
		buffers[buff].vertices = (float*)data;
	}
	buffers[buff].cpu_version++;
}

THE_BufferType THE_GetBufferType(THE_Buffer buff)
{
	THE_ASSERT(IsValidBuffer(buff), "Invalid buffer");
	return buffers[buff].type;
}

void THE_ReleaseBuffer(THE_Buffer buff)
{
	THE_ASSERT(IsValidBuffer(buff), "Invalid buffer");
	// TODO System that seeks MARKED FOR DELETE resources and deletes them in GPU
	// and adds the index to avaiable resource list
	buffers[buff].cpu_version = THE_MARKED_FOR_DELETE;
	THE_FreeBufferData(buff);
	buffers[buff].type = THE_BUFFER_NONE;
}

void THE_FreeBufferData(THE_Buffer buff)
{
	THE_ASSERT(IsValidBuffer(buff), "Invalid buffer");
	THE_Free(buffers[buff].vertices);
	buffers[buff].vertices = NULL;
	buffers[buff].count = 0;
}

// TEXTURE FUNCTIONS
static THE_Texture GetAvailableTexture()
{
	THE_Texture ret;
	if (available_tex != NULL) {
		THE_AvailableNode *node = available_tex;
		available_tex = available_tex->next;
		ret = node->value;
		THE_Free(node);
	} else {
		ret = AddTexture();
	}

	return ret;
}

s32 IsValidTexture(THE_Texture tex)
{
	for (THE_AvailableNode *node = available_tex; node != NULL; node = node->next) {
		if (node->value == tex) {
			return 0;
		}
	}
	return tex < texture_count && tex >= 0;
}

THE_Texture THE_CreateTexture(const char *path, THE_TexType t)
{
	THE_ASSERT(*path != '\0', "For empty textures use THE_CreateEmptyTexture");

	THE_Texture ret = GetAvailableTexture();
        strcpy(textures[ret].path, path);
        textures[ret].pix = NULL;
	textures[ret].internal_id = THE_UNINIT;
        textures[ret].cpu_version = 1;
        textures[ret].gpu_version = 0;
	textures[ret].texture_unit = THE_UNINIT; // TODO: Make sure no runtime casts
        textures[ret].width = 0;
        textures[ret].height = 0;
        textures[ret].type = t;

	return ret;
}

THE_Texture THE_CreateEmptyTexture(u32 width, u32 height, THE_TexType t)
{
	THE_ASSERT(width != 0 && height != 0, "Incorrect dimensions");

	THE_Texture ret = GetAvailableTexture();
	*(textures[ret].path) = '\0';
	textures[ret].pix = NULL;
	textures[ret].internal_id = THE_UNINIT;
	textures[ret].cpu_version = 1;
	textures[ret].gpu_version = 0;
	textures[ret].texture_unit = THE_UNINIT;
	textures[ret].width = width;
	textures[ret].height = height;
	textures[ret].type = t;
	
	return ret;
}

THE_Texture THE_CreateEmptyTextureRelativeToScreen(float width, float height, THE_TexType t)
{
	THE_ASSERT(width > 0.0f && height > 0.0f && width <= 1.0f && height <= 1.0f,
		   "Incorrect dimensions");

	THE_Texture ret = GetAvailableTexture();
	*(textures[ret].path) = '\0';
	textures[ret].pix = NULL;
	textures[ret].internal_id = THE_UNINIT;
	textures[ret].cpu_version = 1;
	textures[ret].gpu_version = 0;
	textures[ret].texture_unit = THE_UNINIT;
	textures[ret].width = (u32)((float)THE_WindowGetWidth() * width);
	textures[ret].height = (u32)((float)THE_WindowGetHeight() * height);
	textures[ret].type = t;

	return ret;
}

void THE_LoadTexture(THE_Texture tex, const char *path)
{
	THE_ASSERT(*path != '\0', "Invalid path");
	THE_ASSERT(IsValidTexture(tex), "Invalid texture");

        int width, height, nchannels = 0;
        stbi_set_flip_vertically_on_load(1);
        /*if (textures[tex].type == THE_TEX_RGB_F16 || textures[tex].type == THE_TEX_RGBA_F16 || textures[tex].type == THE_TEX_LUT) {
		textures[tex].pix = (void*)stbi_loadf(path, &width, &height, &nchannels, 0);
		THE_ASSERT(textures[tex].pix && "The image couldn't be loaded");
        } else {
		int a = 0;
		if (textures[tex].type == THE_TEX_RGB || textures[tex].type == THE_TEX_SRGB) {
			a = 3;
			nchannels = 3;
		} else if (textures[tex].type == THE_TEX_R) {
			a = 1;
			nchannels = 1;
		}
		textures[tex].pix = (void*)stbi_load(path, &width, &height, &nchannels, a);
		THE_ASSERT(textures[tex].pix && "The image couldn't be loaded.");
        }*/

	switch (textures[tex].type)
	{
	case THE_TEX_RGB_F16:
	case THE_TEX_RGBA_F16:
	case THE_TEX_LUT:
		textures[tex].pix = (void*)stbi_loadf(path, &width, &height, &nchannels, 0);
		THE_ASSERT(textures[tex].pix, "The image couldn't be loaded");
		break;
	
	case THE_TEX_RGB:
	case THE_TEX_SRGB:
		nchannels = 3;
		textures[tex].pix = (void*)stbi_load(path, &width, &height, &nchannels, 3);
		THE_ASSERT(textures[tex].pix, "The image couldn't be loaded.");
		break;
	
	case THE_TEX_R:
		nchannels = 1;
		textures[tex].pix = (void*)stbi_load(path, &width, &height, &nchannels, 1);
		THE_ASSERT(textures[tex].pix, "The image couldn't be loaded.");
		break;

	default:
		THE_ASSERT(false, "Default case LoadTexture.");
		return;
		//textures[tex].pix = (void*)stbi_load(path, &width, &height, &nchannels, 0);
		//THE_ASSERT(textures[tex].pix && "The image couldn't be loaded.");
	}

	strcpy(textures[tex].path, path);
	textures[tex].cpu_version++;
	textures[tex].width = width;
	textures[tex].height = height;
}

void THE_ReleaseTexture(THE_Texture tex)
{
	THE_ASSERT(IsValidTexture(tex), "Invalid texture");
	// TODO System that seeks MARKED FOR DELETE resources and deletes them in GPU
	// and adds the index to avaiable resource list
	textures[tex].cpu_version = THE_MARKED_FOR_DELETE;
	THE_FreeTextureData(tex);
	textures[tex].type = THE_TEX_NONE;
}

void THE_FreeTextureData(THE_Texture tex)
{
	THE_ASSERT(IsValidTexture(tex), "Invalid texture");
	if (textures[tex].pix) {
		stbi_image_free(textures[tex].pix);
		textures[tex].pix = NULL;
	}
}

// MESH FUNCTIONS
THE_Mesh THE_GetNewMesh()
{
	THE_Mesh m;
	m.index = THE_UNINIT;
	m.vertex = THE_UNINIT;
	return m;
}

THE_Mesh THE_CreateCubeMesh()
{
	static const int32_t VTX_COUNT = 24 * 8;
	static const int32_t IDX_COUNT = 36;

	THE_Mesh ret;
	float *vert = THE_Alloc(VTX_COUNT * sizeof *vert);
	uint32_t *ind = THE_Alloc(IDX_COUNT * sizeof *ind);
	float vertices[] = { // TODO: Probar a inicializarlo asi en la memoria reservada para evitar la copia de despues
		// positions          // normals           // uv 
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  // 4
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  // 8
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f, // 12
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f, // 16
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f, // 20
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
	};

	for (s32 i = 0; i < VTX_COUNT; ++i) {
		vert[i] = vertices[i];
	}

	u32 indices[] = {
		0,  2,  1,  2,  0,  3,
		4,  5,  6,  6,  7,  4,
		8,  9, 10, 10, 11,  8,
		13, 12, 14, 12, 15, 14,
		16, 17, 18, 18, 19, 16,
		23, 22, 20, 22, 21, 20,
	};

	for (s32 i = 0; i < IDX_COUNT; ++i) {
		ind[i] = indices[i];
	}

	ret.vertex = THE_CreateBuffer();
	THE_SetBufferData(ret.vertex, vert, VTX_COUNT, THE_BUFFER_VERTEX_3P_3N_2UV);
	ret.index = THE_CreateBuffer();
	THE_SetBufferData(ret.index, ind, IDX_COUNT, THE_BUFFER_INDEX);

	return ret;
}

THE_Mesh THE_CreateSphereMesh(s32 x_segments, s32 y_segments)
{
	THE_ASSERT(x_segments > 0 && y_segments > 0, "Invalid number of segments");
	static const float PI = 3.14159265359f;

	THE_Mesh ret;
	float *vert = THE_Alloc(((1 + x_segments) * (1 + y_segments) * 8) * sizeof *vert);
	uint32_t *ind = THE_Alloc((x_segments * y_segments * 6) * sizeof *ind);
	s32 i = 0;

	for (s32 y = 0; y <= y_segments; ++y) {
		for (s32 x = 0; x <= x_segments; ++x) {
			float x_segment = (float)x / (float)x_segments;
			float y_segment = (float)y / (float)y_segments;
			float px = cosf(x_segment * (2.0f * PI)) * sinf(y_segment * PI);
			float py = cosf(y_segment * PI);
			float pz = sinf(x_segment * (2.0f * PI)) * sinf(y_segment * PI);

			float nx = px;
			float ny = py;
			float nz = pz;

			float uvx = atan2(nx, nz) / (2.0f * PI) + 0.5f;
			float uvy = ny * 0.5f + 0.5f;

			vert[i++] = px;
			vert[i++] = py;
			vert[i++] = pz;
			vert[i++] = nx;
			vert[i++] = ny;
			vert[i++] = nz;
			vert[i++] = uvx;
			vert[i++] = uvy;
		}
	}

	i = 0;
	for (s32 y = 0; y < y_segments; ++y) {
		for (s32 x = 0; x < x_segments; ++x) {
			ind[i++] = ((y + 1) * (x_segments + 1) + x);
			ind[i++] = (y       * (x_segments + 1) + x);
			ind[i++] = (y       * (x_segments + 1) + x + 1);
			ind[i++] = ((y + 1) * (x_segments + 1) + x);
			ind[i++] = (y       * (x_segments + 1) + x + 1);
			ind[i++] = ((y + 1) * (x_segments + 1) + x + 1);
		}
	}

	ret.vertex = THE_CreateBuffer();
	THE_SetBufferData(ret.vertex, vert, (1 + x_segments) * (1 + y_segments) * 8, THE_BUFFER_VERTEX_3P_3N_2UV);
	ret.index = THE_CreateBuffer();
	THE_SetBufferData(ret.index, ind, x_segments * y_segments * 6, THE_BUFFER_INDEX);

	return ret;
}

THE_Mesh THE_CreateQuadMesh()
{
	THE_Mesh ret;
	float *v = THE_Alloc(32 * sizeof *v);
	uint32_t *ind = THE_Alloc(6 * sizeof *ind);
	float vertices[] = {
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,  0.0f,
		1.0f, -1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f,  0.0f,
		1.0f,  1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f,  1.0f,
		-1.0f,  1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,  1.0f,
	};

	for (int i = 0; i < 32; ++i) {
		v[i] = vertices[i];
	}

	*(ind + 0) = 0;
	*(ind + 1) = 1;
	*(ind + 2) = 2;
	*(ind + 3) = 0;
	*(ind + 4) = 2;
	*(ind + 5) = 3;

	ret.vertex = THE_CreateBuffer();
	THE_SetBufferData(ret.vertex, v, 32, THE_BUFFER_VERTEX_3P_3N_2UV);
	ret.index = THE_CreateBuffer();
	THE_SetBufferData(ret.index, ind, 6, THE_BUFFER_INDEX);

	return ret;
}

static void FileReader(void *ctx, const char *path, int is_mtl, const char *obj_path, char **buf, size_t *size)
{
	FILE *f = fopen(path, "rb");
	if (!f)
		return;

	fseek(f, 0L, SEEK_END);
	*size = ftell(f);
	rewind(f);

	*buf = (char*)malloc(*size + 1);
	THE_ASSERT(*buf, "Allocation failed.");

	if (fread(*buf, *size, 1, f) != 1) {
		THE_ASSERT(false, "Read failed.");
	}

	fclose(f);
}

THE_Mesh THE_CreateMeshFromFile_OBJ(const char *path)
{
	THE_Mesh ret;

	tinyobj_attrib_t attrib;
	tinyobj_shape_t *shapes = NULL;
	size_t shape_count;
	tinyobj_material_t *mats = NULL;
	size_t mats_count;

	int32_t result = tinyobj_parse_obj(&attrib, &shapes, &shape_count, &mats, &mats_count,
		path, FileReader, NULL, TINYOBJ_FLAG_TRIANGULATE);

	THE_ASSERT(result == TINYOBJ_SUCCESS, "Obj loader failed.");
	if (result != TINYOBJ_SUCCESS)
	{
		THE_LOG_ERROR("Error loading obj. Err: %d", result);
		return *(THE_Mesh*)&ret; // TODO: Better way to hide warning (uninit var).
	}

	size_t tri_count = attrib.num_face_num_verts;
	size_t vertices_count = tri_count * 3 * 14;
	size_t indices_count = tri_count * 3;
	float *vertices = THE_Alloc(vertices_count * sizeof(float));
	uint32_t *indices = THE_Alloc(indices_count * sizeof(uint32_t));
	float *vit = vertices;
	size_t ii = 0;

	uint32_t index_offset = 0;
	for (size_t i = 0; i < attrib.num_face_num_verts; ++i)
	{
	    for (size_t f = 0; f < (size_t)attrib.face_num_verts[i] / 3; ++f)
	    {
	        tinyobj_vertex_index_t idx = attrib.faces[3 * f + index_offset++];
	        float v1[14], v2[14], v3[14];

	        v1[0] = attrib.vertices[3 * idx.v_idx + 0]; // Cast the 3 to int64_t in case of overflow (that would be a large obj)
	        v1[1] = attrib.vertices[3*idx.v_idx+1];
	        v1[2] = attrib.vertices[3*idx.v_idx+2];
	        v1[3] = attrib.normals[3*idx.vn_idx+0];
	        v1[4] = attrib.normals[3*idx.vn_idx+1];
	        v1[5] = attrib.normals[3*idx.vn_idx+2];
	        v1[12] = attrib.texcoords[2*idx.vt_idx+0];
	        v1[13] = attrib.texcoords[2*idx.vt_idx+1];

	        idx = attrib.faces[3 * f + index_offset++];
	        v2[0] = attrib.vertices[3*idx.v_idx+0];
	        v2[1] = attrib.vertices[3*idx.v_idx+1];
	        v2[2] = attrib.vertices[3*idx.v_idx+2];
	        v2[3] = attrib.normals[3*idx.vn_idx+0];
	        v2[4] = attrib.normals[3*idx.vn_idx+1];
	        v2[5] = attrib.normals[3*idx.vn_idx+2];
	        v2[12] = attrib.texcoords[2*idx.vt_idx+0];
	        v2[13] = attrib.texcoords[2*idx.vt_idx+1];

	        idx = attrib.faces[3 * f + index_offset++];
	        v3[0] = attrib.vertices[3*idx.v_idx+0];
	        v3[1] = attrib.vertices[3*idx.v_idx+1];
	        v3[2] = attrib.vertices[3*idx.v_idx+2];
	        v3[3] = attrib.normals[3*idx.vn_idx+0];
	        v3[4] = attrib.normals[3*idx.vn_idx+1];
	        v3[5] = attrib.normals[3*idx.vn_idx+2];
	        v3[12] = attrib.texcoords[2*idx.vt_idx+0];
	        v3[13] = attrib.texcoords[2*idx.vt_idx+1];

	        // Calculate tangent and bitangent
			struct vec3 delta_p1 = svec3_subtract(svec3(v2[0], v2[1], v2[2]), svec3(v1[0], v1[1], v1[2]));
			struct vec3 delta_p2 = svec3_subtract(svec3(v3[0], v3[1], v3[2]), svec3(v1[0], v1[1], v1[2]));
			struct vec2 delta_uv1 = svec2_subtract(svec2(v2[12], v2[13]), svec2(v1[12], v1[13]));
			struct vec2 delta_uv2 = svec2_subtract(svec2(v3[12], v3[13]), svec2(v1[12], v1[13]));
	        float r = 1.0f / (delta_uv1.x * delta_uv2.y - delta_uv1.y * delta_uv2.x);
			struct vec3 tan = svec3_multiply_f(svec3_subtract(svec3_multiply_f(delta_p1, delta_uv2.y), svec3_multiply_f(delta_p2, delta_uv1.y)), r);
			struct vec3 bitan = svec3_multiply_f(svec3_subtract(svec3_multiply_f(delta_p2, delta_uv1.x), svec3_multiply_f(delta_p1, delta_uv2.x)), r);

	        v1[6] = tan.x;
	        v1[7] = tan.y;
	        v1[8] = tan.z;
	        v2[6] = tan.x;
	        v2[7] = tan.y;
	        v2[8] = tan.z;
	        v3[6] = tan.x;
	        v3[7] = tan.y;
	        v3[8] = tan.z;

	        v1[9]  = bitan.x;
	        v1[10] = bitan.y;
	        v1[11] = bitan.z;
	        v2[9]  = bitan.x;
	        v2[10] = bitan.y;
	        v2[11] = bitan.z;
	        v3[9]  = bitan.x;
	        v3[10] = bitan.y;
	        v3[11] = bitan.z;

	        for (s32 j = 0; j < 14; ++j)
	        {
				*vit++ = v1[j];
	        }

	        for (s32 j = 0; j < 14; ++j)
	        {
				*vit++ = v2[j];
	        }

	        for (s32 j = 0; j < 14; ++j)
	        {
				*vit++ = v3[j];
	        }

			for (s32 j = 0; j < 3; ++j, ++ii)
			{
				indices[ii] = ii;
			}
	    }
	}

	ret.vertex = THE_CreateBuffer();
	THE_SetBufferData(ret.vertex, vertices, vertices_count, THE_BUFFER_VERTEX_3P_3N_3T_3B_2UV);
	ret.index = THE_CreateBuffer();
	THE_SetBufferData(ret.index, indices, indices_count, THE_BUFFER_INDEX);

	return ret;
}

void THE_ReleaseMesh(THE_Mesh mesh)
{
	THE_ReleaseBuffer(mesh.vertex);
	THE_ReleaseBuffer(mesh.index);
}

void THE_FreeMeshData(THE_Mesh mesh)
{
	THE_FreeBufferData(mesh.vertex);
	THE_FreeBufferData(mesh.index);
}

// FRAMEBUFFER
static THE_Framebuffer GetAvailableFramebuffer()
{
	THE_Framebuffer ret;
	if (available_fb != NULL) {
		THE_AvailableNode *node = available_fb;
		available_fb = available_fb->next;
		ret = node->value;
		THE_Free(node);
	} else {
		ret = AddFramebuffer();
	}

	return ret;
}

s32 IsValidFramebuffer(THE_Framebuffer fb)
{
	for (THE_AvailableNode *node = available_fb; node != NULL; node = node->next) {
		if (node->value == fb) {
			return 0;
		}
	}
	return fb < framebuffer_count && fb >= 0;
}

THE_Framebuffer THE_CreateFramebuffer(u32 width, u32 height, bool color, bool depth)
{
	THE_ASSERT(width > 0 && height > 0, "Invalid dimensions");

	THE_Framebuffer ret = GetAvailableFramebuffer();

	framebuffers[ret].color_tex = THE_CreateEmptyTexture(width, height, THE_TEX_RGBA_F16);
	framebuffers[ret].depth_tex = THE_CreateEmptyTexture(width, height, THE_TEX_DEPTH);

	framebuffers[ret].internal_id = THE_UNINIT;
        framebuffers[ret].cpu_version = 1;
        framebuffers[ret].gpu_version = 0;
        framebuffers[ret].color = color;
        framebuffers[ret].depth = depth;
        framebuffers[ret].width = width;
        framebuffers[ret].height = height;

	return ret;
}

void THE_ReleaseFramebuffer(THE_Framebuffer fb)
{
	THE_ASSERT(IsValidFramebuffer(fb), "Invalid framebuffer");
	// TODO System that seeks MARKED FOR DELETE resources and deletes them in GPU
	// and adds the index to avaiable resource list
	framebuffers[fb].cpu_version = THE_MARKED_FOR_DELETE;
	THE_ReleaseTexture(framebuffers[fb].color_tex);
	THE_ReleaseTexture(framebuffers[fb].depth_tex);
}

void THE_InitMaterial(THE_MaterialType t, const char* name)
{
	materials[t] = InitInternalMaterial(name);
}

THE_Material THE_GetNewMaterial()
{
	THE_Material mat;
	mat.data = NULL;
	mat.tex = NULL;
	mat.cube_start = -1;
	mat.dcount = 0;
	mat.tcount = 0;
	mat.type = THE_MT_NONE;
	return mat;
}

void THE_InitNewMaterial(THE_Material* mat)
{
	mat->data = NULL;
	mat->tex = NULL;
	mat->cube_start = -1;
	mat->dcount = 0;
	mat->tcount = 0;
	mat->type = THE_MT_NONE;
}

void THE_MaterialSetModel(THE_Material *mat, float *data)
{
	THE_ASSERT(data, "Invalid data parameter");
	THE_ASSERT(mat->data, "This function expects at least 64B allocated in mat->data");
	memcpy(mat->data, data, 64);
}

void THE_MaterialSetFrameData(THE_Material *mat, float* data, s32 count)
{
	THE_ASSERT(!mat->data || THE_IsInsideFramePool(mat->data),
		"There are some non-temporary data in this material that must be freed");

	// Align to fvec4
	s32 offset = count & 3;
	if (offset) {
		count += 4 - offset;
	}

	mat->data = THE_AllocateFrameResource(count * sizeof(float));
	mat->dcount = count;
	memcpy(mat->data, data, count * sizeof(float));
}

void THE_MaterialSetData(THE_Material* mat, float* data, s32 count)
{
	// Align to fvec4
	int32_t offset = count & 3;
	if (offset) {
		count += 4 - offset;
	}
	THE_Free(mat->data);
	mat->data = THE_Alloc(count * sizeof *mat->data);
	mat->dcount = count;
	memcpy(mat->data, data, count * sizeof *mat->data);
}

void THE_MaterialSetFrameTexture(THE_Material* mat, THE_Texture* tex, int32_t count, int32_t cube_start)
{
	THE_ASSERT(!mat->tex || THE_IsInsideFramePool(mat->tex),
		"There are some non-temporary textures in this material that must be freed");

	mat->tex = THE_AllocateFrameResource(count * sizeof *mat->tex);
	mat->tcount = count;
	mat->cube_start = cube_start == -1 ? count : cube_start;
	memcpy(mat->tex, tex, count * sizeof *mat->tex);
}

void THE_MaterialSetTexture(THE_Material* mat, THE_Texture* tex, int32_t count, int32_t cube_start)
{
	THE_Free(mat->tex);
	mat->tex = THE_Alloc(count * sizeof *mat->tex);
	mat->tcount = count;
	mat->cube_start = cube_start == -1 ? count : cube_start;
	memcpy(mat->tex, tex, count * sizeof *mat->tex);
}

// InternalResources Private functions
//.....................................

#include "glad/glad.h"

static THE_ErrorCode LoadFile(const char *path, char **buffer)
{
	int32_t length;
	FILE* fp = fopen(path, "r");

	if (!fp) {
		THE_LOG_ERROR("File %s couldn't be opened.", path);
		return THE_EC_FILE;
	}

	fseek(fp, 0, SEEK_END);
	length = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	*buffer = THE_Alloc(length + 1);

	if (!*buffer) {
		THE_LOG_ERROR("Allocation failed reading file %s.", path);
		fclose(fp);
		return THE_EC_ALLOC;
	}

	memset(*buffer, '\0', length + 1);
	fread(*buffer, 1, length, fp);
	// TODO: Check wtf is happening here because
	// the fucking VisualStudio is throwing exception
	// here. When fixed remember to delete the memset call
	//*buffer[length] = '\0';
	fclose(fp);

	return THE_EC_SUCCESS;
}

u32 InitInternalMaterial(const char* shader_name)
{
	char *vert, *frag;
	char vert_path[256], frag_path[256];
	memset(frag_path, '\0', 256);
	strcpy(frag_path, "../assets/shaders/");
	strcat(frag_path, shader_name);

	strcpy(vert_path, frag_path);
	strcat(vert_path, "-vert.glsl");
	strcat(frag_path, "-frag.glsl");
	LoadFile(vert_path, &vert);
	LoadFile(frag_path, &frag);

	GLint err;
	GLchar output_log[512];
	//  Create and compile vertex shader and print if compilation errors
	GLuint vert_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vert_shader, 1, (const char* const*)&vert, NULL);
	glCompileShader(vert_shader);
	glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &err);
	if (!err) {
		glGetShaderInfoLog(vert_shader, 512, NULL, output_log);
		THE_LOG_ERROR("%s vertex shader compilation failed:\n%s\n", shader_name, output_log);
	}
	//  Create and compile fragment shader and print if compilation errors
	GLuint frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag_shader, 1, (const char* const*)&frag, NULL);
	glCompileShader(frag_shader);
	glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &err);
	if (!err)
	{
		glGetShaderInfoLog(frag_shader, 512, NULL, output_log);
		THE_LOG_ERROR("%s fragment shader compilation failed:\n%s\n", shader_name, output_log);
	}
	//  Create the program with both shaders
	GLuint program = glCreateProgram();
	glAttachShader(program, vert_shader);
	glAttachShader(program, frag_shader);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &err);
	if (!err)
	{
		glGetProgramInfoLog(program, 512, NULL, output_log);
		THE_LOG_ERROR("%s program error:\n%s\n", shader_name, output_log);
	}

	THE_Free(vert);
	THE_Free(frag);

	return program;
}

void UseMaterial(THE_Material* mat)
{
	GLint u_loc;

	// Load textures
	THE_ASSERT(mat->tcount < 16, "So many textures, increase array size");
	s32 tex_units[16];
	for (s32 i = 0; i < mat->tcount; ++i)
	{
		if (mat->tex[i] != THE_UNINIT)
		{
			THE_ASSERT(mat->tex[i] != -1, "Texture not created");
			THE_ASSERT(textures[mat->tex[i]].cpu_version != -1, "Texture released");
			if (textures[mat->tex[i]].gpu_version == 0)
			{
				THE_CommandData cd;
				cd.createtex.release_ram = true;
				cd.createtex.tex = mat->tex[i];
				THE_CreateTextureExecute(&cd);
			}
		}
		tex_units[i] = textures[mat->tex[i]].texture_unit;
	}

	// TODO: Change enum values and if type < MT_ENGINE_MAX
	if (mat->type == THE_MT_FULL_SCREEN_IMAGE ||
		mat->type == THE_MT_SKYBOX ||
		mat->type == THE_MT_EQUIREC_TO_CUBE ||
		mat->type == THE_MT_PREFILTER_ENV ||
		mat->type == THE_MT_LUT_GEN)
	{
		glUseProgram(materials[mat->type]);
	}

	u_loc = glGetUniformLocation(materials[mat->type], "u_entity_data");
	if (u_loc >= 0)
	{
		glUniform4fv(u_loc, mat->dcount / 4, mat->data);
	}

	u_loc = glGetUniformLocation(materials[mat->type], "u_entity_tex");
	if (u_loc >= 0)
	{
		glUniform1iv(u_loc, mat->cube_start, tex_units);
	}

	u_loc = glGetUniformLocation(materials[mat->type], "u_entity_cube");
	if (u_loc >= 0)
	{
		glUniform1iv(u_loc, mat->tcount - mat->cube_start,
			tex_units + mat->cube_start);
	}

	if (mat->type == THE_MT_FULL_SCREEN_IMAGE)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}
