#include "AimCursor.h"

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
	if (DirectXInput::GetInstance().IsKey(DIK_A))
	{
		_cursorPosition.x -= Time::DeltaTime * 6.0f;
	}

	if (DirectXInput::GetInstance().IsKey(DIK_D))
	{
		_cursorPosition.x += Time::DeltaTime * 6.0f;
	}

	if (DirectXInput::GetInstance().IsKey(DIK_W))
	{
		_Pitch -= Time::DeltaTime * 1.0f;
	}

	if (DirectXInput::GetInstance().IsKey(DIK_S))
	{
		_Pitch += Time::DeltaTime * 1.0f;
	}

	_pCursor->SetPosition(SimpleMath::Vector3(_cursorPosition.x, _cursorPosition.y,10));
	SetRotation(SimpleMath::Vector3(0, _Pitch, 0));
}
