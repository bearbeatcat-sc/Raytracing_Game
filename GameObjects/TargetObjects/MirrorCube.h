#pragma once

#include "TargetObject.h"

class CollisionComponent;
class AnimationComponent;
class Vector3AnimationCommand;

class MirrorCube
	:public TargetObject
{
public:
	MirrorCube(GameManager* pGameManager,const SimpleMath::Vector3& addScale,const SimpleMath::Vector3& maxScale);
	~MirrorCube() = default;

private:
	void UpdateActor() override;
	void Init() override;
	void Shutdown() override;
	void OnCollsion(Actor* other) override;

	void Expand();
	void KnockBack();
	void Hit();

private:
	CollisionComponent* _pCollisionComponent;
	std::shared_ptr<AnimationComponent> _pAnimationComponent;
	std::shared_ptr<Vector3AnimationCommand> _pKnockBackAnimationCommand;
	std::shared_ptr<Vector3AnimationCommand> _pExpandkAnimationCommand;

	const SimpleMath::Vector3 _maxScale;
	const SimpleMath::Vector3 _addScale;
	const float _knockBackAmount;

	bool _isKnockBack;
};
