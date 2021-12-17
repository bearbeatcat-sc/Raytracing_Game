﻿#pragma once

#include <Game_Object/Actor.h>
#include <SimpleMath.h>
#include <memory>

#include "Components/Physics/RigidBodyComponent.h"
#include "Device/Raytracing/DXRInstance.h"

class LockOnSystem;
using namespace DirectX::SimpleMath;

class MeshComponent;
class SphereCollisionComponent;
class OBBCollisionComponent;

class Player
	:public Actor
{
public:
	Player(const SimpleMath::Vector3& pos);
	~Player() = default;
	void SetTracker(Actor* pTracker);

private:
	virtual void UpdateActor() override;
	virtual void Init() override;
	virtual void Shutdown() override;
	virtual void OnCollsion(Actor* other) override;

	void CameraUpdate();
	void SpeedUp();
	void Movement();

private:

	Actor* _pTracker;
	
	std::shared_ptr<DXRInstance> _instance;
	SphereCollisionComponent* _sphereCollisionComponent;
	std::shared_ptr<RigidBodyComponent> _rigidBodyComponent;
	std::shared_ptr<Camera> _camera;
	std::shared_ptr<LockOnSystem> _lockOnSystem;

	float _cameraTargetSpeed = 3.2f;
	float _cameraPositionSpeed = 3.0f;

	float _moveSpeed;

	SimpleMath::Vector3 _cameraTarget;
};
