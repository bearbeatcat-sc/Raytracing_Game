#include "AimCursor.h"

#include <algorithm>
#include <Device/DirectX/DirectXInput.h>
#include <Utility/Time.h>
#include <Device/Raytracing/DXRPipeLine.h>

#include "../Cube.h"

AimCursor::AimCursor()
{
	_pCursor = new Cube(SimpleMath::Vector3(0, 0, 0), SimpleMath::Vector3(0.5f), "TestUI");
	SetChild(_pCursor);
}

void AimCursor::UpdateActor()
{
}

void AimCursor::Init()
{

}

void AimCursor::Shutdown()
{
}

void AimCursor::OnCollsion(Actor* other)
{
}

const SimpleMath::Vector3& AimCursor::GetCursorPosition()
{
	return _pCursor->GetPosition();
}

void AimCursor::MoveCusor()
{
	if (DirectXInput::GetInstance().IsActiveGamePad())
	{
		if (DirectXInput::GetInstance().GetGamePadValue(GAMEPAD_ThubStick_RX) >= 0.2f)
		{
			const float input = DirectXInput::GetInstance().GetGamePadValue(GAMEPAD_ThubStick_RX);
			//_cursorPosition.x += Time::DeltaTime * input * 20.0f;

			_cursorPosition.x += (input * 0.4f);
			_cursorPosition.x = std::clamp(_cursorPosition.x, -9.0f, 9.0f);

		}

		if (DirectXInput::GetInstance().GetGamePadValue(GAMEPAD_ThubStick_RX) <= -0.2f)
		{
			const float input = DirectXInput::GetInstance().GetGamePadValue(GAMEPAD_ThubStick_RX);
			//_cursorPosition.x -= Time::DeltaTime * input * 20.0f;

			_cursorPosition.x += (input * 0.4f);
			_cursorPosition.x = std::clamp(_cursorPosition.x, -9.0f, 9.0f);
		}

		if (DirectXInput::GetInstance().GetGamePadValue(GAMEPAD_ThubStick_RY) >= 0.2f)
		{
			const float input = DirectXInput::GetInstance().GetGamePadValue(GAMEPAD_ThubStick_RY);
			//_Pitch -= Time::DeltaTime * input * 20.0f;

			//_cursorPosition.y -= Time::DeltaTime * 8.0f * (input * 2.0f);
			//_cursorPosition.y = std::clamp(_cursorPosition.y, -0.4f, 0.4f);

			_cursorPosition.y += (input * 0.3f);
			_cursorPosition.y = std::clamp(_cursorPosition.y, -9.0f, 9.0f);
		}

		if (DirectXInput::GetInstance().GetGamePadValue(GAMEPAD_ThubStick_RY) <= -0.2f)
		{
			const float input = DirectXInput::GetInstance().GetGamePadValue(GAMEPAD_ThubStick_RY);
			//_Pitch += Time::DeltaTime * input * 20.0f;

			//_cursorPosition.y += Time::DeltaTime * 8.0f * (input * 2.0f);
			//_cursorPosition.y = std::clamp(_cursorPosition.y, -0.4f, 0.4f);

			_cursorPosition.y += (input * 0.3f);
			_cursorPosition.y = std::clamp(_cursorPosition.y, -9.0f, 9.0f);
		}

	}
	else
	{

		if (DirectXInput::GetInstance().IsKey(DIK_LEFTARROW))
		{
			_cursorPosition.x -= Time::DeltaTime * 8.0f;
		}

		if (DirectXInput::GetInstance().IsKey(DIK_RIGHTARROW))
		{
			_cursorPosition.x += Time::DeltaTime * 8.0f;
		}

		if (DirectXInput::GetInstance().IsKey(DIK_UPARROW))
		{
			_cursorPosition.y += Time::DeltaTime * 8.0f;
		}

		if (DirectXInput::GetInstance().IsKey(DIK_DOWNARROW))
		{
			_cursorPosition.y -= Time::DeltaTime * 8.0f;
		}
	}


	_pCursor->SetPosition(SimpleMath::Vector3(_cursorPosition.x, _cursorPosition.y, 10));
	//SetRotation(SimpleMath::Vector3(0, _Pitch, 0));
}
