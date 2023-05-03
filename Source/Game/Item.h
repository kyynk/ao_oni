#pragma once
namespace game_framework {
	class Item : public Entity {
	public:
		enum ItemName {
			lib_book,					 //v
			key_lib,					 //v
			key_3F_L,					 //v
			key_2F_TL, //need password
			key_basement,				 //v
			key_jail, //on chair
			key_annexe, //need password
			broken_dish,				 //v
			tub_once,					 //v
			phillips, //screwdriver		 //v
			tub_fixed,					 //v
			flathead, //screwdriver on chair
			lighter,					 //v
			oil, //on chair
			handkerchief,				 //v
			detergent, //wash			 //v
			door_knob,					 //v
			door_no_knob, //use item
			tatami_l,					 //v
			tatami_r,					 //v
			gate,						 //v
			toilet,						 //v
			bed,						 //v
			bookcase_l,					 //v
			bookcase_r,					 //v
			white_bookcase,				 //v
			bookcase_map21,			     //v
			closet_shake,				 //v need event
			closet_takesi_0, // open	 //v
			closet_takesi_1, // shaking  //v
			closet_hirosi_R,			 //v
			closet_hirosi_L,			 //v
			closet_mika_out, // oni
			door_oni, // event
			door_open, // event
			door_die, //open -> die
			door_half, //first in
		};
		enum TriggerType {
			triggered, 
			close, 
			press, 
			pick, 
			fixed, 
			take, 
			onCorrectPos, 
			useItem
		};
		Item();
		~Item();
		void SetParam(int delay, int boxX, int boxY, 
			ItemName name);
		void Load(vector<string> filename, COLORREF color);
		int GetPosX();
		int GetPosY();
		int GetPosL();
		int GetPosU();
		int GetPosR();
		int GetPosD();
		int GetPosY_offset();
		void SetPlayerPos(int x, int y);
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
		void ResetUtil(); // all control reset
		bool IsPick();
		bool IsFixed();
		bool IsTake();
		bool IsAnimationDone();
		bool IsClose();
		int GetBitMapIndex();
		void EventTrigger();

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
		Direction _pressing;
		ItemName _name;
		// pick -> disappear, take -> a bitmap is leaved
		// index for bool (under)
		// _triggered, _close, _press, _pick, 
		// _fixed, _take, _onCorrectPos, _useItem
		vector<bool> utiltriggers = {
		false, true, false, false, false, 
		false, false, false};
	};
}