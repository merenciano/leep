#include "Crenderer.h"
#include "Crendercommands.h"
#include "core/Cdefinitions.h"
#include "Cinternalresources.h"
#include "core/memory/memory.h"
#include "core/manager.h"

THE_RenderQueue render_queue;

typedef struct THE_AvailableNode {
	THE_AvailableNode *next;
	s32 value;
} THE_AvailableNode;

static THE_AvailableNode *available_buffer;
static THE_AvailableNode *available_tex;

static THE_RenderCommand *curr_pool;
static THE_RenderCommand *curr_pool_last;
static THE_RenderCommand *next_pool;
static THE_RenderCommand *next_pool_last;

static int8_t *frame_pool[2];
static int8_t *frame_pool_last;
static int8_t frame_switch;

static THE_InternalBuffer *buffers;
static THE_InternalTexture *textures;
static u16 buffer_count;
static u16 texture_count;

static THE_Buffer AddBuffer()
{
	LEEP_CORE_ASSERT(buffer_count < THE_MAX_BUFFERS, "Max buffers");
	return buffer_count++;
}

static THE_Texture AddTexture()
{
	LEEP_CORE_ASSERT(texture_count < THE_MAX_TEXTURES, "Max textures");
	return texture_count++;
}

void THE_InitRender()
{
	// TODO Remove cast when compiling with a C compiler.
	curr_pool = (THE_RenderCommand*)malloc(THE_RENDER_QUEUE_CAPACITY * sizeof (THE_RenderCommand));
	next_pool = (THE_RenderCommand*)malloc(THE_RENDER_QUEUE_CAPACITY * sizeof (THE_RenderCommand));
	//memset(curr_pool, '\0', THE_RENDER_QUEUE_CAPACITY * sizeof(THE_RenderCommand));
	//memset(next_pool, '\0', THE_RENDER_QUEUE_CAPACITY * sizeof(THE_RenderCommand));
	curr_pool_last = curr_pool;
	next_pool_last = next_pool;

	buffers = (THE_InternalBuffer*)leep::GM.memory().persistentAlloc(sizeof(THE_InternalBuffer) * THE_MAX_BUFFERS);
	textures = (THE_InternalTexture*)leep::GM.memory().persistentAlloc(sizeof(THE_InternalTexture) * THE_MAX_TEXTURES);
	buffer_count = 0;
	texture_count = 0;

	available_buffer = NULL;
	available_tex = NULL;
	//curr = NULL;
	//curr_last = NULL;
	//next = NULL;
	//next_last = NULL;

	/* 
	2 Frame allocator (2 frame since is the lifetime of render resources)
	first frame making the render queue and second frame for the actual render
	Frame_pool[0] is the entire buffer and frame_pool[1] is a ptr to the half of it
	that way we can alternate freeing only one half each frame so it is synced with
	the render queues
	*/
	frame_pool[0] = (int8_t*)malloc(THE_FRAME_POOL_SIZE);
	frame_pool[1] = frame_pool[0] + THE_FRAME_POOL_SIZE / 2;
	frame_pool_last = frame_pool[0];
	frame_switch = 0;
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
	while (i != render_queue.curr_last)
	{
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
	LEEP_CORE_ASSERT(next_pool_last - next_pool < THE_RENDER_QUEUE_CAPACITY - 1, 
		"Not enough memory in the RenderQueue pool");
	return next_pool_last++;
}

void *THE_AllocateFrameResource(size_t size)
{
	LEEP_CORE_ASSERT((frame_pool_last + size) - frame_pool[frame_switch] < THE_FRAME_POOL_SIZE / 2, 
		"Not enough memory in the frame pool");
	void *ret = frame_pool_last;
	frame_pool_last += size;
	return ret;
}

int32_t THE_IsInsideFramePool(void *address)
{
	return address > frame_pool && address < frame_pool + THE_FRAME_POOL_SIZE;
}

size_t THE_RenderQueueUsed()
{
	return curr_pool_last - curr_pool;
}

// BUFFER FUNCTIONS
THE_Buffer THE_CreateBuffer()
{
	THE_Buffer ret;
	if (available_buffer != NULL)
	{
		THE_AvailableNode *node = available_buffer;
		available_buffer = available_buffer->next;
		ret = node->value;
		leep::GM.memory().generalFree(node);
	}
	else
	{
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

THE_Buffer THE_CreateBuffer(void *data, uint32_t count, THE_BufferType type)
{
	LEEP_CORE_ASSERT(type != THE_BUFFER_NONE, "Invalid buffer type");
	THE_Buffer ret;
	if (available_buffer != NULL) {
		THE_AvailableNode *node = available_buffer;
		available_buffer = available_buffer->next;
		ret = node->value;
		leep::GM.memory().generalFree(node);
	} else {
		ret = AddBuffer();
	}

	if (type == THE_BUFFER_INDEX) {
		buffers[ret].indices = (u32*)data;
	} else {
		buffers[ret].vertices = (float*)data;
	}
	buffers[ret].count = count;
	buffers[ret].cpu_version = 1;
	buffers[ret].gpu_version = 0;
	buffers[ret].type = type;
	buffers[ret].internal_id = THE_UNINIT;
	return ret;
}

void THE_SetBufferData(THE_Buffer buff, void *data, uint32_t count, THE_BufferType t)
{
	LEEP_CORE_ASSERT(t != THE_BUFFER_NONE, "Invalid buffer type");
	LEEP_CORE_ASSERT(data != NULL, "Invalid data (NULL)");
	LEEP_CORE_ASSERT(buffers[buff].vertices == NULL, "There is data to be freed before setting new one");
	buffers[buff].type = t;
	buffers[buff].count = count;
	if (t == THE_BUFFER_INDEX) {
		buffers[buff].indices = (u32*)data;
	} else {
		buffers[buff].vertices = (float*)data;
	}
	buffers[buff].cpu_version++;
}

void THE_ReleaseBuffer(THE_Buffer buff)
{
	// TODO System that seeks MARKED FOR DELETE resources and deletes them in GPU
	// and adds the index to avaiable resource list
	buffers[buff].cpu_version = THE_MARKED_FOR_DELETE;
	THE_FreeBufferData(buff);
	buffers[buff].type = THE_BUFFER_NONE;
}

void THE_FreeBufferData(THE_Buffer buff)
{
	leep::GM.memory().generalFree(buffers[buff].vertices);
	buffers[buff].vertices = NULL;
	buffers[buff].count = 0;
}
