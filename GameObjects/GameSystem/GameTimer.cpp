#include "GameTimer.h"

#include <algorithm>

#include "ScoreObject.h"

#include <iomanip>
#include <sstream>
#include <Utility/Timer.h>

GameTimer::GameTimer(int endTime)
	:_endTime(endTime)
{
	_gameTimer = std::make_shared<Timer>(_endTime);
}

bool GameTimer::IsEnd()
{
	return _gameTimer->IsTime();
}

void GameTimer::UpdateTimeObject()
{
	for (int i = 0; i < _stringTime.size(); ++i)
	{
		const int number = (_stringTime[i] - '0');

		if (_numberObjects[i]._num != number)
		{
			if (_numberObjects[i]._object != nullptr)
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

void GameTimer::Reset()
{
	_gameTimer->Reset();
}

void GameTimer::UpdateActor()
{
	_gameTimer->Update();

	const int currentTime = floor(_gameTimer->GetTime());
	int elapsedTime = _endTime - currentTime;
	elapsedTime = std::clamp(elapsedTime, 0, (int)_endTime);

	if (elapsedTime != _previousTime)
	{
		std::ostringstream sout;
		sout << std::setfill('0') << std::setw(3) << elapsedTime;

		_stringTime = sout.str();
		UpdateTimeObject();
	}

	_previousTime = elapsedTime;
}

void GameTimer::Init()
{
	_numberObjects.clear();
	_numberObjects.resize(3);

	_previousTime = 0;
	int currentTime = _endTime - floor(_gameTimer->GetTime());
	currentTime = std::clamp(currentTime, 0, (int)_endTime);

	std::ostringstream sout;
	sout << std::setfill('0') << std::setw(3) << currentTime;

	_stringTime = sout.str();
	UpdateTimeObject();
}

void GameTimer::Shutdown()
{
	_numberObjects.clear();
}

void GameTimer::OnCollsion(Actor* other)
{
}
