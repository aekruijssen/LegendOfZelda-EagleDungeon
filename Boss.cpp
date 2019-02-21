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
#include "Boss.h"
#include "Fireball.h"
#include "SDL/SDL_mixer.h"


Boss::Boss(Game* game)
	:Actor(game)
{
	mGame = game;
	cc = new CollisionComponent(this);
	cc->SetSize(50.0f, 64.0f);


	as = new AnimatedSprite(this, 150);


	std::vector<SDL_Texture*> idle{
		mGame->GetTexture("Assets/Dragon/Idle0.png"),
		mGame->GetTexture("Assets/Dragon/Idle1.png")
	};
	std::vector<SDL_Texture*> attack{
		mGame->GetTexture("Assets/Dragon/Attack0.png"),
		mGame->GetTexture("Assets/Dragon/Attack1.png")
	};
	std::vector<SDL_Texture*> enraged{
		mGame->GetTexture("Assets/Dragon/Enraged0.png"),
		mGame->GetTexture("Assets/Dragon/Enraged1.png"),
		mGame->GetTexture("Assets/Dragon/Enraged2.png"),
		mGame->GetTexture("Assets/Dragon/Enraged3.png")
	};

	as->AddAnimation("idle", idle);
	as->AddAnimation("attack", attack);
	as->AddAnimation("enraged", enraged);
	as->SetAnimation("idle");
	as->SetIsPaused(true);

	ec = new EnemyComponent(this);

	speed = 50.0f;
	//firetime = Random::GetFloatRange(1.0f, 3.0f);
	dir = Random::GetVector(Vector2(-1, -1), Vector2(1, 1));

	ec->hp = 5;

	firetime = 0.0f;
	invulnerability = 0.0f;

	ec->SetOnTakeDamage([this] {
		if (ec->inv <= 0.0f) {
			ec->hp -= (ec->amt);
			//ec->TakeDamage(1);
			ec->inv = 0.5f;
			Mix_PlayChannel(-1, mGame->GetSound("Assets/Sounds/BossHit.wav"), 0);
		}
	});
	ec->SetOnDeath([this] {
		Mix_PlayChannel(-1, mGame->GetSound("Assets/Sounds/BossDie.wav"), 0);
		std::vector<Door*> doors = mGame->mDoors.at(mGame->currRoom);
		for (auto block : doors) {
			block->updateState("Open");
		}
	});
}

void Boss::OnUpdate(float deltaTime) {
	firetime += deltaTime;
	if (firetime <= 0.25f) {
		as->SetAnimation("attack");
	}
	else {
		as->SetAnimation("idle");
	}

	if (ec->inv > 0.0f) {
		ec->inv -= deltaTime;
	}

	if (ec->hp <= 2) {
		as->SetAnimation("enraged");
	}

	if (as->GetAnimName() == "enraged") {
		
	}

	else if (as->GetAnimName() != "enraged" && firetime >= 2.0f) {
		Fireball* f = new Fireball(this->GetGame());
		f->sc->SetTexture(mGame->GetTexture("Assets/FireballGreen.png"));
		f->SetPosition(GetPosition());
		Vector2 ang = GetPosition() * (-(Math::Pi));
		// Negate y because of SDL +y
		float angle = Math::Atan2(-ang.y, ang.x);		f->SetRotation(angle);

		Fireball* fUp = new Fireball(this->GetGame());
		fUp->sc->SetTexture(mGame->GetTexture("Assets/FireballGreen.png"));
		fUp->SetPosition(GetPosition());
		Vector2 angUp = GetPosition() * (-(Math::Pi)/3);
		// Negate y because of SDL +y
		float angleUp = Math::Atan2(-angUp.y, angUp.x);		fUp->SetRotation(angleUp);

		Fireball* fDown = new Fireball(this->GetGame());
		fDown->sc->SetTexture(mGame->GetTexture("Assets/FireballGreen.png"));
		fDown->SetPosition(GetPosition());
		Vector2 angDown = GetPosition() * (-(2*Math::Pi) / 3);
		// Negate y because of SDL +y
		float angleDown = Math::Atan2(-angDown.y, angDown.x);		fDown->SetRotation(angleDown);

		Mix_PlayChannel(-1, mGame->GetSound("Assets/Sounds/Fireball.wav"), 0);

		firetime = 0.0f;
	}
}