#include "objects.hpp"

static std::vector<InitializedObject> objects;

void add_object(Object *object) {
	if (object == nullptr) return;
		
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
	SetTextureFilter((*object).normal, TEXTURE_FILTER_POINT);
	
	if ((*object).textureRepeat) {
		SetTextureWrap((*object).texture, TEXTURE_WRAP_REPEAT);
		SetTextureWrap((*object).normal, TEXTURE_WRAP_REPEAT);
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

	CollisionBox cBox;

	cBox.transform = &transform;
	
	newObject.collisionBox.size = (*object).scale;

	objects.push_back(newObject);

	InitializedObject &stored = objects.back();
	stored.collisionBox.transform = &stored.transform;
}
