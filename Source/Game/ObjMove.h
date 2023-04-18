#pragma once
namespace game_framework {
	class ObjMove : public Entity {
	public:
		enum ObjType {
			no,
			house1_2F_TL_chair,
			house1_2F_TR_chair,
			house1_basement2_chair,
			house1_basement_chair2,
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
			int offsetX, int offsetY, int resetX, int resetY);
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
		// if change map, obj should be disappear
		// and obj will check if fixed or need to teleport or reset
		void ChangeMap();
		bool isChangeMap();
		int GetPreX() { return _prex; }
		int GetPreY()  {return _prey;}
	private:
		int _humanX, _humanY,
			_step, _moveTime, _overTime,
			_offsetX, _offsetY, 
			_resetX, _resetY;
		int _prex, _prey;
		bool _isFixedPos;
		bool _press;
		bool _collide;
		bool _isCrossMap;
		bool _1stCross;
		bool _notShow;
		ObjType _type;
		move _nowmove;
		move _pressing;
	};
}