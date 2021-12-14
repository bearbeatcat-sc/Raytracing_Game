#include "TargetCube.h"

#include <Components/Collsions/CollisionManager.h>
#include <Components/Collsions/OBBCollisionComponent.h>
#include <Device/Raytracing/DXRPipeLine.h>

TargetCube::TargetCube(const std::string& dxrMeshName)
{
	_instance = DXRPipeLine::GetInstance().AddInstance(dxrMeshName, 0);

	auto mtx = GetWorldMatrix();;
	_instance->SetMatrix(mtx);
	_instance->CreateRaytracingInstanceDesc();
}

void TargetCube::UpdateActor()
{
	auto mtx = GetWorldMatrix();
	_instance->SetMatrix(mtx);
}

void TargetCube::Init()
{
	m_pCollisionComponent = new OBBCollisionComponent(this, GetPosition(), m_Scale, "DynamicObject");
	//m_pCollisionComponent = new SphereCollisionComponent(this, 10.0f, "Object");

	CollisionManager::GetInstance().AddComponent(m_pCollisionComponent);
	CollisionManager::GetInstance().AddRegistTree(m_pCollisionComponent);

	SetTag("Target");
}

void TargetCube::Shutdown()
{
}

void TargetCube::OnCollsion(Actor* other)
{
}
