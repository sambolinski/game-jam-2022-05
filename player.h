#pragma once
#include "olcPixelGameEngine.h"
#include "olcPGEX_TransformedView.h"
#include "object.h"

class Player : public olc::aabb::rect {
public:
	Player() {
		Player({ 0,0 }, {10,10});
	}
	Player(olc::vd2d p, olc::vd2d sz) {
		pos = p;
		size = sz;
		vel = { 0,0 };
	}

	//Interface Implementation
	void Draw(olc::TileTransformedView* tv) override {
		tv->FillRect(pos, size, olc::VERY_DARK_RED);
	}

	//Handles player control
	void Update(olc::PixelGameEngine* pge, olc::TileTransformedView* tv, float fElapsedTime) override {
		if (pge->GetKey(olc::Key::W).bHeld)  vel += { 0, -0.1f};
		if (pge->GetKey(olc::Key::A).bHeld)  vel += {-0.1f, 0};
		if (pge->GetKey(olc::Key::S).bHeld)  vel += { 0, 0.1f};
		if (pge->GetKey(olc::Key::D).bHeld)  vel += { 0.1f, 0};
	}

	void UpdatePosition(float fElapsedTime) {
		pos += vel * fElapsedTime;
	}
};