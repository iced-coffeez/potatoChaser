#pragma once

#include "raylib.h"
#include "raymath.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <cmath>

void move_check(Camera3D *camera);

typedef struct CollisionBox {
	Matrix *transform;
	Vector3 size;
} CollisionBox;

void add_collider(CollisionBox *object);

void list_collider_memory_addresses();

void unload_collider(CollisionBox *object);
