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
	TextureManager::GetInstance().AddTexture("Resources/Texture/TestUI.png", "TestUI");
	TextureManager::GetInstance().AddTexture("Resources/Texture/ScoreResult.png", "ScoreResult");
	TextureManager::GetInstance().AddTexture("Resources/Texture/TitleLogo.png", "TitleLogo");
	TextureManager::GetInstance().AddTexture("Resources/Texture/ControllUI_Key.png", "ControllUI_Key");
	TextureManager::GetInstance().AddTexture("Resources/Texture/ControllUI_Pad.png", "ControllUI_Pad");
	TextureManager::GetInstance().AddTexture("Resources/Texture/BreakTitleUI.png", "BreakTitleUI");
	TextureManager::GetInstance().AddTexture("Resources/Texture/FoundUI.png", "FoundUI");

	TextureManager::GetInstance().AddTexture("Resources/Texture/Number0.png", "Number0");
	TextureManager::GetInstance().AddTexture("Resources/Texture/Number1.png", "Number1");
	TextureManager::GetInstance().AddTexture("Resources/Texture/Number2.png", "Number2");
	TextureManager::GetInstance().AddTexture("Resources/Texture/Number3.png", "Number3");
	TextureManager::GetInstance().AddTexture("Resources/Texture/Number4.png", "Number4");
	TextureManager::GetInstance().AddTexture("Resources/Texture/Number5.png", "Number5");
	TextureManager::GetInstance().AddTexture("Resources/Texture/Number6.png", "Number6");
	TextureManager::GetInstance().AddTexture("Resources/Texture/Number7.png", "Number7");
	TextureManager::GetInstance().AddTexture("Resources/Texture/Number8.png", "Number8");
	TextureManager::GetInstance().AddTexture("Resources/Texture/Number9.png", "Number9");

	auto cubeMeshData = MeshManager::GetInstance().GetMeshData("CubeModelData");
	cubeMeshData->SetPhysicsBaseMaterial(PhysicsBaseMaterial(SimpleMath::Vector4(1.0f, 0.0f, 0.0f, 1.0f), SimpleMath::Vector4(1.0f, 10.0f, 1.0f, 0.5f), 0.5f));

	auto sphereMeshData = MeshManager::GetInstance().FindSpehere(12);
	auto planeMeshData = MeshManager::GetInstance().GetPlaneMeshData();

	auto blenderMonkyMeshData = MeshManager::GetInstance().GetMeshData("BlenderMonkey");


	//sphereMeshData->SetTestMaterial(MeshData::TestMat{ true });

	DXRPipeLine::GetInstance().AddMeshData(planeMeshData, L"HitGroup", "TestUI", PhysicsBaseMaterial(SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f), SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 0.0f), 0.5f), "TestUI");
	DXRPipeLine::GetInstance().AddMeshData(planeMeshData, L"HitGroup", "ScoreResult", PhysicsBaseMaterial(SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f), SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 0.0f), 0.5f), "ScoreResult");
	DXRPipeLine::GetInstance().AddMeshData(planeMeshData, L"HitGroup", "TitleLogo", PhysicsBaseMaterial(SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f), SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 0.0f), 0.5f), "TitleLogo");
	DXRPipeLine::GetInstance().AddMeshData(planeMeshData, L"HitGroup", "ControllUI_Key", PhysicsBaseMaterial(SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f), SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 0.0f), 0.5f), "ControllUI_Key");
	DXRPipeLine::GetInstance().AddMeshData(planeMeshData, L"HitGroup", "ControllUI_Pad", PhysicsBaseMaterial(SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f), SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 0.0f), 0.5f), "ControllUI_Pad");
	DXRPipeLine::GetInstance().AddMeshData(planeMeshData, L"HitGroup", "BreakTitleUI", PhysicsBaseMaterial(SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f), SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 0.0f), 0.5f), "BreakTitleUI");
	DXRPipeLine::GetInstance().AddMeshData(planeMeshData, L"HitGroup", "FoundUI", PhysicsBaseMaterial(SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f), SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 0.0f), 0.5f), "FoundUI");

	DXRPipeLine::GetInstance().AddMeshData(planeMeshData, L"HitGroup", "NumberObject0", PhysicsBaseMaterial(SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f), SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 0.0f), 0.5f,0.8f), "Number0");
	DXRPipeLine::GetInstance().AddMeshData(planeMeshData, L"HitGroup", "NumberObject1", PhysicsBaseMaterial(SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f), SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 0.0f), 0.5f, 0.8f), "Number1");
	DXRPipeLine::GetInstance().AddMeshData(planeMeshData, L"HitGroup", "NumberObject2", PhysicsBaseMaterial(SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f), SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 0.0f), 0.5f, 0.8f), "Number2");
	DXRPipeLine::GetInstance().AddMeshData(planeMeshData, L"HitGroup", "NumberObject3", PhysicsBaseMaterial(SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f), SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 0.0f), 0.5f, 0.8f), "Number3");
	DXRPipeLine::GetInstance().AddMeshData(planeMeshData, L"HitGroup", "NumberObject4", PhysicsBaseMaterial(SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f), SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 0.0f), 0.5f, 0.8f), "Number4");
	DXRPipeLine::GetInstance().AddMeshData(planeMeshData, L"HitGroup", "NumberObject5", PhysicsBaseMaterial(SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f), SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 0.0f), 0.5f, 0.8f), "Number5");
	DXRPipeLine::GetInstance().AddMeshData(planeMeshData, L"HitGroup", "NumberObject6", PhysicsBaseMaterial(SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f), SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 0.0f), 0.5f, 0.8f), "Number6");
	DXRPipeLine::GetInstance().AddMeshData(planeMeshData, L"HitGroup", "NumberObject7", PhysicsBaseMaterial(SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f), SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 0.0f), 0.5f, 0.8f), "Number7");
	DXRPipeLine::GetInstance().AddMeshData(planeMeshData, L"HitGroup", "NumberObject8", PhysicsBaseMaterial(SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f), SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 0.0f), 0.5f, 0.8f), "Number8");
	DXRPipeLine::GetInstance().AddMeshData(planeMeshData, L"HitGroup", "NumberObject9", PhysicsBaseMaterial(SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f), SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 0.0f), 0.5f, 0.8f), "Number9");
	
	DXRPipeLine::GetInstance().AddMeshData(blenderMonkyMeshData, L"HitGroup", "BlenderMonkeyMirror", PhysicsBaseMaterial(SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f), SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 0.0f), 0.5f, 0.1f, 1.24f));
	DXRPipeLine::GetInstance().AddMeshData(blenderMonkyMeshData, L"HitGroup", "BlenderMonkeyClear", PhysicsBaseMaterial(SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f), SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 0.0f), 0.5f, 0.1f, 1.24f));
	DXRPipeLine::GetInstance().AddMeshData(sphereMeshData, L"HitGroup", "ClearSphere", PhysicsBaseMaterial(SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 1.0f), SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 0.0f), 0.1f, 0.1f, 2.24));
	DXRPipeLine::GetInstance().AddMeshData(cubeMeshData, L"HitGroup", "ClearCube", PhysicsBaseMaterial(SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 1.0f), SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 0.0f), 0.1f, 0.1f, 2.24));

	DXRPipeLine::GetInstance().AddMeshData(cubeMeshData, L"HitGroup", "WhiteCube", PhysicsBaseMaterial(SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f), SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 1.0f), 0.1f));
	DXRPipeLine::GetInstance().AddMeshData(cubeMeshData, L"HitGroup", "RoughCube", PhysicsBaseMaterial(SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f), SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 0.0f), 0.5f));
	DXRPipeLine::GetInstance().AddMeshData(cubeMeshData, L"HitGroup", "RedCube", PhysicsBaseMaterial(SimpleMath::Vector4(0.8f, 0.0f, 0.0f, 1.0f), SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 0.0f), 0.5f));
	DXRPipeLine::GetInstance().AddMeshData(cubeMeshData, L"HitGroup", "BlueCube", PhysicsBaseMaterial(SimpleMath::Vector4(0.0f, 0.0f, 0.4f, 1.0f), SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 0.0f), 0.5f));
	DXRPipeLine::GetInstance().AddMeshData(cubeMeshData, L"HitGroup", "BlackCube", PhysicsBaseMaterial(SimpleMath::Vector4(0.082f,0.054,0.082, 1.0f), SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 0.0f), 0.5f));
	DXRPipeLine::GetInstance().AddMeshData(cubeMeshData, L"HitGroup", "RedClearCube", PhysicsBaseMaterial(SimpleMath::Vector4(1.0f, 0.0f, 0.0f, 1.0f), SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 0.0f), 0.5f, 0.4f, 1.24f));

	DXRPipeLine::GetInstance().AddMeshData(cubeMeshData, L"HitGroup", "RedMirrorCube", PhysicsBaseMaterial(SimpleMath::Vector4(1.0f, 0.0f, 0.0f, 1.0f), SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 0.5f), 0.2f));


	DXRPipeLine::GetInstance().AddMeshData(sphereMeshData, L"HitGroup", "Sphere", PhysicsBaseMaterial(SimpleMath::Vector4(0.2f, 0.2f, 0.2f, 1.0f), SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 1.0f), 0.2f));
	DXRPipeLine::GetInstance().AddMeshData(sphereMeshData, L"HitGroup", "RoughSphere", PhysicsBaseMaterial(SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f), SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 0.0f), 0.5f));

	// 必ず、メッシュデータを追加してからパイプラインの初期化を行う。
	DXRPipeLine::GetInstance().InitPipeLine();



	auto dirlight = LightManager::GetInstance().GetDirectionalLight();
	auto dir = SimpleMath::Vector3(0.266f, -0.443f, -0.868f);
	dirlight->UpdateDirectionalLight(dir, SimpleMath::Color(1, 1, 1, 1));

	m_CameraAsistant = new CameraAsistant();



	RenderingPipeLine::GetInstance().SetSkyBox("Resources/Texture/SpaceSkyBox.dds", SimpleMath::Vector3(10.0f));
	RenderingPipeLine::GetInstance().SetDrawFluidFlag(false);



	std::vector<bool> PlayerObjectCollTable =
	{
		false,true,true
	};

	std::vector<bool> TargetObjectCollTable =
	{
		true,false,true
	};

	std::vector<bool> AllHitObjectCollTable =
	{
		true,true,false
	};

	CollisionTagManagaer::GetInstance().AddTag("PlayerObject", PlayerObjectCollTable);
	CollisionTagManagaer::GetInstance().AddTag("TargetObject", TargetObjectCollTable);
	CollisionTagManagaer::GetInstance().AddTag("AllHitObject", AllHitObjectCollTable);

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
