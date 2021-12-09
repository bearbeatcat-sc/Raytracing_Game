#pragma once

#include <Game_Object/Actor.h>

class PointLight;

class PointLightObject
	:public Actor
{
public:
	PointLightObject(const SimpleMath::Vector3& pos,const SimpleMath::Color& color,float distance);
	~PointLightObject() = default;

private:
	void UpdateActor() override;
	void Init() override;
	void Shutdown() override;
	void OnCollsion(Actor* other) override;

private:
	std::shared_ptr<PointLight> _pointLight;

	SimpleMath::Color _color;
	float _distance;
};
