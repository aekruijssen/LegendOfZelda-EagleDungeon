#pragma once

#include "Actor.h"

class Collider : public Actor
{
public:
	Collider(class Game* game);

	//void ChangeTexture(char* texturePath);

	class CollisionComponent* cc;
	class AnimatedSprite* as;

	void SetColliderDims(float w, float h);

private:
};