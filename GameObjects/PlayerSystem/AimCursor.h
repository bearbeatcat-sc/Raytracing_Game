#pragma once

#include <Game_Object/Actor.h>

class DXRInstance;
class Cube;

class AimCursor
	:public Actor
{
public:
	AimCursor();
	~AimCursor() = default;
	const SimpleMath::Vector3& GetCursorPosition();
	void MoveCusor();

private:
	void UpdateActor() override;
	void Init() override;
	void Shutdown() override;
	void OnCollsion(Actor* other) override;

	float _Pitch;

private:
	Cube* _pCursor;
	SimpleMath::Vector2 _cursorPosition;
	std::shared_ptr<DXRInstance> _instance;
};
