#include "BreakEffect.h"

#include <Device/Raytracing/DXRPipeLine.h>
#include <Utility/Time.h>

BreakEffect::BreakEffect(const SimpleMath::Vector3& velocity, const std::string& meshName)
	:_velocity(velocity), _dxrMeshName(meshName)
{
	_instance = DXRPipeLine::GetInstance().AddInstance(_dxrMeshName, 0);

	auto mtx = GetWorldMatrix();;
	_instance->SetMatrix(mtx);
	_instance->CreateRaytracingInstanceDesc();
}

void BreakEffect::UpdateActor()
{
	auto position = GetPosition();

	_velocity += SimpleMath::Vector3(0, -10.0f, 0) * Time::DeltaTime;

	position += _velocity * Time::DeltaTime;

	auto rotate = GetEulerRotation();

	rotate += _velocity * Time::DeltaTime;
	SetRotation(rotate);

	SetPosition(position);
	auto mtx = GetWorldMatrix();;
	_instance->SetMatrix(mtx);
	_instance->CreateRaytracingInstanceDesc();
}

void BreakEffect::Init()
{
}

void BreakEffect::Shutdown()
{
	if (_instance == nullptr)return;

	_instance->Destroy();
}

void BreakEffect::OnCollsion(Actor* other)
{
}
