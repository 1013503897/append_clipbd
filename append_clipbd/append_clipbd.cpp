#include <vector>
#include <string.h>
#include <windows.h>
#include <shlobj.h>
#include <stdio.h>

using namespace std;

void CopyFileToClipboard(wchar_t szFileName[]);

int main()
{
	wchar_t path[] = { L"C:\\Users\\weixingyu\\Desktop\\Dbgview.exe"};
	CopyFileToClipboard(path);
}

void CopyFileToClipboard(wchar_t szFileName[])
{
	OpenClipboard(NULL);
	HDROP hDrop = (HDROP)GetClipboardData(CF_HDROP);
	GlobalLock(hDrop);
	vector<PWCH> files;
	files.push_back(szFileName);
	UINT fileCount = DragQueryFileW(hDrop, 0xFFFFFFFF, 0, 0);
	UINT fileNameLen = 0;
	for (size_t i = 0; i < fileCount; i++)
	{
		WCHAR lpszFileName[MAX_PATH] = { 0 };
		fileNameLen = DragQueryFileW(hDrop, i, 0, 0);
		DragQueryFileW(hDrop, i, lpszFileName, fileNameLen + 1);
		files.push_back(lpszFileName);
	}
	GlobalUnlock(hDrop);

	UINT clpSize = sizeof(DROPFILES);
	for (size_t i = 0; i < files.size(); i++)
	{
		clpSize += sizeof(WCHAR) * (wcslen(files[i]) + 1);
	}
	clpSize += sizeof(WCHAR);

	HDROP hMyDrop = (HDROP)GlobalAlloc(GHND, clpSize);
	DROPFILES* pDropFiles = (DROPFILES*)GlobalLock(hMyDrop);
	pDropFiles->pFiles = sizeof(DROPFILES);
	pDropFiles->fWide = TRUE;
	PWCHAR pDst = (PWCHAR)&pDropFiles[1];
	for (size_t i = 0; i < files.size(); i++)
	{
		wcscpy(pDst, files[i]);
		pDst = &pDst[wcslen(files[i]) + 1];
	}
	GlobalUnlock(hMyDrop);
	EmptyClipboard();
	SetClipboardData(CF_HDROP, hMyDrop);
	CloseClipboard();
}

