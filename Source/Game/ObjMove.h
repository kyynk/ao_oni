#pragma once
#pragma once
namespace game_framework {
	class ObjMove : public Entity {
	public:
		enum ObjType {
			red_chair,
			black_chair,
			flower
		};
		enum move {
			none,
			isup,
			isdown,
			isleft,
			isright
		};
		ObjMove();
		~ObjMove();
		// step and moveTime sould be fast with player
		// e.g. moveTime = 4, step = 8
		void SetParam(ObjType tp, int step, int moveTime, 
			int offsetX, int offsetY);
		void Load(vector<string> filename, COLORREF color);
		void SetPos(int x, int y);
		int GetPosX();
		int GetPosY();
		int GetPosL();
		int GetPosU();
		int GetPosR();
		int GetPosD();
		void GetPlayerPos(int playerX, int playerY);
		void Track(GameMap &map);
		void OnMove(GameMap &map);
		// every time obj move, will track first
		void OnShow();
		void Reset();
		void Fixed();
	private:
		int _humanX, _humanY,
			_step, _moveTime, _overTime,
			_offsetX, _offsetY, 
			_resetX, _resetY, 
			_fixedX, _fixedY;
		bool _isFixedPos;
		ObjType _type;
		move _nowmove;
		move _tracking;
	};
}