#include "Tunnel.h"

#include <algorithm>
#include <stdexcept>
#include <Game_Object/ActorManager.h>
#include <Utility/Time.h>

#include "TunnerlCollisionObject.h"
#include "../Tracker.h"

Tunnel::Tunnel(const SimpleMath::Vector3& scale,GameManager* pGameManager)
	:_tunnelScale(scale), _isCurrentEnter(false), _isPreviousEnter(false),_pointIndex(0), _fistEnter(false),
_pGameManager(pGameManager)
{

}

void Tunnel::SetCollisionEvent(std::function<void(Actor* actor, GameManager* pGameManager)> collision_event)
{
	_pCollisionEvent = collision_event;
}

void Tunnel::SetCreateEvent(std::function<void(Actor* actor,GameManager* pGameManager)> create_event)
{
	_pCreateEvent = create_event;
}


bool Tunnel::IsExit()
{
	if (_isCurrentEnter == false && _isPreviousEnter == true)
	{
		return true;
	}


	return false;
}

void Tunnel::AddPoint(const SimpleMath::Vector3& point)
{
	_Points.push_back(point);
}

void Tunnel::UpdateActor()
{
	_isPreviousEnter = _isCurrentEnter;

	_isCurrentEnter = false;
}

void Tunnel::Init()
{
	_pTunnerlCollsionObject = new TunnerlCollisionObject(this);
	ActorManager::GetInstance().AddActor(_pTunnerlCollsionObject);

	if(_pCreateEvent == nullptr)
	{
		throw std::runtime_error("生成イベントが設定されていません。");
	}

	_pCreateEvent(this,_pGameManager);
	


	_pTunnerlCollsionObject->SetPosition(GetPosition() + SimpleMath::Vector3(0,8,0));
	_pTunnerlCollsionObject->SetScale(_tunnelScale * 1.2f);
	_pTunnerlCollsionObject->SetActorName("TunnerlCollisionObject");
}

void Tunnel::Shutdown()
{
	_pTunnerlCollsionObject->Destroy();
}

void Tunnel::OnCollsion(Actor* other)
{
	if(other->IsContainsTag("Tracker"))
	{

		if (_fistEnter)
		{
			return;
		}

		_fistEnter = true;

		static_cast<Tracker*>(other)->AddPoint(_Points);
	}

	if(other->IsContainsTag("Player"))
	{

		_isCurrentEnter = true;

		if(_pCollisionEvent == nullptr)
		{			
			return;
		}

		// 一度のみ
		_pCollisionEvent(this,_pGameManager);
		_pCollisionEvent = nullptr;
		
	}

}

