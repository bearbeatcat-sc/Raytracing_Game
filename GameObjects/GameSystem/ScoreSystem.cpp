#include "ScoreSystem.h"

#include <algorithm>
#include <iomanip>
#include <sstream>
#include <Game_Object/ActorManager.h>
#include <Utility/Time.h>
#include <Utility/Timer.h>

#include "ScoreObject.h"
#include "../PlayerSystem/Player.h"

ScoreSystem::ScoreSystem(const SimpleMath::Vector3& generatePos)
	:_isEnd(false),_generatePosition(generatePos)
{
	_pMoveResultTimer = std::make_shared<Timer>(0.1f);
	SetPosition(generatePos);
}

void ScoreSystem::AddScore(int score)
{
	_score = std::clamp(_score + score, 0, 100000);

	std::ostringstream sout;

	sout << std::setfill('0') << std::setw(5) << _score;

	_stringScore = sout.str();

	UpdateScoreObject();
}

void ScoreSystem::ChangeResultMode()
{
	_isEnd = true;
}

void ScoreSystem::UpdateActor()
{
	if (!_isEnd)
		return;;

	if(!_pMoveResultTimer->IsTime())
	{
		_pMoveResultTimer->Update();
	}

	auto pos = SimpleMath::Vector3::Lerp(_generatePosition, _resultPosition, _pMoveResultTimer->GetRatio());
	SetPosition(pos);

	auto scale = SimpleMath::Vector3::Lerp(GetScale(), SimpleMath::Vector3(2.0f), _pMoveResultTimer->GetRatio());
	SetScale(scale);

	auto rotate = SimpleMath::Vector3::Lerp(GetEulerRotation(), SimpleMath::Vector3::Zero, _pMoveResultTimer->GetRatio());
	SetRotation(rotate);
}

void ScoreSystem::Init()
{
	_numberObjects.clear();
	_numberObjects.resize(5);

	_score = 0;

	std::ostringstream sout;

	sout << std::setfill('0') << std::setw(5) << _score;

	_stringScore = sout.str();
	UpdateScoreObject();

	_resultPosition = SimpleMath::Vector3(-2, -1, 6);
}

void ScoreSystem::Shutdown()
{
	_numberObjects.clear();
}

void ScoreSystem::OnCollsion(Actor* other)
{
}

void ScoreSystem::UpdateScoreObject()
{
	for(int i = 0; i < _stringScore.size(); ++i)
	{
		int number = (_stringScore[i] - '0');

		if(_numberObjects[i]._num != number)
		{
			if(_numberObjects[i]._object != nullptr)
			{
				_numberObjects[i]._object->Destroy();
			}
			
			const auto size = SimpleMath::Vector3(0.25f);

			auto object = new ScoreObject(number);
			object->SetScale(size);
			object->SetPosition(SimpleMath::Vector3(0.5f * i, 0.0f, 2.0f));

			_numberObjects[i]._object = object;
			_numberObjects[i]._num = number;

			SetChild(object);
		}

		

	}
}
