#include "GameManager.h"

#include <Device/DirectX/DirectXInput.h>
#include <Game_Object/ActorManager.h>

#include "TunnelManager.h"
#include "../PlayerSystem/Player.h"
#include "../TrackerObjects/Tracker.h"
#include "ScoreSystem.h"

void GameManager::AddScore(const int score)
{
	_pScoreSystem->AddScore(score);
}

Player* GameManager::GetPlayer()
{
	return _pPlayer;
}

void GameManager::UpdateActor()
{
	if (DirectXInput::GetInstance().IsKeyDown(DIK_0))
	{
		_pScoreSystem->AddScore(1111);
	}
}

void GameManager::Init()
{
	StartGame();
}

void GameManager::Shutdown()
{
}

void GameManager::OnCollsion(Actor* other)
{
}

void GameManager::StartGame()
{

	auto tonnelManager = new TunnelManager(this);
	tonnelManager->SetActorName("TonnelManager");
	tonnelManager->SetPosition(SimpleMath::Vector3::Zero);
	ActorManager::GetInstance().AddActor(tonnelManager);

	const float y = 8.0f;

	_pPlayer = new Player(SimpleMath::Vector3(0, y, 0),this);
	_pPlayer->SetScale(SimpleMath::Vector3(0.7f));
	_pPlayer->SetActorName("Player");
	ActorManager::GetInstance().AddActor(_pPlayer);

	auto tracker = new Tracker(_pPlayer,this);
	tracker->SetActorName("Tracker");
	tracker->SetPosition(SimpleMath::Vector3(0, y, 20));
	ActorManager::GetInstance().AddActor(tracker);

	_pPlayer->SetTracker(tracker);

	_pScoreSystem = new ScoreSystem();
	_pPlayer->SetChild(_pScoreSystem);
	_pScoreSystem->SetScale(SimpleMath::Vector3(1.3f));
	_pScoreSystem->SetPosition(SimpleMath::Vector3(0.0f, 1.0f, 0.0f));
	_pScoreSystem->SetRotation(SimpleMath::Vector3(0.73f, 0.0f, 0.0f));
}
