#pragma once
#include "ChoiceMenu.h"
#include "Dialog.h"
namespace game_framework {
	class Interface {
	public:
		enum Status {
			none,
			status, 
			item, 
			save, 
			end,
			use_item
		};
		enum Items {
			key_lib,
			key_3F_L,
			key_2F_TL,
			key_basement,
			key_jail,
			key_annexe,
			broken_dish,
			phillips,
			flathead,
			lighter,
			oil,
			handkerchief,
			detergent,
			door_knob
		};
		Interface();
		~Interface();
		void init();
		// game time counter
		bool IsPause() const; // check pause
		bool IsStop() const;
		void StartCount();
		void PauseCount();
		void StopCount();
		time_t GetStartTime() const;
		string GetGameTime();
		// game time counter
		string GetRealTime();
		void StorePlayerStep(int step);
		void StoreItem(string intro, string name, Items item);
		InterfaceData UseItem();
		void ChangeItemStatus(string originalName, string intro, string name, int frame_index);
		void DeleteItem(string name);
		bool FindItem(string name);
		void ShowCursorStatus();
		void ShowCursorItem();
		void ShowCursorSave();
		void ShowCursorEnd();
		void ShowTextStatus(CDC* pDC);
		void ShowTextItem(CDC* pDC);
		void ShowTextSave(CDC* pDC);
		void ShowTextEnd(CDC* pDC);
		void ShowItemBitmap();
		void ShowTotal();
		void OnKeyDown(UINT nChar);
		void ResetChoose();
		void SetShow(bool show);
		bool IsShow() const;
		bool IsTitle() const;
		bool IsEnd() const;
		bool IsChangeStatus();

	private:
		int _cursorX, _cursorY,
			_boxX, _boxY, _txtX, _txtY,
			_lineSpacing, _step, 
			_statusChoose, _itemChoose, 
			_saveChoose, _endChoose, 
			_useItemIndex;
		vector<string> _itemsName;
		vector<string> _itemsIntro;
		time_t start_time, pause_time;
		Status _show;
		bool _isPause;
		bool _isStop;
		bool _isShow;
		bool _IsGoTitle;
		bool _IsEndGame;
		bool _IsUseItem;
		vector<int> _itemsImgShowIndex;
		vector<CMovingBitmap> _itemsImg;
		CMovingBitmap _cursor;
		CMovingBitmap _status;
		CMovingBitmap _item;
		CMovingBitmap _save;
		CMovingBitmap _end;
		Dialog _useItemDialog;
	};
}