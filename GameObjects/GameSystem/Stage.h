﻿#pragma once

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


private:
	const float _floorCreateTime;


	std::vector<Actor*> _floors;
};