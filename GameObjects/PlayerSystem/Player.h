#pragma once

#include <Game_Object/Actor.h>
#include <SimpleMath.h>
#include <memory>

#include "Components/Physics/RigidBodyComponent.h"
#include "Device/Raytracing/DXRInstance.h"


class Cube;
class GameManager;
using namespace DirectX::SimpleMath;

class MeshComponent;
class OBBCollisionComponent;
class PlayerCamera;
class LockOnSystem;
class TargetObject;
class SpriteComponent;
class AimCursor;

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
		PlayerState_Run,
	};
	void SetPlayerState(PlayerState playerState);
	void Move();
	const SimpleMath::Vector3& GetCursorPosition();

private:
	virtual void UpdateActor() override;
	void LockOn();
	void MoveCusor();

	virtual void Init() override;
	virtual void Shutdown() override;
	void Damage();
	virtual void OnCollsion(Actor* other) override;

private:


	PlayerCamera* _pPlayerCamera;
	GameManager* _pGameManager;
	AimCursor* _pAimCursor;

	OBBCollisionComponent* _pCollisionComponent;
	std::shared_ptr<RigidBodyComponent> _rigidBodyComponent;
	std::shared_ptr<LockOnSystem> _lockOnSystem;
	bool _isGenerateLeft;

	PlayerState _playerState;
	float _curosrRadius = 10.0f;
	 float _moveSpeed;
};
