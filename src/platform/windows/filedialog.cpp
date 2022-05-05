#include <noypixel/noypixel.h>

#if NOY_PLATFORM == NOY_WINDOWS

#include <Windows.h>
#include <commdlg.h>

std::optional<std::string> fileDialog(bool save)
{
    OPENFILENAME ofn;
	char szFile[260];
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	// ofn.hwndOwner = hwnd;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "All\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileNameA(&ofn) == TRUE)
		return ofn.lpstrFile;
	return {};
}

#endif
