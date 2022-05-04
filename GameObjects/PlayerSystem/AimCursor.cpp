#include "AimCursor.h"

#include <algorithm>
#include <Device/DirectX/DirectXInput.h>
#include <Utility/Time.h>
#include <Device/Raytracing/DXRPipeLine.h>

#include "AimCursorObject.h"

AimCursor::AimCursor()
{
	_pAimCursorObject = new AimCursorObject();
	_pAimCursorObject->SetScale(SimpleMath::Vector3(0.5f));
	SetChild(_pAimCursorObject);
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
	return _pAimCursorObject->GetPosition();
}

void AimCursor::MoveCusor()
{
	if (DirectXInput::GetInstance().IsActiveGamePad())
	{
		if (DirectXInput::GetInstance().GetGamePadValue(GAMEPAD_ThubStick_LX) >= 0.2f)
		{
			const float input = DirectXInput::GetInstance().GetGamePadValue(GAMEPAD_ThubStick_LX);
			//_cursorPosition.x += Time::DeltaTime * input * 20.0f;

			_cursorPosition.x += (input * 0.4f);
			_cursorPosition.x = std::clamp(_cursorPosition.x, -10.0f, 10.0f);

		}

		if (DirectXInput::GetInstance().GetGamePadValue(GAMEPAD_ThubStick_LX) <= -0.2f)
		{
			const float input = DirectXInput::GetInstance().GetGamePadValue(GAMEPAD_ThubStick_LX);
			//_cursorPosition.x -= Time::DeltaTime * input * 20.0f;

			_cursorPosition.x += (input * 0.4f);
			_cursorPosition.x = std::clamp(_cursorPosition.x, -10.0f, 10.0f);
		}

		if (DirectXInput::GetInstance().GetGamePadValue(GAMEPAD_ThubStick_LY) >= 0.2f)
		{
			const float input = DirectXInput::GetInstance().GetGamePadValue(GAMEPAD_ThubStick_LY);
			//_Pitch -= Time::DeltaTime * input * 20.0f;

			//_cursorPosition.y -= Time::DeltaTime * 8.0f * (input * 2.0f);
			//_cursorPosition.y = std::clamp(_cursorPosition.y, -0.4f, 0.4f);

			_cursorPosition.y += (input * 0.3f);
			_cursorPosition.y = std::clamp(_cursorPosition.y, -5.5f, 5.5f);
		}

		if (DirectXInput::GetInstance().GetGamePadValue(GAMEPAD_ThubStick_LY) <= -0.2f)
		{
			const float input = DirectXInput::GetInstance().GetGamePadValue(GAMEPAD_ThubStick_LY);
			//_Pitch += Time::DeltaTime * input * 20.0f;

			//_cursorPosition.y += Time::DeltaTime * 8.0f * (input * 2.0f);
			//_cursorPosition.y = std::clamp(_cursorPosition.y, -0.4f, 0.4f);

			_cursorPosition.y += (input * 0.3f);
			_cursorPosition.y = std::clamp(_cursorPosition.y, -5.5f, 5.5f);
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


	_pAimCursorObject->SetPosition(SimpleMath::Vector3(_cursorPosition.x, _cursorPosition.y, 10));
	//SetRotation(SimpleMath::Vector3(0, _Pitch, 0));
}
