/*	Frost Open-Source World of Warcraft Bot
	Copyright (C) 2013 https://code.google.com/p/ejt-projects/

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>. */

#include <Application\CSystem.h>

CSystem::CSystem(const char* wndName, const char* className) : AbstractWindow() {
	_windowName = wndName;
	_className = className;
	_hInstance = GetModuleHandle(NULL);
	_style = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
	_x = _y = CW_USEDEFAULT;
	_height = 600;
	_width = 550;
	_styleEx = 0;
	_hwndParent = 0;
	_hMenu = 0;
}

bool CSystem::Initialize() {
	if(!Mem.Initialize())
		return false;

	if(!ObjectManager.Initialize())
		return false;

	ObjectManager.Pulse(); // Force pulse to get local player

	return true;
}

void CSystem::Run() {
	MSG msg;

	ZeroMemory(&msg, sizeof(MSG));

	while(GetMessage(&msg, NULL, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if(Me) {
		delete Me;
		Me = 0;
	}
}

LRESULT __stdcall CSystem::wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch(msg) {
		case WM_CREATE:
			{
				MainTab.Create(hwnd);
				MainTab.AddTab("General");
				//MainTab.AddTab("Object Manager");

				/* GENERAL TAB */
				TextPlayerName.Create(MainTab.Handle());
				TextPlayerName.Text("Player Name");
				MainTab.AddControl("General", TextPlayerName.Handle());

				TextPlayerHealth.Create(MainTab.Handle());
				TextPlayerHealth.Text("Player Health");
				MainTab.AddControl("General", TextPlayerHealth.Handle());

				TextPlayerPower.Create(MainTab.Handle());
				TextPlayerPower.Text("Player Power");
				MainTab.AddControl("General", TextPlayerPower.Handle());

				TextPlayerLevel.Create(MainTab.Handle());
				TextPlayerLevel.Text("Player Level");
				MainTab.AddControl("General", TextPlayerLevel.Handle());

				TextPlayerLocation.Create(MainTab.Handle());
				TextPlayerLocation.Text("Player Location");
				MainTab.AddControl("General", TextPlayerLocation.Handle());

				TextTargetName.Create(MainTab.Handle());
				TextTargetName.Text("Target Name");
				MainTab.AddControl("General", TextTargetName.Handle());

				TextTargetHealth.Create(MainTab.Handle());
				TextTargetHealth.Text("Target Health");
				MainTab.AddControl("General", TextTargetHealth.Handle());

				TextTargetPower.Create(MainTab.Handle());
				TextTargetPower.Text("Target Power");
				MainTab.AddControl("General", TextTargetPower.Handle());

				TextTargetLevel.Create(MainTab.Handle());
				TextTargetLevel.Text("Target Level");
				MainTab.AddControl("General", TextTargetLevel.Handle());

				TextTargetLocation.Create(MainTab.Handle());
				TextTargetLocation.Text("Target Location");
				MainTab.AddControl("General", TextTargetLocation.Handle());

				/* OBJECT MANAGER TAB 
				ObjectsTab.Create(MainTab.Handle());
				ObjectsTab.AddTab("Players");
				ObjectsTab.AddTab("Units");
				ObjectsTab.AddTab("Objects");

				MainTab.AddControl("Object Manager", ObjectsTab.Handle());

				ListPlayers.Create(ObjectsTab.Handle());
				ListPlayers.AddColumn("GUID", 100);
				ListPlayers.AddColumn("Name", 100);
				ListPlayers.AddColumn("Health", 150);
				ListPlayers.AddColumn("Power", 150);
				ListPlayers.AddColumn("Level", 100);

				ObjectsTab.AddControl("Players", ListPlayers.Handle());

				ListUnits.Create(ObjectsTab.Handle());
				ListUnits.AddColumn("GUID", 100);
				ListUnits.AddColumn("Name", 100);
				ListUnits.AddColumn("Health", 150);
				ListUnits.AddColumn("Power", 150);
				ListUnits.AddColumn("Level", 100);

				ObjectsTab.AddControl("Units", ListUnits.Handle());

				ListObjects.Create(ObjectsTab.Handle());
				ListObjects.AddColumn("GUID", 100);

				ObjectsTab.AddControl("Objects", ListObjects.Handle());
				*/
				MainTab.SwitchTab("General", -1);
				//ObjectsTab.SwitchTab("Players", -1);

				SendMessage(hwnd, WM_SIZE, 0, 0); // Force resize

				SetTimer(hwnd, TIMER_UPDATE_INFO, 1000, (TIMERPROC)NULL);
			}
			break;
		case WM_NOTIFY:
			switch(((LPNMHDR)lParam)->code) {
				case TCN_SELCHANGING:
					return false;
					break;
				case TCN_SELCHANGE:
					{
						int page = TabCtrl_GetCurSel(((LPNMHDR)lParam)->hwndFrom);
						
						if(((LPNMHDR)lParam)->hwndFrom == MainTab.Handle())
							MainTab.SwitchTab("", page);
//						else if(((LPNMHDR)lParam)->hwndFrom == ObjectsTab.Handle())
//							ObjectsTab.SwitchTab("", page);
					}
					break;
			}
			break;
		case WM_TIMER:
			switch(wParam) {
				case TIMER_UPDATE_INFO:
					if(Me->InGame()) {
						ObjectManager.Pulse();
						/* TESTING */
						/*
						ListPlayers.Clear();
						for each(WoWPlayer player in ObjectManager.GetPlayers()) {
							if(player.IsValid()) {
								char guid[64] = {0};
								char name[64] = {0};
								char health[64] = {0};
								char power[64] = {0};
								char level[64] = {0};
								sprintf_s(guid, "0x%X", (player.Guid() & 0xFFFFFFFF));
								sprintf_s(name, "%s, 0x%X", player.Name().c_str(), player.Race() & 0xFF);
								if(player.Dead()) {
									sprintf_s(health, "Dead");
									sprintf_s(power, "Dead");
								} else {
									sprintf_s(health, "%d/%d (%d%%)", player.Health(), player.MaxHealth(), player.HealthPercentage());
									sprintf_s(power, "%s %d/%d (%d%%)", player.PowerType(), player.Power(), player.MaxPower(), player.PowerPercentage());
								}
								sprintf_s(level, "%d", player.Level());
								LPSTR row[] = { guid, name, health, power, level };
								ListPlayers.AddRow(row);
							}
						}

						ListUnits.Clear();
						for each(WoWUnit unit in ObjectManager.GetUnits()) {
							if(unit.IsValid()) {
								char guid[64] = {0};
								char name[64] = {0};
								char health[64] = {0};
								char power[64] = {0};
								char level[64] = {0};
								sprintf_s(guid, "0x%X", (unit.Guid() & 0xFFFFFFFF));
								sprintf_s(name, "%s", unit.Name().c_str());
								if(unit.Dead()) {
									sprintf_s(health, "Dead");
									sprintf_s(power, "Dead");
								} else {
									sprintf_s(health, "%d/%d (%d%%)", unit.Health(), unit.MaxHealth(), unit.HealthPercentage());
									sprintf_s(power, "%s %d/%d (%d%%)", unit.PowerType(), unit.Power(), unit.MaxPower(), unit.PowerPercentage());
								}
								sprintf_s(level, "%d", unit.Level());
								LPSTR row[] = { guid, name, health, power, level };
								ListUnits.AddRow(row);
							}
						}
						*/
						/* END OF TESTING */
						
						TextPlayerName.Text("Name: %s", Me->Name().c_str());
						TextPlayerHealth.Text("Health: %d/%d (%d%%)", Me->Health(),
							Me->MaxHealth(),
							Me->HealthPercentage());
						TextPlayerPower.Text("%s: %d/%d (%d%%)", Me->PowerTypeName(),
							Me->Power(),
							Me->MaxPower(),
							Me->PowerPercentage());
						TextPlayerLevel.Text("Level: %d %s %s %s", Me->Level(), Me->GenderName(), Me->RaceName(), Me->ClassName());
						TextPlayerLocation.Text("%s\nF: %4.2f", Me->Location().ToString().c_str(),
							Me->Facing());
						
						if(Me->Target() != NULL && Me->Target()->IsValid()) {
							TextTargetName.Text("Name: %s", Me->Target()->Name().c_str());
							TextTargetHealth.Text("Health: %d/%d (%d%%)", Me->Target()->Health(),
								Me->Target()->MaxHealth(),
								Me->Target()->HealthPercentage());
							TextTargetPower.Text("%s: %d/%d (%d%%)", Me->Target()->PowerTypeName(),
								Me->Target()->Power(),
								Me->Target()->MaxPower(),
								Me->Target()->PowerPercentage());
							TextTargetLevel.Text("Level: %d %s %s %s", Me->Target()->Level(), Me->Target()->GenderName(), Me->Target()->RaceName(), Me->Target()->ClassName());
							TextTargetLocation.Text("%s\n<3D: %4.2f FT: %4.2f>", Me->Target()->Location().ToString().c_str(),
								Me->Location().GetDistanceTo(Me->Target()->Location()),
								Me->Location().FacingTo(Me->Target()->Location()));
						} else {
							TextTargetName.Text("N/A");
							TextTargetHealth.Text("N/A");
							TextTargetPower.Text("N/A");
							TextTargetLevel.Text("N/A");
							TextTargetLocation.Text("N/A");
						}
					}
					break;
			}
			break;
		case WM_SIZE:
			{
				RECT rcClient;
				RECT rcMain = {0};
				GetClientRect(hwnd, &rcClient);
				TabCtrl_AdjustRect(MainTab.Handle(), false, &rcMain);

				MainTab.SetPos(rcClient.left, rcClient.top, rcClient.right, rcClient.bottom);
/*
				ObjectsTab.SetPos(rcClient.left + 5, rcClient.top + 28, rcClient.right - rcClient.left - 5, rcClient.bottom - rcClient.top - 5);

				rcClient.left -= 4;
				rcClient.top -= 4;
				rcClient.bottom = rcClient.bottom - rcClient.top - 2;
				rcClient.right = rcClient.right - rcClient.left - 3;
				
				ListPlayers.SetPos(rcClient.left + 1, rcClient.top + 24, rcClient.right - rcClient.left - 2, rcClient.bottom - rcClient.top - 3);
				ListUnits.SetPos(rcClient.left + 1, rcClient.top + 24, rcClient.right - rcClient.left - 2, rcClient.bottom - rcClient.top - 3);
				ListObjects.SetPos(rcClient.left + 1, rcClient.top + 24, rcClient.right - rcClient.left - 2, rcClient.bottom - rcClient.top - 3);
*/
				TextPlayerName.SetPos(rcMain.left + 10, rcMain.top + 35, 250, 20);
				TextPlayerHealth.SetPos(rcMain.left + 10, rcMain.top + 55, 250, 20);
				TextPlayerPower.SetPos(rcMain.left + 10, rcMain.top + 75, 250, 20);
				TextPlayerLevel.SetPos(rcMain.left + 10, rcMain.top + 95, 250, 20);
				TextPlayerLocation.SetPos(rcMain.left + 10, rcMain.top + 115, 250, 40);
				
				TextTargetName.SetPos(rcMain.left + 260, rcMain.top + 35, 250, 20);
				TextTargetHealth.SetPos(rcMain.left + 260, rcMain.top + 55, 250, 20);
				TextTargetPower.SetPos(rcMain.left + 260, rcMain.top + 75, 250, 20);
				TextTargetLevel.SetPos(rcMain.left + 260, rcMain.top + 95, 250, 20);
				TextTargetLocation.SetPos(rcMain.left + 260, rcMain.top + 115, 250, 40);
			}
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	return 0;
}