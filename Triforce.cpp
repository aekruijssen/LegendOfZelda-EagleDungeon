#include "CollisionComponent.h"
#include "Collectible.h"
#include "Player.h"
#include "SDL/SDL_mixer.h"
#include "Triforce.h"
#include "AnimatedSprite.h"
#include "Game.h"

Triforce::Triforce(Game* game)
	: Actor(game) {
	mGame = game;
	// Adds itself to the game via AddActor(remember Actor has an mGame member variable)
	//mGame->AddActor(this);
	as = new AnimatedSprite(this, 150);

	std::vector<SDL_Texture*> triforce{
		mGame->GetTexture("Assets/Triforce0.png"),
		mGame->GetTexture("Assets/Triforce1.png")
	};
	as->AddAnimation("triforce", triforce);
	as->SetAnimation("triforce");

	cc = new CollisionComponent(this);
	cc->SetSize(20.0f, 20.0f);

	c = new Collectible(this);
	c->SetOnCollect([this] {
		Mix_PlayChannel(-1, mGame->GetSound("Assets/Sounds/Triforce.oog"), 0);
		Mix_HaltChannel(mGame->mChannel);
		mGame->mPlayer->SetState(ActorState::Paused);
	});
}
