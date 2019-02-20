#include "PlayerMove.h"
#include "Actor.h"
#include "CollisionComponent.h"
#include "Game.h"
#include "Math.h"
#include "Player.h"
#include "AnimatedSprite.h"
#include "SDL/SDL_mixer.h"
#include "Collider.h"
#include "Door.h"
#include "SecretBlock.h"
#include "Spawner.h"
#include "Sword.h"
#include "EnemyComponent.h"


PlayerMove::PlayerMove(class Player* owner)
	:MoveComponent(owner)
	, mXSpeed(0.0f)
	, mYSpeed(0.0f)
{
	mPlayer = owner;
	mPlayerVel.x = 0;
	mPlayerVel.y = 0;

	mSpacePressed = false;
	mGame = mPlayer->GetGame();

	mSword = new Sword(mGame);
	attacking = 0.0f;

	invulnerability = 0.0f;
}

// Use the left and right arrow keys 
void PlayerMove::ProcessInput(const Uint8 *keyState) {
	//float move = 0;
	if (keyState[SDL_SCANCODE_RIGHT]) {
		mXSpeed = 130.0f;
		mYSpeed = 0.0f;
	}
	else if (keyState[SDL_SCANCODE_LEFT]) {
		mXSpeed = -130.0f;
		mYSpeed = 0.0f;
	}
	else if (keyState[SDL_SCANCODE_UP]){
		mYSpeed = -130.0f;
		mXSpeed = 0.0f;
	}
	else if (keyState[SDL_SCANCODE_DOWN]) {
		mYSpeed = 130.0f;
		mXSpeed = 0.0f;
	}
	else {
		mXSpeed = 0.0f;
		mYSpeed = 0.0f;
		mPlayer->as->SetIsPaused(true);
	}

	if(keyState[SDL_SCANCODE_SPACE] && !mSpacePressed){
		attacking = 0.25f;
		Mix_PlayChannel(-1, mGame->GetSound("Assets/Sounds/SwordSlash.wav"), 0);
		UpdateSword();
	}
	// At the end of ProcessInput, save the current keyState value in
	mSpacePressed = keyState[SDL_SCANCODE_SPACE];
}

// Update - updates position/rotation of Actor
void PlayerMove::Update(float deltaTime) {

	if (invulnerability > 0.0f) {
		invulnerability -= deltaTime;
	}
	if (attacking > 0.0f) {
		attacking -= deltaTime;
		try {
			if (mPlayer->mGame->mEnemies.size() > 0) {
				if (mPlayer->GetGame()->mEnemies.at(mPlayer->GetGame()->currRoom).size() > 0) {
					std::vector<Actor*> enemies = mPlayer->GetGame()->mEnemies.at(mPlayer->GetGame()->currRoom);
					for (auto s : enemies) {
						if (s!= nullptr && s->GetState() != ActorState::Destroy) {
							float sX = s->GetPosition().x;
							float sY = s->GetPosition().y;
							Vector2 sPos = Vector2(sX, sY);
							float playerX = mPlayer->GetPosition().x;
							float playerY = mPlayer->GetPosition().y;
							Vector2 playerPos = Vector2(playerX, playerY);
							float swordX = mSword->GetPosition().x;
							float swordY = mSword->GetPosition().y;
							Vector2 swordPos = Vector2(swordX, swordY);

							CollisionComponent* c = s->GetComponent<CollisionComponent>();
							CollisionComponent* swordCol = mSword->cc;

							if (c != nullptr && swordCol != nullptr) {
								CollSide cs = (c)->GetMinOverlap(swordCol, sPos);
								if (cs != CollSide::None) {
									EnemyComponent* e = s->GetComponent<EnemyComponent>();
									if (e != nullptr) {
										e->TakeDamage(1);
									}
								}
							}
						}
					}
				}
			}
		}
		catch (std::exception) {

		}
	}
	else {

		try {
			if (mPlayer->mGame->mEnemies.size() > 0) {
				if (mPlayer->GetGame()->mEnemies.at(mPlayer->GetGame()->currRoom).size() > 0) {
					std::vector<Actor*> enemies = mPlayer->GetGame()->mEnemies.at(mPlayer->GetGame()->currRoom);
					for (auto s : enemies) {
						if (s != nullptr && s->GetState() != ActorState::Destroy) {
							float sX = s->GetPosition().x;
							float sY = s->GetPosition().y;
							Vector2 sPos = Vector2(sX, sY);
							float playerX = mPlayer->GetPosition().x;
							float playerY = mPlayer->GetPosition().y;
							Vector2 playerPos = Vector2(playerX, playerY);
							float swordX = mSword->GetPosition().x;
							float swordY = mSword->GetPosition().y;
							Vector2 swordPos = Vector2(swordX, swordY);

							CollisionComponent* c = s->GetComponent<CollisionComponent>();

							if (c != nullptr) {
								CollSide cs = (mPlayer->cc)->GetMinOverlap(c, playerPos);
								if (cs != CollSide::None) {
									if (invulnerability <= 0.0f) {
										mPlayer->TakeDamage(1);
										invulnerability = 0.5f;

										Vector2 curPlayerPos = mPlayer->GetPosition();
										curPlayerPos.x += playerPos.x;
										curPlayerPos.y += playerPos.y;

										if (mPlayer->as->GetAnimName() == "walkLeft" || mPlayer->as->GetAnimName() == "AttackLeft") {
											curPlayerPos.x += 32.0f;
										}
										else if (mPlayer->as->GetAnimName() == "walkRight" || mPlayer->as->GetAnimName() == "AttackRight") {
											curPlayerPos.x -= 32.0f;
										}
										else if (mPlayer->as->GetAnimName() == "walkUp" || mPlayer->as->GetAnimName() == "AttackUp") {
											curPlayerPos.y += 32.0f;
										}
										else if (mPlayer->as->GetAnimName() == "walkDown" || mPlayer->as->GetAnimName() == "AttackDown") {
											curPlayerPos.y -= 32.0f;
										}
										
										mPlayer->SetPosition(curPlayerPos);
									}
								}
							}
						}
					}
				}
			}
		}
		catch (std::exception) {

		}

		Vector2 pos = mPlayer->GetPosition();
		pos.x += mXSpeed * deltaTime;
		pos.y += mYSpeed * deltaTime;
		mOwner->SetPosition(pos);

		pos.x -= 300;
		pos.y -= 224;
		mPlayer->GetGame()->SetCameraPos(pos.x, pos.y);
		UpdateAnim();


		std::vector<SecretBlock*> secretBlocks = mPlayer->GetGame()->mSecretBlocks.at(mPlayer->GetGame()->currRoom);

		int newRoom = 1;
		std::vector<std::string> visited;
		for (auto v : visited) {
			if (v == mGame->currRoom) {
				newRoom = 0;
			}
		}
		if (newRoom == 1) {
			visited.emplace_back(mGame->currRoom);
			std::vector<Spawner*> spawns = mPlayer->GetGame()->mSpawns.at(mPlayer->GetGame()->currRoom);
			for (auto s : spawns) {
				s->Create();
			}
		}
		//if (mGame->visitedRooms.find(mGame->currRoom) != mGame->visitedRooms.end()) {
		//	mGame->visitedRooms.emplace(mGame->currRoom);
		//	std::vector<Spawner*> spawns = mPlayer->GetGame()->mSpawns.at(mPlayer->GetGame()->currRoom);

		//	for (auto s : spawns) {
		//		s->Create();
		//	}
		//}


		//Secret Blocks
		for (auto block : secretBlocks) {
			float blockX = block->GetPosition().x;
			float blockY = block->GetPosition().y;
			Vector2* blockPos = new Vector2(blockX, blockY);
			float playerX = mPlayer->GetPosition().x;
			float playerY = mPlayer->GetPosition().y;
			Vector2 playerPos = Vector2(playerX, playerY);

			CollSide cs = (mPlayer->cc)->GetMinOverlap(block->cc, playerPos);
			if (cs != CollSide::None) {
				Vector2 curPos = block->GetPosition();
				if (block->mDir == "Right") {
					if (curPos.x > block->originalPos.x - 32) {
						if (mXSpeed < 0) {
							curPos.x += playerPos.x;
							block->SetPosition(curPos);
						}
					}
					std::vector<Door*> doors = mPlayer->GetGame()->mDoors.at(mPlayer->GetGame()->currRoom);
					for (auto door : doors) {
						if (door->mState == "Closed") {
							door->mState = "Open";
							door->updateState("Open");
							Mix_PlayChannel(-1, mGame->GetSound("Assets/Sounds/DoorOpen.wav"), 0);
							Mix_PlayChannel(-1, mGame->GetSound("Assets/Sounds/Secret.wav"), 0);
						}
					}
				}
				else if (block->mDir == "Left") {
					if (curPos.x < block->originalPos.x + 32) {
						if (mXSpeed > 0) {
							curPos.x -= playerPos.x;
							block->SetPosition(curPos);
						}
					}
					else {
						std::vector<Door*> doors = mPlayer->GetGame()->mDoors.at(mPlayer->GetGame()->currRoom);
						for (auto door : doors) {
							if (door->mState == "Closed") {
								door->mState = "Open";
								door->updateState("Open");
								Mix_PlayChannel(-1, mGame->GetSound("Assets/Sounds/DoorOpen.wav"), 0);
								Mix_PlayChannel(-1, mGame->GetSound("Assets/Sounds/Secret.wav"), 0);
							}
						}
					}
				}
				else if (block->mDir == "Up") {
					if (curPos.y < block->originalPos.y + 32) {
						if (mYSpeed > 0) {
							curPos.y += playerPos.y;
							block->SetPosition(curPos);
						}
					}
					std::vector<Door*> doors = mPlayer->GetGame()->mDoors.at(mPlayer->GetGame()->currRoom);
					for (auto door : doors) {
						if (door->mState == "Closed") {
							door->mState = "Open";
							door->updateState("Open");
							Mix_PlayChannel(-1, mGame->GetSound("Assets/Sounds/DoorOpen.wav"), 0);
							Mix_PlayChannel(-1, mGame->GetSound("Assets/Sounds/Secret.wav"), 0);
						}
					}
				}
				else if (block->mDir == "Down") {
					if (curPos.y > block->originalPos.y - 32) {
						if (mYSpeed < 0) {
							curPos.y -= playerPos.y;
							block->SetPosition(curPos);
						}
					}
					std::vector<Door*> doors = mPlayer->GetGame()->mDoors.at(mPlayer->GetGame()->currRoom);
					for (auto door : doors) {
						if (door->mState == "Closed") {
							door->mState = "Open";
							door->updateState("Open");
							Mix_PlayChannel(-1, mGame->GetSound("Assets/Sounds/DoorOpen.wav"), 0);
							Mix_PlayChannel(-1, mGame->GetSound("Assets/Sounds/Secret.wav"), 0);
						}
					}
				}

				Vector2 curPlayerPos = mPlayer->GetPosition();
				curPlayerPos.x += playerPos.x;
				curPlayerPos.y += playerPos.y;
				mPlayer->SetPosition(curPlayerPos);
				return;
			}
		}

		//Doors
		std::vector<Door*> doors = mPlayer->GetGame()->mDoors.at(mPlayer->GetGame()->currRoom);
		for (auto block : doors) {
			float blockX = block->GetPosition().x;
			float blockY = block->GetPosition().y;
			Vector2* blockPos = new Vector2(blockX, blockY);
			float playerX = mPlayer->GetPosition().x;
			float playerY = mPlayer->GetPosition().y;
			Vector2 playerPos = Vector2(playerX, playerY);

			CollSide cs = (mPlayer->cc)->GetMinOverlap(block->cc, playerPos);
			if (cs != CollSide::None) {
				if (block->mState == "Locked" && (mPlayer->numKeys > 0)) {
					block->updateState("Open");
					Mix_PlayChannel(-1, mGame->GetSound("Assets/Sounds/DoorOpen.wav"), 0);
					mPlayer->numKeys--;
				}
				if (block->mState == "Open") {
					try {
						if (mPlayer->mGame->mEnemies.size() > 0) {
							if (mPlayer->GetGame()->mEnemies.at(mPlayer->GetGame()->currRoom).size() > 0) {
								std::vector<Actor*> enemies = mPlayer->GetGame()->mEnemies.at(mPlayer->GetGame()->currRoom);
								for (auto s : enemies) {
									s->SetState(ActorState::Paused);
								}
							}
						}
					}
					catch (std::exception) {

					}
					if (block->mDirection == "Right") {
						if (mXSpeed > 0) {
							playerPos.x += 150;
							mPlayer->GetGame()->currRoom = block->mDestination;
						}
					}
					else if (block->mDirection == "Left") {
						if (mXSpeed < 0) {
							playerPos.x -= 150;
							mPlayer->GetGame()->currRoom = block->mDestination;
						}
					}
					else if (block->mDirection == "Up") {
						if (mYSpeed < 0) {
							playerPos.y -= 150;
							mPlayer->GetGame()->currRoom = block->mDestination;
						}
					}
					else if (block->mDirection == "Down") {
						if (mYSpeed < 0) {
							playerPos.y += 150;
							mPlayer->GetGame()->currRoom = block->mDestination;
						}
					}
					Vector2 curPos = mPlayer->GetPosition();
					curPos.x += playerPos.x;
					curPos.y += playerPos.y;
					mPlayer->SetPosition(curPos);

					if (mPlayer->GetGame()->mEnemies.size() > 0) {
						try {
							if (mPlayer->GetGame()->mEnemies.at(mPlayer->GetGame()->currRoom).size() > 0) {
								std::vector<Actor*> enms = mPlayer->GetGame()->mEnemies.at(mPlayer->GetGame()->currRoom);
								for (auto s : enms) {
									s->SetState(ActorState::Active);
								}
							}
						}
						catch (std::exception) {

						}
					}
					return;
				}
			}
		}

		// Colliders
		for (auto block : (mPlayer->GetGame())->mColliders) {
			float blockX = block->GetPosition().x;
			float blockY = block->GetPosition().y;
			Vector2* blockPos = new Vector2(blockX, blockY);
			float playerX = mPlayer->GetPosition().x;
			float playerY = mPlayer->GetPosition().y;
			Vector2 playerPos = Vector2(playerX, playerY);

			CollSide cs = (mPlayer->cc)->GetMinOverlap(block->cc, playerPos);
			// Didn’t collide with the block, so move on
			if (cs != CollSide::None) {
				// Landing on top of block, so set mYSpeed = 0, which stops Mario from moving down
				if (cs == CollSide::Top && mYSpeed > 0.0f) {
					mYSpeed = 0.0f;
				}
				else if (cs == CollSide::Bottom && mYSpeed < 0)
				{
					mYSpeed = 0.0f;
				}
				else if (cs == CollSide::Left && mXSpeed < 0)
				{
					mXSpeed = 0.0f;
				}
				else if (cs == CollSide::Bottom && mXSpeed > 0)
				{
					mXSpeed = 0.0f;
				}
				// Change the player’s position by the offset vector you get from GetMinOverlap
				Vector2 curPos = mPlayer->GetPosition();
				curPos.x += playerPos.x;
				curPos.y += playerPos.y;
				mPlayer->SetPosition(curPos);
			}
		}


		/*
		if (mPlayer->GetState() == ActorState::Active) {
			Vector2 pos = mOwner->GetPosition();
			pos.x += mForwardSpeed * deltaTime;
			pos.y += mYSpeed * deltaTime;
			mOwner->SetPosition(pos);

			if (mPlayer->GetPosition().x > 6368) {
				Mix_PlayChannel(-1, mGame->GetSound("Assets/Sounds/StageClear.wav"), 0);
				Mix_HaltChannel(mGame->mChannel);
				mPlayer->SetState(ActorState::Paused);
			}

			else if (mPlayer->GetPosition().y > 465.0f) {
				mPlayer->ChangeTexture("Assets/Mario/Dead.png");
				Mix_PlayChannel(-1, mGame->GetSound("Assets/Sounds/Dead.wav"), 0);
				Mix_HaltChannel(mGame->mChannel);
				mPlayer->SetState(ActorState::Paused);
			}

			if (mPlayer->GetState() == ActorState::Active) {
				// Block Collision
				bool collide = false;
				for (auto block : (mPlayer->GetGame())->mBlocks) {
					float blockX = block->GetPosition().x;
					float blockY = block->GetPosition().y;
					Vector2* blockPos = new Vector2(blockX, blockY);
					float playerX = mPlayer->GetPosition().x;
					float playerY = mPlayer->GetPosition().y;
					Vector2 playerPos = Vector2(playerX, playerY);

					CollSide cs = (mPlayer->cc)->GetMinOverlap(block->cc, playerPos);
					// Didn’t collide with the block, so move on
					if (cs != CollSide::None) {

						// Landing on top of block, so set mYSpeed = 0, which stops Mario from moving down
						if (cs == CollSide::Top && mYSpeed > 0.0f) {
							mYSpeed = 0.0f;
							mInAir = false;
						}
						else if (cs == CollSide::Bottom && mYSpeed < 0)
						{
							mYSpeed = 0.0f;
							Mix_PlayChannel(-1, mPlayer->GetGame()->GetSound("Assets/Sounds/Bump.wav"), 0);
						}

						// Change the player’s position by the offset vector you get from GetMinOverlap
						Vector2 curPos = mPlayer->GetPosition();
						curPos.x += playerPos.x;
						curPos.y += playerPos.y;
						mPlayer->SetPosition(curPos);
						collide = true;
					}
				}

				if (collide == false) {
					mInAir = true;
				}
				if (mPlayer->GetPosition().y > 488.0f) {
					mInAir = false;
				}

				// Goomba Stomped IF Mario hits the top of the Goomba OR Mario hits the left / right of the Goomba AND mInAir is true
				for (auto enemy : (mPlayer->GetGame())->mGoombas) {
					if (enemy->stomped == false) {
						float enemyX = enemy->GetPosition().x;
						float enemyY = enemy->GetPosition().y;
						Vector2* enemyPos = new Vector2(enemyX, enemyY);
						float playerX = mPlayer->GetPosition().x;
						float playerY = mPlayer->GetPosition().y;
						Vector2 playerPos = Vector2(playerX, playerY);

						CollSide cs = (mPlayer->cc)->GetMinOverlap(enemy->cc, playerPos);
						if (cs != CollSide::None) {
							if (cs == CollSide::Top && mYSpeed > 0.0f) {
								mYSpeed = -350.0f;
								mInAir = true;
								enemy->stomped = true;
								Mix_PlayChannel(-1, mPlayer->GetGame()->GetSound("Assets/Sounds/Stomp.wav"), 0);
							}
							else if (cs == CollSide::Left && mInAir == true)
							{
								mYSpeed = -350.0f;
								enemy->stomped = true;
							}
							else if (cs == CollSide::Right && mInAir == true)
							{
								mYSpeed = -350.0f;
								enemy->stomped = true;
							}
							else {
								mPlayer->ChangeTexture("Assets/Mario/Dead.png");
								Mix_PlayChannel(-1, mGame->GetSound("Assets/Sounds/Dead.wav"), 0);
								Mix_HaltChannel(mGame->mChannel);
								mPlayer->SetState(ActorState::Paused);
							}
							Vector2 curPos = mPlayer->GetPosition();
							curPos.x += playerPos.x;
							curPos.y += playerPos.y;
							mPlayer->SetPosition(curPos);
							collide = true;
						}
					}
				}

				mYSpeed += 2000.0f * deltaTime;

				// Don’t change camera.x if you would decrease camera.x
				if ((mPlayer->GetPosition().x - 300) >= mPlayer->GetGame()->GetCameraPos().x) {
					mPlayer->GetGame()->SetCameraPos(mPlayer->GetPosition().x - 300);
				}
				// Don’t let the camera.x go lower than 0 (to fix the problem at the start)
				if (mPlayer->GetGame()->GetCameraPos().x <= 0) {
					mPlayer->GetGame()->SetCameraPos(0);
				}
				// Don’t let the player.x ever be less than the camera.x
				else if (mPlayer->GetPosition().x <= mPlayer->GetGame()->GetCameraPos().x) {
					Vector2 vec = mPlayer->GetPosition();
					vec.x = mPlayer->GetGame()->GetCameraPos().x;
					mPlayer->SetPosition(vec);
				}
			}
		}
		UpdateAnim(); */
	}
	UpdateSword();
}

void PlayerMove::UpdateSword() {
	if (attacking <= 0.0f) {
		mSword->sc->SetIsVisible(false);
	}
	else {
		mSword->sc->SetIsVisible(true);
		
		//If the player’s facing up : Texture is “Assets / Sword / Up.png” and Offset is(-8, -16)
		if (mPlayer->as->GetAnimName() == "walkUp") {
			mSword->sc->SetTexture(mGame->GetTexture("Assets/Sword/Up.png"));
			mSword->SetPosition(mPlayer->GetPosition() + Vector2(-8, -16));
			mPlayer->as->SetAnimation("AttackUp");
		}
		//Facing down : Texture is “Assets / Sword / Down.png” and Offset is(8, 16)
		else if (mPlayer->as->GetAnimName() == "walkDown") {
			mSword->sc->SetTexture(mGame->GetTexture("Assets/Sword/Down.png"));
			mSword->SetPosition(mPlayer->GetPosition() + Vector2(8, 16));
			mPlayer->as->SetAnimation("AttackDown");
		}
		//Facing left : Texture is “Assets / Sword / Left.png” and Offset is(-16, 4)
		else if (mPlayer->as->GetAnimName() == "walkLeft") {
			mSword->sc->SetTexture(mGame->GetTexture("Assets/Sword/Left.png"));
			mSword->SetPosition(mPlayer->GetPosition() + Vector2(-16, 4));
			mPlayer->as->SetAnimation("AttackLeft");
		}
		else if(mPlayer->as->GetAnimName() == "walkRight") {
			mSword->sc->SetTexture(mGame->GetTexture("Assets/Sword/Right.png"));
			mSword->SetPosition(mPlayer->GetPosition() + Vector2(16, 4));			mPlayer->as->SetAnimation("AttackRight");		}
	}
}

void PlayerMove::UpdateAnim() {
	if (mXSpeed > 0) {
		mPlayer->as->SetIsPaused(false);
		mPlayer->as->SetAnimation("walkRight");
	}
	else if(mXSpeed < 0) {
		mPlayer->as->SetIsPaused(false);
		mPlayer->as->SetAnimation("walkLeft");
	}
	else if (mYSpeed > 0) {
		mPlayer->as->SetIsPaused(false);
		mPlayer->as->SetAnimation("walkDown");
	}
	else if (mYSpeed < 0) {
		mPlayer->as->SetIsPaused(false);
		mPlayer->as->SetAnimation("walkUp");
	}
	else {
		mPlayer->as->SetIsPaused(true);
	}
}