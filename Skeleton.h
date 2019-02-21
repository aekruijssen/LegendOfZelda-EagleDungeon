#pragma once
#include "Actor.h"

class Skeleton : public Actor
{
public:
	Skeleton(class Game* game);

	void OnUpdate(float deltaTime) override;

	class CollisionComponent* cc;
	class AnimatedSprite* as;
	class EnemyComponent* ec;

private:
	float speed;
	float firetime;
	Vector2 dir;
};