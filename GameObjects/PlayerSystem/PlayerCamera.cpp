#include "PlayerCamera.h"

#include <Device/DirectX/DirectXInput.h>
#include <imgui/imgui.h>
#include <Utility/CameraManager.h>
#include <Utility/Random.h>
#include <Utility/Time.h>
#include <Utility/Timer.h>

PlayerCamera::PlayerCamera(Actor* pTracker)
	:_pTracker(pTracker), _shakePower(0.0f), _shake(SimpleMath::Vector2::Zero), _Pitch(0.0f), _Yaw(0.0f)
{
	_shakeTimer = std::make_shared<Timer>(1.0f);

	_camera = std::make_shared<Camera>();
	CameraManager::GetInstance().AddCamera("PlayerCamera", _camera);
	CameraManager::GetInstance().SetMainCamera("PlayerCamera");

	_camera->SetPosition(GetPosition() + SimpleMath::Vector3(0, 4.0f, -10.0f));
}


void PlayerCamera::Shake(const float power, const float duration)
{
	_shakePower = power;
	_shakeTimer->Reset();
	_shakeTimer->SetTime(duration);
}

const SimpleMath::Vector3 PlayerCamera::GetForward()
{
	auto target = _camera->GetTarget();
	auto position = _camera->GetPosition();

	auto vec = target - position;
	vec.Normalize();

	return vec;
}

const float PlayerCamera::GetPitch()
{
	return _Pitch;
}

const float PlayerCamera::GetYaw()
{
	return _Yaw;
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
	auto cameraTarget = _camera->GetTarget();


	//float shake[2] =
	//{
	//_shake.x,
	//_shake.y
	//};

	//float cameraTargets[3] =
	//{
	//cameraTarget.x,
	//cameraTarget.y,
	//cameraTarget.z,
	//};

	//float cameraPosition[3] =
	//{
	//cameraPos.x,
	//cameraPos.y,
	//cameraPos.z,
	//};

	//ImGui::Begin("PlayerParameters");
	//ImGui::DragFloat2("shake", shake, 0.1f);
	//ImGui::DragFloat3("CameraTarget", cameraTargets, 0.1f);
	//ImGui::DragFloat("Pitch", &_Pitch, 0.1f);
	//ImGui::DragFloat("Yaw", &_Yaw, 0.1f);
	//ImGui::DragFloat3("CameraPosition", cameraPosition, 0.1f);
	//ImGui::End();


	_camera->SetTarget(SimpleMath::Vector3::Lerp(_camera->GetTarget(), _pTracker->GetPosition() + _cameraTarget, Time::DeltaTime * _cameraTargetSpeed));
	//_camera->SetPosition(SimpleMath::Vector3::Lerp(cameraPos, GetPosition() + SimpleMath::Vector3(0, 3.0f, -3.0f), Time::DeltaTime * _cameraPositionSpeed));
	_camera->SetPosition(_pTracker->GetPosition());
	_camera->SetPosition(_camera->GetPosition() + SimpleMath::Vector3(_shake.x, _shake.y, 0));

	if (DirectXInput::GetInstance().IsActiveGamePad())
	{
		if (DirectXInput::GetInstance().GetGamePadValue(GAMEPAD_ThubStick_LX) >= 0.2f)
		{
			const float input = DirectXInput::GetInstance().GetGamePadValue(GAMEPAD_ThubStick_LX);
			ViewMove(SimpleMath::Vector3(input,0,0));
		}

		if (DirectXInput::GetInstance().GetGamePadValue(GAMEPAD_ThubStick_LX) <= -0.2f)
		{
			const float input = DirectXInput::GetInstance().GetGamePadValue(GAMEPAD_ThubStick_LX);
			ViewMove(SimpleMath::Vector3(input, 0, 0));
		}

		if (DirectXInput::GetInstance().GetGamePadValue(GAMEPAD_ThubStick_LY) >= 0.2f)
		{
			const float input = DirectXInput::GetInstance().GetGamePadValue(GAMEPAD_ThubStick_LX);
			ViewMove(SimpleMath::Vector3(0, input, 0));
		}

		if (DirectXInput::GetInstance().GetGamePadValue(GAMEPAD_ThubStick_LY) <= -0.2f)
		{
			const float input = DirectXInput::GetInstance().GetGamePadValue(GAMEPAD_ThubStick_LX);
			ViewMove(SimpleMath::Vector3(0, input, 0));
		}
	}
	else
	{
		if (DirectXInput::GetInstance().IsKey(DIK_LEFT))
		{
			//_cameraTarget -= SimpleMath::Vector3(10.0f, 0.0f, 0.0f) * Time::DeltaTime;
			//_cameraTarget.Clamp(SimpleMath::Vector3(-8.0f, 0.0f, 1.0f), SimpleMath::Vector3(8.0f, 10.0f, 1.0f));
			ViewMove(SimpleMath::Vector3::Left);
		}

		if (DirectXInput::GetInstance().IsKey(DIK_RIGHT))
		{
			//_cameraTarget += SimpleMath::Vector3(10.0f, 0.0f, 0.0f) * Time::DeltaTime;
			//_cameraTarget.Clamp(SimpleMath::Vector3(-8.0f, 0.0f, 1.0f), SimpleMath::Vector3(8.0f, 10.0f, 1.0f));
			ViewMove(SimpleMath::Vector3::Right);
		}


	}






	//auto up = SimpleMath::Vector3::Transform(SimpleMath::Vector3::UnitY, _pTracker->GetRotation());
	//SimpleMath::Quaternion qu = SimpleMath::Quaternion::CreateFromAxisAngle(up, _Pitch);

	//SimpleMath::Vector3 viewForward = SimpleMath::Vector3::Transform(SimpleMath::Vector3::Backward, qu);

	//_cameraTarget = viewForward * 30.0f;

	//_camera->SetUp(up);
}

void PlayerCamera::ViewMove(const SimpleMath::Vector3& moveVec)
{
	_Yaw = std::clamp(_Yaw + moveVec.x * 1.0f * Time::DeltaTime, -1.2f, 1.2f);
	_Pitch = std::clamp(_Pitch + moveVec.y * 1.0f * Time::DeltaTime, -1.2f, 1.2f);

	//auto up = SimpleMath::Vector3::Transform(SimpleMath::Vector3::UnitY, _pTracker->GetRotation());
	//SimpleMath::Quaternion qu = SimpleMath::Quaternion::CreateFromYawPitchRoll(_Yaw, _Pitch, 0);

	//SimpleMath::Vector3 viewForward = SimpleMath::Vector3::Transform(SimpleMath::Vector3::Backward, qu);


	//_camera->SetUp(up);

	SimpleMath::Quaternion qu = SimpleMath::Quaternion::CreateFromYawPitchRoll(_Yaw, _Pitch, 0.0f);

	SimpleMath::Vector3 viewForward = SimpleMath::Vector3::Transform(SimpleMath::Vector3::Backward, qu);
	SimpleMath::Vector3 up = SimpleMath::Vector3::Transform(SimpleMath::Vector3::Up, qu);

	SimpleMath::Vector3 newTarget = _camera->GetPosition() + viewForward;

	_camera->SetUp(up);
	//_camera->SetTarget(newTarget);

	_cameraTarget = viewForward * 30.0f;

}
