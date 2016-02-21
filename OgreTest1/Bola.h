#ifndef __Bola_h_
#define __Bola_h_

#include "Ogre.h"
#include "PhysxController.h"

#define RADIO 2.86

class Bola {
public:
	Bola(void);
	Bola(Ogre::ColourValue color, int numero, Ogre::Vector3 posicion);
	void dibujar(void);
	void update(void);

private:
	int numero;
	Ogre::ColourValue color;
	Ogre::SceneNode* nodoBola;
	PxRigidDynamic* body;
};

#endif;
