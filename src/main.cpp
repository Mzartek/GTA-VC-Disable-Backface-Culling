#include <windows.h>
#include <memoryapi.h>

// https://github.com/aap/skygfx_vc/

typedef uintptr_t addr;
typedef unsigned char uchar;

int gtaversion = -1;

enum
{
	VC_10 = 1,
	VC_11,
	VC_STEAM
};

template<typename T>
inline T AddressByVersion(addr addressvc10, addr addressvc11, addr addressvcSteam)
{
	if (gtaversion == -1) {
		if (*(addr*)0x667BF5 == 0xB85548EC) gtaversion = VC_10;
		else if (*(addr*)0x667C45 == 0xB85548EC) gtaversion = VC_11;
		else if (*(addr*)0x666BA5 == 0xB85548EC) gtaversion = VC_STEAM;
		else gtaversion = 0;
	}
	switch (gtaversion) {
	case VC_10:
		return (T)addressvc10;
	case VC_11:
		return (T)addressvc11;
	case VC_STEAM:
		return (T)addressvcSteam;
	default:
		return (T)0;
	}
}

template<typename T, typename AT> inline void
Patch(AT address, T value)
{
	DWORD		dwProtect[2];
	VirtualProtect((void*)address, sizeof(T), PAGE_EXECUTE_READWRITE, &dwProtect[0]);
	*(T*)address = value;
	VirtualProtect((void*)address, sizeof(T), dwProtect[0], &dwProtect[1]);
}

BOOL WINAPI DllMain(HINSTANCE hInst, DWORD reason, LPVOID)
{
	if (reason == DLL_PROCESS_ATTACH) {
		// hope I didn't miss anything
		Patch<uchar>(AddressByVersion<addr>(0x4C9E5F, 0x4C9E7F, 0x4C9D1F), 1);	// in CRenderer::RenderOneNonRoad()
		Patch<uchar>(AddressByVersion<addr>(0x4C9F08, 0x4C9F28, 0x4C9DC8), 1);	// in CRenderer::RenderBoats()
		Patch<uchar>(AddressByVersion<addr>(0x4C9F5D, 0x4C9F7D, 0x4C9E1D), 1);	// in CRenderer::RenderEverythingBarRoads()
		Patch<uchar>(AddressByVersion<addr>(0x4CA157, 0x4CA177, 0x4CA017), 1);	// in CRenderer::RenderFadingInEntities()
		Patch<uchar>(AddressByVersion<addr>(0x4CA199, 0x4CA1B9, 0x4CA059), 1);	// in CRenderer::RenderRoads()
		// ADDRESS
		if (gtaversion == VC_10)
			Patch<uchar>(AddressByVersion<addr>(0x4E0146, 0, 0), 1);	// in CCutsceneObject::Render()
	}

	return TRUE;
}
