﻿#pragma once


#include <Game_Object/Actor.h>
#include <SimpleMath.h>
#include <memory>

using namespace DirectX;

class DXRInstance;
class Timer;
class CollisionComponent;
class RigidBodyComponent;

class Cube
	:public Actor
{
public:
	Cube(const SimpleMath::Vector3& pos, const SimpleMath::Vector3& scale,const std::string& meshName);
	~Cube() = default;

private:
	virtual void UpdateActor() override;
	virtual void Init() override;
	virtual void Shutdown() override;
	virtual void OnCollsion(Actor* other) override;

	void Generate();

private:	
	std::shared_ptr<DXRInstance> _instance;
	std::shared_ptr<Timer> _GenerateTimer;
	SimpleMath::Vector3 _initScale;
	bool _IsGenerate;


private:

	std::string _DXRMeshName;
};
