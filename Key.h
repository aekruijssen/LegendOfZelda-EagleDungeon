#pragma once
#include "Actor.h"

class Key : public Actor
{
public:
	Key(class Game* game);

	class SpriteComponent* sc;
	class CollisionComponent* cc;
	class Collectible* c;

private:
};