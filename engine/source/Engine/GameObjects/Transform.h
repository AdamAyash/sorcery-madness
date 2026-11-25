#pragma once

#include <Common/Common.h>
#include <gtc/matrix_transform.hpp>

namespace engine
{
	class Transform : public common::NonCopyableObject
	{
	public:
		Transform();
		~Transform();

	public:
		const glm::mat4 GetModelMatrix() const;

	public:
		glm::vec3 oPosition = glm::vec3(0);
		glm::vec3 oScale = glm::vec3(0);
		glm::vec3 oRotation = glm::vec3(0);
	};
}