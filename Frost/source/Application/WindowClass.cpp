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

#include <Application\AbstractWindow.h>
#include <Application\WindowClass.h>

WindowClass::WindowClass(HINSTANCE hInst, const char* className) {
	hInstance = hInst;
	lpfnWndProc = AbstractWindow::msgRouter;
	lpszClassName = className;
	lpszMenuName = 0;
	cbSize = sizeof(WNDCLASSEX);
	cbClsExtra = 0;
	cbWndExtra = 0;
	style = 0;
	hIcon = LoadIcon(NULL, IDI_APPLICATION);
	hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	hCursor = LoadCursor(NULL, IDC_ARROW);
	hbrBackground = (HBRUSH)COLOR_WINDOW;
}

bool WindowClass::Register() {
	if(RegisterClassEx(this))
		return true;
	else {
		return false;
	}
}