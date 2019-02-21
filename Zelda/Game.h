#pragma once

#include "SDL/SDL.h"

#include <vector>
#include <unordered_map>
#include <SDL/SDL_stdinc.h>
#include <string>

/*
	• A constructor
	• Initialize – No parameters and returns bool
	• Shutdown – No parameters and returns void
	• RunLoop – No parameters and returns void 
*/

struct vector2 {
	float x;
	float y;
};

class Game {
	public:
		Game();
		bool Initialize();
		void RunLoop();
		void ShutDown();

		class Actor* mActor;
		void AddActor(Actor*);
		void RemoveActor(Actor*);

		class SpriteComponent* mSprite;
		void AddSprite(SpriteComponent*);
		void RemoveSprite(SpriteComponent*);
		SDL_Texture* GetTexture(std::string);

		std::vector<class Collider*> mColliders;
		void AddCollider(Collider*);
		void RemoveCollider(Collider*);

		std::unordered_map<std::string, std::vector<class Actor*> > mEnemies;
		void addEnemy(Actor*);
		void removeEnemy(Actor*, std::string enRoom);
		std::unordered_map<std::string, std::vector<class Spawner*> > mSpawns;
		std::unordered_map<std::string, std::vector<class SecretBlock*> > mSecretBlocks;
		std::unordered_map<std::string, std::vector<class Door*> > mDoors;
		void AddDoor(Door*);
		//void RemoveDoor(Door*);
		std::string currRoom;
		//std::unordered_map<std::string, std::string > visitedRooms;
		std::vector<std::string> visitedRooms;

		vector2 GetCameraPos();
		void SetCameraPos(float newX, float newY);

		class Player* mPlayer;
		Player* GetPlayer();

		class Mix_Chunk* GetSound(const std::string& filename);
		Mix_Chunk* mChunk;
		int mChannel;

	private:
		void processInput();
		void updateGame();
		void generateOutput();
		void LoadLevel();
		void LoadData();
		void UnloadData();

		SDL_Renderer* mRenderer;
		SDL_Window* mWindow;
		bool mIsRunning;
		const int thickness = 15;
		Uint32 mTicksCount;
		vector2 cameraPos;

		std::vector<Actor*> mActors;
		std::unordered_map<std::string, SDL_Texture*> mTextures;
		std::vector<SpriteComponent*> mSprites;
};
