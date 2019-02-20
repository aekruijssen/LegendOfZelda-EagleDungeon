#pragma once
#include "SpriteComponent.h"
#include "SDL/SDL.h"
#include <vector>
#include <unordered_map>
#include <string>

class AnimatedSprite : public SpriteComponent
{
public:
	// (Lower draw order corresponds with further back)
	AnimatedSprite(class Actor* owner, int drawOrder = 100);

	// Update the AnimatedSprite
	void Update(float deltaTime) override;
	
	// Add an animation of the corresponding name to the animation map
	void AddAnimation(const std::string& name,
					  const std::vector<SDL_Texture*>& images);
	
	// Set the current active animation
	// (If resetTimer is true, resets mAnimTimer to 0)
	void SetAnimation(const std::string& name, bool resetTimer = false);
	
	// Use to pause/unpause the animation
	void SetIsPaused(bool pause) { mIsPaused = pause; }
	
	// Get the name of the currently-playing animation
	const std::string& GetAnimName() const { return mAnimName; }
protected:
	// Map of animation name to vector of textures corresponding to the animation
	std::unordered_map<std::string, std::vector<SDL_Texture*>> mAnims;
	
	// Name of current animation
	std::string mAnimName;
	
	// Whether or not the animation is paused (defaults to false)
	bool mIsPaused = false;
	
	// Tracks current elapsed time in animation
	float mAnimTimer = 0.0f;
	
	// The frames per second the animation should run at
	float mAnimFPS = 5.0f;
};
