#pragma once
#pragma once
namespace game_framework {
	class ObjMove : public Entity {
	public:
		enum ObjType {
			no,
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
			int offsetX, int offsetY, int resetX, int resetY,
			int fixedX, int fixedY);
		void Load(string filename, COLORREF color);
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
		void OnKeyDown(UINT nChar);
		void OnKeyUp(UINT nChar);
		void OnShow();
		void Reset();
		void Fixed();
		bool isCollide();
		bool IsFixed();
		
	private:
		int _humanX, _humanY,
			_step, _moveTime, _overTime,
			_offsetX, _offsetY, 
			_resetX, _resetY, 
			_fixedX, _fixedY;
		bool _isFixedPos;
		bool _press;
		bool _collide;
		ObjType _type;
		move _nowmove;
		move _pressing;
	};
}