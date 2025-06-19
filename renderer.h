//=============================================================================
//
// レンダリング処理 [renderer.h]
// Author : TANEKAWA RIKU
//
//=============================================================================
#ifndef _RENDERER_H_// このマクロ定義がされていなかったら
#define _RENDERER_H_// 2重インクルード防止のマクロ定義

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "debugproc.h"


//*****************************************************************************
// レンダラークラス
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
	LPDIRECT3D9 m_pD3D;						// DirectX3Dオブジェクトへのポインタ
	LPDIRECT3DDEVICE9 m_pD3DDevice;			// デバイスへのポインタ
	static CDebugProc* m_pDebug;			// デバッグフォントへのポインタ
	D3DXCOLOR m_bgCol;						// 画面背景の色

	UINT m_ResizeWidth;
	UINT m_ResizeHeight;
	D3DPRESENT_PARAMETERS m_d3dpp;
	HWND m_hWnd;
	int m_nFPS;
};
#endif