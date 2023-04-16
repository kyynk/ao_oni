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
			lib_book,				 //v
			key_lib,				 //v
			key_3F_L,				 //v
			key_2F_TL,
			key_basement,			 //v
			key_jail,
			key_annexe,
			broken_dish,			 //v
			tub_once,				 //v
			phillips, //screwdriver  //v
			tub_fixed,				 //v
			flathead, //screwdriver
			lighter,				 //v
			oil,
			handkerchief,			 //v
			detergent, //wash		 //v
			door_knob,
			door_no_knob,
			tatami_l,				 //v
			tatami_r,				 //v
			gate,					 //v
			toilet,					 //v
			bed,					 //v
			bookcase_l,				 //v
			bookcase_r,				 //v
			white_bookcase,			 //v
			bookcase_map21			 //v
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
		// e.g. 
		// if player on chair a = true, else a = false
		void IsOnTriPos(bool a);
		void Reset(); // all control reset
		bool IsPick();
		bool IsFixed();
		bool IsTake();
		bool IsAnimationDone();

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
		// index for bool (under)
		// _triggered, _close, _press, _pick, 
		// _fixed, _take, _onCorrectPos, _useItem
		vector<bool> utiltriggers = {
		false, true, false, false, false, 
		false, false, false};
	};
}