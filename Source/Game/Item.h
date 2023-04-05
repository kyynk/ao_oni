#pragma once
namespace game_framework {
	class Item : public Entity {
	public:
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
		void SetParam(int delay, int boxX, int boxY, 
			itemName name);
		void Load(vector<string> filename, COLORREF color);
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
		void Animation(int n, int frame);
		void SetTrigger();
		string GetName();
		bool IsFixed();
		// e.g. 
		// if player on chair a = true, else a = false
		void IsOnTriPos(bool a);
		void Reset(); // all control reset

	private:
		// _aniType : 
		// 0, toggleOnce, 
		// 1, toggleOnceReverse, 
		// 2, select
		// 3, repeat
		int _anidelay,
			_playerX, _playerY,
			_boxX, _boxY, 
			_aniType, _aniFrame;
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