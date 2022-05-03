#pragma once
#include "olcPixelGameEngine.h"

namespace GameData {
	class Object {
	public:
		virtual void Draw(olc::TileTransformedView* tv) = 0;
		virtual void Update(olc::PixelGameEngine* pge, olc::TileTransformedView* tv, float fElapsedTime) = 0;
	};
}
