//=============================================================================
//
// ブロック処理 [block.h]
// Author : TANEKAWA RIKU
//
//=============================================================================
#ifndef _BLOCK_H_
#define _BLOCK_H_

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "objectX.h"

//*****************************************************************************
// ブロッククラス
//*****************************************************************************
class CBlock : public CObjectX
{
public:
	CBlock();
	~CBlock();

	//*****************************************************************************
	// ブロックの種類
	//*****************************************************************************
	typedef enum
	{
		TYPE_WOODBOX = 0,
		TYPE_WALL,
		TYPE_AXE,
		TYPE_IKADA,
		TYPE_ROCK,
		TYPE_MAX
	}TYPE;

	static CBlock* Create(const char* pFilepath, D3DXVECTOR3 pos, D3DXVECTOR3 rot,D3DXVECTOR3 size);
	static CBlock* CreateFromType(TYPE type, D3DXVECTOR3 pos);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetType(TYPE type) { m_Type = type; }
	TYPE GetType(void) const { return m_Type; }
	virtual D3DXCOLOR GetCol(void) const override;
	void SetSelected(bool flag) { m_bSelected = flag; }
	bool IsSelected(void) const { return m_bSelected; }
	int GetTextureIndex() const { return m_nIdxTexture; }// 画像表示の時に使う
	static const char* GetTexPathFromType(TYPE type);

private:
	char m_szPath[MAX_PATH];	// ファイルパス
	TYPE m_Type;				// 種類
	D3DXCOLOR m_col;			// アルファ値
	D3DXCOLOR m_baseCol;		// ベースのアルファ値
	bool m_bSelected;
	int m_nIdxTexture;
};

#endif