#pragma once

#include <Engine/Layers/BaseGameLayer.h>

class ExampleBaseGameLayer : public engine::BaseGameLayer
{
public:
	ExampleBaseGameLayer();
	~ExampleBaseGameLayer();

public:
	 void Update() override;
	 void Draw() override;

private:
	void LoadContent() override;
};