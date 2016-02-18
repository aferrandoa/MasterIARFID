#include "Bola.h"

Bola::Bola(void)
{
}

Bola::Bola(Ogre::ColourValue color, int numero, Ogre::Vector3 posicion)
{
	this->color = color;
	this->numero = numero;

	Ogre::SceneManager* sceneManager = Ogre::Root::getSingleton().getSceneManager("escena");
	Ogre::Entity *ent = sceneManager->createEntity("sphere.mesh");

	/*Ogre::MaterialPtr materialBola = Ogre::MaterialManager::getSingleton().create("MaterialBola", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	materialBola->setAmbient(color);
	ent->setMaterial(materialBola);*/
	ent->setMaterialName("Custom/BolaRoja");
	ent->setCastShadows(true);

	nodoBola = sceneManager->getRootSceneNode()->createChildSceneNode();
	nodoBola->setPosition(posicion);
	nodoBola->attachObject(ent);
	nodoBola->scale(Ogre::Vector3(0.25, 0.25, 0.25));

	PxPhysics* gPhysics = PhysxOgr::PhysxController::getInstance()->getPhysics();
	PxScene* gScene = PhysxOgr::PhysxController::getInstance()->getScene();
	PxMaterial* gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);
	PxShape* shape = gPhysics->createShape(PxSphereGeometry(25), *gMaterial);

	PxRigidDynamic* body = gPhysics->createRigidDynamic(PxTransform(PxVec3(posicion.x, 25, posicion.z)));
	body->attachShape(*shape);
	PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
	gScene->addActor(*body);

	shape->release();
}

void Bola::dibujar(void)
{
}
