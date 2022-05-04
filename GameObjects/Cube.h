﻿#pragma once

#include <Game_Object/Actor.h>
#include <SimpleMath.h>
#include <memory>

using namespace DirectX;

class DXRInstance;
class AnimationComponent;


class Cube
	:public Actor
{
public:
	Cube(const SimpleMath::Vector3& pos, const SimpleMath::Vector3& scale,const std::string& meshName,float scalingTime = 2.0f);
	~Cube() = default;
	void Delete();

private:
	virtual void UpdateActor() override;
	virtual void Init() override;
	virtual void Shutdown() override;
	virtual void OnCollsion(Actor* other) override;


private:
	std::shared_ptr<AnimationComponent> _AnimationComponent;
	std::shared_ptr<DXRInstance> _instance;
	SimpleMath::Vector3 _initScale;
	const float _scalingTime;

private:

	std::string _DXRMeshName;
};
