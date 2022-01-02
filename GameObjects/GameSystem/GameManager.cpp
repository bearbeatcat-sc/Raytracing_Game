#include "GameManager.h"

#include <Device/DirectX/DirectXInput.h>
#include <Game_Object/ActorManager.h>

#include "ObjectGenerateInfo.h"
#include "TunnelManager.h"
#include "../PlayerSystem/Player.h"
#include "../TargetObjects/SnakeCube.h"
#include "ScoreSystem.h"
#include "ObjectGenerator.h"

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

void GameManager::CreateStage()
{
	//_pObjectGenerator->AddGenerateInfo(std::make_shared<NineSideGenerateInfo>(3.0f, SimpleMath::Vector3(0, 10, 20)));
	//_pObjectGenerator->AddGenerateInfo(std::make_shared<NineSideGenerateInfo>(6.0f, SimpleMath::Vector3(0, 10, 20)));
	//_pObjectGenerator->AddGenerateInfo(std::make_shared<NineSideGenerateInfo>(9.0f, SimpleMath::Vector3(0, 10, 20)));

	_pObjectGenerator->AddGenerateInfo(std::make_shared<MirroCubeGenerateInfo>(1.0f, SimpleMath::Vector3(0, 8.0f, 300), SimpleMath::Vector3(10.0f, 10.0f, 0.0f), SimpleMath::Vector3(20, 20, 1.0f)));
}

void GameManager::StartGame()
{

	auto tonnelManager = new TunnelManager(this);
	tonnelManager->SetActorName("TonnelManager");
	tonnelManager->SetPosition(SimpleMath::Vector3::Zero);
	ActorManager::GetInstance().AddActor(tonnelManager);

	_pObjectGenerator = new ObjectGenerator(this);
	ActorManager::GetInstance().AddActor(_pObjectGenerator);

	CreateStage();

	const float y = 8.0f;

	_pPlayer = new Player(SimpleMath::Vector3(0, y, 20),this);
	_pPlayer->SetScale(SimpleMath::Vector3(0.7f));
	_pPlayer->SetActorName("Player");
	ActorManager::GetInstance().AddActor(_pPlayer);

	auto snakeHead = new SnakeCube(3, SimpleMath::Vector3::Backward, "RedCube", this);
	snakeHead->SetScale(SimpleMath::Vector3::One);

	auto snakeBody0= new SnakeCube(3, SimpleMath::Vector3::Backward, "RedCube", this);
	snakeBody0->SetScale(SimpleMath::Vector3::One);
	snakeBody0->SetTarget(snakeHead);

	auto snakeBody1 = new SnakeCube(3, SimpleMath::Vector3::Backward, "RedCube", this);
	snakeBody1->SetScale(SimpleMath::Vector3::One);
	snakeBody1->SetTarget(snakeBody0);

	auto snakeBody2 = new SnakeCube(3, SimpleMath::Vector3::Backward, "RedCube", this);
	snakeBody2->SetScale(SimpleMath::Vector3::One);
	snakeBody2->SetTarget(snakeBody1);


	ActorManager::GetInstance().AddActor(snakeHead);
	ActorManager::GetInstance().AddActor(snakeBody0);
	ActorManager::GetInstance().AddActor(snakeBody1);
	ActorManager::GetInstance().AddActor(snakeBody2);

	_pScoreSystem = new ScoreSystem();
	_pPlayer->SetChild(_pScoreSystem);
	_pScoreSystem->SetScale(SimpleMath::Vector3(1.0f));
	_pScoreSystem->SetPosition(SimpleMath::Vector3(0.0f, -1.0f, 3.0f));
	_pScoreSystem->SetRotation(SimpleMath::Vector3(0.73f, 0.0f, 0.0f));
}
