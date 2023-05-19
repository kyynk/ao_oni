#pragma once
namespace game_framework {
	class ObjMove : public Entity {
	public:

		enum ObjName {
			no,
			house1_2F_TL_chair,
			house1_2F_TR_chair,
			house1_basement2_chair,
			house1_basement_chair2,
			flower
		};
		ObjMove();
		~ObjMove();
		// step and moveTime sould be fast with player
		// e.g. moveTime = 4, step = 8
		void SetParam(ObjName tp, int step, int moveTime, 
			int offsetX, int offsetY, int resetX, int resetY);
		void Load(string filename, COLORREF color);
		int GetPosX();
		int GetPosY();
		int GetPosL();
		int GetPosU();
		int GetPosR();
		int GetPosD();
		int GetPosY_offset();
		void StorePlayerPos(int playerX, int playerY);
		void Track(GameMap &map);
		void OnMove(GameMap &map);
		// every time obj move, will track first
		void OnKeyDown(UINT nChar);
		void OnKeyUp(UINT nChar);
		void OnShow();
		void OnShowConditional();
		void Reset();
		void Fixed();
		bool isCollide();
		bool IsFixed();
		// if change map, obj should be disappear
		// and obj will check if fixed or need to teleport or reset
		void ChangeMap();
		bool isChangeMap();
		void SetPrePos();
		void SetPreX(int x);
		void SetPreY(int y);
		int GetPreX();
		int GetPreY();
	private:
		int _playerX, _playerY,
			_step, _moveTime, 
			_offsetX, _offsetY, 
			_resetX, _resetY,
			_preX, _preY;
		bool _isFixedPos;
		bool _press;
		bool _isCrossMap;
		bool _1stCross;
		bool _notShow;
		ObjName _type;
		Direction _nowmove;
		Direction _pressing;
	};
}