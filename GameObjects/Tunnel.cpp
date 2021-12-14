#include "Tunnel.h"

#include <algorithm>
#include <Game_Object/ActorManager.h>
#include <Utility/Time.h>

#include "Cube.h"
#include "Tracker.h"
#include "TunnerlCollisionObject.h"

Tunnel::Tunnel(const SimpleMath::Vector3& pos, const SimpleMath::Vector3& scale)
	:_tunnelScale(scale), _isCurrentEnter(false), _isPreviousEnter(false),_pointIndex(0), _fistEnter(false)
{
	SetPosition(pos + SimpleMath::Vector3(0,0, scale.z));
}

void Tunnel::SetCollisionEvent(std::function<void(Actor* actor)> collision_event)
{
	_pCollisionEvent = collision_event;
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
	
	const int grid_size_x = 8;
	const int grid_size_z = 7;

	const int grid_Count = grid_size_x * grid_size_z;
	
	auto floor = new Cube(SimpleMath::Vector3(0, 0, 0), _tunnelScale, 300.0f, "RoughCube", false);
	floor->SetRotation(SimpleMath::Vector3(0, 0.0f, 0.0f));
	floor->SetActorName("Floor");
	floor->OnStatic(false, false);
	SetChild(floor);

	auto leftDownWall = new Cube(SimpleMath::Vector3(-16.0f, 9.0f, 0), _tunnelScale, 300.0f, "GrayCube", false);
	leftDownWall->SetRotation(SimpleMath::Vector3(0, 0.0f, -0.940f));
	leftDownWall->SetActorName("LeftDown_Wall");
	leftDownWall->SetTag("Wall");
	leftDownWall->OnStatic(false, false);
	SetChild(leftDownWall);

	auto rightDownWall = new Cube(SimpleMath::Vector3(16.0f, 9.0f, 0), _tunnelScale, 300.0f, "GrayCube", false);
	rightDownWall->SetRotation(SimpleMath::Vector3(0, 0.0f, -2.200f));
	rightDownWall->SetActorName("RightDown_Wall");
	rightDownWall->SetTag("Wall");
	rightDownWall->OnStatic(false, false);
	SetChild(rightDownWall);
	
	auto leftUPWall = new Cube(SimpleMath::Vector3(-10.0f, 24.0f, 0.0f), _tunnelScale, 300.0f, "GrayCube", false);
	leftUPWall->SetRotation(SimpleMath::Vector3(0, 0.0f, -2.488f));
	leftUPWall->SetActorName("LeftUpWall");
	leftUPWall->SetTag("Wall");
	leftUPWall->OnStatic(false, false);
	SetChild(leftUPWall);

	auto rightUpWall = new Cube(SimpleMath::Vector3(12.0f, 24.0f, 0.0f), _tunnelScale, 300.0f, "GrayCube", false);
	rightUpWall->SetRotation(SimpleMath::Vector3(0, 0.0f, 2.470f));
	rightUpWall->SetActorName("RightUpWall");
	rightUpWall->SetTag("Wall");
	rightUpWall->OnStatic(false, false);
	SetChild(rightUpWall);

	_pTunnerlCollsionObject->SetPosition(floor->GetPosition() + SimpleMath::Vector3(0,8,0));
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
		_pCollisionEvent(this);
		_pCollisionEvent = nullptr;
		
	}

}

