#include "Player.h"
#include "Game.h"
#include "AnimatedSprite.h"
#include "MoveComponent.h"
#include "CollisionComponent.h"
#include "SDL/SDL.h"
#include "Random.h"
#include "PlayerMove.h"
#include "SDL/SDL_mixer.h"


Player::Player(Game* game)
	:Actor(game)
{
	mGame = game;
	cc = new CollisionComponent(this);
	cc->SetSize(20.0f, 20.0f);

	pm = new PlayerMove(this);

	as = new AnimatedSprite(this, 150);

	std::vector<SDL_Texture*> walkLeft{
		mGame->GetTexture("Assets/Link/WalkLeft0.png"),
		mGame->GetTexture("Assets/Link/WalkLeft1.png")
	};
	std::vector<SDL_Texture*> walkRight{
		mGame->GetTexture("Assets/Link/WalkRight0.png"),
		mGame->GetTexture("Assets/Link/WalkRight1.png")
	};
	std::vector<SDL_Texture*> walkUp{
		mGame->GetTexture("Assets/Link/WalkUp0.png"),
		mGame->GetTexture("Assets/Link/WalkUp1.png")
	};
	std::vector<SDL_Texture*> walkDown{
		mGame->GetTexture("Assets/Link/WalkDown0.png"),
		mGame->GetTexture("Assets/Link/WalkDown1.png")
	};

	std::vector<SDL_Texture*> attackLeft{
		mGame->GetTexture("Assets/Link/AttackLeft.png")
	};
	std::vector<SDL_Texture*> attackRight{
		mGame->GetTexture("Assets/Link/AttackRight.png")
	};
	std::vector<SDL_Texture*> attackUp{
		mGame->GetTexture("Assets/Link/AttackUp.png")
	};
	std::vector<SDL_Texture*> attackDown{
		mGame->GetTexture("Assets/Link/AttackDown.png")
	};
	// Assumes "asc" is the animated sprite component
	as->AddAnimation("walkLeft", walkLeft);
	as->AddAnimation("walkRight", walkRight);
	as->AddAnimation("walkUp", walkUp);
	as->AddAnimation("walkDown", walkDown);
	as->AddAnimation("AttackLeft", attackLeft);
	as->AddAnimation("AttackRight", attackRight);
	as->AddAnimation("AttackUp", attackUp);
	as->AddAnimation("AttackDown", attackDown);
	as->SetAnimation("walkUp");
	as->SetIsPaused(true);

	numKeys = 0;
	hp = 10;
}

void Player::ChangeTexture(char* texturePath) {
	SDL_Texture* texture = mGame->GetTexture(texturePath);
	as->SetTexture(texture);
}

void Player::TakeDamage(int amount) {
	hp -= amount;	if (hp <= 0) {		this->SetState(ActorState::Paused);		Mix_PlayChannel(-1, mGame->GetSound("Assets/Sounds/LinkDie.wav"), 0);		Mix_HaltChannel(-1);	}
	else {
		Mix_PlayChannel(-1, mGame->GetSound("Assets/Sounds/LinkHit.wav"), 0);
	}
}

//int Player::getNumKeys() {
//	return numKeys;
//}