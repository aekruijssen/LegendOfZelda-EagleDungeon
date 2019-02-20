#pragma once
#include "MoveComponent.h"
#include "Math.h"



class PlayerMove : public MoveComponent
{
public:
	PlayerMove(class Player* owner);

	// Update the move component
	void Update(float deltaTime) override;
	void ProcessInput(const Uint8 *keyState) override;

	// Getters/setters
	float GetYSpeed() const { return mYSpeed; }
	float GetXSpeed() const { return mXSpeed; }
	void SetYSpeed(float speed) { mYSpeed = speed; }
	void SetXSpeed(float speed) { mXSpeed = speed; }

	Player* mPlayer;
	Vector2 mPlayerVel;

	class Sword* mSword;

private:

	void UpdateAnim();
	void UpdateSword();

	// Angular speed (in radians/second)
	float mAngularSpeed;
	// Forward speed (in pixels/second)
	float mXSpeed;
	

	float mYSpeed;
	bool mSpacePressed;
	bool mInAir;
	const Uint8 *lastFrame;

	class Game* mGame;

	float attacking;
	float invulnerability;
};
