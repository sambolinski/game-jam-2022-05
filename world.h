#pragma once
#include "olcPixelGameEngine.h"
#include "olcPGEX_TransformedView.h"
#include "vector"
namespace World {
	class Tile {
		olc::vi2d pos; //pos is an int as scene is tile based
		int size;
	public:
		Tile(olc::vi2d p, int sz) {
			pos = p;
			size = sz;
		}
		void Draw(olc::TileTransformedView* tv) {
			olc::vd2d formattedPos(pos * size);
			olc::vd2d formattedSize(size, size);
			tv->DrawRect(formattedPos, formattedSize);
			tv->DrawLine(formattedPos, formattedPos + formattedSize);
			tv->DrawLine(olc::vd2d(formattedPos.x, formattedPos.y+ formattedSize.y ), olc::vd2d(formattedPos.x+ formattedSize.x, formattedPos.y ));
		
		}
	};
	class Scene {

	public:
		//Scene Constants
		const float GRAVITATIONAL_ACCELERATION = 10.0f;
	private:
		std::vector<Tile*> tiles;
	public:
		void LoadScene() {
			for (int x = 0; x < 24; x++) {
				tiles.push_back(new Tile({ x,0 }, 10));
			}
			for (int x = 0; x < 7; x++) {
				for (int y = 15; y < 24; y++) {
					tiles.push_back(new Tile({ x,y }, 10));
				}
			}
			for (int x = 17; x < 24; x++) {
				for (int y = 15; y < 24; y++) {
					tiles.push_back(new Tile({ x,y }, 10));
				}
			}
		}
		void Draw(olc::TileTransformedView* tv) {
			//Draw All Tiles
			for (auto& t : tiles) {
				t->Draw(tv);
			}
		}
		~Scene() {
			//Delete all scene tiles
			for (auto &t:  tiles) {
				delete t;
			}
			tiles.clear();
		}
	};
}
