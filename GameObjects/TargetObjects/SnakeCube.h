
#include "../TargetObjects/TargetObject.h"

class Vector3AnimationCommand;
class AnimationQue;
class CollisionComponent;
class AnimationComponent;
class GameManager;

class SnakeCube
	:public TargetObject
{
public:
	SnakeCube(const int maxHP,float destroyTime,const SimpleMath::Vector3& moveVec,const std::string& dxrMeshName, GameManager* pGameManager);
	~SnakeCube() = default;
	void SetTarget(SnakeCube* pParent);
	void OnDestroyTarget();

private:
	void UpdateActor() override;
	bool IsSetTarget();
	void KnockBack();
	void Move();
	void Chase();
	void Init() override;
	void Shutdown() override;
	void OnCollsion(Actor* other) override;
	void SetTracker(SnakeCube* pTracker);

	bool IsDeath();
	void Damage();
	void ActiveAction(Actor* pPlayer) override;

	SnakeCube* _pTarget;
	SnakeCube* _pTracker;

	CollisionComponent* m_pCollisionComponent;

	std::shared_ptr<AnimationComponent> _AnimationComponent;
	std::shared_ptr<Vector3AnimationCommand> _damageAnimationCommand0;
	std::shared_ptr<Vector3AnimationCommand> _damageAnimationCommand1;

	std::shared_ptr<Vector3AnimationCommand> _knockBackAnimationCommand;


	int _hp;
	const int _maxHP;

	SimpleMath::Vector3 _damageScale;
	SimpleMath::Vector3 _moveVec;

	float _moveSpeed;
	float _keepDistance;

	float _roll;
	const float _rotateSpeed;;
};
