//=============================================================================
//
// レンダリング処理 [renderer.h]
// Author : TANEKAWA RIKU
//
//=============================================================================
#ifndef _RENDERER_H_// このマクロ定義がされていなかったら
#define _RENDERER_H_// 2重インクルード防止のマクロ定義

#include <d3dx9.h>
#include "main.h"
#include "debugproc.h"

// レンダラークラス
class CRenderer
{
public:
	CRenderer();
	~CRenderer();

	HRESULT Init(HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(int fps);
	LPDIRECT3DDEVICE9 GetDevice(void) { return m_pD3DDevice; };
	static CDebugProc* GetDebug(void);
private:
	LPDIRECT3D9 m_pD3D;						// DirectX3Dオブジェクトへのポインタ
	LPDIRECT3DDEVICE9 m_pD3DDevice;			// デバイスへのポインタ
	static CDebugProc* m_pDebug;			// デバッグフォントへのポインタ
};
#endif