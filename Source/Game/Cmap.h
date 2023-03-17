#pragma once
namespace game_framework {
	class Cmap {
	public:
		Cmap();
		void Init(int w,int h,int px,int py);
		void ShowMap(int layer, vector<CMovingBitmap> a);
		void Load(string files, COLORREF color);
	private:
		int _width, _height;
		int _pos_x, _pos_y;
		map <string,int> _resource_list;
		vector<vector<vector<int>>> _map;

	};

}