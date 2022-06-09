#include "ltransform.h"

namespace leep
{
    LTransform& LTransform::operator=(const LTransform& tr)
    {
        transform_ = tr.transform_;
        return *this;
    }

    LTransform& LTransform::rotateYWorld(float radians)
    {
        // Back to position 0,0,0
        glm::mat4 temp = transform_;
        temp[3][0] = 0.0f;
        temp[3][1] = 0.0f;
        temp[3][2] = 0.0f;
        // Rotate in world space (because of the order)
        temp = glm::rotate(radians, glm::vec3(0.0f, 1.0f, 0.0f)) * temp;
        // Back to previous position
        temp[3][0] = transform_[3][0];
        temp[3][1] = transform_[3][1];
        temp[3][2] = transform_[3][2];
        transform_ = temp;
        return *this;
    }

struct mat4 LTransform::rotateYWorld(struct mat4 tr, float radians)
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

    LTransform& LTransform::translate(float x, float y, float z)
    {
        transform_ = glm::translate(transform_, glm::vec3(x, y, z));
        return *this;
    }

    glm::vec3 LTransform::localLocation() const
    {
        return glm::vec3(transform_[3][0], transform_[3][1], transform_[3][2]);
    }

    void LTransform::setLocation(glm::vec3 new_location)
    {
        transform_[3][0] = new_location.x;
        transform_[3][1] = new_location.y;
        transform_[3][2] = new_location.z;
    }
}
