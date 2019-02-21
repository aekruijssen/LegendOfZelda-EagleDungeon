#pragma once
#include "Actor.h"

class Sword : public Actor
{
public:
	Sword(class Game* game);

	//void OnUpdate(float deltaTime) override;

	class CollisionComponent* cc;
	class SpriteComponent* sc;

private:
	float speed;
	float lifetime;
	Vector2 dir;
};