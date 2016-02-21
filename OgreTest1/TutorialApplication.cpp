/*
-----------------------------------------------------------------------------
Filename:    TutorialApplication.cpp
-----------------------------------------------------------------------------

This source file is part of the
   ___                 __    __ _ _    _
  /___\__ _ _ __ ___  / / /\ \ (_) | _(_)
 //  // _` | '__/ _ \ \ \/  \/ / | |/ / |
/ \_// (_| | | |  __/  \  /\  /| |   <| |
\___/ \__, |_|  \___|   \/  \/ |_|_|\_\_|
      |___/
Tutorial Framework (for Ogre 1.9)
http://www.ogre3d.org/wiki/
-----------------------------------------------------------------------------
*/

#include "TutorialApplication.h"

//---------------------------------------------------------------------------
TutorialApplication::TutorialApplication(void)
{
}
//---------------------------------------------------------------------------
TutorialApplication::~TutorialApplication(void)
{
}

//---------------------------------------------------------------------------
void TutorialApplication::createScene(void)
{
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0, 0, 0));
	mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

	if (ARTOOLKITON) {
		crearMiniScreen();
	}
	
	crearEscenario();
	//Bola* bola = new Bola(Ogre::ColourValue::White, 1, Ogre::Vector3(0, 300, 0));
	mBolasController->init();
}
//---------------------------------------------------------------------------

void TutorialApplication::crearMiniScreen(void)
{
	mMiniscreen = new Ogre::Rectangle2D(true);
	mMiniscreen->setCorners(.5, 1.0, 1.0, .5);
	mMiniscreen->setBoundingBox(Ogre::AxisAlignedBox::BOX_INFINITE);
	mARToolkit->procesarFrame();
	mMiniscreen->setMaterial("DynamicTextureMaterial");
	mMiniscreen->setCastShadows(false);

	Ogre::SceneNode* miniscreenNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	miniscreenNode->attachObject(mMiniscreen);
}
//---------------------------------------------------------------------------

void TutorialApplication::crearEscenario(void)
{
	Ogre::Light* luzPrincipal = mSceneMgr->createLight();
	luzPrincipal->setType(Ogre::Light::LT_DIRECTIONAL);
	luzPrincipal->setPosition(Ogre::Vector3(0, 100, 0));
	luzPrincipal->setDirection(Ogre::Vector3(0, -1, 0));
	luzPrincipal->setDiffuseColour(Ogre::ColourValue(1, 1, 1));
	luzPrincipal->setSpecularColour(Ogre::ColourValue(1, 1, 1));

	Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
	Ogre::MeshManager::getSingleton().createPlane("ground", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		plane, 254, 127, 10, 10, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);

	Ogre::Entity* groundEntity = mSceneMgr->createEntity("ground");
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(groundEntity);
	groundEntity->setCastShadows(false);
	groundEntity->setMaterialName("Custom/TelaBillar");

	PxPhysics* gPhysics = PhysxOgr::PhysxController::getInstance()->getPhysics();
	PxRigidStatic* groundPlane = PxCreatePlane(*gPhysics, PxPlane(0, 1, 0, 0), *gPhysics->createMaterial(0.5f, 0.5f, 0.6f));
	mPhysx->getScene()->addActor(*groundPlane);
	
	crearBorde(Ogre::Vector3(-132, 0, 0), Ogre::Vector3(0.1, 0.1, 1.27));
	crearBorde(Ogre::Vector3(132, 0, 0), Ogre::Vector3(0.1, 0.1, 1.27));
	crearBorde(Ogre::Vector3(0, 0, 68.5), Ogre::Vector3(2.54, 0.1, 0.1));
	crearBorde(Ogre::Vector3(0, 0, -68.5), Ogre::Vector3(2.54, 0.1, 0.1));
}
//---------------------------------------------------------------------------

void TutorialApplication::crearBorde(Ogre::Vector3 posicion, Ogre::Vector3 escala)
{
	Ogre::Entity *borde = mSceneMgr->createEntity("cube.mesh");
	borde->setMaterialName("Custom/TelaBillar");
	borde->setCastShadows(false);

	Ogre::SceneNode *nodoBorde = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	nodoBorde->setPosition(posicion);
	nodoBorde->attachObject(borde);
	nodoBorde->scale(escala);
}
//---------------------------------------------------------------------------

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
    int main(int argc, char *argv[])
#endif
    {
        // Create application object
        TutorialApplication app;

        try {
            app.go();
        } catch(Ogre::Exception& e)  {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox(NULL, e.getFullDescription().c_str(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
            std::cerr << "An exception has occurred: " <<
                e.getFullDescription().c_str() << std::endl;
#endif
        }

        return 0;
    }

#ifdef __cplusplus
}
#endif

//---------------------------------------------------------------------------
