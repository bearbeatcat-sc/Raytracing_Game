#pragma once

#include <Game_Object/Actor.h>
#include <Utility/Camera.h>

class PlayerCamera
	:public Actor
{
public:
	PlayerCamera(Actor* pTracker);
	~PlayerCamera() = default;
	void Shake(const float shakeX,const float shakeY);
	const SimpleMath::Vector3 GetForward();
	const float GetPitch();

	std::shared_ptr<Camera> _camera;

private:
	void UpdateActor() override;
	void ShakeUpdate();
	void Init() override;
	void Shutdown() override;
	void OnCollsion(Actor* other) override;
	void CameraUpdate();
	void ViewMove(const SimpleMath::Vector3& moveVec);

private:

	float _cameraTargetSpeed = 6.2f;
	float _cameraPositionSpeed = 3.0f;

	Actor* _pTracker;

	float _Pitch;
	float _Yaw;


	SimpleMath::Vector3 _cameraTarget;

	SimpleMath::Vector2 _shake;
	float _shakePower;

	std::shared_ptr<Timer> _shakeTimer;
};
