//=============================================================================
//
// �����_�����O���� [renderer.h]
// Author : TANEKAWA RIKU
//
//=============================================================================
#ifndef _RENDERER_H_// ���̃}�N����`������Ă��Ȃ�������
#define _RENDERER_H_// 2�d�C���N���[�h�h�~�̃}�N����`

#include <d3dx9.h>
#include "main.h"
#include "debugproc.h"

// �����_���[�N���X
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
private:
	LPDIRECT3D9 m_pD3D;						// DirectX3D�I�u�W�F�N�g�ւ̃|�C���^
	LPDIRECT3DDEVICE9 m_pD3DDevice;			// �f�o�C�X�ւ̃|�C���^
	static CDebugProc* m_pDebug;			// �f�o�b�O�t�H���g�ւ̃|�C���^

	UINT m_ResizeWidth;
	UINT m_ResizeHeight;
	D3DPRESENT_PARAMETERS m_d3dpp;
	HWND m_hWnd;

};
#endif