#include "Key.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "CollisionComponent.h"
#include "Collectible.h"
#include "Player.h"
#include "SDL/SDL_mixer.h"

Key::Key(Game* game)
	: Actor(game){
	mGame = game;
	// Adds itself to the game via AddActor(remember Actor has an mGame member variable)
	//mGame->AddActor(this);
	sc = new SpriteComponent(this);
	sc->SetTexture(mGame->GetTexture("Assets/Key.png"));
	cc = new CollisionComponent(this);
	cc->SetSize(16.0f, 32.0f);

	c = new Collectible(this);
	c->SetOnCollect([this] {
		(mGame->mPlayer->numKeys)++;
		Mix_PlayChannel(-1, mGame->GetSound("Assets/Sounds/Key.wav"), 0);
	});
}
