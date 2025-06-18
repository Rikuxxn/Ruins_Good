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
#include "main.h"
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
		TYPE_FLOOR,
		TYPE_MAX
	}TYPE;

	static CBlock* Create(const char* pFilepath, D3DXVECTOR3 pos, D3DXVECTOR3 rot,D3DXVECTOR3 size);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetType(TYPE type) { m_Type = type; }
	TYPE GetType(void) const { return m_Type; }
	virtual D3DXCOLOR GetCol(void) const override;
	void SetSelected(bool flag) { m_bSelected = flag; }
	bool IsSelected(void) const { return m_bSelected; }

private:
	char m_szPath[MAX_PATH];	// ファイルパス
	D3DXVECTOR3 m_move;			// 移動量
	TYPE m_Type;				// 種類
	D3DXCOLOR m_col;			// アルファ値
	D3DXCOLOR m_baseCol;		// ベースのアルファ値
	bool m_bSelected;
};

#endif