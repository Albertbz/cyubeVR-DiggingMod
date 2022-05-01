#include <windows.h>
#include <locale>
#include <codecvt>
#include <string>

std::wstring getModulePath(void* address)
{
	char path[FILENAME_MAX];
	HMODULE hm = NULL;

	if (!GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
		GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
		(LPCSTR)address,
		&hm))
	{

	}
	GetModuleFileNameA(hm, path, sizeof(path));

	std::string p = path;
	size_t found = p.find_last_of("/\\");
	std::string p1 = p.substr(0, found);
	std::wstring p2 = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(p1);

	return p2;
}