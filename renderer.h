//=============================================================================
//
// �����_�����O���� [renderer.h]
// Author : TANEKAWA RIKU
//
//=============================================================================
#ifndef _RENDERER_H_// ���̃}�N����`������Ă��Ȃ�������
#define _RENDERER_H_// 2�d�C���N���[�h�h�~�̃}�N����`

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "debugproc.h"


//*****************************************************************************
// �����_���[�N���X
//*****************************************************************************
class CRenderer
{
public:
	CRenderer();
	~CRenderer();

	HRESULT Init(HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(int fps);
	void ResetDevice(void);
	void OnResize(UINT width, UINT height);
	bool NeedsReset(void) const;
	LPDIRECT3DDEVICE9 GetDevice(void) { return m_pD3DDevice; };
	static CDebugProc* GetDebug(void);
	void SetFPS(int fps) { m_nFPS = fps; }
	int GetFPS(void) { return m_nFPS; }
	D3DXCOLOR GetBgCol(void) { return m_bgCol; }
	void SetBgCol(D3DXCOLOR col);

private:
	LPDIRECT3D9 m_pD3D;						// DirectX3D�I�u�W�F�N�g�ւ̃|�C���^
	LPDIRECT3DDEVICE9 m_pD3DDevice;			// �f�o�C�X�ւ̃|�C���^
	static CDebugProc* m_pDebug;			// �f�o�b�O�t�H���g�ւ̃|�C���^
	D3DXCOLOR m_bgCol;						// ��ʔw�i�̐F

	UINT m_ResizeWidth;
	UINT m_ResizeHeight;
	D3DPRESENT_PARAMETERS m_d3dpp;
	HWND m_hWnd;
	int m_nFPS;
};
#endif