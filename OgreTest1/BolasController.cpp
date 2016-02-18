#include "BolasController.h"

BolasController* BolasController::mController = NULL;

BolasController * BolasController::getInstance(void)
{
	if (!mController) {
		mController = new BolasController;
	}

	return mController;
}

BolasController::BolasController(void)
{

}

BolasController::~BolasController(void)
{

}

void BolasController::init(void)
{
	int posx, posz = 0;
	int numBola = 0;

	for (int fila = 0; fila < 5; fila++) {
		posx = 44 * fila;
		posz = -25 * fila;
		for (int bola = 0; bola <= fila; bola++) {
			Bola* nuevaBola = new Bola(Ogre::ColourValue::Red, numBola, Ogre::Vector3(posx, 25, posz));
			posz += 50;
			vectorBolas[numBola] = nuevaBola;
			numBola++;
		}
	}
}
