#include "PhysxController.h"

namespace PhysxOgr {

	PhysxController* PhysxController::controller = NULL;

	PhysxController * PhysxOgr::PhysxController::getInstance(void)
	{
		if (!controller) {
			controller = new PhysxController;
		}

		return controller;
	}

	PxScene * PhysxController::getScene(void)
	{
		return gScene;
	}

	PxPhysics * PhysxController::getPhysics(void)
	{
		return gPhysics;
	}

	PhysxOgr::PhysxController::PhysxController(void)
	{
		
	}

	PhysxOgr::PhysxController::~PhysxController(void)
	{
		gScene->release();
		gDispatcher->release();
		PxProfileZoneManager* profileZoneManager = gPhysics->getProfileZoneManager();
		if (gConnection != NULL)
			gConnection->release();
		gPhysics->release();
		profileZoneManager->release();
		gFoundation->release();
	}

	void PhysxOgr::PhysxController::init(void)
	{
		gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);
		PxProfileZoneManager* profileZoneManager = &PxProfileZoneManager::createProfileZoneManager(gFoundation);
		gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true, profileZoneManager);

		if (gPhysics->getPvdConnectionManager())
		{
			gPhysics->getVisualDebugger()->setVisualizeConstraints(true);
			gPhysics->getVisualDebugger()->setVisualDebuggerFlag(PxVisualDebuggerFlag::eTRANSMIT_CONTACTS, true);
			gPhysics->getVisualDebugger()->setVisualDebuggerFlag(PxVisualDebuggerFlag::eTRANSMIT_SCENEQUERIES, true);
			gConnection = PxVisualDebuggerExt::createConnection(gPhysics->getPvdConnectionManager(), PVD_HOST, 5425, 10);
		}

		PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
		sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
		gDispatcher = PxDefaultCpuDispatcherCreate(2);
		sceneDesc.cpuDispatcher = gDispatcher;
		sceneDesc.filterShader = PxDefaultSimulationFilterShader;
		gScene = gPhysics->createScene(sceneDesc);
	}
}
