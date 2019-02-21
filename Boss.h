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

private:
	float speed;
	float firetime;
	int invulnerability;
	Vector2 dir;
};