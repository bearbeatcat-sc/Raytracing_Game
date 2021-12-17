#include "TunnelManager.h"

#include <Game_Object/ActorManager.h>
#include <Utility/Random.h>
#include <Utility/Math/MathUtility.h>

#include "../Cube.h"
#include "Tunnel.h"
#include "GameManager.h"
#include "../TargetObjects/TargetCube.h"
#include "../TargetObjects/TargetGroup.h"
#include "../TargetObjects/TrackerCube.h"

TunnelManager::TunnelManager(GameManager* pGameManager)
	:_pGamaManager(pGameManager)
{
}


void TunnelManager::AddTask(std::function<Tunnel* (Actor* actor, GameManager* pGameManager)> task)
{
	_createTasks.push_back(task);
}

void TunnelManager::UpdateActor()
{
	CreateTunnel();
	DestroyTunnel();
}

void TunnelManager::Init()
{
	_generateIndex = 0;


	AddTask([](Actor* actor,GameManager* pGameManager)
		{
			auto prefab = new Tunnel(SimpleMath::Vector3(8, 8, 100), pGameManager);
			prefab->SetPosition(SimpleMath::Vector3::Zero);

			prefab->SetCreateEvent([](Actor* actor, GameManager* pGameManager)
				{
					const SimpleMath::Vector3 cubeSize = SimpleMath::Vector3(10.0f, 10.0f, 10.0f);

					auto floor = new Cube(SimpleMath::Vector3(0, -20.0f, 120.0f), SimpleMath::Vector3(20.0f, 1.0f, 240.0f), 1000.0f, "RoughCube", false);

					actor->SetChild(floor);

					for (int leftCount = 0; leftCount < 3; ++leftCount)
					{
						auto pos = SimpleMath::Vector3(-30, 0, leftCount * cubeSize.z * 2.0f);

						if (leftCount % 2 == 0)
						{
							auto cube = new Cube(pos, cubeSize, 1000.0f, "RoughCube", false);
							actor->SetChild(cube);
							cube->SetActorName("Cube");
							continue;
						}

						float cos = MathUtility::Cos(leftCount * 3.0f);
						pos.y -= cos;

						auto cube = new Cube(pos, cubeSize, 1000.0f, "WhiteCube", false);
						actor->SetChild(cube);
						cube->SetActorName("Cube");
					}

					for (int rightCount = 0; rightCount < 3; ++rightCount)
					{
						auto pos = SimpleMath::Vector3(30, 0, rightCount * cubeSize.z * 2.0f);

						float cos = MathUtility::Cos(rightCount * 3.0f);
						pos.y += cos;

						if (rightCount % 2 == 0)
						{
							auto cube = new Cube(pos, cubeSize, 1000.0f, "WhiteCube", false);
							actor->SetChild(cube);
							cube->SetActorName("Cube");
							continue;
						}

						auto cube = new Cube(pos, cubeSize, 1000.0f, "RoughCube", false);
						actor->SetChild(cube);
						cube->SetActorName("Cube");
					}
				});

			for (int i = 0; i < 450; ++i)
			{
				float rand_x = MathUtility::Cos(i * 0.2f);
				prefab->AddPoint(SimpleMath::Vector3(rand_x, 10.0f, 20.0f + (0.8f * i)));
			}

			auto targetGroup0 = new TargetGroup(pGameManager,[](Actor* actor,GameManager* pGameManager)
			{
					auto targetCube = new TargetCube("RedCube",pGameManager);
					targetCube->SetScale(SimpleMath::Vector3(1.0f));
					targetCube->SetPosition(SimpleMath::Vector3(-10.0f, 0.0f, 0.0f));
					targetCube->SetActorName("TargetCube");

					actor->SetChild(targetCube);

					auto targetCube2= new TargetCube("RedCube", pGameManager);
					targetCube2->SetScale(SimpleMath::Vector3(1.0f));
					targetCube2->SetPosition(SimpleMath::Vector3(10.0f, 0.0f, 0.0f));
					targetCube2->SetActorName("TargetCube");

					actor->SetChild(targetCube2);

			}
			);

			targetGroup0->SetPosition(SimpleMath::Vector3(0, 10.0f, (20.0f) + 100.0f));
			prefab->SetChild(targetGroup0);
			targetGroup0->SetActorName("TargetGruop0");
			prefab->SetActorName("Tunnel0");

			return prefab;
		});


	AddTask([](Actor* actor, GameManager* pGameManager)
		{
			auto prefab = new Tunnel(SimpleMath::Vector3(8, 8, 100),pGameManager);
			prefab->SetPosition(SimpleMath::Vector3(0, 0, 360.0f));

			prefab->SetCreateEvent([](Actor* actor, GameManager* pGameManager)
				{
					const SimpleMath::Vector3 cubeSize = SimpleMath::Vector3(2.0f, 2.0f, 2.0f);

					auto floor = new Cube(SimpleMath::Vector3(0, -20.0f, 120.0f), SimpleMath::Vector3(20.0f, 1.0f, 240.0f), 1000.0f, "RoughCube", false);
					actor->SetChild(floor);
					floor->SetActorName("Floor");


					//for (int i = 0; i < 90; i++)
					//{
					//	float cos = MathUtility::Cos(i * 2.0f);
					//	float sin = MathUtility::Sin(i * 2.0f);
					//	float range = 30.0f;


					//	auto pos = SimpleMath::Vector3(sin * range, cos * range, i * cubeSize.z * 2.0f);

					//	if (i % 2 == 0)
					//	{
					//		auto cube = new Cube(pos, cubeSize, 1000.0f, "RoughCube", false);
					//		cube->SetActorName("Cube");
					//		actor->SetChild(cube);
					//		continue;
					//	}

					//	auto cube = new Cube(pos, cubeSize, 1000.0f, "WhiteCube", false);
					//	actor->SetChild(cube);
					//	cube->SetActorName("Cube");



					//	//pos.y -= cos;


					//}


				});

			for (int i = 0; i < 900; ++i)
			{
				float rand_x = MathUtility::Cos(i * 0.2f);
				prefab->AddPoint(SimpleMath::Vector3(rand_x, 10.0f, 400.0f + (0.8f * i)));
			}

			prefab->SetActorName("Tunnel1");

			return prefab;
		});


	AddTask([](Actor* actor, GameManager* pGameManager)
		{
			auto prefab = new Tunnel(SimpleMath::Vector3(8, 8, 100),pGameManager);
			prefab->SetPosition(SimpleMath::Vector3(0, 0, 400.0f + 360.0f));

			prefab->SetCreateEvent([](Actor* actor, GameManager* pGameManager)
				{
					const SimpleMath::Vector3 cubeSize = SimpleMath::Vector3(2.0f, 2.0f, 2.0f);

					auto floor = new Cube(SimpleMath::Vector3(0, -20.0f, 120.0f), SimpleMath::Vector3(20.0f, 1.0f, 240.0f), 1000.0f, "RoughCube", false);
					actor->SetChild(floor);

					auto ceiling = new Cube(SimpleMath::Vector3(0, 20.0f, 120.0f), SimpleMath::Vector3(20.0f, 1.0f, 240.0f), 1000.0f, "RoughCube", false);
					actor->SetChild(ceiling);

					auto leftWall = new Cube(SimpleMath::Vector3(-55.0f, 0.0f, 120.0f), SimpleMath::Vector3(1.0f, 20.0f, 240.0f), 1000.0f, "WhiteCube", false);
					leftWall->SetRotation(SimpleMath::Vector3(3.3f, 0.0f, 0.0f));
					actor->SetChild(leftWall);

					auto rightWall = new Cube(SimpleMath::Vector3(55.0f, 0.0f, 120.0f), SimpleMath::Vector3(1.0f, 20.0f, 240.0f), 1000.0f, "WhiteCube", false);
					rightWall->SetRotation(SimpleMath::Vector3(3.0f, 0.0f, 0.0f));
					actor->SetChild(rightWall);

					auto centerWall = new Cube(SimpleMath::Vector3(0.0f, 0.0f, 400.0), SimpleMath::Vector3(12.0f), 1000.0f, "WhiteCube", false);
					actor->SetChild(centerWall);
				});

			for (int i = 0; i < 900; ++i)
			{
				float rand_x = MathUtility::Cos(i * 0.1f);
				prefab->AddPoint(SimpleMath::Vector3(rand_x, 10.0f, 400.0f + 360.0f + (0.4f * i)));
			}

			prefab->SetCollisionEvent([](Actor* actor, GameManager* pGameManager)
				{
					auto targetGroup0 = new TargetGroup(pGameManager, [](Actor* actor, GameManager* pGameManager)
						{
							auto targetCube = new TargetCube("RedCube", pGameManager);
							targetCube->SetScale(SimpleMath::Vector3(1.0f));
							targetCube->SetPosition(SimpleMath::Vector3(-10.0f, 0.0f, 0.0f));
							targetCube->SetActorName("TargetCube");

							actor->SetChild(targetCube);
							const SimpleMath::Vector3 cubeSize = SimpleMath::Vector3(2.0f, 2.0f, 2.0f);



							for (int i = 0; i < 90; i++)
							{
								float cos = MathUtility::Cos(i * 2.0f);
								float sin = MathUtility::Sin(i * 2.0f);
								float range = 30.0f;


								auto pos = SimpleMath::Vector3(sin * range, cos * range, i * cubeSize.z * 2.0f);


								auto targetCube = new TargetCube("RedCube", pGameManager);
								targetCube->SetScale(SimpleMath::Vector3(1.0f));
								targetCube->SetPosition(SimpleMath::Vector3(pos));
								targetCube->SetActorName("TargetCube");

								actor->SetChild(targetCube);



								//pos.y -= cos;


							}
						}
					);

					targetGroup0->SetPosition(SimpleMath::Vector3(0, 10.0f, -300.0f));
					actor->SetChild(targetGroup0);
					targetGroup0->SetActorName("TargetGruop0");
				});

			prefab->SetActorName("Tunnel2");

			return prefab;
		});
}

void TunnelManager::Shutdown()
{
	_generateTunnels.clear();
	_createTasks.clear();
}

void TunnelManager::OnCollsion(Actor* other)
{
}

void TunnelManager::CreateTunnel()
{

	if (_generateTunnels.size() >= _maxGenerateCount
		|| _createTasks.size() <= _generateIndex)
	{
		return;
	}

	auto result = _createTasks.at(_generateIndex)(this,_pGamaManager);
	ActorManager::GetInstance().AddActor(result);
	_generateTunnels.push_back(result);

	_generateIndex++;

}

void TunnelManager::DestroyTunnel()
{

	for (auto itr = _generateTunnels.begin(); itr != _generateTunnels.end();)
	{
		// プレイヤーがトンネルから出ていたら、先頭だけ削除する
		if ((*itr)->IsExit() && (itr != _generateTunnels.begin()))
		{
			(*_generateTunnels.begin())->Destroy();
			itr = _generateTunnels.erase(_generateTunnels.begin());

			return;
		}

		itr++;
	}
}
