#include "SecretBlock.h"
#include "Game.h"
#include "AnimatedSprite.h"
#include "MoveComponent.h"
#include "CollisionComponent.h"
#include "SDL/SDL.h"
#include "Random.h"
#include "SpriteComponent.h"

SecretBlock::SecretBlock(Game* game)
	: Actor(game)
{
	mGame = game;
	cc = new CollisionComponent(this);
	cc->SetSize(32.0f, 32.0f);
	as = new AnimatedSprite(this);
	as->SetTexture(mGame->GetTexture("Assets/SecretBlock.png"));

	originalPos = GetPosition();
}

void SecretBlock::SetDirection(std::string dir) {
	mDir = dir;
}