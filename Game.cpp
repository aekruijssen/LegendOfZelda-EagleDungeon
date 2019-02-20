//
//  Game.cpp
//  Game-mac
//
//  Created by Sanjay Madhav on 5/31/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#include "Game.h"
#include "Actor.h"
#include "Player.h"
#include "SpriteComponent.h"
#include "Random.h"
#include "Spawner.h"
#include "SDL/SDL_mixer.h"
#include "TiledBGComponent.h"
#include "Collider.h"
#include "CSVHelper.h"
#include "Door.h"
#include "SecretBlock.h"

#include <cmath>
#include <SDL/SDL_image.h>
#include <algorithm>

#include <iostream>
#include <fstream>
#include <string>


// Game constructor - initialize renderer and window to null, running bool to true
Game::Game() {
	mRenderer = nullptr;
	mWindow = nullptr;
	mIsRunning = true;
}


// Initialize - SDL video/audio, create window/renderer, Random, SDL Image, ticks, and call LoadData()
bool Game::Initialize() {
	// Initialize sdl video and audio
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);
	if (sdlResult != 0) {
		SDL_Log("Unable to initialize SDL Video: %s", SDL_GetError());
		return false;
	}
	sdlResult = SDL_Init(SDL_INIT_AUDIO);
	if (sdlResult != 0) {
		SDL_Log("Unable to initialize SDL Audio: %s", SDL_GetError());
		return false;
	}

	// Create Window
	mWindow = SDL_CreateWindow(
		"The Legend of Zelda - Adina Kruijssen",
		100,
		100,
		600,
		448,
		0
	);

	if (!mWindow) {
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	// Create Renderer
	mRenderer = SDL_CreateRenderer(
		mWindow, 
		-1, 
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);
	if (!mRenderer) {
		SDL_Log("Unable to create renderer: %s", SDL_GetError());
		return false;
	}

	//Random::Init();

	//initialize SDL Image with IMG_Init
	IMG_Init(IMG_INIT_PNG);

	mTicksCount = 0;
	cameraPos.x = 0;
	cameraPos.y = 0;

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	Game::LoadData();

	return true;
}


// ShutDown  - quit SDL Image, unload data (delete Actors/Textures), destroy renderer/window, quit
void Game::ShutDown() {
	Mix_CloseAudio();
	IMG_Quit();
	UnloadData();
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}

Mix_Chunk* Game::GetSound(const std::string& filename) {
	mChunk = Mix_LoadWAV(filename.c_str());
	return mChunk;
}


// Run Loop - basic game loop (process input -> update game -> generate output)
void Game::RunLoop() {
	while (mIsRunning) {
		processInput();
		updateGame();
		generateOutput();
	}
}


// Process Input - check if exiting/running, get keyboard state, Process Input for all Actors
void Game::processInput() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				mIsRunning = false; 
				break;
		}
	}
	// Get keyboard state
	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE]) {
		mIsRunning = false;
	}

	// Make a copy of the actor vector. 
	std::vector<Actor*> mActorsCopy = mActors;
	// Loop over the copy of all actors and call ProcessInput on each.
	for (auto actor : mActorsCopy) {
		actor->ProcessInput(state);
	}
}


// Generate Output - background, swap, draw sprites
void Game::generateOutput() {
	SDL_SetRenderDrawColor(
		mRenderer,
		0,
		0,
		0,
		0
	);
	SDL_RenderClear(mRenderer);
	
	SDL_SetRenderDrawColor(
		mRenderer,
		255,
		255,
		255,
		255
	);
	//draw sprites
	for (auto sprite : mSprites) {
		if (sprite->IsVisible()) {
			sprite->Draw(mRenderer);
		}
	}
	//present 
	SDL_RenderPresent(mRenderer);
}


//Update game - update ticks, deltaTime, and Actors
void Game::updateGame() {
	mTicksCount = SDL_GetTicks();
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	if (deltaTime > 0.033f) {
		deltaTime = 0.033f;
	}

	//Make a copy(just a shallow copy using = ) of the actor vector
	std::vector<Actor*> mActorsCopy = mActors;
	//Loop over the copy and call Update on each actor
	for (auto actor : mActorsCopy) {
		actor->Update(deltaTime);
	}
	//Make a temporary Actor* vector for actors to destroy
	std::vector<Actor*> mActorsDestroy;
	//Loop over the actor vector, and add any actors in state ActorState::Destroy to temp vec
	for (auto actor : mActors) {
		if (actor->GetState() == ActorState::Destroy) {
			mActorsDestroy.emplace_back(actor);
		}
	}
	//Loop over the vector from step 3 and delete each actor in it
	for (auto actor : mActorsDestroy) {
		delete actor;
	}
}

void Game::LoadLevel() {
	int mChannel = Mix_PlayChannel(-1, GetSound("Assets/Sounds/Music.ogg"), -1);

}

Player* Game::GetPlayer() {
	return mPlayer;
}

// Load data - create actors
void Game::LoadData() {
	currRoom = "Room1";

	for(int z=1; z<8; z++){
		char x;
		std::ifstream inFile;

		std::string fileName = "Assets/Dungeon/Room";
		fileName += std::to_string(z);
		fileName += ".csv";
		inFile.open(fileName);
		if (!inFile) {
			std::cout << "Unable to open file";
			exit(1); // terminate with error
		}
		std::vector<std::vector<std::string> > room;
		std::string line = "";

		while (inFile >> line) {
			std::vector<std::string> CSV = CSVHelper::Split(line);
			room.emplace_back(CSV);
		}
		inFile.close(); 

		std::vector<Door*> RoomDoors;
		std::vector<Spawner*> RoomSpawns;
		std::vector<SecretBlock*> RoomSecretBlocks;

		for (int i = 1; i < room.size(); i++) {
			std::string r = room.at(i).at(0);
			if (room.at(i).at(0) == "Player") {
				Player* link = new Player(this);
				Vector2 pos;
				pos.x = std::stoi(room.at(i).at(1));
				pos.y = std::stoi(room.at(i).at(2));
				link->SetPosition(pos);
				mPlayer = link;
			}
			else if (room.at(i).at(0) == "Collider") {
				Collider* col = new Collider(this);
				float x = std::stof(room.at(i).at(1));
				float y = std::stof(room.at(i).at(2));
				float w = std::stof(room.at(i).at(3));
				float h = std::stof(room.at(i).at(4));
				col->SetPosition(Vector2(x + w / 2, y + h / 2));
				col->SetColliderDims(w, h);
				AddCollider(col);
			}
			else if (room.at(i).at(0) == "Door") {
				Door* door = new Door(this);
				float x = std::stof(room.at(i).at(1));
				float y = std::stof(room.at(i).at(2));
				float w = std::stof(room.at(i).at(3));
				float h = std::stof(room.at(i).at(4));
				door->SetPosition(Vector2(x + w / 2, y + h / 2));
				door->setUp(room.at(i).at(5), room.at(i).at(6), room.at(i).at(7));
				RoomDoors.push_back(door);
			}
			//SecretBlock,864,864,32,32,,Left,
			else if (room.at(i).at(0) == "SecretBlock") {
				SecretBlock* sb = new SecretBlock(this);
				float x = std::stof(room.at(i).at(1));
				float y = std::stof(room.at(i).at(2));
				float w = std::stof(room.at(i).at(3));
				float h = std::stof(room.at(i).at(4));
				std::string dir = (room.at(i).at(6));
				sb->SetPosition(Vector2(x + w / 2, y + h / 2));
				sb->originalPos = Vector2(x + w / 2, y + h / 2);

				sb->SetDirection(dir);

				/*if (dir == "Left") {
					sb->finalPos.x -= sb->GetPosition().x + 32;
				}
				else if (dir == "Right") {
					sb->finalPos.x += sb->GetPosition().x + 32;
				}
				else if (dir == "Up") {
					sb->finalPos.y -= sb->GetPosition().y + 32;
				}
				else if (dir == "Down") {
					sb->finalPos.y += sb->GetPosition().y + 32;
				}*/
				RoomSecretBlocks.push_back(sb);
			}
			else {
				Spawner* spawn = new Spawner(this);
				float x = std::stof(room.at(i).at(1));
				float y = std::stof(room.at(i).at(2));
				float w = std::stof(room.at(i).at(3));
				float h = std::stof(room.at(i).at(4));
				spawn->SetPosition(Vector2(x + w / 2, y + h / 2));
				spawn->SetType(room.at(i).at(0));
				RoomSpawns.push_back(spawn);
			}
		}
		mDoors.emplace(("Room" + std::to_string(z)), RoomDoors);
		mSecretBlocks.emplace(("Room" + std::to_string(z)), RoomSecretBlocks);
		mSpawns.emplace(("Room" + std::to_string(z)), RoomSpawns);
	}

	Actor* load = new Actor(this);
	TiledBGComponent* loadComponent = new TiledBGComponent(load, 50);
	loadComponent->LoadTileCSV("Assets/Dungeon/DungeonMapBG.csv", 32, 32);
	SDL_Texture* loadTexture = GetTexture("Assets/Dungeon/DungeonTiles.png");
	loadComponent->SetTexture(loadTexture);
	//loadComponent->Draw(mRenderer);
	//currRoom = "Room1";

	//Game::LoadLevel();
}


// Unload data - delete Actors and Textures
void Game::UnloadData() {

	Mix_FreeChunk(mChunk);

	while (!mActors.empty()) {
		delete mActors.back();
	}
	mActors.clear();

	for (auto texture : mTextures) {
		SDL_DestroyTexture(std::get<1>(texture));
	}
	mTextures.clear();
}



// Add/Remove functions for Actors, Textures, and Sprite Components:

void Game::AddActor(Actor* actor) {
	mActors.emplace_back(actor);
}

void Game::RemoveActor(Actor* actor) {
	auto search = std::find(std::begin(mActors), std::end(mActors), actor);
	if (search != std::end(mActors)) {
		mActors.erase(search);
	}
}

void Game::AddCollider(Collider* collider) {
	mColliders.emplace_back(collider);
}

void Game::RemoveCollider(Collider* collider) {
	auto search = std::find(std::begin(mColliders), std::end(mColliders), collider);
	if (search != std::end(mColliders)) {
		mColliders.erase(search);
	}
}

void Game::AddDoor(Door* door) {
	mDoors[currRoom].push_back(door);
}

void Game::addEnemy(Actor* enemy) {
	mEnemies[currRoom].push_back(enemy);
}

void Game::removeEnemy(Actor* enemy) {
	std::vector<Actor*> roomEnms = mEnemies.at(currRoom);
	auto search = std::find(std::begin(roomEnms), std::end(roomEnms), enemy);
	if (search != std::end(roomEnms)) {
		roomEnms.erase(search);
	}
}

void Game::AddSprite(SpriteComponent* sprite) {
	mSprites.emplace_back(sprite);
	std::sort(mSprites.begin(), mSprites.end(),
		[](SpriteComponent* a, SpriteComponent* b) {
		return a->GetDrawOrder() < b->GetDrawOrder();
	});
}

void Game::RemoveSprite(SpriteComponent* sprite) {
	auto search = std::find(std::begin(mSprites), std::end(mSprites), sprite);
	if (search != std::end(mSprites)) {
		mSprites.erase(search);
	}
}


// Returns texture from filename
SDL_Texture* Game::GetTexture(std::string filename) {
	auto search = mTextures.find(filename.c_str());
	if (search != mTextures.end()) {
		return mTextures[filename];
		//SDL_Log();
	}
	else {
		SDL_Surface *image;
		image = IMG_Load(filename.c_str());
		SDL_Texture* tex = SDL_CreateTextureFromSurface(mRenderer, image);
		SDL_FreeSurface(image);
		mTextures.insert({ filename, tex });
		return mTextures[filename.c_str()];
	}
	return nullptr;
}


vector2 Game::GetCameraPos() {
	return cameraPos;
}


void Game::SetCameraPos(float newX, float newY) {
	cameraPos.x = newX;
	cameraPos.y = newY;
}