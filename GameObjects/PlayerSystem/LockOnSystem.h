#pragma once

#include <map>
#include <memory>
#include <vector>
#include <Components/Component.h>

#include "LockOnArea.h"

class BombBullet;
class SpriteComponent;
class Timer;
class TargetObject;
class BombInterFace;


class LockOnSystem
	:public Component
{
public:
	LockOnSystem(Actor* pUser);
	~LockOnSystem();
	void LockOn();

	struct AttackTargetInfo
	{
		TargetObject* _target = nullptr;
		int _lockOnCount = 0;

		Actor* _lockOnUI0 = nullptr;
		Actor* _lockOnUI1 = nullptr;
	};

private:
	bool IsGenerateBullet();
	void Attack();
	void UpdateBombUIs();
	void Update() override;
	void Explosion();
	void DrawProperties() override;

	void GenerateBullet();
	int FindIndex();
	void AddBullet(BombBullet* pBombBullet);

private:

	std::map<int,BombBullet*> _generatedBombBullets;
	BombInterFace* _pBombInterFace;

	int _generateBombBulletCount;

	const float m_AimSpeed;
	const size_t _MaxGenerateBulletCount = 3;
};
