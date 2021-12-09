#include "MainGame.h"

#include <vector>
#include "Utility/Camera.h"
#include "Device/DirectX/DirectXInput.h"
#include "Device/DirectX/DirectXGraphics.h"
#include "Device/DirectX/Core/Model/MeshManager.h"
#include "Game_Object/ActorManager.h"
#include "GameObjects/Sphere.h"
#include "Device/Raytracing/DXRPipeLine.h"
#include "GameObjects/Cube.h"
#include "Utility/Timer.h"
#include "Utility/Random.h"

#include "System/CameraAsistant.h"

#include <imgui/imgui.h>
#include <time.h>
#include <Device/Lights/LightManager.h>
#include <Device/DirectX/Core/Sounds/SoundManager.h>
#include <Device/Rendering/RenderingPipeline.h>
#include <Components/Collsions/CollisionTagManager.h>

#include "GameObjects/Player.h"
#include "GameObjects/PointLightObject.h"
#include "GameObjects/Tunnel.h"

MainGame::MainGame()
	:Game(L"MainGame", 1920, 1080)
{
}

MainGame::~MainGame()
{
	delete m_CameraAsistant;

}

void MainGame::Init()
{
	DirectXGraphics::GetInstance().InitFontSystem(L"font/fonttest.spritefont");

	MeshManager::GetInstance().GetSpehereMesh(12, "NormalMeshEffect");
	MeshManager::GetInstance().loadMesh("Resources/Models/Model/", "blenderMonkey.obj", "BlenderMonkey");
	MeshManager::GetInstance().loadMesh("Resources/Models/Model/", "cube0.obj", "Cube0");

	auto cubeMeshData = MeshManager::GetInstance().GetMeshData("CubeModelData");
	cubeMeshData->SetPhysicsBaseMaterial(PhysicsBaseMaterial(SimpleMath::Vector4(1.0f, 0.0f, 0.0f, 1.0f), SimpleMath::Vector4(1.0f, 10.0f, 1.0f, 0.5f), 0.5f));

	auto sphereMeshData = MeshManager::GetInstance().FindSpehere(12);
	//sphereMeshData->SetRaytraceMaterial(MeshData::RaytraceMaterial(SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f), SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 1.0f), 1.0f));

	auto blenderMonkyMeshData = MeshManager::GetInstance().GetMeshData("BlenderMonkey");
	blenderMonkyMeshData->SetPhysicsBaseMaterial(PhysicsBaseMaterial(SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f), SimpleMath::Vector4(1.0f, 10.0f, 1.0f, 1.0f), 1.0f));


	//sphereMeshData->SetTestMaterial(MeshData::TestMat{ true });
	_blenderMonkyMaterial = PhysicsBaseMaterial(SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f), SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 0.6f), 0.8f, 1.0f, 1.22f);


	DXRPipeLine::GetInstance().AddMeshData(cubeMeshData, L"HitGroup", "WhiteCube", PhysicsBaseMaterial(SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f), SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 1.0f), 0.1f));
	DXRPipeLine::GetInstance().AddMeshData(cubeMeshData, L"HitGroup", "GrayCube", PhysicsBaseMaterial(SimpleMath::Vector4(0.2f, 0.2f, 0.2f, 1.0f), SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 1.0f), 0.1));
	DXRPipeLine::GetInstance().AddMeshData(cubeMeshData, L"HitGroup", "RoughCube", PhysicsBaseMaterial(SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f), SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 0.0f), 0.5f));
	DXRPipeLine::GetInstance().AddMeshData(cubeMeshData, L"HitGroup", "ClearCube", PhysicsBaseMaterial(SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f), SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 0.0f), 0.5f,0.0f,1.24f));

	DXRPipeLine::GetInstance().AddMeshData(sphereMeshData, L"HitGroup", "Sphere", PhysicsBaseMaterial(SimpleMath::Vector4(0.2f, 0.2f, 0.2f, 1.0f), SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 1.0f), 0.2f));
	_blenderMonkey = DXRPipeLine::GetInstance().AddMeshData(sphereMeshData, L"HitGroup", "Sphere2", _blenderMonkyMaterial);

	// 必ず、メッシュデータを追加してからパイプラインの初期化を行う。
	DXRPipeLine::GetInstance().InitPipeLine();



	auto dirlight = LightManager::GetInstance().GetDirectionalLight();
	auto dir = SimpleMath::Vector3(-0.169f, -0.840, -0.515);
	dirlight->UpdateDirectionalLight(dir, SimpleMath::Color(1, 1, 1, 1));

	m_CameraAsistant = new CameraAsistant();


	const SimpleMath::Vector3 base_position = Vector3(2.5f, 0.0f, 2.5f);

	RenderingPipeLine::GetInstance().SetSkyBox("Resources/OutputCube.dds", SimpleMath::Vector3(10.0f));
	RenderingPipeLine::GetInstance().SetDrawFluidFlag(false);


	std::vector<bool> StaticObjectCollTable =
	{
		false,true,
	};

	std::vector<bool> DynamicObjectCollTable =
	{
		true,false
	};
	
	CollisionTagManagaer::GetInstance().AddTag("StaticObject", StaticObjectCollTable);
	CollisionTagManagaer::GetInstance().AddTag("DynamicObject", DynamicObjectCollTable);

	_AddTimer = std::make_shared<Timer>(3.0f);
	_GenerateTimer = std::make_shared<Timer>(0.0001f);
	_IsGenerate = false;

	auto tonnel = new Tunnel(SimpleMath::Vector3::Zero,SimpleMath::Vector3(8,8,100));
	ActorManager::GetInstance().AddActor(tonnel);
	tonnel->SetChild(new PointLightObject(SimpleMath::Vector3(0, 40, -74), SimpleMath::Color(1, 1, 1, 1), 300.0f));
	tonnel->SetChild(new PointLightObject(SimpleMath::Vector3(0, 30, -40), SimpleMath::Color(1, 1, 1, 1), 300.0f));
	tonnel->SetChild(new PointLightObject(SimpleMath::Vector3(0, 25, -10), SimpleMath::Color(1, 1, 1, 1), 300.0f));

	auto tonnel2 = new Tunnel(SimpleMath::Vector3(0,-60.0f,230), SimpleMath::Vector3(8, 8, 100));
	ActorManager::GetInstance().AddActor(tonnel2);

	auto tonnel3 = new Tunnel(SimpleMath::Vector3(0, -80.0f, 320), SimpleMath::Vector3(8, 8, 100));
	ActorManager::GetInstance().AddActor(tonnel3);

	auto tonnel4 = new Tunnel(SimpleMath::Vector3(0, -120.0f, 430), SimpleMath::Vector3(8, 8, 100));
	ActorManager::GetInstance().AddActor(tonnel4);

	auto tonnel5 = new Tunnel(SimpleMath::Vector3(0, -140.0f, 580), SimpleMath::Vector3(8, 8, 100));
	ActorManager::GetInstance().AddActor(tonnel5);




	//auto leftWall = new Cube(SimpleMath::Vector3(-7.0f, 3.0f, -0.0f), SimpleMath::Vector3(1, 6, grid_size_x), 300.0f, "RoughCube", false);
	//ActorManager::GetInstance().AddActor(leftWall);

	//auto rightWall = new Cube(SimpleMath::Vector3(7.0f, 3.0f, -0.0f), SimpleMath::Vector3(1, 6, grid_size_x), 300.0f, "RoughCube", false);
	//ActorManager::GetInstance().AddActor(rightWall);

	//auto topWall = new Cube(SimpleMath::Vector3(0.0f, 8, 0.0f), SimpleMath::Vector3(grid_size_x, 1, grid_size_x), 300.0f, "RoughCube", false);
	//ActorManager::GetInstance().AddActor(topWall);

	//auto backWall = new Cube(SimpleMath::Vector3(1.5f, 3.0f, -9.0f), SimpleMath::Vector3(grid_size_x, 6, 1), 300.0f, "RoughCube", false);
	//ActorManager::GetInstance().AddActor(backWall);

	//auto sphere = new Sphere(SimpleMath::Vector3(-2.0f, 0.0f, 6.0f), Sphere::SphereType_Normal);
	//sphere->SetScale(SimpleMath::Vector3(1.0f));
	//sphere->SetRotation(SimpleMath::Vector3(3.0f, 0.0f, 0.0f));
	//ActorManager::GetInstance().AddActor(sphere);

	//auto sphere2 = new Sphere(SimpleMath::Vector3(3.0f, -150.0f, 3.0f), Sphere::SphereType_NormalLowPoly);
	//sphere2->SetScale(SimpleMath::Vector3(300.0f));
	//sphere2->SetRotation(SimpleMath::Vector3(-2.4f, 0.0f, 0.0f));
	//ActorManager::GetInstance().AddActor(sphere2);



	auto player = new Player(SimpleMath::Vector3(0,40,1.0f));
	player->SetScale(SimpleMath::Vector3(2.0f));
	ActorManager::GetInstance().AddActor(player);

	auto cube = new Cube(SimpleMath::Vector3(0, 36, 30), SimpleMath::Vector3(1.0f), 300.0f, "ClearCube", false);
	cube->SetActorName("Cube");
	cube->SetTag("Cube");
	cube->OnStatic(true, true);
	player->SetChild(cube);

}

void MainGame::Update()
{
#ifdef _DEBUG
	m_CameraAsistant->Update();
#endif
	
	if (!_IsGenerate)
	{
		_AddTimer->Update();
		if (_AddTimer->IsTime())
		{
			_IsGenerate = true;
			_GenerateCount = 0;
			_AddTimer->Reset();
		}
		return;
	}




	//_GenerateTimer->Update();
	//if (_GenerateTimer->IsTime())
	//{
	//	if (_GenerateCount >= _MaxGenerateCount)
	//	{
	//		_IsGenerate = false;
	//		_GenerateTimer->Reset();
	//		return;
	//	}

	//	auto pos_x = Random::GetRandom(-2.0f, 2.0f);
	//	auto pos_z = Random::GetRandom(-300.0f, 370.0f);
	//	float pos_y = -24.0f - pos_z * 0.2f;

	//	float scale = Random::GetRandom(0.5f, 1.2f);

	//	int flag = Random::GetRandom(0, 1);


	//	if (flag == 0)
	//	{
	//		//Sphere* sphere = nullptr;
	//		//sphere = new Sphere(SimpleMath::Vector3(pos_x, pos_y, pos_z), Sphere::SphereType_Normal);
	//		//sphere->SetScale(SimpleMath::Vector3(scale));
	//		//ActorManager::GetInstance().AddActor(sphere);
	//		//sphere->Destroy(10.0f);
	//		//sphere->SetActorName("Sphere");

	//		Sphere* sphere = nullptr;
	//		sphere = new Sphere(SimpleMath::Vector3(pos_x, pos_y, pos_z), Sphere::SphereType_Normal);

	//		sphere->SetScale(SimpleMath::Vector3(scale));
	//		ActorManager::GetInstance().AddActor(sphere);
	//		sphere->Destroy(10.0f);
	//		sphere->SetActorName("Sphere");

	//	}
	//	else if (flag == 1)
	//	{
	//		Sphere* sphere = nullptr;
	//		sphere = new Sphere(SimpleMath::Vector3(pos_x, pos_y, pos_z), Sphere::SphereType_Normal);

	//		sphere->SetScale(SimpleMath::Vector3(scale));
	//		ActorManager::GetInstance().AddActor(sphere);
	//		sphere->Destroy(10.0f);
	//		sphere->SetActorName("Sphere");

	//		//auto cube = new Cube(SimpleMath::Vector3(pos_x, pos_y + 6.0f, pos_z), SimpleMath::Vector3(0.5f), 300.0f, "WhiteCube", false);
	//		//cube->SetActorName("Cube");
	//		//ActorManager::GetInstance().AddActor(cube);
	//		//cube->Destroy(10.0f);
	//		//cube->SetRotation(SimpleMath::Vector3(pos_x, pos_y, pos_z) * scale * pos_y);
	//	}



	//	_GenerateCount++;


	//}


}

void MainGame::Destroy()
{
}
