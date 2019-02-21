#pragma once

#include "Actor.h"

class Collider : public Actor
{
public:
	Collider(class Game* game);

	class CollisionComponent* cc;

	void SetColliderDims(float w, float h);

private:
};