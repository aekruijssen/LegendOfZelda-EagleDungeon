#include "SDL/SDL.h"
#include "Random.h"
#include "EnemyComponent.h"
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
#include "Skeleton.h"
#include "Fireball.h"


Skeleton::Skeleton(Game* game)
	:Actor(game)
{
	mGame = game;
	cc = new CollisionComponent(this);
	cc->SetSize(25.0f, 25.0f);


	as = new AnimatedSprite(this, 150);

	std::vector<SDL_Texture*> scuttling{
		mGame->GetTexture("Assets/Skeleton0.png"),
		mGame->GetTexture("Assets/Skeleton1.png")
	};

	as->AddAnimation("scuttle", scuttling);
	as->SetAnimation("scuttle");
	as->SetIsPaused(true);

	ec = new EnemyComponent(this);

	speed = 50.0f;
	firetime = 0.0f;
	dir = Random::GetVector(Vector2(-1, -1), Vector2(1, 1));
}

void Skeleton::OnUpdate(float deltaTime) {
	firetime += deltaTime;
	if (firetime >= 4.0f) {
		Fireball f = new Fireball(mGame);
		firetime = 0.0f;
	}
}