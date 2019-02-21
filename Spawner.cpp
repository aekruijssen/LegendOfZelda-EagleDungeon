#include "Player.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "CollisionComponent.h"
#include "SDL/SDL.h"
#include "Random.h"
#include "PlayerMove.h"
#include "Spawner.h"
#include "Key.h"
#include "Bat.h"
#include "Skeleton.h"
#include "AnimatedSprite.h"
#include "Boss.h"
#include "Triforce.h"

Spawner::Spawner(Game* game)
	:Actor(game)
{
	mGame = game;
}
void Spawner::OnUpdate(float deltaTime) {

}

void Spawner::SetType(std::string type) {
	mType = type;
}

void Spawner::Create() {
	if (mType == "Key") {
		Key* k = new Key(mGame);
		k->SetPosition(this->GetPosition());
		k->sc->SetTexture(mGame->GetTexture("Assets/Key.png"));
		SetState(ActorState::Destroy);
	}
	else if (mType == "Bat") {
		Bat* b = new Bat(mGame);
		b->SetPosition(this->GetPosition());
		b->as->SetTexture(mGame->GetTexture("Assets/Bat0.png"));
		//b->as->SetIsPaused(false);
		//b->SetState(ActorState::Active);
		SetState(ActorState::Destroy);
	}
	else if (mType == "Skeleton") {
		Skeleton* s = new Skeleton(mGame);
		s->SetPosition(this->GetPosition());
		//s->sc->SetTexture(mGame->GetTexture("Assets/Key.png"));
		s->as->SetIsPaused(false);
		s->SetState(ActorState::Active);
		SetState(ActorState::Destroy);
	}
	else if (mType == "Boss") {
		Boss* b = new Boss(mGame);
		b->SetPosition(this->GetPosition());
		b->as->SetIsPaused(false);
		b->SetState(ActorState::Active);
		SetState(ActorState::Destroy);
	}
	else if (mType == "Triforce") {
		Triforce* t = new Triforce(mGame);
		t->SetPosition(this->GetPosition());
		t->as->SetIsPaused(false);
		t->SetState(ActorState::Active);
		SetState(ActorState::Destroy);
	}
}