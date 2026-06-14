#include "objects.hpp"

static std::vector<InitializedObject> objects;

static bool initialized = false;

void init_obj() {
	if (initialized) return;

	objects.reserve(2048);

	initialized = true;
}

void add_object(Object *object) {
	if (object == nullptr) return;

	if (!initialized) {
		init_obj();
	}
	
	bool unusable = true;
	
	for (InitializedObject &obj : objects) {
		if (obj.inactive) { unusable = false; }
	}

	if (objects.empty()) { unusable = false; }

	if (objects.size() < 2048) {
		unusable = false;
	}

	if (unusable) return;
		
	if (object->preset == ObjectPreset::CheckeredCube) {
		Object &cube = *object;
		
		cube.mesh = GenMeshCube(4.0f, 4.0f, 4.0f);
		cube.texture = LoadTexture("textures/checkers.png");
		cube.textureRepeat = true;
		cube.textureRepeatAmount = 4.0f;

	} else if (object->preset == ObjectPreset::WhiteSphere) {
		Object &sphere = *object;

		sphere.mesh = GenMeshSphere(4.0f, 4.0f, 8);
		Image img = GenImageColor(1, 1, WHITE);
		sphere.texture = LoadTextureFromImage(img);
		UnloadImage(img);
		sphere.textureRepeat = false;

	} else if (object->preset == ObjectPreset::Potato) {
		Object &potato = *object;
		
		potato.billboard = true;
		potato.texture = LoadTexture("textures/potato.png");
		potato.textureRepeat = false;
	}

	// ok bois we lock in
	
	InitializedObject newObject;

	if ((*object).texture.id == 0) {
		Image img = GenImageColor(1, 1, PURPLE);
		(*object).texture = LoadTextureFromImage(img);
		UnloadImage(img);
}

	if ((*object).mesh.vertexCount == 0) {
		(*object).mesh = GenMeshCube(3.0f, 3.0f, 3.0f);
	}

	if ((*object).billboard) {
		newObject.type = ObjectType::Billboard;
		newObject.BillboardTexture = &(object->texture);
		newObject.BillboardScale = (*object).billboardScale;
	} else {
		newObject.type = ObjectType::Mesh;
		newObject.mesh = &(object->mesh);
		newObject.scale = (*object).scale;
	}

	Material newMaterial = LoadMaterialDefault();

	SetTextureFilter((*object).texture, TEXTURE_FILTER_POINT);

	if ((*object).normal.id != 0) {
		SetTextureFilter((*object).normal, TEXTURE_FILTER_POINT);
	}
	
	if ((*object).textureRepeat) {
		SetTextureWrap((*object).texture, TEXTURE_WRAP_REPEAT);
		if ((*object).normal.id != 0) {
			SetTextureWrap((*object).normal, TEXTURE_WRAP_REPEAT);
		}

		for (int i = 0; i < (*object).mesh.vertexCount * 2; i++) {
			(*object).mesh.texcoords[i] *= (*object).textureRepeatAmount;
		}
	
		UpdateMeshBuffer(
			(*object).mesh,
			1,
			(*object).mesh.texcoords,
			(*object).mesh.vertexCount * 2 * sizeof(float),
	    		0
		);
	}	

	newMaterial.maps[MATERIAL_MAP_DIFFUSE].texture = (*object).texture;
	
	if ((*object).normal.id != 0) {
		newMaterial.maps[MATERIAL_MAP_NORMAL].texture = (*object).normal;
	}

	newObject.material = newMaterial;

	Matrix matScale = MatrixScale((*object).scale.x, (*object).scale.y, (*object).scale.z);
	Matrix matRot = MatrixRotateXYZ((*object).rotation);
	Matrix matTrans = MatrixTranslate((*object).position.x, (*object).position.y, (*object).position.z);

	Matrix transform = MatrixMultiply(MatrixMultiply(matScale, matRot), matTrans);

	newObject.transform = transform;
	BoundingBox box = GetMeshBoundingBox((*object).mesh);
	Vector3 meshSize = {
		box.max.x - box.min.x,
		box.max.y - box.min.y,
		box.max.z - box.min.z
	};
	
	newObject.collisionBox.size = meshSize;

	// objects.push_back(newObject); // find the next inactive object instead
	
	bool foundInactive = false;
	
	for (InitializedObject &oldobject : objects) {
		if (oldobject.inactive) {
			newObject.inactive = false;

			oldobject = newObject;
			oldobject.collisionBox.transform = &oldobject.transform;
			oldobject.originalObjectToReadPropertiesFromIncaseTheyChange = object;

			add_collider(&oldobject.collisionBox);

			foundInactive = true;

			break;
		}

	}

	if (!foundInactive) {
		newObject.inactive = false;

		objects.push_back(newObject);
	
		InitializedObject *postObject = &objects.back();
		(*postObject).originalObjectToReadPropertiesFromIncaseTheyChange = object;
		(*postObject).collisionBox.transform = &postObject->transform;

		add_collider(&(*postObject).collisionBox);
	}
}

void remove_object(Object *object) {

	if (!initialized) {
		init_obj();
	}

	if (object == nullptr) return;

	/* for (InitializedObject *initObj : objects) {
		if (initObj == nullptr) continue;*/
	
		for (auto it = objects.begin(); it != objects.end();) {
			if (it->originalObjectToReadPropertiesFromIncaseTheyChange == object) {
				UnloadMaterial((*it).material);

				unload_collider(&it->collisionBox);
				
				// objects.erase(it); // instead, i'm gonna mark this as inactive
				// just so memory doesn't shift and break stuff
				
				(*it).inactive = true;

				UnloadMesh((*object).mesh);

				object->mesh = { 0 };
				if ((*object).texture.id != 0) {
					UnloadTexture((*object).texture);
					object->texture = { 0 };
				}

				if ((*object).normal.id != 0) {
					UnloadTexture((*object).normal);
					object->normal = { 0 };
				}

				break;
			} else {
				++it;
			}
		}
	// }
}

void render_objects(Camera3D *camera) {

	if (!initialized) {
		init_obj();
	}

	if (camera == nullptr) return;

	for (InitializedObject &object : objects) {
		if (object.inactive) continue;

		if (object.type == ObjectType::Mesh) {
			DrawMesh(*object.mesh, object.material, object.transform);
		} else if (object.type == ObjectType::Billboard) {
			Vector3 position = { object.transform.m12, object.transform.m13, object.transform.m14 };

			DrawBillboard(*camera, *object.BillboardTexture, position, object.BillboardScale, WHITE);
		}
	}
}
