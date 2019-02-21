#include "AnimatedSprite.h"
#include "Actor.h"
#include "Game.h"

AnimatedSprite::AnimatedSprite(Actor* owner, int drawOrder)
	:SpriteComponent(owner, drawOrder)
{
}

void AnimatedSprite::Update(float deltaTime)
{
	if (!mAnimName.empty()) {
		if (!mIsPaused) {
			mAnimTimer += mAnimFPS * deltaTime;
		}
		// Do any needed wrapping of the time
		while (mAnimTimer >= mAnims.at(mAnimName).size()) {
			mAnimTimer -= mAnims.at(mAnimName).size();
		}
		SetTexture(mAnims.at(GetAnimName()).at((int)mAnimTimer));
	}
}

void AnimatedSprite::SetAnimation(const std::string& name, bool resetTimer)
{
	if (mAnimName != name)
	{
		mAnimName = name;
	}
	
	if (resetTimer)
	{
		mAnimTimer = 0.0f;
	}
}

void AnimatedSprite::AddAnimation(const std::string& name,
								  const std::vector<SDL_Texture*>& images)
{
	mAnims.emplace(name, images);
}
