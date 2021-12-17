#pragma once
#include <memory>
#include <Game.h>

#include "Device/Raytracing/PhysicsBaseMaterial.h"

class Camera;
class Actor;
class CameraAsistant;
class FluidParticleAction;
class Sphere;
class Timer;
class ParticleEmitter;

class CollisionTreeManager;
class InkManager;
class DXRMeshData;

class MainGame
	:public Game
{
public:
	MainGame();
	~MainGame();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Destroy() override;

private:
	CameraAsistant* m_CameraAsistant;
};