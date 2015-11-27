#pragma once

#include "./OPengine.h"

struct OPmeshRenderer {
	OPmaterial* material;
	ui16 meshCount;
	ui16 meshIndex;
	OPmesh** meshes;
	OPmat4* world;
	void** data;
};

void OPmeshRendererInit(OPmeshRenderer* result, ui16 count, OPmaterial* material);
OPmeshRenderer* OPmeshRendererCreate(ui16 count, OPmaterial* material);
void OPmeshRendererDestroy(OPmeshRenderer* result);
void OPmeshRendererFree(OPmeshRenderer* result);
ui16 OPmeshRendererAdd(OPmeshRenderer* renderer, OPmesh* mesh);
void OPmeshRendererDraw(OPmeshRenderer* renderer, OPcam camera);