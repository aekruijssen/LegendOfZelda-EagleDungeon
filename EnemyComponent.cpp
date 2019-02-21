#include "EnemyComponent.h"
#include "Actor.h"
#include "Game.h"
#include "SDL/SDL_mixer.h"
#include "SDL/SDL.h"
#include "AnimatedSprite.h"

EnemyComponent::EnemyComponent(class Actor* owner)
	:Component(owner)
{
	//mOwner = new Actor(owner->mGame);
	mOwner = owner;
	//mGame = mOwner->GetGame();
	mOwner->mGame->addEnemy(mOwner);

	hp = 1;
	damage = 1;
	amt = 0;
	inv = 0;
}

EnemyComponent::~EnemyComponent() {
	mOwner->mGame->removeEnemy(mOwner);
}

void EnemyComponent::TakeDamage(int amount) {
		amt = amount;	if (hp <= 0) {		mOnDeath();		mOwner->SetState(ActorState::Destroy);	}	else {		mOnTakeDamage();	}}void EnemyComponent::SetOnTakeDamage(std::function<void()> onDamage) {
	mOnTakeDamage = onDamage;
}
void EnemyComponent::SetOnDeath(std::function<void()> onDeath) {
	mOnDeath = onDeath;
}