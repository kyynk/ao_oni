#pragma once
namespace game_framework {
	class Item : public Entity {
	public:
		enum ItemName {
			lib_book,
			key_lib,
			key_3F_L,
			key_2F_TL, //need password
			key_basement,
			key_jail, //on chair
			key_annexe, //need password
			broken_dish,
			tub_once,
			phillips, //screwdriver
			tub_fixed,
			flathead, //screwdriver on chair
			lighter,
			oil, //on chair
			handkerchief,
			detergent, //wash
			door_knob,
			door_no_knob, //use item
			tatami_l,
			tatami_r,
			gate,
			toilet,
			bed,
			bookcase_l,
			bookcase_r,
			white_bookcase,
			bookcase_map21,
			closet_shake, // need event
			closet_takesi_0, // open
			closet_takesi_1, // shaking
			closet_hirosi_R,
			closet_hirosi_L,
			closet_mika_out, // oni
			door_oni, // event
			door_open, // event
			door_die, //open -> die
			door_half, //first in
			password_not_open,
			password_get_key,
			piano_blood,
			piano_pwd,
			piano_hint,
			diff_door,
			kabe_pwd,
			candle,
			mika_to_oni,
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
		void StorePlayerPos(int x, int y);
		void OnMove();
		void OnKeyDown(UINT nChar);
		void OnKeyUp(UINT nChar);
		void OnShow();
		bool Collide();
		void SetDirection(int d);
		void Animation(int n, int frame);
		string GetName();
		// e.g. 
		// if player on chair a = true, else a = false
		void ResetUtil(); // all control reset

		void SetIsPick(bool p){	utiltriggers[pick] = p;}
		void SetClose(bool c) { utiltriggers[close] = c; }
		void SetTrigger(){ utiltriggers[triggered] = true; }
		bool IsClose(){ return utiltriggers[close]; }

		bool IsPick(){return utiltriggers[pick]; }
		bool IsFixed(){return utiltriggers[fixed]; }
		bool IsTake(){return utiltriggers[take];}
		void SetOnCorPos(bool ontri) {
			utiltriggers[onCorrectPos] = ontri;
		}
		bool IsOnCorPos(){return utiltriggers[onCorrectPos];}

		bool IsAnimationDone();
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
		vector<bool> utiltriggers = {
		false, true, false, false, 
		false, false, false, false};
	};
}