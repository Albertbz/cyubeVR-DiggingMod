#include <windows.h>
#include <locale>
#include <codecvt>
#include <string>

std::wstring getModulePath(void* address)
{
	wchar_t path[FILENAME_MAX];
	HMODULE hm = NULL;

	if (!GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
		GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
		(LPCSTR)address,
		&hm))
	{
	}

#pragma warning(disable:6386)
	GetModuleFileNameW(hm, path, sizeof(path));
#pragma warning(default:6386)

	std::wstring p = path;
	size_t found = p.find_last_of(L"/\\");
	std::wstring p1 = p.substr(0, found);

	return p1;
}