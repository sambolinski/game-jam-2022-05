#include "olcPixelGameEngine.h"

namespace GameData {
	class Object {
	public:
		virtual void Draw(olc::PixelGameEngine* pge) = 0;
	};

	class Player : public Object{
	private:
		olc::vd2d pos;
		olc::vd2d vel;
	public:
		void Draw(olc::PixelGameEngine* pge) override {
			pge->FillCircle(pos, 10, olc::VERY_DARK_RED);
		}
	};
}