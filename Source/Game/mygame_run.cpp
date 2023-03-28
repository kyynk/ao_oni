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
	}

	void CGameStateRun::OnMove()							// 移動遊戲元素
	{
		inputbox.OnMove();
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
		for (int i = 0; i < 54; i++) {
			GameMap tmp;
			tmp.Load("map_bmp/map" + to_string(i) + ".txt");
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

		//testitem.Load({ "img/item/blueeye.bmp","img/item/book.bmp","img/item/oil.bmp" }, RGB(204, 255, 0));
		//testitem.init(true, false, Item::itemtype::once, 1000);
		grid.LoadBitmapByString({ "img/grid.bmp" }, RGB(0, 0, 0));
		seltile.LoadBitmapByString({ "img/placeholder.bmp" });
		inputbox.Load("img/cursor/input_box.bmp");
		inputbox.init(20 * TILE, 0, 0, 10);
	}

	void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		if (inputbox.IsWrite()) {
			inputbox.BoxOn(nChar);
		}
		else{
			if (nChar == KEY_I) {
				gamemaps[selmap].istileindex = !gamemaps[selmap].istileindex;
			}
			if (nChar == KEY_J) {
				if (selmap > 0)
					selmap--;
			}
			if (nChar == KEY_K) {
				if (selmap < 53)
					selmap++;
			}

			if (nChar == KEY_G) {
				isgrid = !isgrid;
			}

			if (nChar == KEY_E) {
				if (pointtmp.size() % 2 == 1) {
					TRACE("still one point in buffer, pop out or add a new point.\n");
				}
				else {
					isedit = !isedit;
				}
			}
			if (nChar == VK_ENTER) {
				if (inputbox.isInteger()) {
					int index = stoi(string(inputbox.GetString()));
					if (index >= 0 && index <= 53) {
						selmap = index;

					}
				}
			}
			if (nChar == KEY_W) {
				inputbox.TimerStart();
				inputbox.ToggleBox();
			}
			if (nChar == KEY_Q) {
				GotoGameState(GAME_STATE_OVER);
			}
			/*if (nChar == VK_RETURN) {
				testitem.SetTriggered(true);
			}*/
			if (nChar == KEY_U) { // press "U" show dialog -> if finish item control will optimize
				talk.Show();
			}
			if (nChar == KEY_P) { // press "P" show dialog -> if finish item control will optimize
				useItem.Show();
			}
			if (nChar == VK_SPACE) { // press "space" close dialog
				if (!story.isClose())
					story.Close();
				talk.Close();
			}
			if (talk.isClose() && useItem.isClose()) { // if dialog is on, player can't move
				player.OnKeyDown(nChar);
			}
			if (!useItem.isClose()) {
				useItem.GetSelect(nChar);
			}
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

			
			inputbox.Show();
			gamemaps[selmap].ShowMap();
			if (isedit && !ofs.is_open()) {
				ofs.open("maplink.txt", std::ios::app);
				if (!ofs.is_open()) {
					TRACE("Failed to open file.\n");
					throw std::invalid_argument("open failed");
				}
				TRACE("open maplink.txt\n");

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
			//testitem.OnShow();
			if (!talk.isClose()) {
				talk.ShowTotal();
			}
			if (!talk.isClose()) {
				talk.ShowTotal();
			}
			if (!useItem.isClose()) {
				useItem.ShowTotal();
			}
			if (isgrid) {
				grid.ShowBitmap();
			}
			if (isedit) {
				seltile.ShowBitmap();
				
			}
			// show text, will be placed inside a function in the future
			CDC *pDC = CDDraw::GetBackCDC();
			CTextDraw::ChangeFontLog(pDC, 20, "Noto Sans TC", RGB(255, 255, 255));
			CTextDraw::Print(pDC, 0, 0, "map index:"+to_string(selmap)+ "  " + to_string(mousex) + "  " + to_string(mousey) + " edit mode: " + ((isedit) ? "true" : "false"));
			
			CDDraw::ReleaseBackCDC();
		}
	}

};