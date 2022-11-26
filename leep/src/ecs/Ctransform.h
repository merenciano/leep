#include "mathc.h"

struct mat4 THE_TransformRotateYWorld(struct mat4 tr, float radians)
{
	// Back to position 0,0,0
	struct mat4 temp = tr;
	temp.m14 = 0.0f;
	temp.m24 = 0.0f;
	temp.m34 = 0.0f;
	// Rotate in world space (because of the order)
	temp = smat4_multiply(smat4_rotation_y(radians), temp);

	// Back to previous position
	temp.m14 = tr.m14;
	temp.m24 = tr.m24;
	temp.m34 = tr.m34;

	return temp;
}