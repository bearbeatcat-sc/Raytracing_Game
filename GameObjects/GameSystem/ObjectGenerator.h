#pragma once
#include <memory>
#include <Game_Object/Actor.h>

struct ObjectGenerateInfo;
class Timer;
class GameManager;

class ObjectGenerator
	:public Actor
{
public:
	ObjectGenerator(GameManager* pGameManager);
	~ObjectGenerator();
	void AddGenerateInfo(std::shared_ptr<ObjectGenerateInfo> objectGenerateInfo);

private:
	void UpdateActor() override;
	void Init() override;
	void Shutdown() override;
	void OnCollsion(Actor* other) override;


private:
	std::shared_ptr<Timer> _generateTimer;

	std::vector<std::shared_ptr<ObjectGenerateInfo>> _objectGenerateInfos;

	GameManager* _pGameManager;
};
