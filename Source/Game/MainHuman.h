#pragma once
namespace game_framework{
	class GameMap;
	class MapRouter;
	class MainHuman : public Entity {
	public:
		enum Direction {
			up,
			down, 
			left,
			right
		};
		enum bstate {
			s1,
			s2,
			s3,
			s4
		};
		
		enum move {
			none,
			isup,
			isdown,
			isleft,
			isright,
			machinetransmap
		};
		MainHuman();

		void SetXYAndCol(int x, int y);

		void init(int step,int offset,Direction dir);

		void ResetToGrid();
		
		void OnMove(GameMap &map, MapRouter &router, int nowID, vector<vector<int>>&, vector<vector<int>>&, vector<vector<int>>&) ;
		void OnMove();
		void OnMoveBySettings(int block);
		void OnKeyDown(UINT nChar);
		void OnKeyUp(UINT nChar);
		void OnShow();
		void Load(vector<string> filenames,COLORREF color );

		void SetDirection(Direction d) {
			_direction = d;
		}
		int GetDirection() {
			return _direction;
		}
		
		int GetOffset() { return _coroffset; }
		int GetU() { return _uy + _coroffset; }
		int GetD() { return _dy + _coroffset; }
		int GetL() { return _lx; }
		int GetR() { return _rx; }
		int GetX() { return _pos_x; };
		int GetY() { return _pos_y+_coroffset; }
		int NextX() { return _nextmapx; }
		int NextY() { return _nextmapy; }
		int NextMapID() { return _nextMapID; }
		bool &IsMapChanged() { return _isMapChanged; }
		bool IsSwitchMap() { return _switchMapCheck; }
		void SetIsMachine(bool isMachine, move pressing);
		void SetNextMap(int x, int y, int mapID);
		void SetAllMoveFalse();
		void SetNowmove(move m);
		
		int machine_count;
	private:
		int _coroffset;
		bool _walkiter;
		Direction _direction;
		bstate _bstate;
		//move _premove;
		move _nowmove;
		move _pressing;
		bool _isMachine;
		int _step;
		bool _isup;
		bool _isdown;
		bool _isleft;
		bool _isright;
		int _nextmapx;
		int _nextmapy;
		int _nextMapID;
		bool _isMapChanged;
		bool _switchMapCheck;

	};
}