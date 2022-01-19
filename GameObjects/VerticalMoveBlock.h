#pragma once

#include <Game_Object/Actor.h>
#include <SimpleMath.h>
#include <memory>

using namespace DirectX;

class DXRInstance;
class AnimationComponent;

class VerticalMoveBlock
	:public Actor
{
public:
	VerticalMoveBlock(const SimpleMath::Vector3& pos, const SimpleMath::Vector3& scale, const std::string& meshName,float angle = 0.0f);
	~VerticalMoveBlock() = default;
	void Delete();

private:
	virtual void UpdateActor() override;
	virtual void Init() override;
	virtual void Shutdown() override;
	virtual void OnCollsion(Actor* other) override;

	void Move();

private:
	std::shared_ptr<AnimationComponent> _AnimationComponent;

	std::shared_ptr<DXRInstance> _instance;

	SimpleMath::Vector3 _initScale;
	SimpleMath::Vector3 _initPosition;

	float _angle;
	float _moveSpeed;

private:

	std::string _DXRMeshName;
};
