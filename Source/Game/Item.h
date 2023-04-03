#pragma once
namespace game_framework {
	class Item : public Entity {
	public:
		enum itemtype {
			select,
			once,
			repeat
		};
		enum move {
			none,
			isup,
			isdown,
			isleft,
			isright
		};
		enum itemName {
			key_lib,
			key_3F_L,
			key_2F_TL,
			broken_dash,
			tub_once,
			flathead, //screwdriver
			tub_fixed,
			phillips, //screwdriver
			lighter,
			oil,
			handkerchief,
			detergent, //wash
			door_knob,
			door_no_knob
		};
		Item();
		~Item();
		void SetParam(int step, int delay, itemtype type,
			int boxX, int boxY, itemName name);
		void Load(vector<string> filename, COLORREF color);
		void SetPos(int x, int y);
		int GetPosX();
		int GetPosY();
		int GetPosL();
		int GetPosU();
		int GetPosR();
		int GetPosD();
		void GetPlayerPos(int x, int y);
		void CheckMoveDirection();
		void OnMove();
		void OnKeyDown(UINT nChar);
		void OnKeyUp(UINT nChar);
		void OnShow();
		bool Collide();
		void Animation(int n);
		void SetTrigger();
		string GetName();

	private:
		int _step, _anidelay,
			_playerX, _playerY,
			_boxX, _boxY;
		bool _press;
		itemtype _type;
		move _move;
		move _pressing;
		itemName _name;
		bool _triggered, _close;
	};
}