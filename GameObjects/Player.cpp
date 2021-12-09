#include "Player.h"

#include <Components/MeshComponent.h>
#include <Utility/CameraManager.h>
#include <Utility/Time.h>
#include <Utility/Random.h>
#include <Components/Collsions/SphereCollisionComponent.h>
#include <Components/Collsions/OBBCollisionComponent.h>
#include <Device/DirectX/DirectXInput.h>
#include <Utility/Camera.h>

#include "Components/Collsions/CollisionManager.h"
#include "Device/Raytracing/DXRPipeLine.h"

Player::Player(const SimpleMath::Vector3& pos)
	:Actor()
{
	SetPosition(pos);
}

Player::~Player()
{
}

void Player::UpdateActor()
{
	auto mtx = GetWorldMatrix();
	_instance->SetMatrix(mtx);

	CameraUpdate();

	_rigidBodyComponent->AddImpulse(SimpleMath::Vector3(0, 0, 20.0f) * Time::DeltaTime);

	if(DirectXInput::GetInstance().IsKeyDown(DIK_UPARROW))
	{
		_rigidBodyComponent->AddImpulse(SimpleMath::Vector3(0, 0, 10));
	}

	if (DirectXInput::GetInstance().IsKey(DIK_LEFTARROW))
	{
		_rigidBodyComponent->AddImpulse(SimpleMath::Vector3(-0.4f, 0, 0));
	}

	if (DirectXInput::GetInstance().IsKey(DIK_RIGHTARROW))
	{
		_rigidBodyComponent->AddImpulse(SimpleMath::Vector3(0.4f, 0, 0));
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

	_rigidBodyComponent = std::make_shared<RigidBodyComponent>(this, _sphereCollisionComponent);
	AddComponent(_rigidBodyComponent);
	_sphereCollisionComponent->RegistRigidBody(_rigidBodyComponent);
	
	_rigidBodyComponent->_AddGravity = SimpleMath::Vector3(0, -8.0f, 0.0f);
	_rigidBodyComponent->_Mass = 1.0f;
	_rigidBodyComponent->_Elasticty = 1.0f;

	auto mtx = GetWorldMatrix();
	_instance->SetMatrix(mtx);
	_instance->CreateRaytracingInstanceDesc();

	_rigidBodyComponent->AddImpulse(SimpleMath::Vector3(0, 0, 10) );

	SetTag("Player");
}

void Player::Shutdown()
{
	if (_sphereCollisionComponent != nullptr)
		_sphereCollisionComponent->Delete();

	_instance->Destroy();
}

void Player::OnCollsion(Actor* other)
{
	if(other->IsContainsTag("Wall"))
	{
		auto pos = other->GetPosition();
		auto normal = pos - GetPosition();
		normal.Normalize();
		
		_rigidBodyComponent->AddImpulse(SimpleMath::Vector3(normal.x, 4.0f, 20.0f) * Time::DeltaTime);
	}


}

void Player::CameraUpdate()
{
	auto cameraPos = _camera->GetPosition();

	auto target = GetPosition();

	_camera->SetTarget(SimpleMath::Vector3::Lerp(_camera->GetPosition(), GetPosition() + SimpleMath::Vector3(0, 0.0f,1.0f), Time::DeltaTime * 3.0f));
	_camera->SetPosition(SimpleMath::Vector3::Lerp(_camera->GetPosition(), GetPosition() + SimpleMath::Vector3(0, 3.0f, -3.0f), Time::DeltaTime * 2.0f));
}
