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
	if(_pScoreSystem)
	{
		_pScoreSystem->Destroy();
		_pScoreSystem = nullptr;
	}

	if(_pScoreResultObject)
	{
		_pScoreResultObject->Destroy();
		_pScoreResultObject = nullptr;
	}

	if (_pPlayer)
	{
		_pPlayer->Destroy();
		_pPlayer = nullptr;
	}

	if(_pMirror0)
	{
		_pMirror0->Destroy();
		_pMirror0 = nullptr;
	}

	if(_pMirror1)
	{
		_pMirror1->Destroy();
		_pMirror1 = nullptr;
	}

	_pPlayer = new Player(SimpleMath::Vector3(0, 8.0f, 0), this);
	_pPlayer->SetScale(SimpleMath::Vector3(0.7f));
	_pPlayer->SetActorName("Player");
	ActorManager::GetInstance().AddActor(_pPlayer);

	_pMirror0 = new MirrorCube(this, true,2.3f);
	ActorManager::GetInstance().AddActor(_pMirror0);
	_pMirror0->SetScale(SimpleMath::Vector3(1.6f, 2, 0.8f));

	_pMirror1 = new MirrorCube(this, false, 0.8f);
	ActorManager::GetInstance().AddActor(_pMirror1);
	_pMirror1->SetScale(SimpleMath::Vector3(1.6f, 2, 0.8f));

	auto cube = new TargetTitleLogo(30,this);
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

	if(_pGameTimer->IsEnd())
	{
		ChangeGameState(GameStete_Result);
	}
}

void GameManager::TitleUpdate()
{
}

void GameManager::ResultUpdate()
{
	if(DirectXInput::GetInstance().IsKeyDown(DIK_SPACE))
	{
		ChangeGameState(GameStete_Title);
	}

	if(DirectXInput::GetInstance().IsActiveGamePad())
	{
		if(DirectXInput::GetInstance().isButtonDown(GAMEPAD_BUTTON_A))
		{
			ChangeGameState(GameStete_Title);
		}
	}
}

void GameManager::CreateStage()
{

#pragma region 00 ~ 40Time


	_pObjectGenerator->CreateSnake(1.0f, SimpleMath::Vector3(0, 0, 300), 1, 3);
	_pObjectGenerator->CreateSnake(3.0f, SimpleMath::Vector3(-100, 0, 300), 1, 3);
	_pObjectGenerator->CreateSnake(2.0f, SimpleMath::Vector3(100, 0, 300), 1, 3);
	_pObjectGenerator->CreateSnake(1.0f, SimpleMath::Vector3(0, 30, 300), 1, 3);
	_pObjectGenerator->CreateSnake(3.0f, SimpleMath::Vector3(-100, 30, 300), 1, 3);
	_pObjectGenerator->CreateSnake(2.0f, SimpleMath::Vector3(100, 30, 300), 1, 3);

	_pObjectGenerator->CreateSnake(10.0f, SimpleMath::Vector3(0, 0, -300), 1, 3);
	_pObjectGenerator->CreateSnake(10.0f, SimpleMath::Vector3(-100, 0, -300), 1, 3);
	_pObjectGenerator->CreateSnake(10.0f, SimpleMath::Vector3(100, 0, -300), 1, 3);
	_pObjectGenerator->CreateSnake(10.0f, SimpleMath::Vector3(0, 30, -300), 1, 3);
	_pObjectGenerator->CreateSnake(10.0f, SimpleMath::Vector3(-100, 30, -300), 1, 3);
	_pObjectGenerator->CreateSnake(10.0f, SimpleMath::Vector3(100, 30, -300), 1, 3);

	_pObjectGenerator->CreateBlenderMonkeyObjcet(15.0f, SimpleMath::Vector3(0, 8, -20), SimpleMath::Vector3(3.0f), 30, BlenderMonkeyObject::BlenderMonkyObjectType_Clear);

	_pObjectGenerator->CreateNineSideCube(20.0f, SimpleMath::Vector3(100, 30, 300), 30.0f);


	_pObjectGenerator->CreateSnake(30.0f, SimpleMath::Vector3(0, 0, 300), 1, 3);
	_pObjectGenerator->CreateSnake(30.0f, SimpleMath::Vector3(-100, 0, 300), 1, 3);
	_pObjectGenerator->CreateSnake(30.0f, SimpleMath::Vector3(100, 0, 300), 1, 3);
	_pObjectGenerator->CreateSnake(30.0f, SimpleMath::Vector3(0, 30, 300), 1, 3);
	_pObjectGenerator->CreateSnake(30.0f, SimpleMath::Vector3(-100, 30, 300), 1, 3);
	_pObjectGenerator->CreateSnake(30.0f, SimpleMath::Vector3(100, 30, 300), 1, 3);

	_pObjectGenerator->CreateSnake(40.0f, SimpleMath::Vector3(0, 0, -300), 1, 3);
	_pObjectGenerator->CreateSnake(40.0f, SimpleMath::Vector3(-100, 0, -300), 1, 3);
	_pObjectGenerator->CreateSnake(40.0f, SimpleMath::Vector3(100, 0, -300), 1, 3);
	_pObjectGenerator->CreateSnake(40.0f, SimpleMath::Vector3(0, 30, -300), 1, 3);
	_pObjectGenerator->CreateSnake(40.0f, SimpleMath::Vector3(-100, 30, -300), 1, 3);
	_pObjectGenerator->CreateSnake(40.0f, SimpleMath::Vector3(100, 30, -300), 1, 3);
#pragma endregion

	float baseTime = 50.0f;

#pragma region 50 ~ 90Time

	_pObjectGenerator->CreateNineSideCube(baseTime, SimpleMath::Vector3(100, 30, 300), 30.0f);
	_pObjectGenerator->CreateNineSideCube(baseTime + 5.0f, SimpleMath::Vector3(100, 30, 300), 30.0f);

	_pObjectGenerator->CreateBlenderMonkeyObjcet(baseTime + 5.0f, SimpleMath::Vector3(0, 8, -20), SimpleMath::Vector3(3.0f), 30, BlenderMonkeyObject::BlenderMonkyObjectType_Mirror);


	for(int i = 0; i < 5; ++i)
	{
		_pObjectGenerator->CreateSnake(baseTime + 15.0f, SimpleMath::Vector3(10 * i, 60, 300), 3, 1);
	}

	for (int i = 0; i < 5; ++i)
	{
		_pObjectGenerator->CreateSnake(baseTime + 20.0f, SimpleMath::Vector3(10 * i, 0, 300), 3, 1);
	}

	for (int i = 0; i < 5; ++i)
	{
		_pObjectGenerator->CreateSnake(baseTime + 25.0f, SimpleMath::Vector3(10 * i, 30, 300), 3, 1);
	}

	// 後でここテコ入れ

	for (int i = 0; i < 5; ++i)
	{
		_pObjectGenerator->CreateSnake(baseTime + 30.0f, SimpleMath::Vector3( -20 + 10 * i, 30, -300), 3, 3);
	}

	for (int i = 0; i < 5; ++i)
	{
		_pObjectGenerator->CreateSnake(baseTime + 30.0f, SimpleMath::Vector3(-20 + 10 * i, 20, -300), 3, 3);
	}

	for (int i = 0; i < 5; ++i)
	{
		_pObjectGenerator->CreateSnake(baseTime + 30.0f, SimpleMath::Vector3(-20 + 10 * i, 10, -300), 3, 3);
	}


	for (int i = 0; i < 5; ++i)
	{
		_pObjectGenerator->CreateSnake(baseTime + 35.0f, SimpleMath::Vector3(-10 + 10 * i, 30, 300), 3, 3);
	}

	for (int i = 0; i < 5; ++i)
	{
		_pObjectGenerator->CreateSnake(baseTime + 35.0f, SimpleMath::Vector3(-10 + 10 * i, 20, 300), 3, 3);
	}

	for (int i = 0; i < 5; ++i)
	{
		_pObjectGenerator->CreateSnake(baseTime + 35.0f, SimpleMath::Vector3(-10 + 10 * i, 10, 300), 3, 3);
	}

	for (int i = 0; i < 5; ++i)
	{
		_pObjectGenerator->CreateSnake(baseTime + 40.0f, SimpleMath::Vector3(-20 + 10 * i, 30, 340), 3, 3);
	}

	for (int i = 0; i < 5; ++i)
	{
		_pObjectGenerator->CreateSnake(baseTime + 40.0f, SimpleMath::Vector3(-20 + 10 * i, 20, 340), 3, 3);
	}

	for (int i = 0; i < 5; ++i)
	{
		_pObjectGenerator->CreateSnake(baseTime + 40.0f, SimpleMath::Vector3(-20 + 10 * i, 10, 340), 3, 3);
	}

	_pObjectGenerator->CreateBlenderMonkeyObjcet(baseTime + 38.0f, SimpleMath::Vector3(0, 8, 40), SimpleMath::Vector3(6.0f), 30, BlenderMonkeyObject::BlenderMonkyObjectType_Clear);

#pragma endregion
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
