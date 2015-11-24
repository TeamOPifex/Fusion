//////////////////////////////////////
// Application Entry Point
//////////////////////////////////////
#include "./OPengine.h"

//////////////////////////////////////
// Application Methods
//////////////////////////////////////

void ApplicationInit() {
	OPchar* assetDir = NULL;
#ifdef OPIFEX_ASSETS
	assetDir = OPIFEX_ASSETS;
#endif
	OPloadersAddDefault();
	OPcmanInit(assetDir);

	OPrenderInit();
}

int ApplicationUpdate(OPtimer* timer) {
	OPrenderClear(0,0,0);
	OPrenderPresent();
	return 0;
}

void ApplicationDestroy() {

}

void ApplicationSetup() {
	OPinitialize = ApplicationInit;
	OPupdate = ApplicationUpdate;
	OPdestroy = ApplicationDestroy;
}

//////////////////////////////////////
// Application Entry Point
//////////////////////////////////////
OP_MAIN {
	OPlog("Starting up OPifex Engine");

	OPinitialize = ApplicationInit;
	OPupdate = ApplicationUpdate;
	OPdestroy = ApplicationDestroy;

	OP_MAIN_START
	OP_MAIN_END
	OP_MAIN_SUCCESS
}