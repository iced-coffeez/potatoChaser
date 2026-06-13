#pragma once

#include "raylib.h"
#include "raymath.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include "potatoNewton.hpp"

enum class ObjectPreset {
	CheckeredCube,
	WhiteSphere,
	Potato
};

enum class ObjectType {
	Mesh,
	Billboard
};

typedef struct Object {
	ObjectPreset preset;

	Mesh mesh = { 0 };
	Vector3 position = { 0.0f };
	Vector3 rotation = { 0.0f };
	Vector3 scale = { 1.0f, 1.0f, 1.0f };
	float billboardScale = 3.0f;

	Texture2D texture = { 0 };
	Texture2D normal = { 0 };
	bool textureRepeat = false;
	bool billboard = false;
	float textureRepeatAmount = 1.0f;
} Object;

typedef struct InitializedObject {
	ObjectType type;

	CollisionBox collisionBox;
	Matrix transform;
	Texture2D *BillboardTexture = { 0 };
	Material material;
	Mesh *mesh;
	Vector3 scale = { 1.0f };
	float BillboardScale = 3.0f;
} InitializedObject;

void add_object(Object *object);
void remove_object(Object *object);

void draw_objects();
