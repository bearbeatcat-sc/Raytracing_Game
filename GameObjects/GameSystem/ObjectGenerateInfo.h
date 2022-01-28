#pragma once

#include <Game_Object/Actor.h>
#include <Utility/Random.h>

#include "../TargetObjects/NineSideCube.h"
#include "../TargetObjects/SnakeCube.h"
#include "../TargetObjects/JumpTargetCube.h"
#include "../TargetObjects/BlenderMonkeyObject.h"
#include "../TargetObjects/SlideTargetCube.h"
#include "../PointLightObject.h"

struct ObjectGenerateInfo
{
public:
	ObjectGenerateInfo(float generateTime, float destroyTime, const SimpleMath::Vector3& position)
		:_generateTime(generateTime), _generatePosition(position), _destroyTime(destroyTime)
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
	float _destroyTime;
	SimpleMath::Vector3 _generatePosition;
};

struct SlideCubeGenerateInfo
	:public ObjectGenerateInfo
{
public:
	SlideCubeGenerateInfo(const SimpleMath::Vector3& moveVec, float moveSpeed, float generateTime, float destroyTime, int hp, const SimpleMath::Vector3& position, const SimpleMath::Vector3& scale, const std::string& dxrMeshName)
		:ObjectGenerateInfo(generateTime, destroyTime, position), _moveVec(moveVec),_moveSpeed(moveSpeed),_dxrMeshName(dxrMeshName), _maxHP(hp), _scale(scale)
	{

	}

	std::vector<Actor*> Create(GameManager* pGameManager) override
	{
		auto instance = new SlideTargetCube(_moveVec,_moveSpeed,_maxHP, _destroyTime, _dxrMeshName, pGameManager);
		instance->SetPosition(_generatePosition);
		instance->SetScale(_scale);

		float rotate_x = Random::GetRandom(-1.0f, 1.0f);
		float rotate_y = Random::GetRandom(-1.0f, 1.0f);

		instance->SetRotation(SimpleMath::Vector3(rotate_x, rotate_y, 0));

		return std::vector<Actor*>{instance};
	}

	std::string _dxrMeshName;
	int _maxHP;
	SimpleMath::Vector3 _scale;
	SimpleMath::Vector3 _moveVec;
	float _moveSpeed;
};

struct JumpTargetGenerateInfo
	:public ObjectGenerateInfo
{
public:
	JumpTargetGenerateInfo(const SimpleMath::Vector3& moveVec, float moveSpeed, float generateTime, float destroyTime, int hp, const SimpleMath::Vector3& position, const SimpleMath::Vector3& scale, const std::string& dxrMeshName)
		:ObjectGenerateInfo(generateTime, destroyTime, position), _moveVec(moveVec), _moveSpeed(moveSpeed), _dxrMeshName(dxrMeshName), _maxHP(hp), _scale(scale)
	{

	}

	std::vector<Actor*> Create(GameManager* pGameManager) override
	{
		auto instance = new JumpTargetCube(_moveVec, _moveSpeed, _maxHP, _destroyTime, _dxrMeshName, pGameManager);
		instance->SetPosition(_generatePosition);
		instance->SetScale(_scale);

		float rotate_x = Random::GetRandom(-1.0f, 1.0f);
		float rotate_y = Random::GetRandom(-1.0f, 1.0f);

		instance->SetRotation(SimpleMath::Vector3(rotate_x, rotate_y, 0));

		return std::vector<Actor*>{instance};
	}

	std::string _dxrMeshName;
	int _maxHP;
	SimpleMath::Vector3 _scale;
	SimpleMath::Vector3 _moveVec;
	float _moveSpeed;
};

struct PointLightGenerateInfo
	:public ObjectGenerateInfo
{
public:
	PointLightGenerateInfo(float generateTime, float destroyTime, const SimpleMath::Vector3& position, const SimpleMath::Color& color, float distance)
		:ObjectGenerateInfo(generateTime, destroyTime, position), _color(color),_distance(distance)
	{

	}

	std::vector<Actor*> Create(GameManager* pGameManager) override
	{
		auto instance = new PointLightObject(_generatePosition, _color, _distance);
		instance->Destroy(_destroyTime);

		return std::vector<Actor*>{instance};
	}

	float _distance;
	SimpleMath::Color _color;
};

struct NineSideGenerateInfo
	:public ObjectGenerateInfo
{
	NineSideGenerateInfo(float generateTime, float destroyTime, const SimpleMath::Vector3& position, float radius)
		: ObjectGenerateInfo(generateTime, destroyTime, position), _radius(radius)
	{

	}

	std::vector<Actor*> Create(GameManager* pGameManager) override
	{
		auto instance = new NineSideCube(pGameManager, _radius, _destroyTime);
		instance->SetPosition(_generatePosition);

		return std::vector<Actor*>{instance};
	}

	float _radius;
};

struct BlenderMonkeyObjectGenerateInfo
	:public ObjectGenerateInfo
{
	BlenderMonkeyObjectGenerateInfo(float generateTime, float destroyTime, const SimpleMath::Vector3& position, const SimpleMath::Vector3& scale, int maxHp, BlenderMonkeyObject::BlenderMonkyObjectType type)
		:ObjectGenerateInfo(generateTime, destroyTime, position), _type(type), _maxHP(maxHp), _scale(scale)
	{

	}

	std::vector<Actor*> Create(GameManager* pGameManager) override
	{
		auto instance = new BlenderMonkeyObject(_maxHP, _destroyTime, _type, pGameManager);
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
	SnakeCubesGenerateInfo(float generateTime, float destroyTime, const SimpleMath::Vector3& position,
		int maxHp, int bodyCount, const SimpleMath::Vector3& velocity)
		:ObjectGenerateInfo(generateTime, destroyTime, position), _maxHP(maxHp), _bodyCount(bodyCount), _veloicty(velocity)
	{

	}

	std::vector<Actor*> Create(GameManager* pGameManager) override
	{
		std::vector<Actor*> _acts;

		auto head = new SnakeCube(_maxHP, _destroyTime, _veloicty, "RedCube", pGameManager);
		head->SetScale(SimpleMath::Vector3::One * 1.2f);
		head->SetPosition(_generatePosition);
		_acts.push_back(head);

		std::vector<SnakeCube*> bodys;

		for (int i = 0; i < _bodyCount; ++i)
		{
			auto body = new SnakeCube(_maxHP, _destroyTime, _veloicty, "RedCube", pGameManager);
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

