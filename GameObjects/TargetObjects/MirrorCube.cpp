#include "MirrorCube.h"

#include <algorithm>
#include <Components/Animations/AnimationCommandList.h>
#include <Components/Animations/Vector3AnimationCommand.h>
#include <Components/Collsions/CollisionManager.h>
#include <Components/Collsions/OBBCollisionComponent.h>
#include <Device/DirectX/DirectXInput.h>
#include <Device/Raytracing/DXRPipeLine.h>
#include <Utility/Time.h>
#include <Utility/Math/MathUtility.h>

#include "../GameSystem/GameManager.h"
#include "../PlayerSystem/Player.h"

MirrorCube::MirrorCube(GameManager* pGameManager,bool controllFlag,float angle)
	:_pGameManager(pGameManager),_radius(6.0f), _angle(angle), _rotateSpeed(3.0f),_controllFlag(controllFlag)
{
	_instance = DXRPipeLine::GetInstance().AddInstance("WhiteCube", 0);

	auto mtx = GetWorldMatrix();;
	_instance->SetMatrix(mtx);
	_instance->CreateRaytracingInstanceDesc();
}

void MirrorCube::UpdateActor()
{

	auto mtx = GetWorldMatrix();;
	_instance->SetMatrix(mtx);

	//Rotate(0.0f);
	//if(DirectXInput::GetInstance().IsActiveGamePad())
	//{
	//	if(_controllFlag)
	//	{
	//		if (DirectXInput::GetInstance().GetGamePadValue(GAMEPAD_ThubStick_LX) >= 0.2f)
	//		{
	//			const float input = -DirectXInput::GetInstance().GetGamePadValue(GAMEPAD_ThubStick_LX);
	//			Rotate(input);
	//		}

	//		if (DirectXInput::GetInstance().GetGamePadValue(GAMEPAD_ThubStick_LX) <= -0.2f)
	//		{
	//			const float input = DirectXInput::GetInstance().GetGamePadValue(GAMEPAD_ThubStick_LX);
	//			Rotate(input * -1.0f);
	//		}
	//		return;
	//	}
	//	else
	//	{
	//		if (DirectXInput::GetInstance().GetGamePadValue(GAMEPAD_ThubStick_RX) >= 0.2f)
	//		{
	//			const float input = DirectXInput::GetInstance().GetGamePadValue(GAMEPAD_ThubStick_RX);
	//			Rotate(input * -1.0f);
	//		}

	//		if (DirectXInput::GetInstance().GetGamePadValue(GAMEPAD_ThubStick_RX) <= -0.2f)
	//		{
	//			const float input = DirectXInput::GetInstance().GetGamePadValue(GAMEPAD_ThubStick_RX);
	//			Rotate(input * -1.0f);
	//		}
	//	}

	//	return;
	//}

	//if(_controllFlag)
	//{
	//	if (DirectXInput::GetInstance().IsKey(DIK_LEFTARROW))
	//	{
	//		Rotate(1.0f);
	//	}

	//	if (DirectXInput::GetInstance().IsKey(DIK_RIGHTARROW))
	//	{
	//		Rotate(-1.0f);
	//	}
	//}



}

void MirrorCube::Init()
{
	SetTag("Mirror");
	SetActorName("MirrorCube");

	m_pCollisionComponent = new OBBCollisionComponent(this, GetPosition(), m_Scale, "AllHitObject");
	//m_pCollisionComponent = new SphereCollisionComponent(this, 10.0f, "Object");

	CollisionManager::GetInstance().AddComponent(m_pCollisionComponent);
	CollisionManager::GetInstance().AddRegistTree(m_pCollisionComponent);

	auto pPlayer = _pGameManager->GetPlayer();

	if (pPlayer == nullptr)
	{
		throw std::runtime_error("Unable to retrieve the player.");
	}

}

void MirrorCube::Shutdown()
{
	m_pCollisionComponent->Delete();
	_instance->Destroy();
}

void MirrorCube::OnCollsion(Actor* other)
{

}

void MirrorCube::Rotate(float angle)
{
	auto pPlayer = _pGameManager->GetPlayer();

	if (pPlayer == nullptr)return;




	_angle += Time::DeltaTime * _rotateSpeed * angle;
	_angle = std::clamp(_angle, 1.0f, 2.2f);

	auto playerPosition = pPlayer->GetPosition();

	SimpleMath::Vector3 pos = playerPosition + SimpleMath::Vector3(_radius * cos(_angle), 0, _radius * sin(_angle));

	SetPosition(pos);


	auto rotate = MathUtility::LookAt(GetPosition(), playerPosition);
	SetRotation(rotate);

}
