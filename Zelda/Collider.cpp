#include "Collider.h"
#include "Game.h"
#include "MoveComponent.h"
#include "CollisionComponent.h"
#include "SDL/SDL.h"
#include "Random.h"

Collider::Collider(Game* game)
	: Actor(game)
{
	mGame = game;
	cc = new CollisionComponent(this);
}

void Collider::SetColliderDims(float w, float h) {
	cc->SetSize(w, h);
}