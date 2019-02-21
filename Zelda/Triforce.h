#pragma once
#include "Actor.h"

class Triforce : public Actor
{
public:
	Triforce(class Game* game);

	class AnimatedSprite* as;
	class CollisionComponent* cc;
	class Collectible* c;

private:
};