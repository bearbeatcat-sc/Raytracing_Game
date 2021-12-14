#pragma once


#include "TargetObject.h"

class CollisionComponent;

class TargetCube
	:public TargetObject
{
public:
	TargetCube(const std::string& dxrMeshName);
	~TargetCube() = default;

private:
	void UpdateActor() override;
	void Init() override;
	void Shutdown() override;
	void OnCollsion(Actor* other) override;

private:
	CollisionComponent* m_pCollisionComponent;
};
