#include "potatoNewton.hpp"

static Vector3 velocity = { 0.0f };

static std::vector<CollisionBox*> colBoxes;

std::vector<CollisionBox*> check_collisions() {
	std::vector<CollisionBox*> hits;
	
	for (CollisionBox *box : colBoxes) {
		float x = box->transform.m12;
		float y = box->transform.m13;
		float z = box->transform.m14;
		
		Vector3 size = box->size;
		Vector3 scale;

		scale.x = Vector3Length({ m.m0, m.m1, m.m2 });
		scale.y = Vector3Length({ m.m4, m.m5, m.m6 });
		scale.z = Vector3Length({ m.m8, m.m9, m.m10 });

		Vector3 endSize = Vector3Multiply(size, scale);
		
		// pick up tmrw
	}

	return hits;
}

void move_check(Camera3D *camera) {
	float speed = 8.0f;

	float jPower = 32.0f;

	float velocityCap = IsKeyDown(KEY_LEFT_SHIFT) ? 32.0f : 16.0f;
	
	float velocityDecay = 1.5f;

	float dt = GetFrameTime();
	Vector3 movement = { 0.0f, 0.0f, 0.0f };

	float currentMovement = IsKeyDown(KEY_LEFT_SHIFT) ? (speed * 1.5) : speed;

	// beautiful keyboard shenanigans //

	if (IsKeyDown(KEY_W)) {
		velocity.x += currentMovement;
	} else if (IsKeyDown(KEY_S)) {
		velocity.x -= currentMovement;
	} if (IsKeyDown(KEY_A)) {
		velocity.y -= currentMovement;
	} else if (IsKeyDown(KEY_D)) {
		velocity.y += currentMovement;
	}

	// jumpies!

	if (IsKeyPressed(KEY_SPACE)) {
		velocity.z += jPower;
	}

	// velocity handler :] //
	
	// handle positive values

	if (velocity.x > 0) {
		if ((velocity.x - velocityDecay) < 0) {
			velocity.x -= velocity.x;
		} else {
			velocity.x -= velocityDecay;
		}
	}

	if (velocity.y > 0) {
		if ((velocity.y - velocityDecay) < 0) {
			velocity.y -= velocity.y;
		} else {
			velocity.y -= velocityDecay;
		}
	}
	
	if (velocity.z > 0) {
		if ((velocity.z - velocityDecay) < 0) {
			velocity.z -= velocity.z;
		} else {
			velocity.z -= velocityDecay;
			// std::cout << velocity.z << std::endl; // DEBUGGING
		}
	}

	// handle negative values

	if (velocity.x < 0) {
		if ((velocity.x + velocityDecay) > 0) {
			velocity.x += std::abs(velocity.x);
		} else {
			velocity.x += velocityDecay;
		}
	}

	if (velocity.y < 0) {
		if ((velocity.y + velocityDecay) > 0) {
			velocity.y += std::abs(velocity.y);
		} else {
			velocity.y += velocityDecay;
		}
	}
	
	if (velocity.z < 0) {
		if ((velocity.z + velocityDecay) > 0) {
			velocity.z += std::abs(velocity.z);
		} else {
			velocity.z += velocityDecay;
			// std::cout << velocity.z << std::endl; // DEBUGGING
		}
	}


	// leveling out & applying velocity //
	
	velocity.x = (velocity.x > velocityCap) ? velocityCap : velocity.x;
	velocity.y = (velocity.y > velocityCap) ? velocityCap : velocity.y;
	// velocity.z = (velocity.z > velocityCap) ? velocityCap : velocity.z;

	velocity.x = (std::abs(velocity.x) > velocityCap) ? (velocityCap * -1) : velocity.x;
	velocity.y = (std::abs(velocity.y) > velocityCap) ? (velocityCap * -1) : velocity.y;
	// velocity.z = (std::abs(velocity.z) > velocityCap) ? (velocityCap * -1) : velocity.z;

	movement.x += velocity.x * dt;
	movement.y += velocity.y * dt;
	movement.z += velocity.z * dt;

	// time for mouse crap //

	float sensitivity = 0.1f;
	Vector2 md = GetMouseDelta();

	Vector3 rotation = {
		md.x * sensitivity,
		md.y * sensitivity,
		0.0f
	};

	UpdateCameraPro(camera, movement, rotation, 0);	
}

void add_collider(CollisionBox *object) {
	colBoxes.push_back(object);
}

void list_collider_memory_addresses() {
	for (CollisionBox *box : colBoxes) {
		std::cout << box << std::endl;
	}
}

void unload_collider(CollisionBox *object) {
	auto it = std::find(colBoxes.begin(), colBoxes.end(), object);

	if (it != colBoxes.end()) {
		colBoxes.erase(it);
	}
}
