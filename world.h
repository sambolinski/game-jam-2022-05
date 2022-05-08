#pragma once
#include "olcPixelGameEngine.h"
#include "olcPGEX_TransformedView.h"
#include "object.h"
#include "player.h"
#include "physics.h"
#include "vector"
namespace World {

	//Main Scene class, will handle tile layout, player, and scene updates.
	class Scene {
	public:
		//Scene Constants
		const olc::vd2d gravity = { 0.0f,10.0f };
	private:
		std::map<std::pair<int,int>,Tile*>level;
		Player player;
		physics::Rope rope;
	public:
		void LoadScene() {
			//default size for rects
			olc::vd2d size({ 1,1 });
			//Load Player
			player = Player({ 2,5 }, size);

			rope = physics::Rope();

			//Test tile scene
			for (int x = 0; x < 24; x++) {
				level[std::pair<int,int>(x,0)] = new Tile(size * olc::vd2d{x * 1.0f , 0}, size);
			}
			for (int x = 0; x < 7; x++) {
				for (int y = 15; y < 24; y++) {
					level[std::pair<int, int>(x, y)] = new Tile(size * olc::vd2d{ x * 1.0f , y * 1.0f }, size);
				}
			}
			for (int x = 17; x < 24; x++) {
				for (int y = 15; y < 24; y++) {
					level[std::pair<int, int>(x, y)] = new Tile(size * olc::vd2d{ x * 1.0f , y*1.0f }, size);
				}
			}
		}

		void Draw(olc::TileTransformedView* tv) {
			//Draw all Tiles
			for (auto t : level) {
				t.second->Draw(tv);
			}

			//Draw Player
			player.Draw(tv);

			rope.Draw(tv);
		}

		void Update(olc::PixelGameEngine* pge, olc::TileTransformedView* tv, float fElapsedTime) {
			
			//Follow player
				//tv->SetWorldOffset(player.pos + olc::vd2d(-12,-12));
			// Update the player

			rope.Update(fElapsedTime, gravity, true);

			//Update positions
			player.ApplyForce(gravity);
			player.Update(pge, tv, fElapsedTime);
			player.UpdateVelocity(fElapsedTime);

			//HandleCollision
			HandleCollisions(fElapsedTime);

			//Update positions
			player.UpdatePosition(fElapsedTime);
		}

		void HandleCollisions(float fElapsedTime) {
			olc::vf2d cp, cn;
			float t = 0, min_t = INFINITY;
			std::vector<std::pair<std::pair<int,int>, float>> z;

			// Work out collision point, add it to vector along with rect ID
			//Only check in range next to player
			olc::vi2d playerTile((int)(player.pos.x), (int)(player.pos.y));
			int i = 0;
			for (int x = playerTile.x - 2; x <= playerTile.x + 2; x++) {
				for (int y = playerTile.y - 2; y <= playerTile.y + 2; y++) {
					if (level.count(std::pair<int, int>(x, y)) == 0) {
						continue;
					}
					if (olc::aabb::DynamicRectVsRect(&player, fElapsedTime,  *level[std::pair<int, int>(x, y)], cp, cn, t))
					{
						z.push_back({ {x,y}, t });
					}
					i++;
				}
			}

			std::sort(z.begin(), z.end(), [](const std::pair<std::pair<int, int>, float>& a, const std::pair<std::pair<int, int>, float>& b)
				{
					return a.second < b.second;
				});

			// Now resolve the collision in correct order 
			for (auto j : z)
				olc::aabb::ResolveDynamicRectVsRect(&player, fElapsedTime, level[j.first]);
		}

		~Scene() {
			//Delete all scene tiles
			for (auto &t:  level) {
				delete t.second;
			}
			level.clear();
		}
	};
};
