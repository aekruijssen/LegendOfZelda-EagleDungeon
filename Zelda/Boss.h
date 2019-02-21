#pragma once
#include "Actor.h"

class Boss : public Actor
{
public:
	Boss(class Game* game);

	void OnUpdate(float deltaTime) override;

	class CollisionComponent* cc;
	class AnimatedSprite* as;
	class EnemyComponent* ec;
	int health;

	int invulnerability;

private:
	float speed;
	float firetime;
	Vector2 dir;
	float leftangle;
};