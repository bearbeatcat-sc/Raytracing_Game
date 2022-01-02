#pragma once

#include <Game_Object/Actor.h>

#include "../TargetObjects/NineSideCube.h"
#include "../TargetObjects/MirrorCube.h"

struct ObjectGenerateInfo
{
public:
	ObjectGenerateInfo(float generateTime, const SimpleMath::Vector3& position)
		:_generateTime(generateTime), _generatePosition(position)
	{

	}

	virtual ~ObjectGenerateInfo() = default;

	bool IsGenerate(float currentTime) const
	{
		return currentTime >= _generateTime;
	}

	virtual Actor* Create( GameManager* pGameManager) = 0;


protected:
	float _generateTime;
	SimpleMath::Vector3 _generatePosition;
};

struct NineSideGenerateInfo
	:public ObjectGenerateInfo
{
	NineSideGenerateInfo(float generateTime, const SimpleMath::Vector3& position)
		: ObjectGenerateInfo(generateTime, position)
	{

	}

	Actor* Create(GameManager* pGameManager) override
	{
		auto instance = new NineSideCube(pGameManager);
		instance->SetPosition(_generatePosition);

		return instance;
	}
};

struct MirroCubeGenerateInfo
	:public ObjectGenerateInfo
{
	MirroCubeGenerateInfo(float generateTime,const SimpleMath::Vector3& position,const SimpleMath::Vector3& addScale,const SimpleMath::Vector3& maxScale)
		:ObjectGenerateInfo(generateTime,position),_addScale(addScale),_maxScale(maxScale)
	{
		
	}

	Actor* Create(GameManager* pGameManager) override
	{
		auto instance = new MirrorCube(pGameManager,_addScale,_maxScale);
		instance->SetPosition(_generatePosition);

		return instance;
	}

	SimpleMath::Vector3 _maxScale;
	SimpleMath::Vector3 _addScale;
};