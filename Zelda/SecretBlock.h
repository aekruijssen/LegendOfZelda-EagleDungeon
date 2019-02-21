#pragma once
#include "Actor.h"

class SecretBlock : public Actor
{
public:
	SecretBlock(class Game* game);

	//void ChangeTexture(char* texturePath);

	class CollisionComponent* cc;
	class AnimatedSprite* as;

	//void SetColliderDims(float w, float h);
	std::string mDir;
	void SecretBlock::SetDirection(std::string dir);

	Vector2 originalPos;

private:
	
};