#include "SlideTargetCube.h"

#include <Utility/Time.h>

SlideTargetCube::SlideTargetCube(const SimpleMath::Vector3& moveVec,float moveSpeed, const int maxHP, float destroyTime,
                                 const std::string& dxrMeshName, GameManager* pGameManager)
		:TargetCube(maxHP,destroyTime,dxrMeshName,pGameManager), _moveVec(moveVec),_moveSpeed(moveSpeed)
{
}

void SlideTargetCube::Move()
{
	auto pos = GetPosition();

	pos += _moveVec * Time::DeltaTime * _moveSpeed;

	SetPosition(pos);
}
