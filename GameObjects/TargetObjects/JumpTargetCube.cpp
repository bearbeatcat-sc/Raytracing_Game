#include "JumpTargetCube.h"

#include <Utility/Time.h>

JumpTargetCube::JumpTargetCube(const SimpleMath::Vector3& vec, float moveSpeed, const int maxHP, float destroyTime,
                               const std::string& dxrMeshName, GameManager* pGameManager)
		:TargetCube(maxHP,destroyTime,dxrMeshName,pGameManager),_moveVec(vec),_moveSpeed(moveSpeed)
{
}

void JumpTargetCube::Move()
{
	auto pos = GetPosition();

	pos += _moveVec * Time::DeltaTime * _moveSpeed;
	_moveVec += SimpleMath::Vector3(0, -1.0f, 0) * Time::DeltaTime;

	SetPosition(pos);
}
