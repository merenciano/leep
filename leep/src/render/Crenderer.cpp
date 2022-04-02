#include "Crenderer.h"
#include "Crendercommands.h"

THE_RenderQueue render_queue;

static THE_RenderCommand *curr_pool;
static THE_RenderCommand *curr_pool_last;
static THE_RenderCommand *next_pool;
static THE_RenderCommand *next_pool_last;

static int8_t *frame_pool[2];
static int8_t *frame_pool_last;
static int8_t frame_switch;

void THE_InitRender()
{
	// TODO Remove cast when compiling with a C compiler.
	curr_pool = (THE_RenderCommand*)malloc(THE_RENDER_QUEUE_CAPACITY * sizeof (THE_RenderCommand));
	next_pool = (THE_RenderCommand*)malloc(THE_RENDER_QUEUE_CAPACITY * sizeof (THE_RenderCommand));
	//memset(curr_pool, '\0', THE_RENDER_QUEUE_CAPACITY * sizeof(THE_RenderCommand));
	//memset(next_pool, '\0', THE_RENDER_QUEUE_CAPACITY * sizeof(THE_RenderCommand));
	curr_pool_last = curr_pool;
	next_pool_last = next_pool;

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
	//memset(next_pool, '\0', THE_RENDER_QUEUE_CAPACITY * sizeof(THE_RenderCommand));

	
	frame_switch = !frame_switch;
	frame_pool_last = frame_pool[frame_switch];
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

size_t THE_RenderQueueUsed()
{
	return curr_pool_last - curr_pool;
}