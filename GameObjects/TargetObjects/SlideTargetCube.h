#pragma once

#include "TargetCube.h"

class SlideTargetCube
	:public TargetCube
{
public:
	SlideTargetCube(const SimpleMath::Vector3& moveVec,float moveSpeed,const int maxHP, float destroyTime, const std::string& dxrMeshName, GameManager* pGameManager);
	~SlideTargetCube() = default;

private:
	void Move() override;

	SimpleMath::Vector3 _moveVec;
	float _moveSpeed;
};
