#pragma once

#include <Game_Object/Actor.h>

class SoundInstance;
class ScoreResultObject;
class ScoreSystem;
class Player;
class ObjectGenerator;
class GameTimer;
class TitleStage;
class PlayStage;
class PlayerTimeline;
class IXAudio2SourceVoice;

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
	void CreateTitleStage();
	void ResetGame();

	void GamePlayUpdate();
	void TitleUpdate();
	void ResultUpdate();

	void CreateStage();
	void StartGame();

private:
	Player* _pPlayer = nullptr;
	ScoreSystem* _pScoreSystem = nullptr;
	ObjectGenerator* _pObjectGenerator = nullptr;
	PlayerTimeline* _pPlayerTimeline = nullptr;
	ScoreResultObject* _pScoreResultObject = nullptr;
	TitleStage* _pTitleStage = nullptr;
	PlayStage* _pPlayStage = nullptr;

	GameState _gameState;

	GameTimer* _pGameTimer;

	std::shared_ptr<SoundInstance> _pAmbientBGMSoundInstance;
	std::shared_ptr<SoundInstance> _pBGMSoundInstance;
};
