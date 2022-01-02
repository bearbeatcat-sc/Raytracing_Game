#include "ObjectGenerator.h"

#include <Game_Object/ActorManager.h>
#include <Utility/Timer.h>

#include "ObjectGenerateInfo.h"

ObjectGenerator::ObjectGenerator(GameManager* pGameManager)
	:_pGameManager(pGameManager)
{
	_generateTimer = std::make_shared<Timer>(9999999.0f);
}

ObjectGenerator::~ObjectGenerator()
{
	_objectGenerateInfos.clear();
}

void ObjectGenerator::AddGenerateInfo(std::shared_ptr<ObjectGenerateInfo> objectGenerateInfo)
{
	_objectGenerateInfos.push_back(objectGenerateInfo);
}

void ObjectGenerator::UpdateActor()
{
	_generateTimer->Update();

	for(auto itr = _objectGenerateInfos.begin(); itr != _objectGenerateInfos.end();)
	{
		if ((*itr)->IsGenerate(_generateTimer->GetTime()))
		{
			ActorManager::GetInstance().AddActor((*itr)->Create(_pGameManager));

			itr = _objectGenerateInfos.erase(itr);
			continue;
		}

		++itr;
	}


}

void ObjectGenerator::Init()
{
}

void ObjectGenerator::Shutdown()
{

}

void ObjectGenerator::OnCollsion(Actor* other)
{
}
