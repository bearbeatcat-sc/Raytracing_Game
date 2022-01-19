#pragma once

#include <Game_Object/Actor.h>
#include <Device/Raytracing/DXRInstance.h>
#include <Utility/Timer.h>

#include "../GameSystem/GameManager.h"

class GameManager;

class TargetObject
	:public Actor
{
public:
	TargetObject(GameManager* pGameManager,float destroyTime)
		:_pGameManager(pGameManager), _isDelete(false), _findUI(nullptr)
	{
		_deleteTimer = std::make_shared<Timer>(destroyTime);
	}

	bool IsDelete()
	{
		return _isDelete;
	}

	virtual ~TargetObject() {}

	const int GetInstanceID()
	{
		if (_instance == nullptr) return -1;

		return _instance->_instnaceID;
	}

	void DestoryOrder()
	{
		_pGameManager->DestroyObject(this);
		this->Destroy();
	}

	void Active()
	{
		_isActive = true;
		ActiveAction();
	}

	bool IsActive()
	{
		return _isPreviousActive && _isActive;
	}

	bool IsOnActive()
	{
		return !_isPreviousActive && _isActive;
	}

	bool IsExitActive()
	{
		return _isPreviousActive && !_isActive;
	}

	void ActiveUpdate()
	{
		_isPreviousActive = _isActive;
		_isActive = false;
	}

private:
	virtual void UpdateActor() = 0;
	virtual void Init() = 0;
	virtual void Shutdown() = 0;
	virtual void OnCollsion(Actor* other) = 0;
	virtual void ActiveAction() = 0;

protected:
	std::shared_ptr<DXRInstance> _instance;
	GameManager* _pGameManager;
	std::shared_ptr<Timer> _deleteTimer;

	Actor* _findUI;

	bool _isDelete;

	bool _isActive;
	bool _isPreviousActive;
};
