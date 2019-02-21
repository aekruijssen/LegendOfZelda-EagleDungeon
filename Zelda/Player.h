#pragma once
#include "Actor.h"

class Player : public Actor
{
public:
	Player(class Game* game);

	void ChangeTexture(char* texturePath);

	class CollisionComponent* cc;
	class AnimatedSprite* as;

	void TakeDamage(int amount);
	//int getNumKeys();
	int numKeys;
	class PlayerMove* pm;

private:
	class MoveComponent* mc;
	int hp;
};