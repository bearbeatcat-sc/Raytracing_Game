#pragma once

#include <Game_Object/Actor.h>
#include <SimpleMath.h>
#include <memory>

class CollisionComponent;
class TunnerlCollisionObject;
class Timer;

using namespace DirectX;
class Tunnel
	:public Actor
{
public:
	Tunnel(const SimpleMath::Vector3& pos,const SimpleMath::Vector3& scale);
	~Tunnel() = default;
	void SetCollisionEvent(std::function<void(Actor* actor)> );
	bool IsExit();
	void AddPoint(const SimpleMath::Vector3& point);

private:
	void UpdateActor() override;
	void Init() override;
	void Shutdown() override;
	void OnCollsion(Actor* other) override;


private:
	std::vector<SimpleMath::Vector3> _Points;
	int _pointIndex;
	bool _fistEnter;
	
	SimpleMath::Vector3 _tunnelScale;
	
	bool _isCurrentEnter;
	bool _isPreviousEnter;

	TunnerlCollisionObject* _pTunnerlCollsionObject;
	std::function<void(Actor* actor)> _pCollisionEvent = nullptr;
};
