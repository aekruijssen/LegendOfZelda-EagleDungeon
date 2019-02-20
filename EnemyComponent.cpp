#include "EnemyComponent.h"
#include "Actor.h"
#include "Game.h"
#include "SDL/SDL_mixer.h"
#include "SDL/SDL.h"

EnemyComponent::EnemyComponent(class Actor* owner)
	:Component(owner)
{
	//mOwner = new Actor(owner->mGame);
	mOwner = owner;
	//mGame = mOwner->GetGame();
	mOwner->mGame->addEnemy(mOwner);

	hp = 1;
	damage = 1;
}

EnemyComponent::~EnemyComponent() {
	mOwner->mGame->removeEnemy(mOwner);
}

void EnemyComponent::TakeDamage(int amount) {
	hp -= amount;	if (hp <= 0) {		mOwner->SetState(ActorState::Destroy);		Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/EnemyDie.wav"), 0);		mOnDeath();	}	else {		mOnTakeDamage();	}}void EnemyComponent::SetOnCollect(std::function<void()> onDeath) {
	mOnDeath = onDeath;
}