#pragma once

#include <Game_Object/Actor.h>

class VerticalMoveBlock;

class PlayStage
	:public Actor
{
public:
	PlayStage();
	~PlayStage() = default;
	void Delete();

private:
	void UpdateActor() override;
	void Init() override;
	void Shutdown() override;
	void OnCollsion(Actor* other) override;

private:
	const float _floorCreateTime;
	std::vector<VerticalMoveBlock*> _floors;
};
