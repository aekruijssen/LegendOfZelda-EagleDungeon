#include "TiledBGComponent.h"
#include "Actor.h"
#include "Game.h"
#include "CSVHelper.h"

#include <iostream>
#include <fstream>
#include <string>

TiledBGComponent::TiledBGComponent(Actor* owner, int drawOrder)
	:SpriteComponent(owner, drawOrder)
{
	mOwner->GetGame()->AddSprite(this);
	mDrawOrder = drawOrder;
}

void TiledBGComponent::Draw(SDL_Renderer* renderer)
{
	if (mTexture)
	{
		int tilesPerRow = mTexWidth / mTileWidth;
		int numTile = 0;
		for (int i = 0; i < mCSV.size(); i++) {
			for (int j = 0; j < mCSV.at(i).size(); j++) {
				
				numTile = (mCSV.at(i)).at(j);
				int row = numTile / tilesPerRow;
				int col = numTile % tilesPerRow;

				SDL_Rect r;
				r.w = static_cast<int>(mTileWidth);
				r.h = static_cast<int>(mTileHeight);

				// Center the rectangle around the position of the owner
				// ScreenPos = WorldPos – CameraPos
				r.x = static_cast<int>(j*mTileWidth - mOwner->GetGame()->GetCameraPos().x);
				r.y = static_cast<int>(i*mTileHeight - mOwner->GetGame()->GetCameraPos().y);

				SDL_Rect srcRect;
				srcRect.w = static_cast<int>(mTileWidth);
				srcRect.h = static_cast<int>(mTileHeight);
				srcRect.x = static_cast<int>(col * mTileWidth);
				srcRect.y = static_cast<int>(row * mTileHeight);

				if ((mCSV.at(i)).at(j) != -1) {
				// Draw (have to convert angle from radians to degrees, and clockwise to counter)
					SDL_RenderCopyEx(renderer,
						mTexture,
						&srcRect, // Source rectangle (what to take from image)
						&r, // Destination rectangle (what to draw on screen)
						0.0, // No rotation
						nullptr,
						SDL_FLIP_NONE);

				}
			}
		}
	}
}

void TiledBGComponent::LoadTileCSV(const std::string& fileName, int tileWidth, int tileHeight) {
	char x;
	std::ifstream inFile;

	mTileWidth = tileWidth;
	mTileHeight = tileHeight;

	inFile.open(fileName);
	if (!inFile) {
		std::cout << "Unable to open file";
		exit(1); // terminate with error
	}

	std::string line = "";
	int row = 0;
	while (inFile >> line) {
		if (!line.empty()) {
			std::vector<std::string> CSV = CSVHelper::Split(line);
			std::vector<int> CSVRow;
			int col = 0;
			for (auto s : CSV) {
				int CSVInt = std::stoi(s);
				CSVRow.emplace_back(CSVInt);
				col++;
			}
			mCSV.emplace_back(CSVRow);
			row++;
		}
	}
	mCSV.pop_back();

	inFile.close(); 
}