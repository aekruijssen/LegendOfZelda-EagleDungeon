#include "Collectible.h"
#include "Actor.h"
#include "CollisionComponent.h"
#include "Game.h"
#include "Key.h"
#include <algorithm>
#include "Player.h"
#include "Spawner.h"
#include "SpriteComponent.h"
#include <SDL/SDL_stdinc.h>


Collectible::Collectible(class Actor* owner)
	:Component(owner)
{
	//mOwner = new Actor(owner->mGame);
	mOwner = owner;
	//ownerScale = mOwner->GetScale();
}

//if the owner’s collision component intersects with the
//player’s collision component.If it does, you should set the owning actor to ActorState::Destroy
void Collectible::Update(float deltaTime)
{
	CollisionComponent* c = mOwner->GetComponent<CollisionComponent>();
	Vector2 playerPos = mOwner->mGame->mPlayer->GetPosition();
	CollSide cs = (mOwner->GetGame()->mPlayer->cc)->GetMinOverlap(c, playerPos);
	if (cs != CollSide::None) {
		mOwner->SetState(ActorState::Destroy);
		if (mOnCollect)
		{
			mOnCollect();
			//mOwner->SetState(ActorState::Destroy);
		}

	}
}

void Collectible::SetOnCollect(std::function<void()> onCollect) {
	mOnCollect = onCollect;
}