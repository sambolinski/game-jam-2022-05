#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#define OLC_PGEX_TRANSFORMEDVIEW
#include "olcPGEX_TransformedView.h"
#include "object.h"
#include "world.h"

// Override base class with your custom functionality
class Game : public olc::PixelGameEngine
{
public:
	Game()
	{
		// Name your application
		sAppName = "Example";
	}
private:
	olc::TileTransformedView tv;
	World::Scene scene;
public:
public:
	bool OnUserCreate() override
	{
		tv = olc::TileTransformedView({ ScreenWidth(),ScreenHeight() }, {10,10});
		tv.SetWorldOffset({ 0,0});

		scene.LoadScene();
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		Clear(olc::VERY_DARK_BLUE);
		scene.Update(this, &tv, fElapsedTime);
		scene.Draw(&tv);
		return true;
	}
};

int main()
{
	Game demo;
	if (demo.Construct(241, 241, 4, 4))
		demo.Start();
	return 0;
}