#include "TunnerlCollisionObject.h"

#include <Components/Collsions/CollisionManager.h>
#include <Components/Collsions/OBBCollisionComponent.h>

TunnerlCollisionObject::TunnerlCollisionObject(Actor* user)
	:_user(user)
{
}

void TunnerlCollisionObject::UpdateActor()
{
}

void TunnerlCollisionObject::Init()
{
	m_pCollisionComponent = new OBBCollisionComponent(this, GetPosition(), m_Scale, "StaticObject");
	//m_pCollisionComponent = new SphereCollisionComponent(this, 10.0f, "Object");

	CollisionManager::GetInstance().AddComponent(m_pCollisionComponent);
	CollisionManager::GetInstance().AddRegistTree(m_pCollisionComponent);
}

void TunnerlCollisionObject::Shutdown()
{
	m_pCollisionComponent->Delete();
}

void TunnerlCollisionObject::OnCollsion(Actor* other)
{
	_user->OnCollsion(other);
}
