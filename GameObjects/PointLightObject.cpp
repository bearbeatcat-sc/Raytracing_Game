#include "PointLightObject.h"

#include <Device/Lights/PointLight.h>
#include <Device/Lights/LightManager.h>
#include <Utility/Time.h>
#include <Utility/Math/MathUtility.h>

PointLightObject::PointLightObject(const SimpleMath::Vector3& pos, const SimpleMath::Color& color, float distance)
	:_color(color), _targetDistance(distance)
{
	SetPosition(pos);
	_distance = 0.0f;
}

void PointLightObject::UpdateActor()
{
	_distance = MathUtility::Lerp(_distance, _targetDistance, Time::DeltaTime);
	_pointLight->UpdatePointLight(GetPosition(), _color, _distance, 1.0f);
}

void PointLightObject::Init()
{

	GetPosition();

	_pointLight = std::make_shared<PointLight>(GetPosition(), _color, _distance, 1.0f);
	LightManager::GetInstance().AddPointLight(_pointLight);
}

void PointLightObject::Shutdown()
{
	_pointLight->Destroy();
}

void PointLightObject::OnCollsion(Actor* other)
{
}
