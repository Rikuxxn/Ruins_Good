//=============================================================================
//
// テクスチャ処理 [texture.h]
// Author : TANEKAWA RIKU
//
//=============================================================================
#ifndef _TEXTURE_H_// このマクロ定義がされていなかったら
#define _TEXTURE_H_// 2重インクルード防止のマクロ定義

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "main.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_TEXTURE (32)


//*****************************************************************************
// テクスチャクラス
//*****************************************************************************
class CTexture
{
public:
	CTexture();
	~CTexture();

	HRESULT Load(void);
	void Unload(void);
	int Register(const char* pFilename);
	int RegisterX(const char* pFilename);
	LPDIRECT3DTEXTURE9 GetAddress(int nIdx);

private:
	const char* TEXTURE[MAX_TEXTURE] =
	{
		"data/TEXTURE/num001.png",
		"data/TEXTURE/pause.png",
		"data/TEXTURE/time.png",
		"data/TEXTURE/colon.png",
		"data/TEXTURE/ground.png",
		"data/TEXTURE/wall001.jpg",
		"data/TEXTURE/selectBG.png",
		"data/TEXTURE/shadow000.jpg",
		"data/TEXTURE/eye.png",
		"data/TEXTURE/rock.png",
		"data/TEXTURE/woodbox.png",
		"data/TEXTURE/Axe.png",
		"data/TEXTURE/ikada.png",
		"data/TEXTURE/wall.png",

	};

	LPDIRECT3DTEXTURE9 m_apTexture[MAX_TEXTURE];
	static int m_nNumAll;
};

#endif