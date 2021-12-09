#include "Tunnel.h"


#include "Cube.h"

Tunnel::Tunnel(const SimpleMath::Vector3& pos, const SimpleMath::Vector3& scale)
	:_tunnelScale(scale)
{
	SetPosition(pos + SimpleMath::Vector3(0,0, scale.z));
}

void Tunnel::UpdateActor()
{
}

void Tunnel::Init()
{
	const int grid_size_x = 8;
	const int grid_size_z = 7;

	const int grid_Count = grid_size_x * grid_size_z;
	
	auto floor = new Cube(SimpleMath::Vector3(0, 0, 0), _tunnelScale, 300.0f, "RoughCube", false);
	floor->SetRotation(SimpleMath::Vector3(0, 0.240f, 0.0f));
	floor->SetActorName("Floor");
	floor->OnStatic(true, true);
	SetChild(floor);

	auto leftDownWall = new Cube(SimpleMath::Vector3(-16.0f, 9.0f, 0), _tunnelScale, 300.0f, "GrayCube", false);
	leftDownWall->SetRotation(SimpleMath::Vector3(0, 0.240f, -0.940f));
	leftDownWall->SetActorName("LeftDown_Wall");
	leftDownWall->SetTag("Wall");
	leftDownWall->OnStatic(true, true);
	SetChild(leftDownWall);

	auto rightDownWall = new Cube(SimpleMath::Vector3(16.0f, 9.0f, 0), _tunnelScale, 300.0f, "GrayCube", false);
	rightDownWall->SetRotation(SimpleMath::Vector3(0, 0.240f, -2.200f));
	rightDownWall->SetActorName("RightDown_Wall");
	rightDownWall->SetTag("Wall");
	rightDownWall->OnStatic(true, true);
	SetChild(rightDownWall);
	
	auto leftUPWall = new Cube(SimpleMath::Vector3(-10.0f, 24.0f, 0.0f), _tunnelScale, 300.0f, "GrayCube", false);
	leftUPWall->SetRotation(SimpleMath::Vector3(0, 0.240f, -2.488f));
	leftUPWall->SetActorName("LeftUpWall");
	leftUPWall->SetTag("Wall");
	leftUPWall->OnStatic(true, true);
	SetChild(leftUPWall);

	auto rightUpWall = new Cube(SimpleMath::Vector3(12.0f, 24.0f, 0.0f), _tunnelScale, 300.0f, "GrayCube", false);
	rightUpWall->SetRotation(SimpleMath::Vector3(0, 0.240f, 2.470f));
	rightUpWall->SetActorName("RightUpWall");
	rightUpWall->SetTag("Wall");
	rightUpWall->OnStatic(true, true);
	SetChild(rightUpWall);

}

void Tunnel::Shutdown()
{
}

void Tunnel::OnCollsion(Actor* other)
{
}
