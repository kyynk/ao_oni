#pragma once
namespace game_framework {
	class Cmap {
	public:
		Cmap();
		void Init(int w,int h,int px,int py);
		void ShowMap(int layer);
		void Load(vector<string> files, COLORREF color);
	private:
		int _width, _height;
		int _pos_x, _pos_y;
		vector<vector<vector<CMovingBitmap>>> _cmap;

	};

}