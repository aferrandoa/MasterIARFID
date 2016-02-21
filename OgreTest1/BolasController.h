#ifndef __BolasController_h_
#define __BolasController_h_

#include "Bola.h"

#define NUM_BOLAS 15

class BolasController {

public:
	BolasController(void);
	~BolasController(void);

	static BolasController* getInstance(void);
	void init(void);
	void actualizarBolas(void);

private:
	static BolasController *mController;
	Bola *vectorBolas[NUM_BOLAS];

};

#endif