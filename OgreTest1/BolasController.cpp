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
	float posx, posz = 0;
	int numBola = 0;

	for (int fila = 0; fila < 5; fila++) {
		posx = ((RADIO*2) - 0.75) * fila;
		posz = -RADIO * fila;
		for (int bola = 0; bola <= fila; bola++) {
			Bola* nuevaBola = new Bola(Ogre::ColourValue::Red, numBola, Ogre::Vector3(posx, RADIO, posz));
			posz += RADIO*2;
			vectorBolas[numBola] = nuevaBola;
			numBola++;
		}
	}
}

void BolasController::actualizarBolas(void)
{
	for (int i = 0; i < NUM_BOLAS; i++) {
		vectorBolas[i]->update();
	}
}
