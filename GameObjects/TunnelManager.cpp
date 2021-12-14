#include "TunnelManager.h"

#include <Game_Object/ActorManager.h>

#include "Tunnel.h"

TunnelManager::TunnelManager()
{
}


void TunnelManager::AddTask(std::function<Tunnel* (Actor* actor)> task)
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

	auto result = _createTasks.at(_generateIndex)(this);
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
