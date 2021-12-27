#include "VerticalMoveBlock.h"

#include <Utility/Time.h>
#include <Utility/Math/MathUtility.h>

#include "Utility/Random.h"
#include "Utility/Timer.h"
#include "Device/Raytracing/DXRPipeLine.h"

VerticalMoveBlock::VerticalMoveBlock(const SimpleMath::Vector3& pos, const SimpleMath::Vector3& scale,const std::string& meshName,float angle)
	:_initScale(scale), _IsGenerate(false), _DXRMeshName(meshName), Actor(), _moveSpeed(3.0f),_angle(angle)

{
	_initPosition = pos;
	SetPosition(pos);
	SetScale(scale);

	_GenerateTimer = std::make_shared<Timer>(1.0f);
}

void VerticalMoveBlock::UpdateActor()
{
	if (!_IsGenerate)
	{
		Generate();
		return;
	}


	Move();
	auto mtx = GetWorldMatrix();
	_instance->SetMatrix(mtx);


}

void VerticalMoveBlock::Init()
{
	_instance = DXRPipeLine::GetInstance().AddInstance(_DXRMeshName, 0);

	auto mtx = GetWorldMatrix();;
	_instance->SetMatrix(mtx);
	_instance->CreateRaytracingInstanceDesc();
}


void VerticalMoveBlock::Shutdown()
{
	_instance->Destroy();
}

void VerticalMoveBlock::OnCollsion(Actor* other)
{
}

void VerticalMoveBlock::Generate()
{
	_GenerateTimer->Update();

	if (_GenerateTimer->IsTime())
	{
		_GenerateTimer->Reset();
		_IsGenerate = true;
		return;
	}

	m_Scale = SimpleMath::Vector3::Lerp(SimpleMath::Vector3(0.0f), _initScale, _GenerateTimer->GetRatio() * 1.0f);
	auto mtx = GetWorldMatrix();
	_instance->SetMatrix(mtx);
}

void VerticalMoveBlock::Move()
{
	float y = MathUtility::Sin(_angle);
	_angle += Time::DeltaTime * _moveSpeed;

	auto pos = _initPosition + SimpleMath::Vector3(0, y, 0);
	SetPosition(pos);
}
