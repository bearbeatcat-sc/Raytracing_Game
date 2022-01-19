#pragma once

#include <Game_Object/Actor.h>

class VerticalMoveBlock;

class TitleStage
	:public Actor
{
public:
	TitleStage();
	~TitleStage() = default;
	void Delete();

private:
	void UpdateActor() override;
	void Init() override;
	void Shutdown() override;
	void OnCollsion(Actor * other) override;

private:
	const float _floorCreateTime;
	std::vector<VerticalMoveBlock*> _floors;
};
