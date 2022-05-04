#include "TraceEffect.h"

#include <Device/Raytracing/DXRPipeLine.h>

TraceEffect::TraceEffect(const std::string& meshName)
{
	_instance = DXRPipeLine::GetInstance().AddInstance(meshName, 0);

	auto mtx = GetWorldMatrix();;
	_instance->SetMatrix(mtx);
	_instance->CreateRaytracingInstanceDesc();
}

TraceEffect::~TraceEffect()
{
	_instance->Destroy();
}

void TraceEffect::UpdateActor()
{
	auto mtx = GetWorldMatrix();;
	_instance->SetMatrix(mtx);
}

void TraceEffect::Init()
{
}

void TraceEffect::Shutdown()
{
}

void TraceEffect::OnCollsion(Actor* other)
{
}
