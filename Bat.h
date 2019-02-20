#pragma once
#include "Actor.h"

class Bat : public Actor
{
public:
	Bat(class Game* game);

	void OnUpdate(float deltaTime) override;

	class CollisionComponent* cc;
	class AnimatedSprite* as;
	class EnemyComponent* ec;

private:
	float speed;
	float lifetime;
	Vector2 dir;
};