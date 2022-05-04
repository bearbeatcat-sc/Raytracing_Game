#include "AimCursorObject.h"

#include <Device/Raytracing/DXRPipeLine.h>

void AimCursorObject::UpdateActor()
{
	auto mtx = GetWorldMatrix();
	_instance->SetMatrix(mtx);
}

void AimCursorObject::Init()
{
	_instance = DXRPipeLine::GetInstance().AddInstance("AimCursorUI", 0,1);
	auto mtx = GetWorldMatrix();;
	_instance->SetMatrix(mtx);
	_instance->CreateRaytracingInstanceDesc();
}

void AimCursorObject::Shutdown()
{
	if (_instance)
		_instance->Destroy();
}

void AimCursorObject::OnCollsion(Actor* other)
{
}
