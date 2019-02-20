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


	hp -= amount;
	mOnDeath = onDeath;
}