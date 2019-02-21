#pragma once

#include "SpriteComponent.h"
#include "SDL/SDL.h"
#include <vector>
#include <unordered_map>
#include <string>

class TiledBGComponent : public SpriteComponent
{
public:
	TiledBGComponent(class Actor* owner, int drawOrder = 50);

	void Draw(SDL_Renderer* renderer) override;

	void LoadTileCSV(const std::string& fileName, int tileWidth, int tileHeight);

private:
	int mTileWidth;
	int mTileHeight;
	std::vector<std::vector<int> > mCSV;
};