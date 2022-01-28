#include "GameManager.h"

#include <Device/DirectX/DirectXInput.h>
#include <Game_Object/ActorManager.h>
#include <Utility/CameraManager.h>
#include <Utility/Camera.h>
#include <Utility/Random.h>

#include "ObjectGenerateInfo.h"
#include "../PlayerSystem/Player.h"
#include "ScoreSystem.h"
#include "ObjectGenerator.h"
#include "StageObjects/PlayStage.h"
#include "ScoreResultObject.h"

#include "../TargetObjects/TargetTitleLogo.h"
#include "../PlayerSystem/PlayerTimeline.h"

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


	_pObjectGenerator = new ObjectGenerator(this);
	ActorManager::GetInstance().AddActor(_pObjectGenerator);

	_pPlayerTimeline = new PlayerTimeline(this);
	ActorManager::GetInstance().AddActor(_pPlayerTimeline);

	ChangeGameState(GameStete_Title);

	auto camera = std::make_shared<Camera>();
	CameraManager::GetInstance().AddCamera("PlayerCamera", camera);
	CameraManager::GetInstance().SetMainCamera("PlayerCamera");
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

	if (_pPlayStage)
	{
		_pPlayStage->Destroy();
		_pPlayStage = nullptr;
	}

	//_pGameTimer->Destroy();

	_pPlayer->SetPlayerState(Player::PlayerState_Stay);
	_pPlayer->SetPosition(SimpleMath::Vector3(0, 0, 0));

	_pScoreResultObject = new ScoreResultObject();
	_pScoreResultObject->SetPosition(SimpleMath::Vector3(0, 0, 100));
	_pPlayer->SetChild(_pScoreResultObject);

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

	//if (_pMirror0)
	//{
	//	_pMirror0->Destroy();
	//	_pMirror0 = nullptr;
	//}

	//if (_pMirror1)
	//{
	//	_pMirror1->Destroy();
	//	_pMirror1 = nullptr;
	//}


	_pPlayer = new Player(SimpleMath::Vector3(0, 8.0f, 0), this);
	_pPlayer->SetScale(SimpleMath::Vector3(0.7f));
	_pPlayer->SetActorName("Player");
	ActorManager::GetInstance().AddActor(_pPlayer);

	//_pMirror0 = new MirrorCube(this, true, 2.3f);
	//_pMirror0->SetScale(SimpleMath::Vector3(0.23f, 1.30f, 1.2f));
	//_pMirror0->SetRotation(SimpleMath::Vector3(0.65f, 0, 0));
	//_pMirror0->SetPosition(SimpleMath::Vector3(2, 0, 2.4f));

	//_pMirror1 = new MirrorCube(this, true, 2.3f);
	//_pMirror1->SetScale(SimpleMath::Vector3(0.23f, 1.30f, 1.2f));
	//_pMirror1->SetRotation(SimpleMath::Vector3(5.620f, 0, 0));
	//_pMirror1->SetPosition(SimpleMath::Vector3(-2.060f,0,2.43f));

	//_pPlayer->SetChild(_pMirror0);
	//_pPlayer->SetChild(_pMirror1);

	auto cube = new TargetTitleLogo(10, this);
	cube->SetPosition(SimpleMath::Vector3(3.0f, 17.6f, 30.0f));
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
	//_pGameTimer->Update();

	//if (_pGameTimer->IsEnd())
	//{
	//	ChangeGameState(GameStete_Result);
	//}
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
	float baseTime = 0.0f;

	// チュートリアルステージ的な感じ
	_pObjectGenerator->CreateSlideTargetCube(SimpleMath::Vector3(-1, 0, 0), 6.0f, baseTime + 2.0f, 20.0f, 6, SimpleMath::Vector3(10, 10, 60), SimpleMath::Vector3(1.0f), "RedMirrorCube");
	_pObjectGenerator->CreateSlideTargetCube(SimpleMath::Vector3(-1, 0, 0), 6.0f, baseTime + 2.0f, 20.0f, 6, SimpleMath::Vector3(15, 10, 60), SimpleMath::Vector3(1.0f), "RedMirrorCube");
	_pObjectGenerator->CreateSlideTargetCube(SimpleMath::Vector3(-1, 0, 0), 6.0f, baseTime + 2.0f, 20.0f, 6, SimpleMath::Vector3(20, 10, 60), SimpleMath::Vector3(1.0f), "RedMirrorCube");

	_pObjectGenerator->CreateJumpTargetCube(SimpleMath::Vector3(0, 1, 0), 20.0f, baseTime + 10.0f, 20.0f, 6, SimpleMath::Vector3(0, 10, 160), SimpleMath::Vector3(12.0f), "BlenderMonkeyMirror");

	_pObjectGenerator->CreateSlideTargetCube(SimpleMath::Vector3(-1, 0, 0), 6.0f, baseTime + 15.0f, 20.0f, 6, SimpleMath::Vector3(10, 10, 160), SimpleMath::Vector3(1.0f), "RedMirrorCube");
	_pObjectGenerator->CreateSlideTargetCube(SimpleMath::Vector3(-1, 0, 0), 6.0f, baseTime + 15.0f, 20.0f, 6, SimpleMath::Vector3(15, 10, 160), SimpleMath::Vector3(1.0f), "RedMirrorCube");
	_pObjectGenerator->CreateSlideTargetCube(SimpleMath::Vector3(-1, 0, 0), 6.0f, baseTime + 15.0f, 20.0f, 6, SimpleMath::Vector3(20, 10, 160), SimpleMath::Vector3(1.0f), "RedMirrorCube");

	_pObjectGenerator->CreateSlideTargetCube(SimpleMath::Vector3(1, 0, 0), 6.0f, baseTime + 15.0f, 20.0f, 6, SimpleMath::Vector3(-10, 10, 160), SimpleMath::Vector3(1.0f), "RedMirrorCube");
	_pObjectGenerator->CreateSlideTargetCube(SimpleMath::Vector3(1, 0, 0), 6.0f, baseTime + 15.0f, 20.0f, 6, SimpleMath::Vector3(-15, 10, 160), SimpleMath::Vector3(1.0f), "RedMirrorCube");
	_pObjectGenerator->CreateSlideTargetCube(SimpleMath::Vector3(1, 0, 0), 6.0f, baseTime + 15.0f, 20.0f, 6, SimpleMath::Vector3(-20, 10, 160), SimpleMath::Vector3(1.0f), "RedMirrorCube");

	_pObjectGenerator->CreateNineSideCube(baseTime + 18.0f, 10.0f, SimpleMath::Vector3(100, 30, 300), 30.0f);


	// 中盤
	baseTime = 20.0f;

	_pPlayerTimeline->AddTask(baseTime + 4.0f, Player::PlayerState_Run);

	_pObjectGenerator->CreateJumpTargetCube(SimpleMath::Vector3(0, 2, 0), 20.0f, baseTime + 8.0f, 30.0f, 6, SimpleMath::Vector3(-10, -10, 450), SimpleMath::Vector3(2.0f), "RedMirrorCube");
	_pObjectGenerator->CreateJumpTargetCube(SimpleMath::Vector3(0, 2, 0), 20.0f, baseTime + 8.0f, 30.0f, 6, SimpleMath::Vector3(0, -10, 450), SimpleMath::Vector3(2.0f), "RedMirrorCube");
	_pObjectGenerator->CreateJumpTargetCube(SimpleMath::Vector3(0, 2, 0), 20.0f, baseTime + 8.0f, 30.0f, 6, SimpleMath::Vector3(10, -10, 450), SimpleMath::Vector3(2.0f), "RedMirrorCube");

	_pObjectGenerator->CreateSlideTargetCube(SimpleMath::Vector3(1, 0, 0), 20.0f, baseTime + 10.0f, 30.0f, 6, SimpleMath::Vector3(-30, 10, 500), SimpleMath::Vector3(2.0f), "RedMirrorCube");
	_pObjectGenerator->CreateSlideTargetCube(SimpleMath::Vector3(1, 0, 0), 20.0f, baseTime + 10.0f, 30.0f, 6, SimpleMath::Vector3(-20, 10, 500), SimpleMath::Vector3(2.0f), "RedMirrorCube");
	_pObjectGenerator->CreateSlideTargetCube(SimpleMath::Vector3(1, 0, 0), 20.0f, baseTime + 10.0f, 30.0f, 6, SimpleMath::Vector3(-10, 10, 500), SimpleMath::Vector3(2.0f), "RedMirrorCube");

	_pObjectGenerator->CreateSlideTargetCube(SimpleMath::Vector3(-1, 0, 0), 20.0f, baseTime + 12.0f, 30.0f, 6, SimpleMath::Vector3(30, 10, 550), SimpleMath::Vector3(2.0f), "RedMirrorCube");
	_pObjectGenerator->CreateSlideTargetCube(SimpleMath::Vector3(-1, 0, 0), 20.0f, baseTime + 12.0f, 30.0f, 6, SimpleMath::Vector3(20, 10, 550), SimpleMath::Vector3(2.0f), "RedMirrorCube");
	_pObjectGenerator->CreateSlideTargetCube(SimpleMath::Vector3(-1, 0, 0), 20.0f, baseTime + 12.0f, 30.0f, 6, SimpleMath::Vector3(10, 10, 550), SimpleMath::Vector3(2.0f), "RedMirrorCube");

	_pObjectGenerator->CreateSlideTargetCube(SimpleMath::Vector3(0, 0, 8), 6.0f, baseTime + 15.0f, 30.0f, 6, SimpleMath::Vector3(-2, 10, 450), SimpleMath::Vector3(1.0f), "RedMirrorCube");
	_pObjectGenerator->CreateSlideTargetCube(SimpleMath::Vector3(0, 0, 8), 6.0f, baseTime + 15.0f, 30.0f, 6, SimpleMath::Vector3(-0, 10, 450), SimpleMath::Vector3(1.0f), "RedMirrorCube");
	_pObjectGenerator->CreateSlideTargetCube(SimpleMath::Vector3(0, 0, 8), 6.0f, baseTime + 15.0f, 30.0f, 6, SimpleMath::Vector3(2, 10, 450), SimpleMath::Vector3(1.0f), "RedMirrorCube");

	_pObjectGenerator->CreateSlideTargetCube(SimpleMath::Vector3(0, 0, 8), 6.0f, baseTime + 15.0f, 30.0f, 6, SimpleMath::Vector3(-2, -10, 450), SimpleMath::Vector3(1.0f), "RedMirrorCube");
	_pObjectGenerator->CreateSlideTargetCube(SimpleMath::Vector3(0, 0, 8), 6.0f, baseTime + 15.0f, 30.0f, 6, SimpleMath::Vector3(-0, -10, 450), SimpleMath::Vector3(1.0f), "RedMirrorCube");
	_pObjectGenerator->CreateSlideTargetCube(SimpleMath::Vector3(0, 0, 8), 6.0f, baseTime + 15.0f, 30.0f, 6, SimpleMath::Vector3(2, -10, 450), SimpleMath::Vector3(1.0f), "RedMirrorCube");

	baseTime = 40.0f;

	_pPlayerTimeline->AddTask(baseTime, Player::PlayerState_Stay);

	for(int i = 0; i < 90; ++i)
	{
		float pos_x = Random::GetRandom(-0.5f, 0.5f);
		float pos_z = Random::GetRandom(-0.5f, 0.5f);

		_pObjectGenerator->CreateJumpTargetCube(SimpleMath::Vector3(pos_x * -1.0f, 1.0f, pos_z * -1.0f), 20.0f, baseTime + (i * 0.1f), 30.0f, 6,  SimpleMath::Vector3(pos_x, 5, 730 + pos_z), SimpleMath::Vector3(0.5f), "RedMirrorCube");

	}

	// 後半
	baseTime = 51.0f;

	_pPlayerTimeline->AddTask(baseTime, Player::PlayerState_Run);

}

void GameManager::StartGame()
{
	_pPlayer->SetPlayerState(Player::PlayerState_Move);

	_pPlayStage = new PlayStage(this);
	ActorManager::GetInstance().AddActor(_pPlayStage);

	_pObjectGenerator->Reset();
	_pPlayerTimeline->Reset();
	CreateStage();


	_pScoreSystem = new ScoreSystem(SimpleMath::Vector3(-1.0f, -1.0f, 2.0f));
	_pPlayer->SetChild(_pScoreSystem);
	_pScoreSystem->SetScale(SimpleMath::Vector3(1.0f));
	_pScoreSystem->SetRotation(SimpleMath::Vector3(0.0f, 0.0f, 0.0f));

	//_pGameTimer = new GameTimer(220);
	//_pPlayer->SetChild(_pGameTimer);
	//_pGameTimer->SetScale(SimpleMath::Vector3(1.0f));
	//_pGameTimer->SetPosition(SimpleMath::Vector3(-1.5f, -1.0f, 1.6f));
	//_pGameTimer->SetRotation(SimpleMath::Vector3(-0.620f, 0.0f, 0.0f));
}
