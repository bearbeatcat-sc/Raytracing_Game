#include "LockOnUIObject.h"

#include <Device/Raytracing/DXRPipeLine.h>
#include <Utility/Timer.h>

LockOnUIObject::LockOnUIObject(const float destroyTime, Actor* lockonTarget)
	:_isGenerate(false),_pTarget(lockonTarget)
{
	_generateTimer = std::make_shared<Timer>(0.4f);
	_destroyTimer = std::make_shared<Timer>(destroyTime);
}

void LockOnUIObject::UpdateActor()
{
	if(_pTarget->GetDestroyFlag())
	{
		Destroy();
		return;
	}

	SetPosition(_pTarget->GetPosition());
	auto mtx = GetWorldMatrix();
	_instance->SetMatrix(mtx);


	if (_isGenerate)
	{
		_destroyTimer->Update();
		SetScale(SimpleMath::Vector3::Lerp(GetScale(), SimpleMath::Vector3::Zero, _destroyTimer->GetRatio()));

		if(_destroyTimer->IsTime())
		{
			Destroy();
		}

		return;
	}

	_generateTimer->Update();
	if (_generateTimer->IsTime())
	{
		_isGenerate = true;
	}

	SetScale(SimpleMath::Vector3::Lerp(GetScale(), _initScale, _generateTimer->GetRatio()));
}

void LockOnUIObject::Init()
{
	_initScale = GetScale();
	SetScale(SimpleMath::Vector3::Zero);

	_instance = DXRPipeLine::GetInstance().AddInstance("TestUI", 0);

	auto mtx = GetWorldMatrix();
	_instance->SetMatrix(mtx);
	_instance->CreateRaytracingInstanceDesc();


}

void LockOnUIObject::Shutdown()
{
	_instance->Destroy();
}

void LockOnUIObject::OnCollsion(Actor* other)
{
}
