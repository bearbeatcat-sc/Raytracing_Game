#pragma once

#include <Game_Object/Actor.h>
#include <SimpleMath.h>
#include <memory>

#include "Components/Physics/RigidBodyComponent.h"
#include "Device/Raytracing/DXRInstance.h"


class GameManager;
using namespace DirectX::SimpleMath;

class MeshComponent;
class OBBCollisionComponent;
class PlayerCamera;
class LockOnSystem;
class TargetObject;
class SpriteComponent;

class Player
	:public Actor
{
public:
	Player(const SimpleMath::Vector3& pos, GameManager* pGameManager);
	~Player() = default;
	void Shot();

	enum PlayerState
	{
		PlayerState_Stay,
		PlayerState_Move,
	};
	void SetPlayerState(PlayerState playerState);
	void Move();

private:
	virtual void UpdateActor() override;
	void LockOn();
	virtual void Init() override;
	virtual void Shutdown() override;
	void Damage();
	virtual void OnCollsion(Actor* other) override;

private:


	PlayerCamera* _pPlayerCamera;
	GameManager* _pGameManager;

	std::shared_ptr<DXRInstance> _instance;
	OBBCollisionComponent* _pCollisionComponent;
	std::shared_ptr<RigidBodyComponent> _rigidBodyComponent;
	std::shared_ptr<LockOnSystem> _lockOnSystem;
	bool _isGenerateLeft;

	PlayerState _playerState;

};
