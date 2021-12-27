#include "MainGame.h"

#include <vector>
#include "Utility/Camera.h"
#include "Device/DirectX/DirectXInput.h"
#include "Device/DirectX/DirectXGraphics.h"
#include "Device/DirectX/Core/Model/MeshManager.h"
#include "Game_Object/ActorManager.h"
#include "Device/Raytracing/DXRPipeLine.h"
#include "GameObjects/Cube.h"


#include "System/CameraAsistant.h"

#include <imgui/imgui.h>
#include <Device/Lights/LightManager.h>
#include <Device/DirectX/Core/Sounds/SoundManager.h>
#include <Device/Rendering/RenderingPipeline.h>
#include <Components/Collsions/CollisionTagManager.h>
#include <Device/TextureManager.h>

#include "GameObjects/GameSystem/GameManager.h"


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
	TextureManager::GetInstance().AddTexture("Resources/TestUI.png", "TestUI");

	TextureManager::GetInstance().AddTexture("Resources/Number0.png", "Number0");
	TextureManager::GetInstance().AddTexture("Resources/Number1.png", "Number1");
	TextureManager::GetInstance().AddTexture("Resources/Number2.png", "Number2");
	TextureManager::GetInstance().AddTexture("Resources/Number3.png", "Number3");
	TextureManager::GetInstance().AddTexture("Resources/Number4.png", "Number4");
	TextureManager::GetInstance().AddTexture("Resources/Number5.png", "Number5");
	TextureManager::GetInstance().AddTexture("Resources/Number6.png", "Number6");
	TextureManager::GetInstance().AddTexture("Resources/Number7.png", "Number7");
	TextureManager::GetInstance().AddTexture("Resources/Number8.png", "Number8");
	TextureManager::GetInstance().AddTexture("Resources/Number9.png", "Number9");

	auto cubeMeshData = MeshManager::GetInstance().GetMeshData("CubeModelData");
	cubeMeshData->SetPhysicsBaseMaterial(PhysicsBaseMaterial(SimpleMath::Vector4(1.0f, 0.0f, 0.0f, 1.0f), SimpleMath::Vector4(1.0f, 10.0f, 1.0f, 0.5f), 0.5f));

	auto sphereMeshData = MeshManager::GetInstance().FindSpehere(12);
	auto planeMeshData = MeshManager::GetInstance().GetPlaneMeshData();

	auto blenderMonkyMeshData = MeshManager::GetInstance().GetMeshData("BlenderMonkey");
	blenderMonkyMeshData->SetPhysicsBaseMaterial(PhysicsBaseMaterial(SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f), SimpleMath::Vector4(1.0f, 10.0f, 1.0f, 1.0f), 1.0f));


	//sphereMeshData->SetTestMaterial(MeshData::TestMat{ true });

	DXRPipeLine::GetInstance().AddMeshData(planeMeshData, L"HitGroup", "TestUI", PhysicsBaseMaterial(SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f), SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 0.0f), 0.5f), "TestUI");

	DXRPipeLine::GetInstance().AddMeshData(planeMeshData, L"HitGroup", "NumberObject0", PhysicsBaseMaterial(SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f), SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 0.0f), 0.5f), "Number0");
	DXRPipeLine::GetInstance().AddMeshData(planeMeshData, L"HitGroup", "NumberObject1", PhysicsBaseMaterial(SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f), SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 0.0f), 0.5f), "Number1");
	DXRPipeLine::GetInstance().AddMeshData(planeMeshData, L"HitGroup", "NumberObject2", PhysicsBaseMaterial(SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f), SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 0.0f), 0.5f), "Number2");
	DXRPipeLine::GetInstance().AddMeshData(planeMeshData, L"HitGroup", "NumberObject3", PhysicsBaseMaterial(SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f), SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 0.0f), 0.5f), "Number3");
	DXRPipeLine::GetInstance().AddMeshData(planeMeshData, L"HitGroup", "NumberObject4", PhysicsBaseMaterial(SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f), SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 0.0f), 0.5f), "Number4");
	DXRPipeLine::GetInstance().AddMeshData(planeMeshData, L"HitGroup", "NumberObject5", PhysicsBaseMaterial(SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f), SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 0.0f), 0.5f), "Number5");
	DXRPipeLine::GetInstance().AddMeshData(planeMeshData, L"HitGroup", "NumberObject6", PhysicsBaseMaterial(SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f), SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 0.0f), 0.5f), "Number6");
	DXRPipeLine::GetInstance().AddMeshData(planeMeshData, L"HitGroup", "NumberObject7", PhysicsBaseMaterial(SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f), SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 0.0f), 0.5f), "Number7");
	DXRPipeLine::GetInstance().AddMeshData(planeMeshData, L"HitGroup", "NumberObject8", PhysicsBaseMaterial(SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f), SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 0.0f), 0.5f), "Number8");
	DXRPipeLine::GetInstance().AddMeshData(planeMeshData, L"HitGroup", "NumberObject9", PhysicsBaseMaterial(SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f), SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 0.0f), 0.5f), "Number9");
	
	DXRPipeLine::GetInstance().AddMeshData(cubeMeshData, L"HitGroup", "WhiteCube", PhysicsBaseMaterial(SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f), SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 1.0f), 0.1f));
	DXRPipeLine::GetInstance().AddMeshData(cubeMeshData, L"HitGroup", "GrayCube", PhysicsBaseMaterial(SimpleMath::Vector4(0.2f, 0.2f, 1.0f, 1.0f), SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 0.5f), 0.1));
	DXRPipeLine::GetInstance().AddMeshData(cubeMeshData, L"HitGroup", "RoughCube", PhysicsBaseMaterial(SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f), SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 0.0f), 0.5f));
	DXRPipeLine::GetInstance().AddMeshData(cubeMeshData, L"HitGroup", "RedCube", PhysicsBaseMaterial(SimpleMath::Vector4(0.2f, 0.0f, 0.0f, 1.0f), SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 0.0f), 0.5f));
	DXRPipeLine::GetInstance().AddMeshData(cubeMeshData, L"HitGroup", "ClearCube", PhysicsBaseMaterial(SimpleMath::Vector4(1.0f, 0.0f, 0.0f, 1.0f), SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 0.0f), 0.5f, 0.4f, 1.24f));

	DXRPipeLine::GetInstance().AddMeshData(sphereMeshData, L"HitGroup", "Sphere", PhysicsBaseMaterial(SimpleMath::Vector4(0.2f, 0.2f, 0.2f, 1.0f), SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 1.0f), 0.2f));
	DXRPipeLine::GetInstance().AddMeshData(sphereMeshData, L"HitGroup", "RoughSphere", PhysicsBaseMaterial(SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f), SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 0.0f), 0.5f));

	// 必ず、メッシュデータを追加してからパイプラインの初期化を行う。
	DXRPipeLine::GetInstance().InitPipeLine();



	auto dirlight = LightManager::GetInstance().GetDirectionalLight();
	auto dir = SimpleMath::Vector3(-0.169f, -0.840, -0.515);
	dirlight->UpdateDirectionalLight(dir, SimpleMath::Color(1, 1, 1, 1));

	m_CameraAsistant = new CameraAsistant();



	RenderingPipeLine::GetInstance().SetSkyBox("Resources/SpaceSkyBox.dds", SimpleMath::Vector3(10.0f));
	RenderingPipeLine::GetInstance().SetDrawFluidFlag(false);


	std::vector<bool> StaticObjectCollTable =
	{
		false,true,
	};

	std::vector<bool> DynamicObjectCollTable =
	{
		true,true
	};

	CollisionTagManagaer::GetInstance().AddTag("StaticObject", StaticObjectCollTable);
	CollisionTagManagaer::GetInstance().AddTag("DynamicObject", DynamicObjectCollTable);

	auto gameManager = new GameManager();
	ActorManager::GetInstance().AddActor(gameManager);


}

void MainGame::Update()
{
#ifdef _DEBUG
	m_CameraAsistant->Update();
#endif



}

void MainGame::Destroy()
{
}
