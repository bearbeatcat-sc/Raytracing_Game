#pragma once

#include <Game_Object/Actor.h>

class Stage
	:public Actor
{
public:
	Stage();
	~Stage() = default;

private:
	void UpdateActor() override;
	void Init() override;
	void Shutdown() override;
	void OnCollsion(Actor* other) override;
	void DeleteFloor();

	void CreateFloor();

private:
	const float _floorCreateTime;
	const int _maxCreateFloorCount = 2400;
	std::shared_ptr<Timer> _floorCreateTimer;
	int _createFloorCount;

	std::vector<Actor*> _floors;
};
