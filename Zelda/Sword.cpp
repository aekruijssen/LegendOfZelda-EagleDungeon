#include "SDL/SDL.h"
#include "Random.h"
#include "PlayerMove.h"
#include "Actor.h"
#include "CollisionComponent.h"
#include "Game.h"
#include "Math.h"
#include "Player.h"
#include "AnimatedSprite.h"
#include "Collider.h"
#include "Door.h"
#include "SecretBlock.h"
#include "Spawner.h"
#include "Sword.h"


Sword::Sword(Game* game)
	:Actor(game)
	//cc(this)
{
	mGame = game;
	cc = new CollisionComponent(this);
	cc->SetSize(25.0f,25.0f);


	sc = new SpriteComponent(this, 150);

	speed = 50.0f;
	lifetime = 0.0f;
	dir = Random::GetVector(Vector2(-1, -1), Vector2(1, 1));
}
