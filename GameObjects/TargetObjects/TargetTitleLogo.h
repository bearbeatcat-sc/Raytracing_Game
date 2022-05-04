#pragma once

#include <Game_Object/Actor.h>

class DXRInstance;
class Vector3AnimationCommand;
class AnimationQue;
class CollisionComponent;
class AnimationComponent;
class GameManager;

class TargetTitleLogo
	:public Actor
{
public:
	TargetTitleLogo(const int maxHP,GameManager* pGameManager);
	~TargetTitleLogo() = default;

private:
	void UpdateActor() override;
	void Init() override;
	void Shutdown() override;
	bool IsDeath();
	void Damage(float damage);
	void OnCollsion(Actor* other) override;

	void ChangeInstance(float damage);

private:


	CollisionComponent* m_pCollisionComponent;
	GameManager* _pGameManager;

	const std::string _dxrMeshName;

	std::shared_ptr<AnimationComponent> _AnimationComponent;
	std::shared_ptr<Vector3AnimationCommand> _damageAnimationCommand0;
	std::shared_ptr<Vector3AnimationCommand> _damageAnimationCommand1;

	std::shared_ptr<DXRInstance> _titleLogoInstance3;
	std::shared_ptr<DXRInstance> _titleLogoInstance2;
	std::shared_ptr<DXRInstance> _titleLogoInstance1;

	int _hp;
	const int _maxHP;
	SimpleMath::Vector3 _damageScale;
	SimpleMath::Vector3 _initScale;
};
