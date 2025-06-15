//=============================================================================
//
// Xファイル処理 [objectX.h]
// Author : TANEKAWA RIKU
//
//=============================================================================
#ifndef _OBJECTX_H_// このマクロ定義がされていなかったら
#define _OBJECTX_H_// 2重インクルード防止のマクロ定義

#include "main.h"
#include "object.h"
#include "shadow.h"


// Xファイルクラス
class CObjectX : public CObject
{
public:
	CObjectX();
	~CObjectX();

	// モデルの種類
	typedef enum
	{
		TYPE_BEAR = 0,
		TYPE_WALL,
		TYPE_TEST,
		TYPE_MAX
	}MODELTYPE;

	static CObjectX* Create(const char* pFilepath, D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	D3DXVECTOR3 GetPos(void);
	void SetPath(const char* path);

private:
	int* m_nIdxTexture;
	D3DXVECTOR3 m_pos;					// 位置
	D3DXVECTOR3 m_rot;					// 向き
	LPD3DXMESH m_pMesh;					// メッシュへのポインタ
	LPD3DXBUFFER m_pBuffMat;			// マテリアルへのポインタ
	DWORD m_dwNumMat;					// マテリアル数
	D3DXMATRIX m_mtxWorld;				// ワールドマトリックス
	int m_nType;						// 種類
	char m_szPath[MAX_PATH];			// ファイルパス
	CShadow* m_pShadow;					// 影へのポインタ

};

#endif