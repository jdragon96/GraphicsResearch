#pragma once

#include <vector>
#include <unordered_map>

#include "engine/Macro.h"
#include "engine/Dx11MeshBuffer.h"
#include "engine/common/VertexData.h"
#include "engine/dx11/Dx11EngineManager.h"

#include "physx/PxActor.h"
#include "physx/PxPhysicsAPI.h"

class Dx11MemoryManager
{
public:
	SINGLETON(Dx11MemoryManager);

	std::unordered_map<physx::PxActor*, MeshBuffer<VertexData>::SharedPtr> EnemyHash;
	void RemoveEnemy()
	{
		for (auto pxActor : EngineManager::instance().simulationCallback.actorsToRemove)
		{
			if (EnemyHash.find(pxActor) != EnemyHash.end())
			{
				EnemyHash.erase(pxActor);
				EngineManager::instance().gScene->removeActor(*pxActor);
				pxActor->release();
			}
		}
		EngineManager::instance().simulationCallback.actorsToRemove.clear();
	}
};