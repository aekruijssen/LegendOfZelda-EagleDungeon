#include "Collider.h"
#include "Game.h"
#include "AnimatedSprite.h"
#include "MoveComponent.h"
#include "CollisionComponent.h"
#include "SDL/SDL.h"
#include "Random.h"

Collider::Collider(Game* game)
	: Actor(game)
{
	mGame = game;
	cc = new CollisionComponent(this);
	//cc->SetSize(20.0f, 20.0f);

	//as = new AnimatedSprite(this, 50);
}

void Collider::SetColliderDims(float w, float h) {
	cc->SetSize(w, h);
}