#pragma once
#include "Component.h"
#include "Math.h"
#include <functional>

class Collectible : public Component
{
public:
	Collectible(class Actor* owner);
	void Update(float deltaTime);
	void SetOnCollect(std::function<void()> onCollect);


private:
	class Actor* mOwner;

	std::function<void()> mOnCollect;
};

