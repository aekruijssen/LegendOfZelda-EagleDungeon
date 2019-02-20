#include "SDL/SDL.h"
#include "Random.h"
#include "EnemyComponent.h"
#include "PlayerMove.h"
#include "Actor.h"
#include "CollisionComponent.h"
#include "Game.h"
#include "Player.h"
#include "AnimatedSprite.h"
#include "Collider.h"
#include "Door.h"
#include "SecretBlock.h"
#include "Spawner.h"
#include "Fireball.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"


Fireball::Fireball(Game* game)
	:Actor(game)
{
	mGame = game;
	cc = new CollisionComponent(this);
	cc->SetSize(16.0f, 16.0f);

	sc = new SpriteComponent(this);
	sc->SetTexture(mGame->GetTexture("Assets/FireballRed.png"));

	mc = new MoveComponent(this);
	mc->SetForwardSpeed(200.0f);

	lifetime = 0;
	invulnerability = 0.0f;
}

void Fireball::OnUpdate(float deltaTime) {
	lifetime += deltaTime;
	if (lifetime >= 2.0f) {
		SetState(ActorState::Destroy);
	}
	if (invulnerability > 0) {
		invulnerability -= deltaTime;
	}
	CollisionComponent* playerCC = mGame->GetPlayer()->cc;
	Vector2 playerPos = Vector2(mGame->GetPlayer()->GetPosition().x, mGame->GetPlayer()->GetPosition().y);
	if (cc != nullptr && playerCC != nullptr && mGame->GetPlayer()->GetState() != ActorState::Paused) {
		CollSide cs = (playerCC)->GetMinOverlap(cc, playerPos);
		if (cs != CollSide::None) {
			if (invulnerability <= 0.0f) {
				mGame->GetPlayer()->TakeDamage(1);
				SetState(ActorState::Destroy);
				invulnerability = 0.5f;
			}
		}
	}
}