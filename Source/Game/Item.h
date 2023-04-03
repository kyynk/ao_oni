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
			lib_book,
			key_lib,
			key_3F_L,
			key_2F_TL,
			key_basement,
			key_jail,
			key_annexe,
			broken_dish,
			tub_once,
			phillips, //screwdriver
			tub_fixed,
			flathead, //screwdriver
			lighter,
			oil,
			handkerchief,
			detergent, //wash
			door_knob,
			door_no_knob,
			tatami,
			gate, 
			toilet
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
		bool IsFixed();
		// e.g. 
		// if player on chair a = true, else a = false
		void IsOnTriPos(bool a);

	private:
		int _step, _anidelay,
			_playerX, _playerY,
			_boxX, _boxY;
		itemtype _type;
		move _move;
		move _pressing;
		itemName _name;
		// pick -> disappear, take -> leave
		bool _triggered, _close, 
			_press, _pick, _fixed, 
			_take, _onCorrectPos, 
			_useItem;
	};
}