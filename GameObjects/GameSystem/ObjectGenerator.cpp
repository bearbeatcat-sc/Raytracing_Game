#include "ObjectGenerator.h"

#include <Game_Object/ActorManager.h>
#include <Utility/LogSystem.h>
#include <Utility/Timer.h>

#include "ObjectGenerateInfo.h"

ObjectGenerator::ObjectGenerator(GameManager* pGameManager)
	:_pGameManager(pGameManager)
{
}

ObjectGenerator::~ObjectGenerator()
{
	_objectGenerateInfos.clear();
}

void ObjectGenerator::CreateSnake(float generateTime, float destroyTime, const SimpleMath::Vector3& position, int maxHp, int bodyCount,
	const SimpleMath::Vector3& velocity)
{
	AddGenerateInfo(std::make_shared<SnakeCubesGenerateInfo>(generateTime, destroyTime,position, maxHp, bodyCount, velocity));
}

void ObjectGenerator::CreateSnake(float generateTime, float destroyTime, const SimpleMath::Vector3& position, int maxHp, int bodyCount)
{
	auto createPoint = position;
	auto vec = SimpleMath::Vector3(0, 8, 0) - createPoint;;
	vec.Normalize();

	AddGenerateInfo(std::make_shared<SnakeCubesGenerateInfo>(generateTime, destroyTime,createPoint, maxHp, bodyCount, vec));
}

void ObjectGenerator::CreateBlenderMonkeyObjcet(float generateTime, float destroyTime, const SimpleMath::Vector3& position,
	const SimpleMath::Vector3& scale, int maxHP, BlenderMonkeyObject::BlenderMonkyObjectType type)
{
	AddGenerateInfo(std::make_shared<BlenderMonkeyObjectGenerateInfo>(generateTime, destroyTime, position,scale, maxHP, type));
}

void ObjectGenerator::CreateSlideTargetCube(const SimpleMath::Vector3& moveVec, float moveSpeed, float generateTime,
	float destroyTime, int maxHP, const SimpleMath::Vector3& position, const SimpleMath::Vector3& scale,
	const std::string& dxrMeshName)
{
	AddGenerateInfo(std::make_shared<SlideCubeGenerateInfo>(moveVec, moveSpeed,generateTime, destroyTime, maxHP, position, scale, dxrMeshName));
}

void ObjectGenerator::CreateJumpTargetCube(const SimpleMath::Vector3& moveVec, float moveSpeed, float generateTime,
	float destroyTime, int maxHP, const SimpleMath::Vector3& position, const SimpleMath::Vector3& scale,
	const std::string& dxrMeshName)
{
	AddGenerateInfo(std::make_shared<JumpTargetGenerateInfo>(moveVec, moveSpeed, generateTime, destroyTime, maxHP, position, scale, dxrMeshName));
}

void ObjectGenerator::CreatePointLightObject(float generateTime, float destroyTime, const SimpleMath::Vector3& position,
                                             const SimpleMath::Color& color, float distance)
{
	AddGenerateInfo(std::make_shared<PointLightGenerateInfo>(generateTime, destroyTime, position, color, distance));
}

void ObjectGenerator::CreateNineSideCube(float generateTime, float destroyTime, const SimpleMath::Vector3& position, float radius)
{
	AddGenerateInfo(std::make_shared<NineSideGenerateInfo>(generateTime, destroyTime, position,radius));
}

void ObjectGenerator::Reset()
{
	_objectGenerateInfos.clear();
	_generateTimer.reset();
}

void ObjectGenerator::OnDestroyObject(Actor* pActor)
{
	auto result = std::find(_pGenerateObjects.begin(), _pGenerateObjects.end(), pActor);

	if(result != _pGenerateObjects.end())
	{
		_pGenerateObjects.erase(result);
	}
}

void ObjectGenerator::End()
{
	for(auto object : _pGenerateObjects)
	{
		object->Destroy();
	}

	_pGenerateObjects.clear();

	LogSystem::AddLog("Clear All Objects");
}

void ObjectGenerator::AddGenerateInfo(std::shared_ptr<ObjectGenerateInfo> objectGenerateInfo)
{
	_objectGenerateInfos.push_back(objectGenerateInfo);
}

void ObjectGenerator::UpdateActor()
{
	if (_generateTimer == nullptr)
	{
		_generateTimer = std::make_shared<Timer>(9999999.0f);
	}

	_generateTimer->Update();

	for(auto itr = _objectGenerateInfos.begin(); itr != _objectGenerateInfos.end();)
	{
		if ((*itr)->IsGenerate(_generateTimer->GetTime()))
		{
			auto pObjects = (*itr)->Create(_pGameManager);

			for(auto pObject : pObjects)
			{
				ActorManager::GetInstance().AddActor(pObject);
				_pGenerateObjects.push_back(pObject);

			}

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
