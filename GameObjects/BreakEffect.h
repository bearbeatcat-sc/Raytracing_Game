#pragma once

#include <memory>
#include <Game_Object/Actor.h>

class DXRInstance;

class BreakEffect
	:public Actor
{
public:
	BreakEffect(const SimpleMath::Vector3& velocity, const std::string& meshName);
	~BreakEffect() = default;

	void UpdateActor() override;
	void Init() override;
	void Shutdown() override;
	void OnCollsion(Actor* other) override;

private:
	std::shared_ptr<DXRInstance> _instance;
	const std::string _dxrMeshName;

	SimpleMath::Vector3 _velocity;
};
