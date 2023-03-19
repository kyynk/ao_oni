#pragma once
namespace game_framework {
	class vector3d {
	private:
		vector<int> _vector;
		int _DimX, _DimY, _DimZ;

	public:
		vector3d() {
			_DimX = 0;
			_DimY = 0;
			_DimZ = 0;
		}
		~vector3d() {}
		void SetDimension(int x, int y, int z);
		int GetValue(int x, int y, int z);
		void AssignValue(int x, int y, int z, int value);
		int V3dSize();

	};

}