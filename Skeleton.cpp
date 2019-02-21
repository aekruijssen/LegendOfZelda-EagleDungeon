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
#include "SDL/SDL_mixer.h"


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
	ec->SetOnTakeDamage([this] {
		ec->hp -= ec->amt;
		//ec->TakeDamage(1);
		Mix_PlayChannel(-1, mGame->GetSound("Assets/Sounds/EnemyHit.wav"), 0);
	});
	ec->SetOnDeath([this] {
		Mix_PlayChannel(-1, mGame->GetSound("Assets/Sounds/EnemyDie.wav"), 0);
	});

	speed = 50.0f;
	firetime = Random::GetFloatRange(1.0f, 3.0f);
	dir = Random::GetVector(Vector2(-1, -1), Vector2(1, 1));
}

void Skeleton::OnUpdate(float deltaTime) {
	firetime += deltaTime;
	if (firetime >= 4.0f) {
		Fireball* f = new Fireball(this->GetGame());
		Mix_PlayChannel(-1, mGame->GetSound("Assets/Sounds/Fireball.wav"), 0);
		f->SetPosition(GetPosition());
		Vector2 ang = mGame->GetPlayer()->GetPosition() - this->GetPosition();
		// Negate y because of SDL +y
		float angle = Math::Atan2(-ang.y, ang.x);		f->SetRotation(angle);

		firetime = 0.0f;
	}
}