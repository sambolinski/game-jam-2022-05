#pragma once
#include "olcPixelGameEngine.h"
#include "olcPGEX_TransformedView.h"
#include "object.h"

namespace GameData {
	class Player : public GameData::Object {
	private:
		olc::vd2d pos;
		olc::vd2d vel;
	public:
		Player() {
			Player({ 0,0 });
		}
		Player(olc::vd2d p) {
			pos = p;
			vel = { 0,0 };
		}

		//Interface Implementation
		void Draw(olc::TileTransformedView* tv) override {
			tv->FillRect(pos, { 10, 10 }, olc::VERY_DARK_RED);
		}

		//Handles player control
		void Update(olc::PixelGameEngine* pge, olc::TileTransformedView* tv, float fElapsedTime) override {
			if (pge->GetKey(olc::Key::W).bHeld)  vel += { 0, -1};
			if (pge->GetKey(olc::Key::A).bHeld)  vel += {-1, 0};
			if (pge->GetKey(olc::Key::S).bHeld)  vel += { 0, 1};
			if (pge->GetKey(olc::Key::D).bHeld)  vel += { 1, 0};
			pos += vel * fElapsedTime;
		}

		//Helpers

		olc::vd2d GetPos() const {
			return pos;
		}
	};
}