#include "./Game/inc/GameState.h"
#include "./Human/include/Rendering/Primitives/OPvertex.h"

#include "./Game/inc/MeshRenderer.h"

OPmodel model;
OPmesh mesh;
OPmesh* turretMesh;
OPmesh* wallMesh;
OPmaterial material;
OPmaterial texMaterial;
OPeffect effect;
OPeffect texEffect;
OPcam camera;
OPtexture* texture;

OPvec3 pos;
OPfloat rotY = 0;
OPfloat rotX = 0;
OPvec3 camPos;
OPvec3 camTarg;
OPuint selected = 0;
f32 zoom = 10.0f;

struct GameModel {
	OPmodel model;
	OPvec3 position;
};
ui32 gameModelsIndex;
GameModel gameModels[10 * 10];

OPmeshRenderer renderer;

void GameStateEnter(OPgameState* last) {
	gameModelsIndex = 0;

	camPos = OPvec3Create(0, 5, 5);
	camTarg = OPVEC3_ZERO;

	turretMesh = (OPmesh*)OPcmanLoadGet("turret.opm");
	wallMesh = (OPmesh*)OPcmanLoadGet("wall.opm");
	texture = (OPtexture*)OPcmanLoadGet("turret.png");

	OPmeshBuilder builder;
	OPmeshBuilderInit(&builder, sizeof(OPvertexColor));

	OPvertexColor verts[4];
	verts[0].Color = verts[1].Color = verts[2].Color = verts[3].Color = OPvec3Create(0.6, 0.9, 0.6);
	verts[0].Position = OPvec3Create(0.45, 0, -0.45);
	verts[1].Position = OPvec3Create(-0.45, 0, -0.45);
	verts[2].Position = OPvec3Create(-0.45, 0, 0.45);
	verts[3].Position = OPvec3Create(0.45, 0, 0.45);

	OPmeshBuilderAdd(&builder, &verts[0], &verts[1], &verts[2], &verts[3]);
	mesh = OPmeshBuilderGen(&builder);
	OPmeshBuilderDestroy(&builder);

	model.mesh = &mesh;
	model.world = OPmat4Translate(0, 10, 0);


	effect = OPeffectGen(
		"ColoredModel.vert",
		"ColoredModel.frag",
		OPATTR_POSITION | OPATTR_COLOR,
		"Model Effect",
		mesh.VertexSize);

	texEffect = OPeffectGen(
		"Common/Texture3D.vert",
		"Common/Texture.frag",
		OPATTR_POSITION | OPATTR_NORMAL | OPATTR_UV,
		"Model Effect",
		turretMesh->VertexSize);


	OPmaterialInit(&material, &effect);

	OPmaterialInit(&texMaterial, &texEffect);
	OPmaterialAddParam(&texMaterial, "uTexture", texture);

	OPmeshRendererInit(&renderer, 10 * 10, &texMaterial);


	camera = OPcamPersp(
		OPVEC3_ONE * 20.0,
		OPVEC3_UP,
		OPVEC3_UP,
		0.1f,
		1000.0f,
		45.0f,
		OPRENDER_WIDTH / (f32)OPRENDER_HEIGHT
		);
	

	pos = OPvec3Create(-5, 0, 0);


	OPrenderDepth(1);
}

OPint GameStateUpdate(OPtimer* time) {

	/////////////////////
	// UPDATE
	/////////////////////

	if (OPkeyboardWasReleased(OPKEY_1)) {
		selected = 1;
		OPlog("Turret Selected");
	}
	if (OPkeyboardWasReleased(OPKEY_2)) {
		selected = 2;
		OPlog("Wall Selected");
	}

	// Move the grid
	pos.x += time->Elapsed * 0.001f;
	if (pos.x > 0) pos.x = 0;

	// Determine intersection point
	OPvec3 positionHit = { 0, 0, 0 };
	OPint intersecting = 0;
	if (OPmouseIsDown(OPMOUSE_LBUTTON) && selected > 0) {
		OPray3D ray = OPcamUnproject(
			&camera,
			OPmousePositionX(),
			OPmousePositionY()
			);
		OPplane3D plane = {
			OPvec3Create(0, 0, 0),
			OPvec3Create(0, 1, 0)
		};
		if (OPplane3DIntersects(plane, ray, &positionHit)) {
			OPint x = OPfloor(positionHit.x + 0.5 - pos.x);
			OPint z = OPfloor(positionHit.z + 0.5 - pos.z);
			if (x < 10 && x >= 0 && z >= 0 && z < 10) {
				ui8 found = 0;
				for (ui32 i = 0; i < gameModelsIndex; i++) {
					if (gameModels[i].position.x == x && gameModels[i].position.z == z) {
						found = 1; break;
					}
				}
				if (!found) {
					OPlog("Point at %d, %d", x, z);
					ui16 ind;
					if (selected == 1) {
						gameModels[gameModelsIndex].model.mesh = turretMesh;
						ind = OPmeshRendererAdd(&renderer, turretMesh);
					}
					else if (selected == 2) {
						gameModels[gameModelsIndex].model.mesh = wallMesh;
						ind = OPmeshRendererAdd(&renderer, wallMesh);
					}
					gameModels[gameModelsIndex].model.world = OPmat4Translate(x, 0, z);
					gameModels[gameModelsIndex].position = OPvec3Create(x, 0, z);

					renderer.world[ind] = OPmat4Translate(x, 0, z);
					gameModelsIndex++;
				}
				intersecting = 1;
				selected = 0;
			}
		}
	}

	rotY -= time->Elapsed * 0.001f * OPkeyboardIsDown(OPKEY_LEFT);
	rotY += time->Elapsed * 0.001f * OPkeyboardIsDown(OPKEY_RIGHT);
	rotY += time->Elapsed * 0.001f * (OPmouseIsDown(OPMOUSE_RBUTTON) || (OPmouseIsDown(OPMOUSE_LBUTTON) && OPkeyboardIsDown(OPKEY_LCONTROL))) * OPmousePositionMovedX();// OPkeyboardIsDown(OPKEY_LEFT);
	rotX += time->Elapsed * 0.001f * (OPmouseIsDown(OPMOUSE_RBUTTON) || (OPmouseIsDown(OPMOUSE_LBUTTON) && OPkeyboardIsDown(OPKEY_LCONTROL))) * OPmousePositionMovedY();// OPkeyboardIsDown(OPKEY_LEFT);

	camPos.x -= time->Elapsed * 0.01f * OPkeyboardIsDown(OPKEY_A);
	camPos.x += time->Elapsed * 0.01f * OPkeyboardIsDown(OPKEY_D);
	camPos.z -= time->Elapsed * 0.01f * OPkeyboardIsDown(OPKEY_S);
	camPos.z += time->Elapsed * 0.01f * OPkeyboardIsDown(OPKEY_W);

	//camTarg.x -= time->Elapsed * 0.01f * OPkeyboardIsDown(OPKEY_A);
	//camTarg.x += time->Elapsed * 0.01f * OPkeyboardIsDown(OPKEY_D);
	//camTarg.z -= time->Elapsed * 0.01f * OPkeyboardIsDown(OPKEY_S);
	//camTarg.z += time->Elapsed * 0.01f * OPkeyboardIsDown(OPKEY_W);

	zoom += time->Elapsed * 0.01f * OPmouseWheelMoved();

	//f32 x = OPcos(rotY) * 10;
	//f32 z = OPsin(rotY) * 10;
	OPvec3 Position = OPvec3Create(zoom);
	OPmat4 Model = OPmat4RotY(rotY) * OPmat4RotX(rotX);
	OPvec3 Transformed = Model * Position;
	//OPvec3 Transformed = OPvec3Create(x, 10, z);

	//OPlog("%f", rotY);
	//OPvec3Log("Rotated", Transformed);
	camera._pos = Transformed + camPos;
	if (camera._pos.y < 1) camera._pos.y = 1;
	//camera._pos.y = 10;

	camera._targ = camTarg + camPos;// camTarg;
	camera._targ.y = 0;
	camera._viewStale = 1;

	/////////////////////
	// RENDER
	/////////////////////

	OPrenderClear(0.3, 0.3, 0.3, 1);

	OPmat4 view, proj, world;
	OPvec3 lightDirection = OPVEC3_UP;

	// Bind the mesh and effect
	OPmeshBind(&mesh);
	OPeffectBind(&effect);
	OPeffectParamVec3("vLightDirection", &lightDirection);

	OPcamGetView(camera, &view);
	OPcamGetProj(camera, &proj);

	OPeffectParamMat4("uView", &view);
	OPeffectParamMat4("uProj", &proj);

	// Draw each one of the squares
	for (OPuint i = 0; i < 10; i++) {
		for (OPuint j = 0; j < 10; j++) {
			if (!intersecting || 
				!(positionHit.x > i - 0.5 + pos.x
				&& positionHit.x <= i + 0.5 + pos.x
				&& positionHit.z > j - 0.5
				&& positionHit.z <= j + 0.5)) {

				world = OPmat4Translate(i + pos.x, 0, j);
				OPeffectParamMat4("uWorld", &world);
				OPmeshRender();
			}
		}
	}

	for (ui32 i = 0; i < gameModelsIndex; i++) {
		gameModels[i].model.world = OPmat4Scl(1.0 / 9.0);
		gameModels[i].model.world = OPmat4Translate(gameModels[i].model.world, pos.x + gameModels[i].position.x, 0, pos.z + gameModels[i].position.z);
		renderer.world[i] = gameModels[i].model.world;
	}
	OPmeshRendererDraw(&renderer, camera);

	//for (ui32 i = 0; i < gameModelsIndex; i++) {
	//	gameModels[i].model.world = OPmat4Scl(1.0 / 9.0);
	//	gameModels[i].model.world = OPmat4Translate(gameModels[i].model.world, pos.x + gameModels[i].position.x, 0, pos.z + gameModels[i].position.z);
	//	OPmodelDraw(gameModels[i].model, &texMaterial, camera);
	//}


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