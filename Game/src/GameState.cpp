#include "./Game/inc/GameState.h"
#include "./Human/include/Rendering/Primitives/OPvertex.h"

OPmodel model;
OPmesh mesh;
OPmaterial material;
OPeffect effect;
OPcam camera;

OPvec3 pos;

void GameStateEnter(OPgameState* last) {
	OPmeshBuilder builder;
	OPmeshBuilderInit(&builder, sizeof(OPvertexColor));
	OPvertexColor verts[4];

	verts[0].Color = OPvec3Create(1, 0, 0);
	verts[1].Color = OPvec3Create(1, 0, 0);
	verts[2].Color = OPvec3Create(1, 0, 0);
	verts[3].Color = OPvec3Create(1, 0, 0);
	verts[0].Position = OPvec3Create(0.45, 0, -0.45);
	verts[1].Position = OPvec3Create(-0.45, 0, -0.45);
	verts[2].Position = OPvec3Create(-0.45, 0, 0.45);
	verts[3].Position = OPvec3Create(0.45, 0, 0.45);

	OPmeshBuilderAdd(&builder, &verts[0], &verts[1], &verts[2], &verts[3]);
	mesh = OPmeshBuilderGen(&builder);
	OPmeshBuilderDestroy(&builder);
	//mesh = OPquadCreate(1, 1);
	//mesh = OPcubeCreate(OPvec3Create(1, 0, 0));
	model.mesh = &mesh;
	model.world = OPmat4Translate(0, 10, 0);

	effect = OPeffectGen(
		"ColoredModel.vert",
		"ColoredModel.frag",
		OPATTR_POSITION | OPATTR_COLOR,
		"Model Effect",
		mesh.VertexSize);

	OPmaterialInit(&material, &effect);

	camera = OPcamPersp(
		OPVEC3_ONE * 20.0,
		OPVEC3_UP,
		OPVEC3_UP,
		0.1f,
		1000.0f,
		45.0f,
		OPRENDER_WIDTH / (f32)OPRENDER_HEIGHT
		);

	OPrenderDepth(1);

	pos = OPvec3Create(-5, 0, 0);
}

OPint GameStateUpdate(OPtimer* time) {
	OPrenderClear(0.3, 0.3, 0.3, 1);
	
	//OPmodelDraw(model, &material, camera);

	OPmat4 world = OPmat4RotY(0);
	OPvec3 lightDirection = OPVEC3_UP;

	OPmeshBind(&mesh);
	OPeffectBind(&effect);
	// Sets the vLightDirection uniform on the Effect that is currently bound (modelExample->Effect)
	OPeffectParamVec3("vLightDirection", &lightDirection);


	OPmat4 view, proj;
	OPmat4Identity(&world);
	//OPmat4BuildRotX(&world,- OPpi / 2.0);

	OPcamGetView(camera, &view);
	OPcamGetProj(camera, &proj);

	OPeffectParamMat4("uView", &view);
	OPeffectParamMat4("uProj", &proj);

	pos.x += time->Elapsed * 0.001f;
	if (pos.x > 0) pos.x = 0;

	OPray3D ray = { 0, 0, 0, 0, 0, 0 };
	OPvec3 positionHit = { 0, 0, 0 };
	OPint intersecting = 0;
	if (OPmouseIsDown(OPMOUSE_LBUTTON)) {

		ray = OPcamUnproject(
			&camera,
			OPmousePositionX(),
			OPmousePositionY()
			);

	}

	intersecting = 0;
	OPint hit = 0;
	if (OPmouseIsDown(OPMOUSE_LBUTTON)) {
		OPplane3D plane = {
			OPvec3Create(0, 0, 0),
			OPvec3Create(0, 1, 0)
		};
		if (OPplane3DIntersects(plane, ray, &positionHit)) {
			intersecting = 1;
			hit = 1;
		}
	}

	for (OPuint i = 0; i < 10; i++) {
		for (OPuint j = 0; j < 10; j++) {
			intersecting = 0;
			if (hit
				&& positionHit.x > i - 0.5 + pos.x
				&& positionHit.x < i + 0.5 + pos.x
				&& positionHit.z > j - 0.5
				&& positionHit.z < j + 0.5) {
				intersecting = 1;
			}

			if(!intersecting) {
				world = OPmat4Translate(i + pos.x, 0, j);
				OPeffectParamMat4("uWorld", &world);
				OPmeshRender();
			}
		}
	}

	OPrenderPresent();

	return false;
}

OPint GameStateExit(OPgameState* next) {
	return 0;
}

OPgameState GameState = {
	GameStateEnter,
	GameStateUpdate,
	GameStateExit
};