#include "PlayerTimeline.h"

#include <Utility/Timer.h>

#include "../GameSystem/GameManager.h"

PlayerTimeline::PlayerTimeline(GameManager* pGameManager)
	:_pGameManager(pGameManager)
{

}

void PlayerTimeline::AddTask(float generateTime, Player::PlayerState playerState)
{
	_playerTasks.push_back(std::make_shared<PlayerTask>(generateTime, playerState));
}

void PlayerTimeline::Reset()
{
	_playerTasks.clear();
	_generateTimer->Reset();

	_pPlayer = _pGameManager->GetPlayer();
}

void PlayerTimeline::UpdateActor()
{
	if (_generateTimer == nullptr)
	{
		_generateTimer = std::make_shared<Timer>(9999999.0f);
	}

	_generateTimer->Update();

	for (auto itr = _playerTasks.begin(); itr != _playerTasks.end();)
	{
		if ((*itr)->IsActive(_generateTimer->GetTime()))
		{
			_pPlayer->SetPlayerState((*itr)->_changeState);

			itr = _playerTasks.erase(itr);
			continue;
		}

		++itr;
	}

}

void PlayerTimeline::Init()
{
}

void PlayerTimeline::Shutdown()
{
	_playerTasks.clear();
}

void PlayerTimeline::OnCollsion(Actor* other)
{
}
