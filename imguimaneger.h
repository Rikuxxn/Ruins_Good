//=============================================================
//
// ImGui�̐��䏈�� [ImguiManager.h]
// Author : TANEKAWA RIKU
//
//=============================================================
#ifndef _IMGUIMANEGER_H_// ���̃}�N����`������Ă��Ȃ�������
#define _IMGUIMANEGER_H_// 2�d�C���N���[�h�h�~�̃}�N����`

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"


//*****************************************************************************
// ImGui�}�l�[�W���[�N���X
//*****************************************************************************
class CImGuiManager
{
public :
	CImGuiManager();
	~CImGuiManager();

	typedef enum
	{
		IMGUITYPE_DEFOULT = 0,
		IMGUITYPE_NOMOVE,
		IMGUITYPE_NOMOVESIZE,
		IMGUITYPE_NOMOVEANDSIZE,
		IMGUITYPE_MAX
	}IMGUITYPE;

	static void InitImgui(HWND hWnd, LPDIRECT3DDEVICE9 pDevice);
	static void UninitImgui();
	static void SetPosImgui(ImVec2 Pos);
	static void SetSizeImgui(ImVec2 Size);
	static void StartImgui(const char* ImguiName, IMGUITYPE nType);
	static void EndImgui();
	static void EndImguiRender();
	static void NewFlameImgui();

private:

};

#endif // !_IMGUIMANEGER_H_