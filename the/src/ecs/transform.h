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

struct mat4 THE_TransformRotateInPlace(struct mat4 tr, float x, float y)
{
	// Back to position 0,0,0
	struct mat4 temp = tr;
	temp.m14 = 0.0f;
	temp.m24 = 0.0f;
	temp.m34 = 0.0f;
	// Rotate in world space (because of the order)
	temp = smat4_multiply(smat4_rotation_x(x), temp);
	temp = smat4_multiply(temp, smat4_rotation_y(y));

	// Back to previous position
	temp.m14 = tr.m14;
	temp.m24 = tr.m24;
	temp.m34 = tr.m34;

	return temp;
}

struct mat4 THE_TransformTranslateLocal(struct mat4 tr, struct vec3 translation)
{
	struct mat4 result = tr;

	struct vec4 col1 = {tr.m11, tr.m21, tr.m31, tr.m41};
	struct vec4 col2 = {tr.m12, tr.m22, tr.m32, tr.m42};
	struct vec4 col3 = {tr.m13, tr.m23, tr.m33, tr.m43};
	struct vec4 col4 = {tr.m14, tr.m24, tr.m34, tr.m44};

	col1 = svec4_multiply_f(col1, translation.x);
	col2 = svec4_multiply_f(col2, translation.y);
	col3 = svec4_multiply_f(col3, translation.z);

	col4 = svec4_add(col4, col1);
	col4 = svec4_add(col4, col2);
	col4 = svec4_add(col4, col3);

	result.m14 = col4.x;
	result.m24 = col4.y;
	result.m34 = col4.z;
	result.m44 = col4.w;

	return result;
}