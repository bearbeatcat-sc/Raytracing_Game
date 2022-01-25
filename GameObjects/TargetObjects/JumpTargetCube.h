#pragma once

#include "TargetCube.h"

class JumpTargetCube
	:public TargetCube
{
public:
	JumpTargetCube(const SimpleMath::Vector3& vec,float moveSpeed, const int maxHP, float destroyTime, const std::string& dxrMeshName, GameManager* pGameManager);
	JumpTargetCube() = default;

private:
	void Move() override;

	SimpleMath::Vector3 _moveVec;
	float _moveSpeed;

};
