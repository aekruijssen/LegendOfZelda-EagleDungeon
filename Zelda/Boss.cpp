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
#include "Game.h"
#include "Sword.h"

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

	ec = new EnemyComponent(this, mGame->currRoom);

	speed = 50.0f;
	//firetime = Random::GetFloatRange(1.0f, 3.0f);
	dir = Random::GetVector(Vector2(-1, -1), Vector2(1, 1));

	ec->hp = 5;
	int health = 5;

	firetime = 0.0f;
	invulnerability = 0.0f;

	ec->SetOnTakeDamage([this] {
		/*if (ec->inv <= 0.0f) {
			ec->hp -= (ec->amt);
			//ec->TakeDamage(1);
			ec->inv = 1.0f;
			Mix_PlayChannel(-1, mGame->GetSound("Assets/Sounds/BossHit.wav"), 0);
		}*/
	});
	ec->SetOnDeath([this] {
		Mix_PlayChannel(-1, mGame->GetSound("Assets/Sounds/BossDie.wav"), 0);
		std::vector<Door*> doors = mGame->mDoors.at(mGame->currRoom);
		for (auto block : doors) {
			block->updateState("Open");
		}
		SetState(ActorState::Destroy);
	});
}

void Boss::OnUpdate(float deltaTime) {
	//if (health <= 0) {

	//}
	if (mState != ActorState::Destroy) {
		if (ec->roomAddedTo != mGame->currRoom) {
			SetState(ActorState::Paused);
		}
		else {
			SetState(ActorState::Active);
			as->SetIsPaused(false);

			ec->hp = health;

			float swordX = mGame->mPlayer->pm->mSword->GetPosition().x;
			float swordY = mGame->mPlayer->pm->mSword->GetPosition().y;
			Vector2 swordPos = Vector2(swordX, swordY);
			float curX = GetPosition().x;
			float curY = GetPosition().y;
			Vector2 curPos = Vector2(curX, curY);

			CollisionComponent* swordCol = mGame->mPlayer->pm->mSword->cc;

			CollSide cs = cc->GetMinOverlap(swordCol, curPos);
			if (cs != CollSide::None) {
				if (invulnerability <= 0.0f) {
					//	ec->hp -= 1;
						//ec->TakeDamage(1);
						//ec->inv = 1.0f;
					health--;
					invulnerability = 5.0f;

					Mix_PlayChannel(-1, mGame->GetSound("Assets/Sounds/BossHit.wav"), 0);
				}
			}

			firetime += deltaTime;
			if (firetime <= 0.25f) {
				as->SetAnimation("attack");
			}
			else {
				as->SetAnimation("idle");
			}

			//	if (ec->inv > 0.0f) {
				//	ec->inv -= deltaTime;
				//}
			if (invulnerability > 0.0f) {
				invulnerability -= deltaTime;
			}

			if (health <= 2) {
				as->SetAnimation("enraged");
			}

			if (as->GetAnimName() == "enraged" && firetime >= 2.0f) {
				Fireball* f = new Fireball(this->GetGame());
				f->sc->SetTexture(mGame->GetTexture("Assets/FireballGreen.png"));
				f->SetPosition(GetPosition());
				Vector2 ang = Vector2(this->GetPosition().x - 200, this->GetPosition().y) - this->GetPosition();
				// Negate y because of SDL +y
				float angle = Math::Atan2(-ang.y, ang.x);
				f->SetRotation(angle);

				Fireball* fUp = new Fireball(this->GetGame());
				fUp->sc->SetTexture(mGame->GetTexture("Assets/FireballGreen.png"));
				fUp->SetPosition(GetPosition());
				//Vector2 angUp = GetPosition() * (-(Math::Pi)/3);
				Vector2 angUp = Vector2(this->GetPosition().x - 200, this->GetPosition().y - 115) - this->GetPosition();
				// Negate y because of SDL +y
				float angleUp = Math::Atan2(-angUp.y, angUp.x);
				fUp->SetRotation(angleUp);

				Fireball* fDown = new Fireball(this->GetGame());
				fDown->sc->SetTexture(mGame->GetTexture("Assets/FireballGreen.png"));
				fDown->SetPosition(GetPosition());
				Vector2 angDown = Vector2(this->GetPosition().x - 200, this->GetPosition().y + 115) - this->GetPosition();
				// Negate y because of SDL +y
				float angleDown = Math::Atan2(-angDown.y, angDown.x);
				fDown->SetRotation(angleDown);

				Fireball* fUpUp = new Fireball(this->GetGame());
				fUpUp->sc->SetTexture(mGame->GetTexture("Assets/FireballGreen.png"));
				fUpUp->SetPosition(GetPosition());
				Vector2 angUpUp = Vector2(this->GetPosition().x - 200, this->GetPosition().y + 60) - this->GetPosition();
				// Negate y because of SDL +y
				float angleUpUp = Math::Atan2(-angUpUp.y, angUpUp.x);
				f->SetRotation(angleUpUp);

				Fireball* fDownDown = new Fireball(this->GetGame());
				fDownDown->sc->SetTexture(mGame->GetTexture("Assets/FireballGreen.png"));
				fDownDown->SetPosition(GetPosition());
				//Vector2 angUp = GetPosition() * (-(Math::Pi)/3);
				Vector2 angDownDown = Vector2(this->GetPosition().x - 200, this->GetPosition().y - 60) - this->GetPosition();
				// Negate y because of SDL +y
				float angleDownDown = Math::Atan2(-angDown.y, angDown.x);
				fDownDown->SetRotation(angleDownDown);

				Fireball* fUpUpUp = new Fireball(this->GetGame());
				fUpUpUp->sc->SetTexture(mGame->GetTexture("Assets/FireballGreen.png"));
				fUpUpUp->SetPosition(GetPosition());
				Vector2 angUpUpUp = Vector2(this->GetPosition().x - 200, this->GetPosition().y) - this->GetPosition();
				// Negate y because of SDL +y
				float angleUpUpUp = Math::Atan2(-angUpUpUp.y, angUpUpUp.x);
				f->SetRotation(angleUpUpUp);

				Fireball* fDDD = new Fireball(this->GetGame());
				fDDD->sc->SetTexture(mGame->GetTexture("Assets/FireballGreen.png"));
				fDDD->SetPosition(GetPosition());
				//Vector2 angUp = GetPosition() * (-(Math::Pi)/3);
				Vector2 angDDD = Vector2(this->GetPosition().x - 200, this->GetPosition().y - 170) - this->GetPosition();
				// Negate y because of SDL +y
				float angleDDD = Math::Atan2(-angDDD.y, angDDD.x);
				fDDD->SetRotation(angleDDD);

				Fireball* fStraightUp = new Fireball(this->GetGame());
				fStraightUp->sc->SetTexture(mGame->GetTexture("Assets/FireballGreen.png"));
				fStraightUp->SetPosition(GetPosition());
				Vector2 angStraightUp = Vector2(this->GetPosition().x, Math::PiOver2) - this->GetPosition();
				// Negate y because of SDL +y
				float angleStraightUp = Math::Atan2(angStraightUp.y, angStraightUp.x);
				fStraightUp->SetRotation(angleStraightUp);

				Fireball* fStraightDown = new Fireball(this->GetGame());
				fStraightDown->sc->SetTexture(mGame->GetTexture("Assets/FireballGreen.png"));
				fStraightDown->SetPosition(GetPosition());
				Vector2 angStraightDown = Vector2(this->GetPosition().x, Math::PiOver2) - this->GetPosition();
				// Negate y because of SDL +y
				float angleStraightDown = Math::Atan2(-angStraightDown.y, angStraightDown.x);
				fStraightDown->SetRotation(angleStraightDown);

				Mix_PlayChannel(-1, mGame->GetSound("Assets/Sounds/Fireball.wav"), 0);

				firetime = 0.0f;
			}

			else if (as->GetAnimName() != "enraged" && firetime >= 2.0f) {
				Fireball* f = new Fireball(this->GetGame());
				f->sc->SetTexture(mGame->GetTexture("Assets/FireballGreen.png"));
				f->SetPosition(GetPosition());
				Vector2 ang = Vector2(this->GetPosition().x - 200, this->GetPosition().y) - this->GetPosition();
				// Negate y because of SDL +y
				float angle = Math::Atan2(-ang.y, ang.x);
				f->SetRotation(angle);

				Fireball* fUp = new Fireball(this->GetGame());
				fUp->sc->SetTexture(mGame->GetTexture("Assets/FireballGreen.png"));
				fUp->SetPosition(GetPosition());
				//Vector2 angUp = GetPosition() * (-(Math::Pi)/3);
				Vector2 angUp = Vector2(this->GetPosition().x - 200, this->GetPosition().y - 115) - this->GetPosition();
				// Negate y because of SDL +y
				float angleUp = Math::Atan2(-angUp.y, angUp.x);
				fUp->SetRotation(angleUp);

				Fireball* fDown = new Fireball(this->GetGame());
				fDown->sc->SetTexture(mGame->GetTexture("Assets/FireballGreen.png"));
				fDown->SetPosition(GetPosition());
				Vector2 angDown = Vector2(this->GetPosition().x - 200, this->GetPosition().y + 115) - this->GetPosition();
				// Negate y because of SDL +y
				float angleDown = Math::Atan2(-angDown.y, angDown.x);
				fDown->SetRotation(angleDown);

				Mix_PlayChannel(-1, mGame->GetSound("Assets/Sounds/Fireball.wav"), 0);

				firetime = 0.0f;
			}
		}
	}
}