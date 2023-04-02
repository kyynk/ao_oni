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

	void CGameStateRun::OnBeginState()
	{
		mousex_foc = 0;
		mousey_foc = 0;
		istwoway = 0;
		isedit = false;
		isgrid = false;
		_nowID = 13;
		player.init(4,16);
		oni1.SetParam(Oni::OniType::normal, 4, 8);
	}

	void CGameStateRun::OnMove()							// 移動遊戲元素
	{
		
		inputbox.OnMove();

		
		player.OnMove(gamemaps.at(_nowID),banlist);
		//oni need to set XY if map change
		//oni1.SetXY()
		oni1.GetPlayerPos(player.GetX1(), player.GetY1());
		if (oni1.isCatch()) {

			//GotoGameState(GAME_STATE_OVER);
		}
		else {
			oni1.OnMove();

		}

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
			tmp.SetTopLeftMap((SIZE_X-16-w*TILE)/2 , (SIZE_Y-20-h* TILE)/2);
			gamemaps.push_back(tmp);
		}
		// dialog
		story.SetNow(Dialog::character::none);
		story.SetParam({"We heard rumors about the mansion", 
			"they say on the outskirts of town...", 
			"there is a monster living here...!"}, false);
		story.Show();
		talk.SetNow(Dialog::character::mika);
		talk.SetParam({ "Hi", "how", "r u?" }, false);
		useItem.SetNow(Dialog::character::hirosi);
		useItem.SetParam({ "Do u want to use that?" }, true);
		// item 
		testitem.Load({ "img/item/blueeye.bmp","img/item/book.bmp","img/item/oil.bmp" }, RGB(204, 255, 0));
		testitem.SetParam(30, 100, Item::itemtype::repeat, 0, 0);
		// debug
		grid.LoadBitmapByString({ "img/grid.bmp" }, RGB(0, 0, 0));
		seltile.LoadBitmapByString({ "img/placeholder.bmp" });
		inputbox.Load("img/cursor/input_box.bmp");
		inputbox.init(20 * TILE, 0, 0, 10);
		// map link data
		//router.Load("map_bmp/maplink.txt");
		//router.debug();
		banlist.push_back(0);
		banlist.push_back(12);
		banlist.push_back(28);

	}

	void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		testitem.GetPlayerPos(32, 0);
		testitem.OnMove(nChar);  // press G vanish
		if (inputbox.IsWrite()) {
			inputbox.BoxOn(nChar);
		}
		else {
			if (nChar == KEY_I) {
				gamemaps.at(_nowID).isshowtileindex = (gamemaps.at(_nowID).isshowtileindex) ? false : true;
			}
			if (nChar == KEY_9) {
				if (gamemaps.at(_nowID).indexlayer > 0)gamemaps.at(_nowID).indexlayer--;
			}
			if (nChar == KEY_0) {
				if(gamemaps.at(_nowID).indexlayer < gamemaps.at(_nowID).GetLayer() - 1)gamemaps.at(_nowID).indexlayer++;
			}
			if (nChar == KEY_J) {
				if (_nowID > 0)
					_nowID -= 1;
			}
			if (nChar == KEY_K) {
				if (_nowID < 53)
					_nowID++;
			}
			if (nChar == KEY_G) {
				isgrid = !isgrid;
			}

			if (nChar == KEY_E) {
				if (pointtmp.size() % 6 == 3) {
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
				player.OnKeyDown(nChar,gamemaps.at(_nowID));
			}
			if (!useItem.isClose()) {
				useItem.GetSelect(nChar);
			}

			if (nChar == VK_RETURN) {
				testitem.SetTrigger();
				testitem.Animation(0);
			}
	}
	

	void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		if (talk.isClose() && useItem.isClose()) {
			player.OnKeyUp(nChar);
		}
	}

	void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
	{
		if (isedit && (istwoway !=0 || (pointtmp.size()%6 == 3))) {
			
			mousex_foc = mousex;
			mousey_foc = mousey;
			seltile.SetTopLeft(mousex_foc*TILE, mousey_foc*TILE);
			if (istwoway != 0) {
				twowayvec.push_back(istwoway);
				TRACE("push twoway {%d}\n", istwoway);
				istwoway = 0;
			}
			pointtmp.push_back(_nowID);
			pointtmp.push_back(mousex_foc*TILE - gamemaps.at(_nowID).GetX());
			pointtmp.push_back(mousey_foc*TILE - gamemaps.at(_nowID).GetY());
			TRACE("push {%d, %d ,%d }\n", _nowID, mousex_foc*TILE-gamemaps.at(_nowID).GetX(), mousey_foc*TILE-gamemaps.at(_nowID).GetY());
		}
		else {
			TRACE("please specify istwoway?(oneway/twoway)(7/8)\n");
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
				TRACE("nothing to be popped\n");
			}
			else {
				int len = int(pointtmp.size());
				int len2 = int(twowayvec.size());
				if (int(pointtmp.size() % 6 == 3)) {
					TRACE("twoway popped{%d}\n",twowayvec[len2-1]);
					twowayvec.pop_back();
				}
				TRACE("element {%d,%d,%d} popped\n", pointtmp[len - 3], pointtmp[len - 2], pointtmp[len - 1] );
				pointtmp.pop_back();
				pointtmp.pop_back();
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
			///////////////////// debug section
			// 
			string maplink = "map_bmpmaplink.txt";
			gamemaps.at(_nowID).ShowMapAll(player);
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
				for (auto f : pointtmp) {
					if (i % 6 == 1) {
						ofs << twowayvec[j++];
						ofs << " ";
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
				pointtmp.clear();
				TRACE("twowayvec & pointtmp cleared \n");
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
			gamemaps.at(_nowID).ShowTileIndexLayer();
			if (isgrid) {
				grid.ShowBitmap();
			}
			if (isedit) {
				seltile.ShowBitmap();
			}

			CDC *pDC = CDDraw::GetBackCDC();
			CTextDraw::ChangeFontLog(pDC, 20, "Noto Sans TC", RGB(255, 255, 255));

			CTextDraw::Print(pDC, 0, 0, "map index:" + to_string(_nowID));
			CTextDraw::Print(pDC, 0, 0, "map index:" + to_string(_nowID));
			(isedit) ? CTextDraw::Print(pDC, 0, TILE, " edit mode: true") : CTextDraw::Print(pDC, 0, TILE, " edit mode: false");

			CTextDraw::Print(pDC, 0, TILE * 2, "mouse window tile coordinate : " + to_string(mousex) + "  " + to_string(mousey));
			CTextDraw::Print(pDC, 0, TILE * 3, "mouse map tile coordinate : " + to_string(mousex - gamemaps.at(_nowID).GetX()/TILE)+ "  " + to_string(mousey - gamemaps.at(_nowID).GetY() / TILE));

			CTextDraw::Print(pDC, 0, TILE * 4,"player tile coordinate on map: "+ to_string((player.GetX1()-gamemaps.at(_nowID).GetX())/TILE) + " " + to_string((player.GetY1()- gamemaps.at(_nowID).GetY()) /TILE)+
				"(check for out of grid) player cor point x : " + to_string((player.GetX1() - gamemaps.at(_nowID).GetX()) % TILE) + " y : " + to_string((player.GetY1() - gamemaps.at(_nowID).GetY()) % TILE));
			CTextDraw::Print(pDC, 0, TILE * 5,"     up            :     "+ to_string(gamemaps.at(_nowID).GetMapData(gamemaps.at(_nowID).indexlayer, (player.GetX1()-gamemaps.at(_nowID).GetX())/TILE , (player.GetU()- gamemaps.at(_nowID).GetY()) /TILE)) );
			CTextDraw::Print(pDC, 0, TILE * 6,"left    right      : "+ 
				to_string(gamemaps.at(_nowID).GetMapData(gamemaps.at(_nowID).indexlayer, (player.GetL() - gamemaps.at(_nowID).GetX()) / TILE, (player.GetY1() - gamemaps.at(_nowID).GetY()) / TILE))+
				"    " + to_string(gamemaps.at(_nowID).GetMapData(gamemaps.at(_nowID).indexlayer, (player.GetR() - gamemaps.at(_nowID).GetX()) / TILE, (player.GetY1() - gamemaps.at(_nowID).GetY()) / TILE)
			));
			CTextDraw::Print(pDC, 0, TILE * 7,"    down           :     "+ to_string(gamemaps.at(_nowID).GetMapData(gamemaps.at(_nowID).indexlayer, (player.GetX1() - gamemaps.at(_nowID).GetX()) / TILE, (player.GetD() - gamemaps.at(_nowID).GetY()) / TILE))  );
			CTextDraw::Print(pDC, 0, TILE * 8, "is twoway : " +to_string(istwoway));
			
			
			int len = int(pointtmp.size());
			if(len % 6 == 0 && len !=0){
				CTextDraw::Print(pDC, 0, TILE * 20,"point1  " + to_string(pointtmp[len-6]) +"  "+ to_string(pointtmp[len-5]) + "  " + to_string(pointtmp[len - 4]) + "  tile x:  " + to_string(pointtmp[len - 5] / TILE) + "  tile y:  " + to_string(pointtmp[len - 4] / TILE));
				CTextDraw::Print(pDC, 0, TILE * 21,"point2  " + to_string(pointtmp[len-3]) + "  " + to_string(pointtmp[len-2]) + "  " + to_string(pointtmp[len - 1]) + "  tile x:  " + to_string(pointtmp[len - 2] / TILE) + "  tile y:  " + to_string(pointtmp[len - 1] / TILE));
			
			}
			else if (len % 3 == 0 && len != 0) {
				CTextDraw::Print(pDC, 0, TILE * 20,"point1  " +  to_string(pointtmp[len - 3]) + "  " + to_string(pointtmp[len - 2]) + "  " + to_string(pointtmp[len - 1]) + "  tile x:  " + to_string(pointtmp[len - 2] / TILE) + "  tile y:  " + to_string(pointtmp[len - 1] / TILE));
			}	
			CDDraw::ReleaseBackCDC();
			//////////////////////// debug section end
			
			oni1.OnShow();
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
		}
	}

};