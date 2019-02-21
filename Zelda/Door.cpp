#include "Door.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "CollisionComponent.h"
#include "SDL/SDL.h"
#include "Random.h"


Door::Door(Game* game)
	:Actor(game)
{
	mGame = game;
	cc = new CollisionComponent(this);

	sc = new SpriteComponent(this);

}

void Door::setUp(std::string dest, std::string dir, std::string state) {
	mDestination = dest;
	mDirection = dir;
	mState = state;

	std::string doorPath = "Assets/Door/";
	doorPath += mDirection;
	doorPath += mState;
	doorPath += ".png";
	sc->SetTexture(mGame->GetTexture(doorPath.c_str()));

	if (mDirection == "Up" || mDirection == "Down") {
		cc->SetSize(30, 64);
	}
	else {
		cc->SetSize(64, 30);
	}
}

void Door::updateTexture(SDL_Texture* text) {
	sc->SetTexture(text);
}

void Door::updateState(std::string s) {
	mState = s;
	if (s == "Open") {
		std::string doorPath = "Assets/Door/";
		doorPath += mDirection;
		doorPath += "Open";
		doorPath += ".png";
		updateTexture(mGame->GetTexture(doorPath.c_str()));
	}
}