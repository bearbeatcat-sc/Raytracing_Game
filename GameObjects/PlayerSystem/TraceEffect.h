#pragma once

#include <memory>
#include <Game_Object/Actor.h>

class DXRInstance;

class TraceEffect
	:public Actor
{
public:
	TraceEffect(const std::string& meshName);
	~TraceEffect() = default;

	void UpdateActor() override;
	void Init() override;
	void Shutdown() override;
	void OnCollsion(Actor* other) override;

private:
	std::shared_ptr<DXRInstance> _instance;
	const std::string _dxrMeshName;
};
