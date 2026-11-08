#include "Object.h"

glm::mat4 Object::modelMatrix(glm::mat4 parentTransform) const {
    glm::mat4 model = glm::translate(parentTransform, position);
    if (angle != 0.0f && glm::length(rotation) > 0.0f)
        model = glm::rotate(model, glm::radians(angle), glm::normalize(rotation));
    return glm::scale(model, scale);
}
