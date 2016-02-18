#ifndef __ARToolkitController_h_
#define __ARToolkitController_h_

#include <AR/ar.h>
#include <AR/gsub.h>
#include <AR/video.h>
#include <Ogre.h>

#define CPARA_NAME "Data/camera_para.dat"
#define VPARA_NAME "Data/cameraSetting-%08x%08x.dat"
#define PATT_NAME  "Data/patt.hiro"

namespace ARToolkit {

	class ARToolkitController
	{
	public:
		ARToolkitController(void);
		~ARToolkitController(void);
		static ARToolkitController *getInstance(void);
		void init(void);
		void procesarFrame(void);

	private:
		static ARToolkitController *controller;
		int xsize, ysize;
		int patt_id;
		ARHandle *arHandle;
		ARPattHandle *arPattHandle;
		AR3DHandle *ar3DHandle;
		ARGViewportHandle *vp;
		ARParamLT *gCparamLT = NULL;
	};
}
#endif // #ifndef __ARToolkitController_h_