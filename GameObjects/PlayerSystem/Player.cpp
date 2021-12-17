#include "Player.h"

#include <Components/MeshComponent.h>
#include <Utility/CameraManager.h>
#include <Utility/Time.h>
#include <Utility/Random.h>
#include <Components/Collsions/SphereCollisionComponent.h>
#include <Components/Collsions/OBBCollisionComponent.h>
#include <Device/DirectX/DirectXInput.h>
#include <imgui/imgui.h>
#include <Utility/Camera.h>

#include "LockOnSystem.h"
#include "Components/Collsions/CollisionManager.h"
#include "Device/Raytracing/DXRPipeLine.h"
#include "../PointLightObject.h"

Player::Player(const SimpleMath::Vector3& pos)
	:Actor(), _moveSpeed(0.01f), _cameraTarget(SimpleMath::Vector3::Forward)
{
	SetPosition(pos);
	SetTag("Player");
}

void Player::SetTracker(Actor* pTracker)
{
	_pTracker = pTracker;
}

void Player::UpdateActor()
{
	auto mtx = GetWorldMatrix();
	_instance->SetMatrix(mtx);

	CameraUpdate();
	Movement();
	SpeedUp();

	if(DirectXInput::GetInstance().IsKey(DIK_SPACE))
	{
		_lockOnSystem->LockOn();
	}
}

void Player::Init()
{
	
	_camera = std::make_shared<Camera>();
	CameraManager::GetInstance().AddCamera("PlayerCamera", _camera);
	CameraManager::GetInstance().SetMainCamera("PlayerCamera");

	_camera->SetPosition(GetPosition() + SimpleMath::Vector3(0, 4.0f, -10.0f));
	
	CameraUpdate();
	
	_instance = DXRPipeLine::GetInstance().AddInstance("Sphere", 0);
	_sphereCollisionComponent = new SphereCollisionComponent(this, m_Scale.x * 0.5f, "DynamicObject");
	CollisionManager::GetInstance().AddComponent(_sphereCollisionComponent);
	CollisionManager::GetInstance().AddRegistTree(_sphereCollisionComponent);


	auto mtx = GetWorldMatrix();
	_instance->SetMatrix(mtx);
	_instance->CreateRaytracingInstanceDesc();

	 _lockOnSystem = std::make_shared<LockOnSystem>(this);
	AddComponent(_lockOnSystem);
}

void Player::Shutdown()
{
	if (_sphereCollisionComponent != nullptr)
		_sphereCollisionComponent->Delete();

	_instance->Destroy();
}

void Player::OnCollsion(Actor* other)
{

}

void Player::CameraUpdate()
{
	auto cameraPos = _camera->GetPosition();

	auto target = GetPosition();

	_camera->SetTarget(SimpleMath::Vector3::Lerp(_camera->GetTarget(), _pTracker->GetPosition() + _cameraTarget, Time::DeltaTime * _cameraTargetSpeed));
	_camera->SetPosition(SimpleMath::Vector3::Lerp(_camera->GetPosition(), GetPosition() + SimpleMath::Vector3(0, 3.0f, -3.0f), Time::DeltaTime * _cameraPositionSpeed));

	if(DirectXInput::GetInstance().IsKey(DIK_LEFT))
	{
		_cameraTarget -= SimpleMath::Vector3(0.2f, 0.0f, 0.0f);
		_cameraTarget.Clamp(SimpleMath::Vector3(-4.0f, 0.0f, 1.0f), SimpleMath::Vector3(4.0f, 10.0f, 1.0f));
	}

	if (DirectXInput::GetInstance().IsKey(DIK_RIGHT))
	{
		_cameraTarget += SimpleMath::Vector3(0.2f, 0.0f, 0.0f);
		_cameraTarget.Clamp(SimpleMath::Vector3(-4.0f, 0.0f, 1.0f), SimpleMath::Vector3(4.0f, 10.0f, 1.0f));
	}
}

void Player::SpeedUp()
{
	_moveSpeed = std::clamp(_moveSpeed + Time::DeltaTime * 0.1f, 0.01f, 1.0f);

}

void Player::Movement()
{
	auto trackerPos = _pTracker->GetPosition();
	auto pos = trackerPos;

	SetPosition(SimpleMath::Vector3::Lerp(GetPosition(), pos, Time::DeltaTime * _moveSpeed));
}

