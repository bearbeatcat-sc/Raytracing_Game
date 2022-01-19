#include "GameManager.h"

#include <Device/DirectX/DirectXInput.h>
#include <Game_Object/ActorManager.h>

#include "ObjectGenerateInfo.h"
#include "../TargetObjects/MirrorCube.h"
#include "../PlayerSystem/Player.h"
#include "ScoreSystem.h"
#include "ObjectGenerator.h"
#include "Stage.h"
#include "GameTimer.h"
#include "ScoreResultObject.h"

#include "../Cube.h"
#include "../TargetObjects/TargetTitleLogo.h"

void GameManager::AddScore(const int score)
{
	_pScoreSystem->AddScore(score);
}

void GameManager::DestroyObject(Actor* pObject)
{
	_pObjectGenerator->OnDestroyObject(pObject);
}

Player* GameManager::GetPlayer()
{
	return _pPlayer;
}

void GameManager::UpdateActor()
{
	switch (_gameState)
	{
	case GameStete_Title:
		TitleUpdate();
		break;

	case GameStete_GamePlay:
		GamePlayUpdate();
		break;

	case GameStete_Result:
		ResultUpdate();
		break;

	default:;
	}

}

void GameManager::Init()
{
	auto stage = new Stage();
	stage->SetPosition(SimpleMath::Vector3(-7.7f, 0, -56.0f));
	ActorManager::GetInstance().AddActor(stage);

	_pObjectGenerator = new ObjectGenerator(this);
	ActorManager::GetInstance().AddActor(_pObjectGenerator);

	ChangeGameState(GameStete_Title);


}


void GameManager::Shutdown()
{
}

void GameManager::OnCollsion(Actor* other)
{
}

void GameManager::EndGame()
{
	_pObjectGenerator->End();

	_pGameTimer->Destroy();

	_pScoreResultObject = new ScoreResultObject();
	_pScoreResultObject->SetPosition(SimpleMath::Vector3(0, 0, 100));
	ActorManager::GetInstance().AddActor(_pScoreResultObject);

	_pScoreSystem->ChangeResultMode();
}

void GameManager::ResetGame()
{
	if (_pScoreSystem)
	{
		_pScoreSystem->Destroy();
		_pScoreSystem = nullptr;
	}

	if (_pScoreResultObject)
	{
		_pScoreResultObject->Destroy();
		_pScoreResultObject = nullptr;
	}

	if (_pPlayer)
	{
		_pPlayer->Destroy();
		_pPlayer = nullptr;
	}

	if (_pMirror0)
	{
		_pMirror0->Destroy();
		_pMirror0 = nullptr;
	}

	_pPlayer = new Player(SimpleMath::Vector3(0, 8.0f, 0), this);
	_pPlayer->SetScale(SimpleMath::Vector3(0.7f));
	_pPlayer->SetActorName("Player");
	ActorManager::GetInstance().AddActor(_pPlayer);

	_pMirror0 = new MirrorCube(this, true, 2.3f);
	ActorManager::GetInstance().AddActor(_pMirror0);
	_pMirror0->SetScale(SimpleMath::Vector3(1.2f, 2, 0.8f));



	auto cube = new TargetTitleLogo(30, this);
	cube->SetPosition(SimpleMath::Vector3(0, 17.6f, -18.0f));
	cube->SetScale(SimpleMath::Vector3(12, 4, 1.0f));
	cube->SetActorName("TargetTitleLogo");
	ActorManager::GetInstance().AddActor(cube);


}

void GameManager::ChangeGameState(GameState gameState)
{
	_gameState = gameState;

	switch (gameState)
	{
	case GameStete_Title:
		ResetGame();
		break;

	case GameStete_GamePlay:
		StartGame();
		break;

	case GameStete_Result:
		EndGame();
		break;

	default:;
	}
}

void GameManager::GamePlayUpdate()
{
	_pGameTimer->Update();

	if (_pGameTimer->IsEnd())
	{
		ChangeGameState(GameStete_Result);
	}
}

void GameManager::TitleUpdate()
{
}

void GameManager::ResultUpdate()
{
	if (DirectXInput::GetInstance().IsKeyDown(DIK_SPACE))
	{
		ChangeGameState(GameStete_Title);
	}

	if (DirectXInput::GetInstance().IsActiveGamePad())
	{
		if (DirectXInput::GetInstance().isButtonDown(GAMEPAD_BUTTON_A))
		{
			ChangeGameState(GameStete_Title);
		}
	}
}

void GameManager::CreateStage()
{
	_pObjectGenerator->CreateNineSideCube(1.0f,10.0f, SimpleMath::Vector3(100, 30, 300), 30.0f);

	for(int i = 0; i < 10; ++i)
	{
		_pObjectGenerator->CreateTargetCube(1.0f + (0.5f * i), 2.0f, 1, SimpleMath::Vector3(1.0f + (5.0f * i), 10, -30), SimpleMath::Vector3(1.0f), "RedCube");
	}

	for (int i = 0; i < 10; ++i)
	{
		_pObjectGenerator->CreateTargetCube(1.0f + (0.5f * i), 30.0f, 1, SimpleMath::Vector3(1.0f + (5.0f * i), 10, 30), SimpleMath::Vector3(1.0f), "RedCube");
	}
}

void GameManager::StartGame()
{



	_pObjectGenerator->Reset();
	CreateStage();


	_pScoreSystem = new ScoreSystem(SimpleMath::Vector3(-0.6f, -1.0f, 2.0f));
	_pPlayer->SetChild(_pScoreSystem);
	_pScoreSystem->SetScale(SimpleMath::Vector3(1.0f));
	_pScoreSystem->SetRotation(SimpleMath::Vector3(0.6f, 0.0f, 0.0f));

	_pGameTimer = new GameTimer(220);
	_pPlayer->SetChild(_pGameTimer);
	_pGameTimer->SetScale(SimpleMath::Vector3(1.0f));
	_pGameTimer->SetPosition(SimpleMath::Vector3(-1.5f, -1.0f, 1.6f));
	_pGameTimer->SetRotation(SimpleMath::Vector3(-0.620f, 0.0f, 0.0f));
}
