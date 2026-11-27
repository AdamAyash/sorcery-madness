#pragma once

#include <Common/Common.h>
#include "Transform.h"
#include "../Graphics/Renderer2D/Renderer2D.h"

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
		virtual void Draw(IN Renderer2D& oRenderer2D) PURE;

	public:
		Transform oTransform;
	};

	using GameObjectsList = std::vector<GameObject*>;
}