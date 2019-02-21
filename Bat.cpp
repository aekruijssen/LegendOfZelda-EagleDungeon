#include "Bat.h"
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
#include "SDL/SDL_mixer.h"


Bat::Bat(Game* game)
	:Actor(game)
{
	mGame = game;
	cc = new CollisionComponent(this);
	cc->SetSize(25.0f, 25.0f);


	as = new AnimatedSprite(this, 150);

	std::vector<SDL_Texture*> flying{
		mGame->GetTexture("Assets/Bat0.png"),
		mGame->GetTexture("Assets/Bat1.png")
	};

	as->AddAnimation("fly", flying);
	as->SetAnimation("fly");
	as->SetIsPaused(true);

	ec = new EnemyComponent(this);
	ec->SetOnTakeDamage([this] {
		ec->hp -= (ec->amt);
		//ec->TakeDamage(1);
		Mix_PlayChannel(-1, mGame->GetSound("Assets/Sounds/EnemyHit.wav"), 0);
	});
	ec->SetOnDeath([this] {
		Mix_PlayChannel(-1, mGame->GetSound("Assets/Sounds/EnemyDie.wav"), 0);
	});

	speed = 50.0f;
	lifetime = 0.0f;
	dir = Random::GetVector(Vector2(-1, -1), Vector2(1, 1));
}
void Bat::OnUpdate(float deltaTime) {
	//float curX = GetPosition().x;
	//float curY = GetPosition().y;
	//Vector2 dir;
	lifetime += deltaTime;
	if (lifetime >= 3.0f) {
		//Vector2 randPos = Random::GetVector(Vector2(GetPosition().x-100, GetPosition().y-100), Vector2(GetPosition().x+100, GetPosition().y+100));
		Vector2 randPos = Random::GetVector(Vector2(-1, -1), Vector2(1, 1));
		//Vector2 pos = GetPosition();
		//pos.x += speed * deltaTime;
		//pos.y += speed * deltaTime;
		//SetPosition(pos);
		//randPos.Normalize();
		//dir = randPos - this->GetPosition();
		//dir.Normalize();
		dir = randPos;
		lifetime = 0.0f;
	}
	//Vector2 empty;
	//if (dir.x == empty.x && dir.y == empty.y) {
		//dir.Normalize();
		/*Vector2 normDir = dir;
		Vector2 newPos;
		newPos.x = GetPosition().x * deltaTime*dir.x;
		newPos.y = GetPosition().y * deltaTime*dir.y;
		SetPosition(newPos);*/
		this->SetPosition(this->GetPosition() + deltaTime * 50 * dir);
		//this->SetPosition(50*deltaTime*dir);
	//}
		for (auto block : (mGame->mColliders)) {
			float blockX = block->GetPosition().x;
			float blockY = block->GetPosition().y;
			Vector2* blockPos = new Vector2(blockX, blockY);
			float playerX = GetPosition().x;
			float playerY = GetPosition().y;
			Vector2 playerPos = Vector2(playerX, playerY);
			CollSide cs = (cc)->GetMinOverlap(block->cc, playerPos);
			// Didn’t collide with the block, so move on
			if (cs != CollSide::None) {
				Vector2 curPos = GetPosition();
				curPos.x += playerPos.x;
				curPos.y += playerPos.y;
				SetPosition(curPos);
			}
		}
	
}