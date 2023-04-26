#pragma once
namespace game_framework{
	class GameMap;
	class MapRouter;
	class MainHuman : public Entity {
	public:
		MainHuman();
		void SetXYAndCol(int x, int y);
		void init(int step,int offset,Direction dir);
		void ResetToGrid();
		void SwitchMap(GameMap& map);
		void OnMove(GameMap &map, MapRouter &router, int nowID, const vector<vector<int>>&, const vector<vector<int>>&, const vector<vector<int>>&) ;
		void RouterCheckChangeMap(GameMap& map, MapRouter& router, int nowID);
		void OnMove();
		void OnMoveBySettings(int block);
		void OnKeyDown(UINT nChar);
		void OnKeyUp(UINT nChar);
		void OnShow();
		void Load(vector<string> filenames,COLORREF color );
		void SetDirection(Direction d) {
			_direction = d;
		}
		const int GetDirection() {
			return _direction;
		}
		
		const int GetOffset() { return _coroffset; }
		const int GetU() { return _uy + _coroffset; }
		const int GetD() { return _dy + _coroffset; }
		const int GetL() { return _lx; }
		const int GetR() { return _rx; }
		const int GetX() { return _pos_x; };
		const int GetY() { return _pos_y+_coroffset; }
		const int NextX() { return _nextmapx; }
		const int NextY() { return _nextmapy; }
		const int NextMapID() { return _nextMapID; }
		bool &IsMapChanged() { return _isMapChanged; }
		const bool IsSwitchMap() { return _switchMapCheck; }

		void SetMachine(Direction pressing);
		void SetNextMap(int x, int y, int mapID);
		void SetAllMoveFalse();
		void SetNowmove(Direction m);
		
	private:
		enum {
			HUMAN_DOWN =  0,
			HUMAN_DOWN_1 = 1,
			HUMAN_DOWN_2 = 2,
			HUMAN_UP = 9,
			HUMAN_UP_1 = 10,
			HUMAN_UP_2 = 11,
			HUMAN_LEFT = 3,
			HUMAN_LEFT_1 = 4,
			HUMAN_LEFT_2 = 5,
			HUMAN_RIGHT = 6,
			HUMAN_RIGHT_1 = 7,
			HUMAN_RIGHT_2 = 8 
		};
		int machine_count;
		int _coroffset;
		bool _walkiter;
		bstate _bstate;
		Direction _direction;
		Direction _nowmove;
		Direction _pressing;
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