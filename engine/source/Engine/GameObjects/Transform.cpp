#include "Transform.h"

namespace engine
{
	Transform::Transform()
	{
	}

	Transform::~Transform()
	{
	}

	const glm::mat4 Transform::GetModelMatrix() const
	{
		auto modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::translate(modelMatrix, oPosition);
		modelMatrix = glm::scale(modelMatrix, oScale);

		return modelMatrix;
	}
}