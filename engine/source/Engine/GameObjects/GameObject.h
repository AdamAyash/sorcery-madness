#pragma once

#include <Common/Common.h>
#include "Transform.h"

namespace engine
{
	class GameObject : public common::NonCopyableObject
	{
	protected:
		GameObject();

	public:
		virtual ~GameObject();

	public:
		virtual void Update();
		virtual void Draw();

	public:
		Transform oTransform;
	};

	using GameObjectsList = std::vector<GameObject*>;
}