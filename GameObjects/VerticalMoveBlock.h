#pragma once

#include <Game_Object/Actor.h>
#include <SimpleMath.h>
#include <memory>

using namespace DirectX;

class DXRInstance;

class VerticalMoveBlock
	:public Actor
{
public:
	VerticalMoveBlock(const SimpleMath::Vector3& pos, const SimpleMath::Vector3& scale, const std::string& meshName,float angle = 0.0f);
	~VerticalMoveBlock() = default;

private:
	virtual void UpdateActor() override;
	virtual void Init() override;
	virtual void Shutdown() override;
	virtual void OnCollsion(Actor* other) override;

	void Generate();
	void Move();

private:
	std::shared_ptr<DXRInstance> _instance;
	std::shared_ptr<Timer> _GenerateTimer;
	SimpleMath::Vector3 _initScale;
	SimpleMath::Vector3 _initPosition;
	bool _IsGenerate;

	float _angle;
	float _moveSpeed;

private:

	std::string _DXRMeshName;
};
