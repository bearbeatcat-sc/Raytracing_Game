#include "LockOnUIObject.h"

#include <Device/Raytracing/DXRPipeLine.h>

void LockOnUIObject::UpdateActor()
{
	auto mtx = GetWorldMatrix();
	_instance->SetMatrix(mtx);
}

void LockOnUIObject::Init()
{

	_instance = DXRPipeLine::GetInstance().AddInstance("TestUI", 0);

	auto mtx = GetWorldMatrix();
	_instance->SetMatrix(mtx);
	_instance->CreateRaytracingInstanceDesc();
}

void LockOnUIObject::Shutdown()
{
}

void LockOnUIObject::OnCollsion(Actor* other)
{
}
