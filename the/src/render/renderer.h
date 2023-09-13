#ifndef THE_RENDER_RENDERER_H
#define THE_RENDER_RENDERER_H

#include "core/definitions.h"
#include "renderertypes.h"
#include "material.h"

#define THE_RENDER_QUEUE_CAPACITY 12000
#define THE_FRAME_POOL_SIZE 1048576
#define THE_MAX_TEXTURES 63
#define THE_MAX_BUFFERS 127
#define THE_MAX_FRAMEBUFFERS 8

typedef struct THE_RenderCommand THE_RenderCommand;

typedef struct {
	THE_RenderCommand *curr;
	THE_RenderCommand *curr_last;
	THE_RenderCommand *next;
	THE_RenderCommand *next_last;
} THE_RenderQueue;

extern THE_Mesh SPHERE_MESH;
extern THE_Mesh CUBE_MESH;
extern THE_Mesh QUAD_MESH;
extern THE_RenderQueue render_queue;
extern THE_Camera camera;
extern struct vec4 sun_dir_intensity;

void THE_InitRender(void);
void THE_AddCommands(THE_RenderCommand *rc);
void THE_RenderFrame(void);
void THE_SubmitFrame(void);
THE_RenderCommand *THE_AllocateCommand(void);
void *THE_AllocateFrameResource(size_t size);
int32_t THE_IsInsideFramePool(void *address);
size_t THE_RenderQueueUsed(void);

// Buffer
THE_Buffer THE_CreateBuffer(void);
void THE_SetBufferData(THE_Buffer buff, void *data, uint32_t count, THE_BufferType t);
THE_BufferType THE_GetBufferType(THE_Buffer buff);
void THE_ReleaseBuffer(THE_Buffer buff); // Marks for deletion
void THE_FreeBufferData(THE_Buffer buff); // Frees the RAM data (not the VRAM)
int32_t IsValidBuffer(THE_Buffer buff);

// Texture
THE_Texture THE_CreateTexture(const char *path, THE_TexType t);
THE_Texture THE_CreateEmptyTexture(uint32_t width, uint32_t height, THE_TexType t);
THE_Texture THE_CreateEmptyTextureRelativeToScreen(float width, float height, THE_TexType t);
void THE_LoadTexture(THE_Texture tex, const char *path);
void THE_ReleaseTexture(THE_Texture tex);
void THE_FreeTextureData(THE_Texture tex); // Frees the texture from RAM (not the VRAM)
int32_t IsValidTexture(THE_Texture tex);

// Mesh
THE_Mesh THE_GetNewMesh(void);
THE_Mesh THE_CreateCubeMesh(void);
THE_Mesh THE_CreateSphereMesh(int32_t x_segments, int32_t y_segments);
THE_Mesh THE_CreateQuadMesh(void);
THE_Mesh THE_CreateMeshFromFile_OBJ(const char *path); // TODO tinyObjLoader C verison
void THE_ReleaseMesh(THE_Mesh mesh);
void THE_FreeMeshData(THE_Mesh mesh); // Frees the data from Ram (not the VRAM)

// Framebuffer
THE_Framebuffer THE_CreateFramebuffer(uint32_t width, uint32_t height, bool color, bool depth);
void THE_ReleaseFramebuffer(THE_Framebuffer fb);

// Material
void THE_InitMaterial(THE_MaterialType t, const char* name); // name is the name of the shader files without extensions
void THE_UseMaterial(THE_Material* mat);

#endif

