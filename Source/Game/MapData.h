#pragma once
namespace game_framework {
	class MapData {
	private:
		vector<int> _vector;
		int _DimX, _DimY, _DimZ;

	public:
		MapData() {
			_DimX = 0;
			_DimY = 0;
			_DimZ = 0;
		}
		MapData(int x, int y, int z) {
			SetDimension(x, y, z);
		}
		MapData(int x, int y) {
			SetDimension(x,y);
		}
		~MapData() {}
		void SetDimension(int x, int y, int z);
		int GetValue(int x, int y, int z);
		void AssignValue(int x, int y, int z, int value);
		void SetDimension(int x, int y);
		int GetValue(int x, int y);
		void AssignValue(int x, int y, int value);
		int Msize();

	};

}