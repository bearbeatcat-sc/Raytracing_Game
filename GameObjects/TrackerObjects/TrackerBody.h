#pragma once

#include "../TargetObjects/TargetObject.h"

class AnimationQue;
class CollisionComponent;
class AnimationComponent;
class GameManager;

class TrackerBody
	:public TargetObject
{
public:
	TrackerBody(const int maxHP,const std::string& dxrMeshName, GameManager* pGameManager);
	~TrackerBody() = default;

private:
	void UpdateActor() override;
	void Init() override;
	void Shutdown() override;
	bool IsDeath();
	void Damage();
	void OnCollsion(Actor * other) override;

private:
	CollisionComponent* m_pCollisionComponent;

	std::shared_ptr<AnimationComponent> _AnimationComponent;


	int _hp;
	const int _maxHP;
	SimpleMath::Vector3 _damageScale;
};
