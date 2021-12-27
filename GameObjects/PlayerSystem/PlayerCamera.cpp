#include "PlayerCamera.h"

#include <Device/DirectX/DirectXInput.h>
#include <imgui/imgui.h>
#include <Utility/CameraManager.h>
#include <Utility/Random.h>
#include <Utility/Time.h>
#include <Utility/Timer.h>

PlayerCamera::PlayerCamera(Actor* pTracker)
	:_pTracker(pTracker), _shakePower(0.0f), _shake(SimpleMath::Vector2::Zero)
{
	_shakeTimer = std::make_shared<Timer>(1.0f);
}


void PlayerCamera::Shake(const float power,const float duration)
{
	_shakePower = power;
	_shakeTimer->Reset();
	_shakeTimer->SetTime(duration);
}

void PlayerCamera::UpdateActor()
{
	CameraUpdate();
	ShakeUpdate();
}

void PlayerCamera::ShakeUpdate()
{
	if (_shakeTimer->IsTime())
	{
		_shake = SimpleMath::Vector2::Zero;
		return;
	}

	_shakeTimer->Update();

	_shake = SimpleMath::Vector2(Random::GetRandom(-1.0f, 1.0f), Random::GetRandom(-1.0f, 1.0f)) * _shakePower;
}
void PlayerCamera::Init()
{
	_camera = std::make_shared<Camera>();
	CameraManager::GetInstance().AddCamera("PlayerCamera", _camera);
	CameraManager::GetInstance().SetMainCamera("PlayerCamera");

	_camera->SetPosition(GetPosition() + SimpleMath::Vector3(0, 4.0f, -10.0f));

}

void PlayerCamera::Shutdown()
{
}

void PlayerCamera::OnCollsion(Actor* other)
{
}

void PlayerCamera::CameraUpdate()
{
	auto cameraPos = _camera->GetPosition();


	float shake[2] =
	{
	_shake.x,
	_shake.y
	};

	ImGui::Begin("PlayerParameters");
	ImGui::DragFloat2("shake", shake, 0.1f);
	ImGui::End();


	_camera->SetTarget(SimpleMath::Vector3::Lerp(_camera->GetTarget(), _pTracker->GetPosition() + _cameraTarget, Time::DeltaTime * _cameraTargetSpeed));
	_camera->SetPosition(SimpleMath::Vector3::Lerp(cameraPos, GetPosition() + SimpleMath::Vector3(0, 3.0f, -3.0f), Time::DeltaTime * _cameraPositionSpeed));
	_camera->SetPosition(_camera->GetPosition() + SimpleMath::Vector3(_shake.x, _shake.y, 0));

	if (DirectXInput::GetInstance().IsKey(DIK_LEFT))
	{
		_cameraTarget -= SimpleMath::Vector3(1.0f, 0.0f, 0.0f);
		_cameraTarget.Clamp(SimpleMath::Vector3(-8.0f, 0.0f, 1.0f), SimpleMath::Vector3(8.0f, 10.0f, 1.0f));
	}

	if (DirectXInput::GetInstance().IsKey(DIK_RIGHT))
	{
		_cameraTarget += SimpleMath::Vector3(1.0f, 0.0f, 0.0f);
		_cameraTarget.Clamp(SimpleMath::Vector3(-8.0f, 0.0f, 1.0f), SimpleMath::Vector3(8.0f, 10.0f, 1.0f));
	}
}
