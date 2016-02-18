#ifndef __PhysxController_h_
#define __PhysxController_h_

#include "PxPhysicsAPI.h"

#define PVD_HOST "127.0.0.1"

using namespace physx;

namespace PhysxOgr {
	class PhysxController
	{
	public:
		PhysxController(void);
		~PhysxController(void);
		static PhysxController *getInstance(void);
		PxScene *getScene(void);
		PxPhysics *getPhysics(void);
		void init(void);

	private:
		static PhysxController *controller;

		PxFoundation* gFoundation = NULL;
		PxPhysics* gPhysics = NULL;
		PxDefaultAllocator gAllocator;
		PxDefaultErrorCallback gErrorCallback;
		PxDefaultCpuDispatcher*	gDispatcher = NULL;
		PxScene* gScene = NULL;
		PxVisualDebuggerConnection* gConnection = NULL;
	};
}
#endif