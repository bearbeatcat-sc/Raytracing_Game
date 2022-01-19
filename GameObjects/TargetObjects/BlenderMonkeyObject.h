#pragma once

#include "../TargetObjects/TargetObject.h"

class Vector3AnimationCommand;
class AnimationQue;
class CollisionComponent;
class AnimationComponent;
class GameManager;

class BlenderMonkeyObject
	:public TargetObject
{
public:
	enum BlenderMonkyObjectType
	{
		BlenderMonkyObjectType_Mirror,
		BlenderMonkyObjectType_Clear,
	};

	BlenderMonkeyObject(const int maxHP,float destroyTime, BlenderMonkyObjectType blenderMonkyType, GameManager* pGameManager);
	~BlenderMonkeyObject() = default;



private:
	void UpdateActor() override;
	void Init() override;
	void Shutdown() override;
	void OnCollsion(Actor* other) override;
	bool IsDeath();
	void Damage();
	void ActiveAction() override;

	CollisionComponent* m_pCollisionComponent;

	 std::string _dxrMeshName;
	 std::string _effectMeshName;


	std::shared_ptr<AnimationComponent> _AnimationComponent;
	std::shared_ptr<Vector3AnimationCommand> _damageAnimationCommand0;
	std::shared_ptr<Vector3AnimationCommand> _damageAnimationCommand1;

	BlenderMonkyObjectType _blenderMonkeyType;

	float _bombRadius;
	int _hp;
	const int _maxHP;
	SimpleMath::Vector3 _damageScale;
	SimpleMath::Vector3 _initScale;
};
