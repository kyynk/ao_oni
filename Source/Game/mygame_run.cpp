#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include <bitset>
#include <fstream>
#include <ostream>
#include "Dialog.h"
#include "GameMap.h"
#include "mygame.h"

namespace game_framework {

	/////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
	/////////////////////////////////////////////////////////////////////////////

	CGameStateRun::CGameStateRun(CGame *g) : CGameState(g)
	{
	}

	CGameStateRun::~CGameStateRun()
	{
		MapRes::GetInstance()->Cleanup();
		ofs.close();

	}

	void CGameStateRun::OnBeginState()
	{
		mousex_foc = 0;
		mousey_foc = 0;
		isedit = false;

	}

	void CGameStateRun::OnMove()							// 移動遊戲元素
	{
		//gamemaps["house1_lobby"].SetTopLeftMap(TILE*4, TILE*4);
		player.OnMove();

	}

	void CGameStateRun::OnInit()  								// 遊戲的初值及圖形設定
	{

		isgrid = false;
		ShowInitProgress(33, "loading game mid");
		vector<string> tmp;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 3; j++) {
				tmp.push_back("img/hiroshi_move/Hiroshi_" + to_string(i) + to_string(j) + ".bmp");
			}
		}
		player.Load(tmp, RGB(204, 255, 0));
		player.init(4);
		std::ifstream mapres_in("map_bmp/mapsize.txt");
		string name;
		int count;
		for (int i = 0; i < 12; i++) {
			mapres_in >> name >> count;
			ShowInitProgress(33 + i, name);
			MapRes::GetInstance()->Load(name, count);
		}

		for (int i = 0; i < 23; i++) {
			GameMap tmp;
			tmp.Load("map" + to_string(i) + ".txt");
			int w = tmp.GetWidth();
			int h = tmp.GetHeight();
			w = w + (w % 2 == 0) ? 0 : 1;
			h = h + (w % 2 == 0) ? 0 : 1;
			tmp.SetTopLeftMap(SIZE_X - w*TILE/2 , SIZE_Y - h*TILE/2);
			gamemaps.push_back(tmp);
		}
		
		talk.SetNow(Dialog::character::takurou);
		talk.SetParam(180, 640, 25, { "Hi", "how", "are u" });

		t2.Load({ "img/item/blueeye.bmp","img/item/book.bmp","img/item/oil.bmp" }, RGB(204, 255, 0));
		t2.init(true, false, Item::itemtype::once, 1000);
		grid.LoadBitmapByString({ "img/aa.bmp" }, RGB(0, 0, 0));
	}

	void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		if (nChar == 0x47) {
			isgrid = !isgrid;
		}

		if (nChar == 0x45) {
			isedit = !isedit;
		}
		if (nChar == VK_RETURN) {
			t2.SetTriggered(true);
		}
		if (nChar == 0x55) { // press "U" show conment -> if finish item control will optimize
			talk.Show();
		}
		if (nChar == VK_SPACE) { // press "space" colse conment
			talk.Close();
		}
		if (talk.isClose()) { // if in conversation, then player cannot moving
			player.OnKeyDown(nChar);
		}
	}

	void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
	{

		player.OnKeyUp(nChar);
	}

	void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
	{
		if (isedit) {
			mousex_foc = mousex;
			mousey_foc = mousey;
		}
	}

	void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
	{
	}

	void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point)	// 處理滑鼠的動作
	{
		mousex = point.x / 32;
		mousey = point.y / 32;
	}

	void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
	{
	}

	void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
	{

		t2.Load({ "img/item/blueeye.bmp","img/item/book.bmp","img/item/oil.bmp" }, RGB(204, 255, 0));
		t2.init(true, false, Item::itemtype::once, 1000);
		grid.LoadBitmapByString({ "img/aa.bmp" }, RGB(0, 0, 0));

		//sTest.LoadBitmapByString({ "img/shadow/shadow1.bmp" }, RGB(0, 0, 0));
	}


	void CGameStateRun::OnShow()
	{

		//gamemaps["house1_lobby"].ShowMap();

		if (isedit && !ofs.is_open()) {
			ofs.open("mappos.txt");
			if (!ofs.is_open()) {
				TRACE("Failed to open file.\n");
				throw std::invalid_argument("open failed");
			}
			TRACE("write\n");
		}

		if (ofs.is_open() && !isedit) {
			//ofs << gamemaps["house1_lobby"].GetName() << " " << gamemaps["house1_lobby"].GetXY().x / 32 << " " << gamemaps["house1_lobby"].GetXY().y / 32 << "\n";
			ofs.close();
			TRACE("close\n");
		}
		player.OnShow();
		t2.OnShow();
		if (!talk.isClose()) {
			talk.ShowTotal();
		}
		if (isgrid)grid.ShowBitmap();
		// show text, will be placed inside a function in the future
		CDC *pDC = CDDraw::GetBackCDC();
		//CFont *fp;
		CTextDraw::ChangeFontLog(pDC, 20, "Noto Sans TC", RGB(255, 255, 255));
		CTextDraw::Print(pDC, 0, 0, to_string(mousex) + "  " + to_string(mousey) + " edit mode: " + ((isedit) ? "true" : "false"));
		CDDraw::ReleaseBackCDC();
	}

};