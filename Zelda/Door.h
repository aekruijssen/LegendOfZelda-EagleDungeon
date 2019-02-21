#pragma once
#include "Actor.h"
#include "SDL/SDL.h"

class Door : public Actor
{
public:
	Door(class Game* game);

	//void ChangeTexture(char* texturePath);

	class CollisionComponent* cc;
	class SpriteComponent* sc;

	//void SetColliderDims(float w, float h);

	void setUp(std::string dest, std::string dir, std::string state);
	void updateTexture(SDL_Texture* text);
	void updateState(std::string s);

	std::string mDirection;
	std::string mState;
	std::string mDestination;

protected:
	//std::string mDestination;
	//std::string mDirection;
	//std::string mState;
};