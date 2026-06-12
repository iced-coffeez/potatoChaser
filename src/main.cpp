#include "raylib.h"
#include "raymath.h"
#include "potatoNewton.hpp"

int main() {
	SetConfigFlags(FLAG_VSYNC_HINT);

	InitWindow(GetMonitorWidth(GetCurrentMonitor()) / 1.5, GetMonitorHeight(GetCurrentMonitor()) / 1.5, "potato chaser");
	SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));

	if (!IsWindowFullscreen()) {
		ToggleFullscreen();
	}

	DisableCursor();

	Font font = LoadFont("fonts/Opsilon-Regular.ttf");

	Camera3D camera;

	camera.position = Vector3{ 0, 5, 0 };
	camera.target = Vector3{ 90, 0, 0 };
	camera.up = Vector3{ 0, 1, 0 };
	camera.fovy = 45.0f;
	camera.projection = CAMERA_PERSPECTIVE;

	/* CAMERA USAGE INFO \/
	typedef struct Camera3D {
	    Vector3 position;       // Camera position
	    Vector3 target;         // Camera target it looks-at
	    Vector3 up;             // Camera up vector (rotation over its axis)
	    float fovy;             // Camera field-of-view aperture in Y (degrees) in perspective, used as near plane height in world units in orthographic
	    int projection;         // Camera projection: CAMERA_PERSPECTIVE or CAMERA_ORTHOGRAPHIC
	} Camera3D; */

	Material floorMat = LoadMaterialDefault();

	Mesh floor = GenMeshCube(1000.0f, 2.0f, 1000.0f);

	float repeat = 100.0f;

	for (int i = 0; i < floor.vertexCount * 2; i++) {
		floor.texcoords[i] *= repeat;
	}
	
	UpdateMeshBuffer(
	    floor,
	    1, // texture coordinates buffer
	    floor.texcoords,
	    floor.vertexCount * 2 * sizeof(float),
	    0
	);

	Texture2D dirt = LoadTexture("textures/dirt.png");
	Texture2D dirt_nrm = LoadTexture("textures/dirt_nrm.png");

	SetTextureFilter(dirt, TEXTURE_FILTER_POINT);
	SetTextureWrap(dirt, TEXTURE_WRAP_REPEAT);

	SetTextureFilter(dirt_nrm, TEXTURE_FILTER_POINT);
	SetTextureWrap(dirt_nrm, TEXTURE_WRAP_REPEAT);

	floorMat.maps[MATERIAL_MAP_DIFFUSE].texture = dirt;
	floorMat.maps[MATERIAL_MAP_NORMAL].texture = dirt_nrm;

	Vector3 position = { 0, -1.0f, 5 };
	Vector3 rotation = { 0, 0, 0 };
	Vector3 scale = { 1.0f, 1.0f, 1.0f };

	Matrix matScale = MatrixScale(scale.x, scale.y, scale.z);
	Matrix matRot = MatrixRotateXYZ(rotation);
	Matrix matTrans = MatrixTranslate(position.x, position.y, position.z);

	Matrix transform = MatrixMultiply(MatrixMultiply(matScale, matRot), matTrans);

	Vector3 velocity = { 0.0f };

	CollisionBox floorC;
	floorC.transform = &transform;
	floorC.size = { 1000.0f, 2.0f, 1000.0f };

	add_collider(&floorC);

	// list_collider_memory_addresses();
	
	while(!WindowShouldClose()) {
		if (IsKeyPressed(KEY_F4)) {
			ToggleFullscreen();
		}	

		move_check(&camera);
		
		//UpdateCamera(&camera, CAMERA_FREE);
		
		BeginDrawing();
		ClearBackground(BLACK);

		BeginMode3D(camera);

		DrawMesh(floor, floorMat, transform);
		
		EndMode3D();

		const char* title = "potato chaser";
		int FS = 64;
		
		float textwidth = MeasureTextEx(font, title, FS, 2).x;

		float x = (GetScreenWidth() / 2) - (textwidth / 2);
		
		// DrawRectangle((x - 25), 0, (FS + 8), textwidth, DARKGRAY);

		// DrawRectangle((x - 12.25), 0, textwidth, (FS + 4), DARKGRAY);

		DrawTextEx(font, title, Vector2{ x, 0 }, FS, 0, RAYWHITE);

		DrawText(TextFormat("%f",camera.position.y), 0, 0, 16, RAYWHITE);
			
		EndDrawing();
	}
	
	UnloadTexture(dirt);
	UnloadTexture(dirt_nrm);
	UnloadMesh(floor);
	UnloadMaterial(floorMat);
	UnloadFont(font);
	CloseWindow();
}
