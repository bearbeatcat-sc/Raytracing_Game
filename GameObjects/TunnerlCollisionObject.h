#pragma once

#include <Game_Object/Actor.h>

class CollisionComponent;

class TunnerlCollisionObject
	:public Actor
{
public:
	TunnerlCollisionObject(Actor* user);
	~TunnerlCollisionObject() = default;

private:
	void UpdateActor() override;
	void Init() override;
	void Shutdown() override;
	void OnCollsion(Actor* other) override;

private:
	CollisionComponent* m_pCollisionComponent;
	Actor* _user;
};
