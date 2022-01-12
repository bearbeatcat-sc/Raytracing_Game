﻿#pragma once

#include <Game_Object/Actor.h>

class ScoreResultObject;
class ScoreSystem;
class Player;
class ObjectGenerator;
class GameTimer;

class GameManager
	:public Actor
{
public:
	GameManager() = default;
	~GameManager() = default;

	void AddScore(const int score);
	void DestroyObject(Actor* pObject);

	Player* GetPlayer();


	enum GameState
	{
		GameStete_Title,
		GameStete_GamePlay,
		GameStete_Result,
	};
	void ChangeGameState(GameState gameState);

private:
	void UpdateActor() override;
	void Init() override;
	void Shutdown() override;
	void OnCollsion(Actor* other) override;

	void EndGame();
	void ResetGame();

	void GamePlayUpdate();
	void TitleUpdate();
	void ResultUpdate();

	void CreateStage();
	void StartGame();

private:
	Player* _pPlayer;
	ScoreSystem* _pScoreSystem;
	ObjectGenerator* _pObjectGenerator;
	ScoreResultObject* _pScoreResultObject;

	Actor* _pMirror0;
	Actor* _pMirror1;

	GameState _gameState;

	GameTimer* _pGameTimer;
	
};