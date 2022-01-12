#pragma once

#include <Game_Object/Actor.h>

#include "../TargetObjects/NineSideCube.h"
#include "../TargetObjects/SnakeCube.h"
#include "../TargetObjects/BlenderMonkeyObject.h"

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

	virtual std::vector<Actor*> Create(GameManager* pGameManager) = 0;


protected:
	float _generateTime;
	SimpleMath::Vector3 _generatePosition;
};

struct NineSideGenerateInfo
	:public ObjectGenerateInfo
{
	NineSideGenerateInfo(float generateTime, const SimpleMath::Vector3& position, float radius)
		: ObjectGenerateInfo(generateTime, position), _radius(radius)
	{

	}

	std::vector<Actor*> Create(GameManager* pGameManager) override
	{
		auto instance = new NineSideCube(pGameManager, _radius);
		instance->SetPosition(_generatePosition);

		return std::vector<Actor*>{instance};
	}

	float _radius;
};

struct BlenderMonkeyObjectGenerateInfo
	:public ObjectGenerateInfo
{
	BlenderMonkeyObjectGenerateInfo(float generateTime, const SimpleMath::Vector3& position, const SimpleMath::Vector3& scale, int maxHp,BlenderMonkeyObject::BlenderMonkyObjectType type)
		:ObjectGenerateInfo(generateTime,position),_type(type),_maxHP(maxHp),_scale(scale)
	{
		
	}

	std::vector<Actor*> Create(GameManager* pGameManager) override
	{
		auto instance = new BlenderMonkeyObject(_maxHP, _type, pGameManager);
		instance->SetPosition(_generatePosition);
		instance->SetScale(_scale);

		return std::vector<Actor*>{instance};
	}

	BlenderMonkeyObject::BlenderMonkyObjectType _type;
	int _maxHP;
	SimpleMath::Vector3 _scale;
};

struct SnakeCubesGenerateInfo
	:public ObjectGenerateInfo
{
	SnakeCubesGenerateInfo(float generateTime, const SimpleMath::Vector3& position,
		int maxHp, int bodyCount, const SimpleMath::Vector3& velocity)
		:ObjectGenerateInfo(generateTime, position), _maxHP(maxHp), _bodyCount(bodyCount), _veloicty(velocity)
	{

	}

	std::vector<Actor*> Create(GameManager* pGameManager) override
	{
		std::vector<Actor*> _acts;

		auto head = new SnakeCube(_maxHP, _veloicty, "RedCube", pGameManager);
		head->SetScale(SimpleMath::Vector3::One * 1.2f);
		head->SetPosition(_generatePosition);
		_acts.push_back(head);

		std::vector<SnakeCube*> bodys;

		for (int i = 0; i < _bodyCount; ++i)
		{
			auto body = new SnakeCube(_maxHP, _veloicty, "RedCube", pGameManager);
			body->SetScale(SimpleMath::Vector3::One);
			body->SetPosition(_generatePosition - (_veloicty * SimpleMath::Vector3(1.0f)));
			bodys.push_back(body);
			_acts.push_back(body);
		}

		if (bodys.size() <= 0) 		return _acts;

		bodys[0]->SetTarget(head);

		for (int i = bodys.size() - 1; i > 0; --i)
		{
			SnakeCube* back = bodys[i];
			SnakeCube* first = bodys[i - 1];

			back->SetTarget(first);
		}

		return _acts;
	}

	int _maxHP;
	int _bodyCount;
	SimpleMath::Vector3 _veloicty;
};

