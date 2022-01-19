#pragma once

#include "../TargetObjects/TargetObject.h"

class Vector3AnimationCommand;
class AnimationQue;
class CollisionComponent;
class AnimationComponent;
class GameManager;

class TargetCube
	:public TargetObject
{
public:
	TargetCube(const int maxHP,float destroyTime,const std::string& dxrMeshName, GameManager* pGameManager);
	~TargetCube() = default;

private:
	void ActiveAction() override;
	void UpdateActor() override;
	void Init() override;
	void Shutdown() override;
	bool IsDeath();
	void Damage();
	void OnCollsion(Actor * other) override;

private:
	CollisionComponent* m_pCollisionComponent;

	const std::string _dxrMeshName;

	std::shared_ptr<AnimationComponent> _AnimationComponent;
	std::shared_ptr<Vector3AnimationCommand> _damageAnimationCommand0;
	std::shared_ptr<Vector3AnimationCommand> _damageAnimationCommand1;


	int _hp;
	const int _maxHP;
	SimpleMath::Vector3 _damageScale;
	SimpleMath::Vector3 _initScale;
};
