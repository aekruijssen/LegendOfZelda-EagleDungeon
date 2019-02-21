#pragma once
#include "Actor.h"

class Fireball : public Actor
{
public:
	Fireball(class Game* game);

	void OnUpdate(float deltaTime) override;

	class CollisionComponent* cc;
	class SpriteComponent* sc;
	class MoveComponent* mc;

private:
	float lifetime;
	float invulnerability;
};