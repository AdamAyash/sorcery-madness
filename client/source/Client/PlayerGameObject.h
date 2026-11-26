#pragma once

#include <Engine/GameObjects/GameObject.h>

class PlayerGameObject final : public engine::GameObject
{
public:
	PlayerGameObject();
	~PlayerGameObject();

public:
	void Draw(IN engine::Renderer2D& oRenderer2D) override;

private:
	common::Scope<engine::Shader> m_pShader = nullptr;
};