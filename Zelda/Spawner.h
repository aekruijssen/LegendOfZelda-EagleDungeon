#pragma once
#include "Actor.h"

class Spawner : public Actor
{
public:
	Spawner(class Game* game);
	void OnUpdate(float deltaTime) override;
	void SetType(std::string type);
	void Create();

private:
	std::string mType;
};
