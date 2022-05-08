#pragma once
#include "olcPixelGameEngine.h"
#include "olcPGEX_TransformedView.h"
#include "object.h"

namespace item {
	class Item {
	protected:
		enum state {
			Standby,
			Ready,
			Activated
		};
		state currentState;
	public:
		Item() {
			currentState = state::Standby;
		}
		~Item() {}

		//Used when item in ready position (right click held)
		virtual void SetReady() = 0 {
			if (currentState == state::Standby) currentState = state::Ready;
					
		}
		//Used when left click pressed (once in standby)
		virtual void Activate() = 0  {
			if (currentState == state::Ready && currentState != state::Activated) currentState = state::Activated;
		}

		virtual void Draw() = 0  {}

		void DrawInvSprite(olc::TileTransformedView* tv, olc::vd2d boxLocation, olc::vd2d boxSize) {
			tv->FillCircle(boxLocation + boxSize * 0.5f, boxSize.x * 0.25f);//Change to sprite
		}
	};
	class Grapple: public Item {
	public:
		Grapple() {
		}
		~Grapple() {}

		//Used when item in ready position (right click held)
		virtual void SetReady() override {	
			Item::SetReady();
		}
		//Used when left click pressed (once in standby)
		virtual void Activate() override{
			Item::Activate();
		}

		virtual void Draw()override {}
	};

}

class Inventory {
	std::string selectedItem;
	std::map<std::string, item::Item*> store;
public:
	Inventory() {

	}
	~Inventory() {
		for (auto i : store) {
			delete i.second;
		}
		store.clear();
	}
	void AddToInventory(std::string name) {
		store.emplace(name, new item::Grapple());
	}
	void Draw(olc::TileTransformedView* tv, olc::vd2d boxLocation, olc::vd2d boxSize){
		tv->FillRect(boxLocation, boxSize * 0.8f, olc::DARK_GREY);//Change to sprite
		tv->FillRect(boxLocation+boxSize*0.025f, boxSize * 0.75f, olc::GREY);//Change to sprite
		store[selectedItem]->DrawInvSprite(tv, boxLocation, boxSize*0.8f);
	}
};

class Player : public olc::aabb::rect {
public:
	Inventory inventory;
public:
	Player() {
		Player({ 0,0 }, {10,10});
	}
	Player(olc::vd2d p, olc::vd2d sz) {
		pos = p;
		size = sz;
		vel = { 0,0 };
		inventory.AddToInventory("");
	}

	//Interface Implementation
	void Draw(olc::TileTransformedView* tv) override {
		tv->FillRect(pos, size, olc::VERY_DARK_RED);
		inventory.Draw(tv, { 1.0f,0.5f}, {4,4});
	}

	//Handles player control
	void Update(olc::PixelGameEngine* pge, olc::TileTransformedView* tv, float fElapsedTime) override {
		if (pge->GetKey(olc::Key::A).bHeld) ApplyForce({-10.0f, 0.0f });
		if (pge->GetKey(olc::Key::D).bHeld) ApplyForce({ 10.0f, 0.0f });
		if (pge->GetKey(olc::Key::SPACE).bPressed)  ApplyForce( { 0, -1000.0f });
	}
};