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
#include "ChoiceMenu.h"
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

	}

	void CGameStateRun::OnBeginState()
	{
		mousex_foc = 0;
		mousey_foc = 0;
		isedit = false;
		selmap = 0;
		isgrid = false;
		iswrite = false;
		ttt = true;
	}

	void CGameStateRun::OnMove()							// 移動遊戲元素
	{
		player.OnMove();
		player.init(4);

	}

	void CGameStateRun::OnInit()  								// 遊戲的初值及圖形設定
	{

		ShowInitProgress(33, "loading game mid");
		vector<string> playervec;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 3; j++) {
				playervec.push_back("img/hiroshi_move/Hiroshi_" + to_string(i) + to_string(j) + ".bmp");
			}
		}
		player.Load(playervec, RGB(204, 255, 0));
		std::ifstream mapres_in("map_bmp/mapsize.txt");
		string name;
		int count;
		for (int i = 0; i < 12; i++) {
			mapres_in >> name >> count;
			ShowInitProgress(33 + i, name);
			MapRes::GetInstance()->Load(name, count);
		}

		for (int i = 0; i < 46; i++) {
			GameMap tmp;
			tmp.Load("map" + to_string(i) + ".txt");
			int w = tmp.GetWidth();
			int h = tmp.GetHeight();
			w = w + ((w % 2 == 0) ? 1 : 0);
			h = h + ((h % 2 == 0) ? 1 : 0);
			//TRACE("w:%d, h:%d i:%d\n", w, h,i);
			tmp.SetTopLeftMap((SIZE_X-16-w*TILE)/2 , (SIZE_Y-20-h* TILE)/2);
			tmp.istileindex = false;
			gamemaps.push_back(tmp);
		}
		story.SetNow(Dialog::character::none);
		story.SetParam({"We heard rumors about the mansion", 
			"they say on the outskirts of town...", 
			"there is a monster living here...!"}, false);
		story.Show();
		talk.SetNow(Dialog::character::mika);
		talk.SetParam({ "Hi", "how", "r u?" }, false);
		useItem.SetNow(Dialog::character::hirosi);
		useItem.SetParam({ "Do u want to use that?" }, true);

		testitem.Load({ "img/item/blueeye.bmp","img/item/book.bmp","img/item/oil.bmp" }, RGB(204, 255, 0));
		testitem.init(true, false, Item::itemtype::once, 1000);
		grid.LoadBitmapByString({ "img/grid.bmp" }, RGB(0, 0, 0));
		seltile.LoadBitmapByString({ "img/placeholder.bmp" });
	}

	void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		if (nChar == 0x49) { // i
			gamemaps[selmap].istileindex = !gamemaps[selmap].istileindex;
		}
		if (nChar == 0x4A) { //j
			if(selmap>0)
			selmap--;
		}
		if (nChar == 0x4B) {// k
			if(selmap<45)
			selmap++;
		}
		if (nChar == 0x57) { //w
			iswrite = true;
		}
		if (nChar == 0x47) { //g
			isgrid = !isgrid;
		}

		if (nChar == 0x45) { //e
			isedit = !isedit;
		}
		if (nChar == VK_RETURN) {
			testitem.SetTriggered(true);
		}
		if (nChar == 0x55) { // press "U" show dialog -> if finish item control will optimize
			talk.Show();
		}
		if (nChar == 0x50) { // press "P" show dialog -> if finish item control will optimize
			useItem.Show();
		}
		if (nChar == VK_SPACE) { // press "space" colse conment
			if (!story.isClose())
				story.Close();
			talk.Close();
		}
		if (talk.isClose() && useItem.isClose()) { // if in conversation, then player cannot moving
			player.OnKeyDown(nChar);
		}
		if (!useItem.isClose())
			useItem.GetSelect(nChar);
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
			seltile.SetTopLeft(mousex_foc*TILE,mousey_foc*TILE);
			pointtmp.push_back({ selmap,mousex_foc*TILE, mousey_foc*TILE });
			TRACE("push {%d, %d ,%d }\n", selmap, mousex_foc*TILE, mousey_foc*TILE);
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
		if (isedit) {
			if (pointtmp.empty()) {
				TRACE("nothing to be popped");
			}
			else {
				TRACE("element {%d,%d,%d} popped\n", pointtmp.at(pointtmp.size() - 1)[0], pointtmp.at(pointtmp.size() - 1)[1] , pointtmp.at(pointtmp.size() - 1)[2] );
				
				pointtmp.pop_back();
			}
		}

}

	void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
	{

	}


	void CGameStateRun::OnShow()
	{
		if (!story.isClose()) {
			story.ShowTotal();
		}
		if (story.isClose()) {

			

			gamemaps[selmap].ShowMap();
			if (isedit && !ofs.is_open()) {
				ofs.open("maplink.txt", std::ios::app);
				if (!ofs.is_open()) {
					TRACE("Failed to open file.\n");
					throw std::invalid_argument("open failed");
				}
				TRACE("openmaplink\n");

			}
			if (ofs.is_open() && !isedit) {
				int i = 0;
				for (auto f : pointtmp) {
					if (i % 2 == 0) {
						ofs << f.at(0) << " " << f.at(1) << " " << f.at(2) << " ";
					}
					else {
						ofs << f.at(0) << " " << f.at(1) << " " << f.at(2) << "\n";

					}
					i++;
				}
				ofs.close();
				TRACE("close\n");
			}
			player.OnShow();
			testitem.OnShow();
			if (!talk.isClose()) {
				talk.ShowTotal();
			}
			if (!talk.isClose()) {
				talk.ShowTotal();
			}
			if (!useItem.isClose()) {
				useItem.ShowTotal();
			}
			if (isgrid)grid.ShowBitmap();
			if (isedit)seltile.ShowBitmap();
			// show text, will be placed inside a function in the future
			CDC *pDC = CDDraw::GetBackCDC();
			CTextDraw::ChangeFontLog(pDC, 20, "Noto Sans TC", RGB(255, 255, 255));
			CTextDraw::Print(pDC, 0, 0, to_string(mousex) + "  " + to_string(mousey) + " edit mode: " + ((isedit) ? "true" : "false") + to_string(selmap));
			CDDraw::ReleaseBackCDC();
		}
	}

};