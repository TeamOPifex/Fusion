#include "./Game/inc/MeshRenderer.h"

void OPmeshRendererInit(OPmeshRenderer* result, ui16 count, OPmaterial* material) {
	result->material = material;
	result->meshCount = count;
	result->meshIndex = 0;
	result->meshes = (OPmesh**)OPalloc(sizeof(OPmesh) * count);
	result->world = (OPmat4*)OPalloc(sizeof(OPmat4) * count);
	result->data = (void**)OPalloc(sizeof(void*) * count);
}

OPmeshRenderer* OPmeshRendererCreate(ui16 count, OPmaterial* material) {
	OPmeshRenderer* result = (OPmeshRenderer*)OPalloc(sizeof(OPmeshRenderer));
	OPmeshRendererInit(result, count, material);
	return result;
}

void OPmeshRendererDestroy(OPmeshRenderer* result) {
	OPfree(result->meshes);
	OPfree(result->world);
	OPfree(result->data);
}

void OPmeshRendererFree(OPmeshRenderer* result) {
	OPmeshRendererDestroy(result);
	OPfree(result);
}

ui16 OPmeshRendererAdd(OPmeshRenderer* renderer, OPmesh* mesh) {
	renderer->meshes[renderer->meshIndex] = mesh;
	return renderer->meshIndex++;
}

void OPmeshRendererDraw(OPmeshRenderer* renderer, OPcam camera) {
	if (renderer->meshIndex == 0) return;

	OPmeshBind(renderer->meshes[0]);
	OPmaterialBind(renderer->material);
	OPeffectParam(camera);
	for (ui16 i = 0; i < renderer->meshIndex; i++) {
		OPmeshBind(renderer->meshes[i]);
		OPeffectBind(renderer->material->effect);
		OPeffectParam("uWorld", renderer->world[i]);
		OPmeshRender();
	}
}