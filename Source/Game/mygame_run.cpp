#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include <bitset>
#include <fstream>
#include <filesystem>
#include <olebind.h>
#include "ChoiceMenu.h"
#include "Dialog.h"
#include "MapNode.h"
#include "GameMap.h"
#include "MapRouter.h"

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
	void CGameStateRun::OnInit()  								// 遊戲的初值及圖形設定
	{
		ShowInitProgress(33, "loading game mid");
		// main character
		vector<string> playervec;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 3; j++) {
				playervec.push_back("img/hiroshi_move/Hiroshi_" + to_string(i) + to_string(j) + ".bmp");
			}
		}
		// player map x, player map y, map ID
		specialblockL.push_back({ 8,5,13 });
		specialblockL.push_back({ 5,4,7 });
		specialblockL.push_back({ 8,11,7 });
		specialblockL.push_back({ 6,10,33 });
		specialblockL.push_back({ 2,3,34 });
		specialblockL.push_back({ 10,4,42 });
		specialblockL.push_back({ 10,6,23 });


		specialblockR.push_back({ 4,4,7 });
		specialblockR.push_back({ 7,11,7 });
		specialblockR.push_back({ 7,5,13 });
		specialblockR.push_back({ 5,10,33 });
		specialblockR.push_back({ 1,3 ,34});
		specialblockR.push_back({ 9,4,42 });
		specialblockR.push_back({ 9,6,23 });

		specialblockTN.push_back({ 4,5,7 });
		specialblockTN.push_back({ 6,9,33 });
		specialblockTN.push_back({ 2,4,34 });
		specialblockTN.push_back({ 9,5,42 });
		specialblockTN.push_back({ 9,5,23 });

		//specialblockTN.push_back({3,9,16});

		player.Load(playervec, RGB(204, 255, 0));
		// map resources
		std::ifstream mapres_in("map_bmp/mapsize.txt");
		string name;
		int count;
		for (int i = 0; i < 12; i++) {
			mapres_in >> name >> count;
			ShowInitProgress(33 + i, name);
			MapRes::GetInstance()->Load(name, count);
		}
		// map data
		for (int i = 0; i < 54; i++) {
			GameMap tmp;
			tmp.Load("map_bmp/map" + to_string(i) + ".txt");
			int w = tmp.GetWidth();
			int h = tmp.GetHeight();
			w = w + ((w % 2 == 0) ? 1 : 0);
			h = h + ((h % 2 == 0) ? 1 : 0);
			tmp.SetTopLeftMap((SIZE_X - 16 - w * TILE) / 2, (SIZE_Y - 20 - h * TILE) / 2);
			gamemaps.push_back(tmp);
		}
		ifstream indexlayer("map_bmp/mapindexlayer.txt");
		int i2;
		for (int i = 0; i < 46; i++) {
			indexlayer >> i2;
			mapoverlayindex.push_back(i2);
		}
		// dialog
		story.SetNow(Dialog::character::none);
		story.SetParam({ "We heard rumors about the mansion",
			"they say on the outskirts of town...",
			"there is a monster living here...!" }, false);
		story.Show();
		talk.SetNow(Dialog::character::mika);
		talk.SetParam({ "Hi", "how", "r u?" }, false);
		useItem.SetNow(Dialog::character::hirosi);
		useItem.SetParam({ "Do u want to use that?" }, true);
		// item
		items.resize(30);
		items.at(TOILET).SetParam(-1, 0, 0, Item::itemName::toilet);
		items.at(TUB_ONCE).SetParam(100, 0, TILE, Item::itemName::tub_once);
		items.at(PHILLIPS).SetParam(100, 0, TILE, Item::itemName::phillips);
		items.at(TUB_FIXED).SetParam(-1, 0, TILE, Item::itemName::tub_fixed);
		items.at(BROKEN_DISH).SetParam(-1, 0, 0, Item::itemName::broken_dish);
		items.at(LIB_BOOK).SetParam(-1, 0, 0, Item::itemName::lib_book);
		items.at(KEY_3F_L).SetParam(100, 0, 0, Item::itemName::key_3F_L);
		items.at(KEY_LIB).SetParam(100, 0, 0, Item::itemName::key_lib);
		items.at(DOOR_KNOB).SetParam(100, 0, TILE, Item::itemName::door_knob);
		items.at(DOOR_NO_KNOB).SetParam(100, 0, TILE, Item::itemName::door_no_knob);
	/*	toilet
		tub_once
		phillips
		tub_fixed
		broken_dish
		lib_book
		key_3F_L
		key_lib
		door_knob
		door_no_knob*/

		// objMove
		redChair.SetParam(ObjMove::ObjType::red_chair,
			8, 4, 0, 0, 15 * TILE, 9 * TILE,
			16 * TILE, 9 * TILE);
		// debug
		/*items.push_back(move(toilet));
		items.push_back(move(tub_once));
		items.push_back(move(phillips));
		items.push_back(move(tub_fixed));
		items.push_back(move(broken_dish));
		items.push_back(move(lib_book));
		items.push_back(move(key_3F_L));
		items.push_back(move(key_lib));
		items.push_back(move(door_knob));
		items.push_back(move(door_no_knob));*/

		grid.LoadBitmapByString({ "img/grid.bmp" }, RGB(0, 0, 0));
		tileplaceholder.LoadBitmapByString({ "img/placeholder.bmp" });
		inputbox.Load("img/cursor/input_box.bmp");
		inputbox.init(20 * TILE, 0, 0, 10);
		// map link data
		router.init();
		router.Load("map_bmp/maplink.txt");
		router.debug();


	}
	void CGameStateRun::OnBeginState()
	{
		oni1.SetXY(10 * TILE, 11 * TILE + 80);
		mousex_foc = 0;
		mousey_foc = 0;
		//isbs = 0;
		istwoway = 0;
		isteleportblock = false;
		isedit = false;
		isgrid = false;
		_nowID = 13;
		player.init(4, 16, Human::down);
		oni1.SetParam(Oni::OniType::normal, 4, 8);
		redChair.Reset();
		items.at(TOILET).SetXY(12 * TILE, 15 * TILE);
		items.at(TUB_ONCE).SetXY(9 * TILE, 12 * TILE);
		items.at(PHILLIPS).SetXY(9 * TILE, 12 * TILE);
		items.at(TUB_FIXED).SetXY(9 * TILE, 12 * TILE);
		items.at(BROKEN_DISH).SetXY(12 * TILE, 8 * TILE);
		items.at(LIB_BOOK).SetXY(7 * TILE, 14 * TILE);
		items.at(KEY_3F_L).SetXY(7 * TILE, 14 * TILE);
		items.at(KEY_LIB).SetXY(15 * TILE, 9 * TILE);
		items.at(DOOR_KNOB).SetXY(12 * TILE, 6 * TILE);
		items.at(DOOR_NO_KNOB).SetXY(12 * TILE, 6 * TILE);
		items.at(LIGHTER).SetXY(6 * TILE, 6 * TILE);
		items.at(TATAMI_L).SetXY(6 * TILE, 6 * TILE);
		items.at(TATAMI_R).SetXY(9 * TILE, 6 * TILE);
	}

	void CGameStateRun::OnMove()							// 移動遊戲元素
	{
		inputbox.OnMove();
		if (player.IsMapChanged() && player.IsSwitchMap()) {

			_nowID = player.NextMapID();
		}
		player.OnMove(gamemaps.at(_nowID), router, _nowID, specialblockL, specialblockR, specialblockTN);
		// test Item
		if (_nowID == 19) {
			items.at(TOILET).GetPlayerPos(player.GetX(), player.GetY());
			items.at(TOILET).OnMove();
		}
		else if (_nowID == 18) {
			if (!items.at(TUB_ONCE).IsFixed() || !items.at(TUB_ONCE).IsAnimationDone()) {
				items.at(TUB_ONCE).GetPlayerPos(player.GetX(), player.GetY());
				items.at(TUB_ONCE).OnMove();
			}
			else if (items.at(TUB_ONCE).IsFixed() && items.at(TUB_ONCE).IsAnimationDone()) {
				items.at(PHILLIPS).GetPlayerPos(player.GetX(), player.GetY());
				items.at(PHILLIPS).OnMove();
			}
			else if (items.at(PHILLIPS).IsPick()) {
				items.at(TUB_FIXED).GetPlayerPos(player.GetX(), player.GetY());
				items.at(TUB_FIXED).OnMove();
			}
		}
		else if (_nowID == 11) {
			items.at(BROKEN_DISH).GetPlayerPos(player.GetX(), player.GetY());
			items.at(BROKEN_DISH).OnMove();
		}
		else if (_nowID == 12) {
			items.at(LIB_BOOK).GetPlayerPos(player.GetX(), player.GetY());
			items.at(LIB_BOOK).OnMove();
			if (items.at(LIB_BOOK).IsFixed()) {
				items.at(KEY_3F_L).GetPlayerPos(player.GetX(), player.GetY());
				items.at(KEY_3F_L).OnMove();
			}
		}
		else if (_nowID == 10) {
			items.at(TATAMI_R).GetPlayerPos(player.GetX(), player.GetY());
			items.at(TATAMI_R).OnMove();
			/*lighter.GetPlayerPos(player.GetX1(), player.GetY1());
			lighter.OnMove();
			tatami_L.GetPlayerPos(player.GetX1(), player.GetY1());
			tatami_L.OnMove();*/
		}
		else if (_nowID == 14) {
			redChair.GetPlayerPos(player.GetX(), player.GetY());
			redChair.OnMove(gamemaps.at(_nowID));
			if (redChair.IsFixed()) {
				items.at(KEY_LIB).GetPlayerPos(player.GetX(), player.GetY());
				items.at(KEY_LIB).OnMove();
			}
		}
		else if (_nowID == 15) {
			if (!items.at(DOOR_KNOB).IsPick()) {
				items.at(DOOR_KNOB).GetPlayerPos(player.GetX(), player.GetY());
				items.at(DOOR_KNOB).OnMove();
			}
			if (items.at(DOOR_KNOB).IsPick()) {
				items.at(DOOR_NO_KNOB).GetPlayerPos(player.GetX(), player.GetY());
				items.at(DOOR_NO_KNOB).OnMove();
			}
		}
		// test Item end
		oni1.GetPlayerPos(player.GetX(), player.GetY());
		if (oni1.isCatch()) {
			//GotoGameState(GAME_STATE_OVER);
		}
		else
			oni1.OnMove(gamemaps.at(_nowID));
	}



	void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{

		if (inputbox.IsWrite()) {
			inputbox.BoxOn(nChar);
		}
		else {
			if (nChar == KEY_A) {
				TRACE("%d %d \n", player.GetX(), player.GetY());
				isteleportblock = !isteleportblock;
			}
			if (nChar == KEY_I) {
				gamemaps.at(_nowID).isshowtileindex = (gamemaps.at(_nowID).isshowtileindex) ? false : true;
			}
			if (nChar == KEY_9) {
				if (gamemaps.at(_nowID).indexlayer > 0)gamemaps.at(_nowID).indexlayer--;
			}
			if (nChar == KEY_0) {
				if (gamemaps.at(_nowID).indexlayer < gamemaps.at(_nowID).GetLayer() - 1)gamemaps.at(_nowID).indexlayer++;
			}
			if (nChar == KEY_J) {
				if (_nowID > 0)
					_nowID -= 1;
			}
			if (nChar == KEY_K) {
				if (_nowID < 45)
					_nowID++;
			}
			if (nChar == KEY_G) {
				isgrid = !isgrid;
			}

			if (nChar == KEY_E) {
				if (pointvec.size() % 6 == 3) {
					TRACE("still one point in buffer, pop out or add a new point.\n");
				}
				else {
					isedit = !isedit;
				}
			}
			if (isedit && !inputbox.IsWrite()) {
				if (nChar == KEY_7) {
					istwoway = 1;
				}
				else if (nChar == KEY_8) {
					istwoway = 2;
				}
				
			}

			if (nChar == KEY_W) {
				inputbox.ClearBuffer();
				inputbox.TimerStart();
				inputbox.ToggleBox();
			}
			if (nChar == KEY_Q) {
				GotoGameState(GAME_STATE_OVER);
			}
			if (nChar == KEY_U) { // press "U" show dialog -> if finish item control will optimize
				talk.Show();
			}
			if (nChar == KEY_P) { // press "P" show dialog -> if finish item control will optimize
				useItem.Show();
			}
			if (nChar == VK_SPACE) { // press "space" close dialog
				if (inputbox.isInteger()) {
					int index = stoi(string(inputbox.GetString()));
					if (index >= 0 && index <= 53) {
						_nowID = index;
					}
				}
				if (!story.isClose()) {
					story.Close();
				}
				talk.Close();

			}
		}
		if (talk.isClose() && useItem.isClose()) { // if dialog is on, player can't move
			player.OnKeyDown(nChar);

			//test Item
			if (_nowID == 19) {
				items.at(TOILET).OnKeyDown(nChar);
			}
			else if (_nowID == 18) {
				if (!items.at(TUB_ONCE).IsFixed()) {
					//TRACE("\n\nhhhhh\n\n");
					items.at(TUB_ONCE).OnKeyDown(nChar);
					if (nChar != VK_SPACE)
						items.at(PHILLIPS).OnKeyDown(nChar);
				}
				if (items.at(TUB_ONCE).IsFixed()) {
					items.at(PHILLIPS).OnKeyDown(nChar);
				}
				if (items.at(PHILLIPS).IsPick()) {
					items.at(TUB_FIXED).OnKeyDown(nChar);
				}
			}
			else if (_nowID == 11) {
				items.at(BROKEN_DISH).OnKeyDown(nChar);
			}
			else if (_nowID == 12) {
				items.at(LIB_BOOK).OnKeyDown(nChar);
				// 3F L
				if (nChar != VK_SPACE)
					items.at(KEY_3F_L).OnKeyDown(nChar);
				if (items.at(LIB_BOOK).IsFixed())
					items.at(KEY_3F_L).OnKeyDown(nChar);
			}
			else if (_nowID == 14) {
				redChair.OnKeyDown(nChar);
				if (nChar != VK_SPACE)
					items.at(KEY_LIB).OnKeyDown(nChar);
				if (redChair.IsFixed())
					items.at(KEY_LIB).OnKeyDown(nChar);
			}
			else if (_nowID == 15) {
				if (!items.at(DOOR_KNOB).IsPick()) {
					items.at(DOOR_KNOB).OnKeyDown(nChar);
					if (nChar != VK_SPACE)
						items.at(DOOR_NO_KNOB).OnKeyDown(nChar);
				}
				if (items.at(DOOR_KNOB).IsPick())
					items.at(DOOR_NO_KNOB).OnKeyDown(nChar);
			}
		}
		if (!useItem.isClose()) {
			useItem.GetSelect(nChar);
		}

		if (nChar == VK_RETURN) {
		}
	}


	void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		if (talk.isClose() && useItem.isClose()) {
			player.OnKeyUp(nChar);
			if (_nowID == 14) {
				redChair.OnKeyUp(nChar);
			}
		}
	}

	void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
	{
		if (isedit && ((istwoway != 0 ) || (pointvec.size() % 6 == 3))) {

			mousex_foc = mousex;
			mousey_foc = mousey;
			tileplaceholder.SetTopLeft(mousex_foc * TILE, mousey_foc * TILE);
			if (istwoway != 0) {
				twowayvec.push_back(istwoway);
				//bsvec.push_back(isbs);
				(istwoway == 1) ? TRACE("push twoway {%d} true\n", istwoway) : TRACE("push twoway {%d} false\n", istwoway);
				//(isbs == 1) ? TRACE("push bs {%d} true \n", isbs) : TRACE("push bs {%d} false \n", isbs);
				istwoway = 0;
				//isbs = 0;
			}
			pointvec.push_back(_nowID);
			pointvec.push_back(mousex_foc * TILE - gamemaps.at(_nowID).GetX());
			pointvec.push_back(mousey_foc * TILE - gamemaps.at(_nowID).GetY());
			TRACE("push {%d, %d ,%d }\n", _nowID, mousex_foc * TILE - gamemaps.at(_nowID).GetX(), mousey_foc * TILE - gamemaps.at(_nowID).GetY());
		}
		else if (isedit) {
			TRACE("\n please specify istwoway?(oneway/twoway)(7/8)\n and isbs?(yes/no)(5/6)\n");
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
			if (pointvec.empty()) {
				TRACE("nothing to be popped\n");
			}
			else {
				int len = int(pointvec.size());
				int len2 = int(twowayvec.size());
				//int len3 = int(bsvec.size());
				if (int(pointvec.size() % 6 == 3)) {
					TRACE("twoway popped{%d}\n", twowayvec[len2 - 1]);
					//TRACE("bs popped{%d}\n",bsvec[len3-1]);
					twowayvec.pop_back();
					//bsvec.pop_back();
				}
				TRACE("element {%d,%d,%d} popped\n", pointvec[len - 3], pointvec[len - 2], pointvec[len - 1]);
				pointvec.pop_back();
				pointvec.pop_back();
				pointvec.pop_back();
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
			///////////////////// debug section
			//
			string maplink = "map_bmp/maplink.txt";
			gamemaps.at(_nowID).ShowMapAll(player, oni1, mapoverlayindex.at(_nowID));
			// test Item
			if (_nowID == 19) {
				items.at(TOILET).OnShow();
			}
			else if (_nowID == 18) {
				if (!items.at(TUB_ONCE).IsFixed() || !items.at(TUB_ONCE).IsAnimationDone()) {
					(items.at(TUB_ONCE).IsAnimationDone()) ? TRACE("\n\n\naaaaaaaaaaa\n\n") : TRACE("\n\n\asdsdfdfdsadfsdfdd\n\n");
					items.at(TUB_ONCE).OnShow();
				}
				if (items.at(TUB_ONCE).IsFixed() && items.at(TUB_ONCE).IsAnimationDone()) {
					items.at(PHILLIPS).OnShow();
				}
				if (items.at(PHILLIPS).IsPick()) {
					items.at(TUB_FIXED).OnShow();
				}
			}
			else if (_nowID == 11) {
				items.at(BROKEN_DISH).OnShow();
			}
			else if (_nowID == 12) {
				items.at(LIB_BOOK).OnShow();
				if (items.at(LIB_BOOK).IsFixed())
					items.at(KEY_3F_L).OnShow();
			}
			else if (_nowID == 14) {
				redChair.OnShow();
				if (redChair.IsFixed())
					items.at(KEY_LIB).OnShow();
			}
			else if (_nowID == 15) {
				if (!items.at(DOOR_KNOB).IsPick())
					items.at(DOOR_KNOB).OnShow();
				if (items.at(DOOR_KNOB).IsPick())
					items.at(DOOR_NO_KNOB).OnShow();
			}
			// test Item end
			if (isedit && !ofs.is_open()) {
				ofs.open(maplink, std::ios::app);
				if (!ofs.is_open()) {
					TRACE("Failed to open file.\n");
					throw std::invalid_argument("open failed");
				}
				TRACE("open maplink.txt\n");
			}
			if (ofs.is_open() && !isedit) {

				int i = 1;
				int j = 0;
				int k = 0;
				for (auto f : pointvec) {
					if (i % 6 == 1) {
						ofs << twowayvec[j++];
						ofs << " ";
						//ofs << bsvec[k++];
						//ofs << " ";
					}
					ofs << f;
					if (i % 6 == 0) {
						ofs << "\n";
					}
					else {
						ofs << " ";
					}
					i++;
				}
				ofs.close();
				twowayvec.clear();
				pointvec.clear();
				//bsvec.clear();
				TRACE("twowayvec & pointtmp & bsvec cleared \n");
				ifstream file(maplink);
				string content;
				string line;
				int lineNumber = 0;
				while (getline(file, line)) {
					lineNumber++;
					if(lineNumber!=1)content += line + "\n";
				}
				file.close();
				TRACE("link number : %d\n", lineNumber-1);
				// Write the modified content back to the file
				ofstream outputFile(maplink,ios::trunc);
				outputFile << lineNumber -1 <<"\n";
				outputFile << content;
				outputFile.close();
				TRACE("close\n");
			}

			inputbox.Show();
			if (isteleportblock) {
				for (int i = 0; i < router.GetRecord(_nowID); i++) {
					for (int j = 0; j < router.GetNowMapPortal(_nowID)[i].GetSize(); j++) {
						tileplaceholder.SetTopLeft(router.GetNowMapPortal(_nowID)[i].GetPointByIndex(j).GetW()+gamemaps.at(_nowID).GetX(), router.GetNowMapPortal(_nowID)[i].GetPointByIndex(j).GetX() + gamemaps.at(_nowID).GetY());
						tileplaceholder.ShowBitmap();

					}
				}

			}
			gamemaps.at(_nowID).ShowTileIndexLayer();
			if (isgrid) {
				grid.ShowBitmap();
			}
			if (isedit) {
				tileplaceholder.ShowBitmap();
			}

			CDC *pDC = CDDraw::GetBackCDC();
			CTextDraw::ChangeFontLog(pDC, 20, "Noto Sans TC", RGB(255, 255, 255));

			CTextDraw::Print(pDC, 0, 0, "map index:" + to_string(_nowID) +"  "+ gamemaps.at(_nowID).GetName() );
			(isedit) ? CTextDraw::Print(pDC, 0, TILE, " edit mode: true") : CTextDraw::Print(pDC, 0, TILE, " edit mode: false");

			CTextDraw::Print(pDC, 0, TILE * 2, "mouse window tile coordinate : " + to_string(mousex) + "  " + to_string(mousey));
			CTextDraw::Print(pDC, 0, TILE * 3, "mouse map tile coordinate : " + to_string(mousex - gamemaps.at(_nowID).GetX()/TILE)+ "  " + to_string(mousey - gamemaps.at(_nowID).GetY() / TILE));

			CTextDraw::Print(pDC, 0, TILE * 4, "player tile coordinate on map: " + to_string((player.GetX() - gamemaps.at(_nowID).GetX()) / TILE) + " " + to_string((player.GetY() - gamemaps.at(_nowID).GetY()) / TILE));
			CTextDraw::Print(pDC, 0, TILE * 5, "player tile coordinate on window: " + to_string(player.GetX()/TILE) + " " + to_string(player.GetY()/TILE));
			CTextDraw::Print(pDC, 0, TILE * 6, "(check for out of grid) player cor point x : " + to_string((player.GetX() - gamemaps.at(_nowID).GetX()) % TILE) + " y : " + to_string((player.GetY() - gamemaps.at(_nowID).GetY()) % TILE));
			CTextDraw::Print(pDC, 0, TILE * 17,"     up            :     "+ to_string(gamemaps.at(_nowID).GetMapData(gamemaps.at(_nowID).indexlayer, (player.GetX()-gamemaps.at(_nowID).GetX())/TILE , (player.GetU()- gamemaps.at(_nowID).GetY()) /TILE)) );
			CTextDraw::Print(pDC, 0, TILE * 18,"left    right      : "+ 
				to_string(gamemaps.at(_nowID).GetMapData(gamemaps.at(_nowID).indexlayer, (player.GetL() - gamemaps.at(_nowID).GetX()) / TILE, (player.GetY() - gamemaps.at(_nowID).GetY()) / TILE))+
				"    " + to_string(gamemaps.at(_nowID).GetMapData(gamemaps.at(_nowID).indexlayer, (player.GetR() - gamemaps.at(_nowID).GetX()) / TILE, (player.GetY() - gamemaps.at(_nowID).GetY()) / TILE)
			));
			CTextDraw::Print(pDC, 0, TILE * 19,"    down           :     "+ to_string(gamemaps.at(_nowID).GetMapData(gamemaps.at(_nowID).indexlayer, (player.GetX() - gamemaps.at(_nowID).GetX()) / TILE, (player.GetD() - gamemaps.at(_nowID).GetY()) / TILE))  );
			(istwoway != 0)?((istwoway == 1) ? CTextDraw::Print(pDC, 0, TILE * 9, "is twoway : yes" ): CTextDraw::Print(pDC, 0, TILE * 9, "is twoway : no")):CTextDraw::Print(pDC, 0, TILE * 9, "is twoway : none");
			//(isbs != 0) ? ((isbs == 1) ? CTextDraw::Print(pDC, 0, TILE * 10, "is block sensitive : yes" ): CTextDraw::Print(pDC, 0, TILE * 10, "is block sensitive : no")): CTextDraw::Print(pDC,0, TILE * 10, "is block sensitive : none");
			
			int len = int(pointvec.size());
			if(len % 6 == 0 && len !=0){
				CTextDraw::Print(pDC, 0, TILE * 20,"point1  " + to_string(pointvec[len-6]) +"  "+ to_string(pointvec[len-5]) + "  " + to_string(pointvec[len - 4]) + "  tile x:  " + to_string(pointvec[len - 5] / TILE) + "  tile y:  " + to_string(pointvec[len - 4] / TILE));
				CTextDraw::Print(pDC, 0, TILE * 21,"point2  " + to_string(pointvec[len-3]) + "  " + to_string(pointvec[len-2]) + "  " + to_string(pointvec[len - 1]) + "  tile x:  " + to_string(pointvec[len - 2] / TILE) + "  tile y:  " + to_string(pointvec[len - 1] / TILE));
			
			}
			else if (len % 3 == 0 && len != 0) {
				CTextDraw::Print(pDC, 0, TILE * 20,"point1  " +  to_string(pointvec[len - 3]) + "  " + to_string(pointvec[len - 2]) + "  " + to_string(pointvec[len - 1]) + "  tile x:  " + to_string(pointvec[len - 2] / TILE) + "  tile y:  " + to_string(pointvec[len - 1] / TILE));
			}	
			CDDraw::ReleaseBackCDC();
			//////////////////////// debug section end
			
			//oni1.OnShow();
		

			if (!talk.isClose()) {
				talk.ShowTotal();
			}
			if (!talk.isClose()) {
				talk.ShowTotal();
			}
			if (!useItem.isClose()) {
				useItem.ShowTotal();
			}
		}
	}

};