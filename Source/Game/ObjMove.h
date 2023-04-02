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
		// step and moveTime sould be same with player
		void SetParam(ObjType tp, int step, int moveTime);
		void SetPos(int x, int y);
		int GetPosX();
		int GetPosY();
		int GetPosL();
		int GetPosU();
		int GetPosR();
		int GetPosD();
		void Load(string filename, COLORREF color);
		void GetPlayerPos(int playerX, int playerY);
		void Track(GameMap &map);
		void OnMove(GameMap &map);	// every time chair move, will track first
		void OnShow();
		void Reset();
		void Fixed();
	private:
		int _humanX, _humanY,
			_step, _moveTime, _overTime,
			_offsetX, _offsetY;
		bool _isFixedPos;
		objType _type;
		move _nowmove;
		move _tracking;
	};
}