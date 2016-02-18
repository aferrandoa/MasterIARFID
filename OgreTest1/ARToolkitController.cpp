#include "ARToolkitController.h"

namespace ARToolkit {

	static void mainFunc(void) {
		int a = 1;
	}

	ARToolkitController* ARToolkitController::controller = NULL;

	ARToolkitController* ARToolkitController::getInstance(void) {
		if (!controller) {
			controller = new ARToolkitController;
		}

		return controller;
	}

	ARToolkitController::ARToolkitController(void)
	{

	}

	ARToolkitController::~ARToolkitController(void)
	{
		arVideoCapStop();
		argCleanup();
		arPattDetach(arHandle);
		arPattDeleteHandle(arPattHandle);
		ar3DDeleteHandle(&ar3DHandle);
		arDeleteHandle(arHandle);
		arParamLTFree(&gCparamLT);
		arVideoClose();
	}

	void ARToolkitController::init(void)
	{
		ARParam         cparam;
		char            vconf[512];
		AR_PIXEL_FORMAT pixFormat;
		ARUint32        id0, id1;

		vconf[0] = '\0';

		
		/* open the video path */
		ARLOGi("Using video configuration '%s'.\n", vconf);
		if (arVideoOpen(vconf) < 0) exit(0);
		if (arVideoGetSize(&xsize, &ysize) < 0) exit(0);
		ARLOGi("Image size (x,y) = (%d,%d)\n", xsize, ysize);
		if ((pixFormat = arVideoGetPixelFormat()) < 0) exit(0);
		if (arVideoGetId(&id0, &id1) == 0) {
			ARLOGi("Camera ID = (%08x, %08x)\n", id1, id0);
			sprintf_s(vconf, VPARA_NAME, id1, id0);
			//sprintf(vconf, VPARA_NAME, id1, id0);
			if (arVideoLoadParam(vconf) < 0) {
				ARLOGe("No camera setting data!!\n");
			}
		}

		/* set the initial camera parameters */
		if (arParamLoad(CPARA_NAME, 1, &cparam) < 0) {
			ARLOGe("Camera parameter load error !!\n");
			exit(0);
		}
		arParamChangeSize(&cparam, xsize, ysize, &cparam);
		ARLOG("*** Camera Parameter ***\n");
		arParamDisp(&cparam);
		if ((gCparamLT = arParamLTCreate(&cparam, AR_PARAM_LT_DEFAULT_OFFSET)) == NULL) {
			ARLOGe("Error: arParamLTCreate.\n");
			exit(-1);
		}

		if ((arHandle = arCreateHandle(gCparamLT)) == NULL) {
			ARLOGe("Error: arCreateHandle.\n");
			exit(0);
		}
		if (arSetPixelFormat(arHandle, pixFormat) < 0) {
			ARLOGe("Error: arSetPixelFormat.\n");
			exit(0);
		}

		if ((ar3DHandle = ar3DCreateHandle(&cparam)) == NULL) {
			ARLOGe("Error: ar3DCreateHandle.\n");
			exit(0);
		}

		if ((arPattHandle = arPattCreateHandle()) == NULL) {
			ARLOGe("Error: arPattCreateHandle.\n");
			exit(0);
		}
		if ((patt_id = arPattLoad(arPattHandle, PATT_NAME)) < 0) {
			ARLOGe("pattern load error !!\n");
			exit(0);
		}
		arPattAttach(arHandle, arPattHandle);

		if (arVideoCapStart() != 0) {
			ARLOGe("video capture start error !!\n");
			exit(0);
		}

		arUtilTimerReset();
	}

	void ARToolkitController::procesarFrame(void)
	{
		static ARUint8 *dataPtr = NULL;

		/* grab a video frame */
		if ((dataPtr = (ARUint8 *)arVideoGetImage()) == NULL) {
			arUtilSleep(2);
			return;
		}

		// Create the texture
		Ogre::TexturePtr texture;
		texture = Ogre::TextureManager::getSingleton().getByName("DynamicTexture");
		
		if (texture.isNull()) {
			texture = Ogre::TextureManager::getSingleton().createManual(
				"DynamicTexture", // name
				Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
				Ogre::TEX_TYPE_2D,      // type
				xsize, ysize,         // width & height
				0,                // number of mipmaps
				Ogre::PF_BYTE_BGRA,     // pixel format
				Ogre::TU_DYNAMIC_WRITE_ONLY_DISCARDABLE);      // usage; should be TU_DYNAMIC_WRITE_ONLY_DISCARDABLE for
															   // textures updated very often (e.g. each frame)
		}
			
		// Get the pixel buffer
		Ogre::HardwarePixelBufferSharedPtr pixelBuffer = texture->getBuffer();

		// Lock the pixel buffer and get a pixel box
		pixelBuffer->lock(Ogre::HardwareBuffer::HBL_DISCARD); // for best performance use HBL_DISCARD!
		const Ogre::PixelBox& pixelBox = pixelBuffer->getCurrentLock();

		Ogre::uint8* pDest = static_cast<Ogre::uint8*>(pixelBox.data);

		// Fill in some pixel data. This will give a semi-transparent blue,
		// but this is of course dependent on the chosen pixel format.
		for (size_t j = 0; j < xsize; j++)
		{
			for (size_t i = 0; i < ysize; i++)
			{
				*pDest++ = *dataPtr++; // B
				*pDest++ = *dataPtr++; // G
				*pDest++ = *dataPtr++; // R
				*pDest++ = 255; // A
			}

			pDest += pixelBox.getRowSkip() * Ogre::PixelUtil::getNumElemBytes(pixelBox.format);
		}

		// Unlock the pixel buffer
		pixelBuffer->unlock();

		// Create a material using the texture
		Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName("DynamicTextureMaterial");

		if(material.isNull()){
			material = Ogre::MaterialManager::getSingleton().create(
				"DynamicTextureMaterial", // name
				Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		}
		
		material->getTechnique(0)->getPass(0)->removeAllTextureUnitStates();
		material->getTechnique(0)->getPass(0)->setEmissive(Ogre::ColourValue::White);
		material->getTechnique(0)->getPass(0)->createTextureUnitState("DynamicTexture");
		//material->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
	}
}