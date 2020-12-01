#include "transform.h"
//#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"

namespace leep
{
    Transform& Transform::operator=(const Transform& tr)
    {
        transform_ = tr.transform_;
        return *this;
    }

    Transform& Transform::rotateYWorld(float radians)
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
}
