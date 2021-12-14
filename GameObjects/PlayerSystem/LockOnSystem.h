#pragma once

#include <Components/Component.h>

class LockOnArea;

class LockOnSystem
	:public Component
{
public:
	LockOnSystem(Actor* pUser);
	~LockOnSystem();
	void LockOn();

private:
	void Update() override;
	void DrawProperties() override;

	LockOnArea* _pLockOnArea;
};
