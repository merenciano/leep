#include "Crenderer.h"
#include "Crendercommands.h"

/*
 * Concatenates a list of commands to the render queue.
 */
void THE_AddCommands(leep::Renderer *r, THE_RenderCommand *rc)
{
	if (r->next_last) {
		r->next_last->next = rc;
	} else {
		r->next = rc;
	}

	THE_RenderCommand *c = NULL;
	for (c = rc; c->next != NULL; c = c->next);
	r->next_last = c;
}

void THE_RenderFrame(leep::Renderer *r)
{
	THE_RenderCommand *i = r->curr;
	THE_RenderCommand *prev = i;
	if (!i) {
		return;
	}

	i->execute(&(i->data));
	while(i != r->curr_last)
	{
		i = i->next;
		free(prev);
		prev = i;
		i->execute(&(i->data));
	}
	r->curr = NULL;
	r->curr_last = NULL;
	// Todo delete resources
}

void THE_RenderSubmitFrame(leep::Renderer *r)
{
	// Swap quques
	r->curr = r->next;
	r->curr_last = r->next_last;
	r->next = NULL;
	r->next_last = NULL;
}