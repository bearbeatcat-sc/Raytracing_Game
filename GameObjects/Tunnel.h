#pragma once

#include <Game_Object/Actor.h>
#include <SimpleMath.h>
#include <memory>

using namespace DirectX;
class Tunnel
	:public Actor
{
public:
	Tunnel(const SimpleMath::Vector3& pos,const SimpleMath::Vector3& scale);
	~Tunnel() = default;

private:
	void UpdateActor() override;
	void Init() override;
	void Shutdown() override;
	void OnCollsion(Actor* other) override;

private:
	SimpleMath::Vector3 _tunnelScale;
};
