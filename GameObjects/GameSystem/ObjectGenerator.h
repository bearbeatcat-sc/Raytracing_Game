#pragma once
#include <memory>
#include <Game_Object/Actor.h>

#include "../TargetObjects/BlenderMonkeyObject.h"

struct ObjectGenerateInfo;
class Timer;
class GameManager;

class ObjectGenerator
	:public Actor
{
public:
	ObjectGenerator(GameManager* pGameManager);
	~ObjectGenerator();
	void CreateSnake(float generateTime,float destroyTime, const SimpleMath::Vector3& position,
		int maxHp, int bodyCount, const SimpleMath::Vector3& velocity);
	void CreateSnake(float generateTime, float destroyTime, const SimpleMath::Vector3& position,
		int maxHp, int bodyCount);
	void CreateBlenderMonkeyObjcet(float generateTime, float destroyTime, const SimpleMath::Vector3& position,
		const SimpleMath::Vector3& scale,int maxHP, BlenderMonkeyObject::BlenderMonkyObjectType type);
	void CreateTargetCube(float generateTime, float destroyTime, int maxHP, const SimpleMath::Vector3& position,
		const SimpleMath::Vector3& scale,const std::string& dxrMeshName);
	void CreatePointLightObject(float generateTime, float destroyTime, const SimpleMath::Vector3& position,
	                            const SimpleMath::Color& color, float distance);

	void CreateNineSideCube(float generateTime, float destroyTime, const SimpleMath::Vector3& position,float radius);
	void Reset();
	void OnDestroyObject(Actor* pActor);
	void End();

private:
	void UpdateActor() override;
	void Init() override;
	void Shutdown() override;
	void OnCollsion(Actor* other) override;
	void AddGenerateInfo(std::shared_ptr<ObjectGenerateInfo> objectGenerateInfo);

private:
	std::shared_ptr<Timer> _generateTimer;
	std::vector<std::shared_ptr<ObjectGenerateInfo>> _objectGenerateInfos;
	GameManager* _pGameManager;

	std::vector<Actor*> _pGenerateObjects;
};
