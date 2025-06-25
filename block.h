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
#include "debugproc3D.h"

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
		TYPE_WALL2,
		TYPE_WALL3,
		TYPE_WALL4,
		TYPE_AXE,
		TYPE_IKADA,
		TYPE_ROCK,
		TYPE_TORCH,
		TYPE_TORCH2,
		TYPE_FLOOR,
		TYPE_FLOOR2,
		TYPE_DOOR,
		TYPE_CEILING,
		TYPE_CEILING2,
		TYPE_DOOR_BODY,
		TYPE_MAX
	}TYPE;

	static CBlock* Create(const char* pFilepath, D3DXVECTOR3 pos, D3DXVECTOR3 rot,D3DXVECTOR3 size);
	static CBlock* CreateFromType(TYPE type, D3DXVECTOR3 pos);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void UpdateCollider(void);
	void Draw(void);
	void SetType(TYPE type) { m_Type = type; }
	TYPE GetType(void) const { return m_Type; }
	virtual D3DXCOLOR GetCol(void) const override;
	void SetSelected(bool flag) { m_bSelected = flag; }
	bool IsSelected(void) const { return m_bSelected; }
	int GetTextureIndex() const { return m_nIdxTexture; }// 画像表示の時に使う
	static const char* GetTexPathFromType(TYPE type);
	void CreatePhysics(const D3DXVECTOR3& pos, const D3DXVECTOR3& size);
	void ReleasePhysics(void);  // 破棄用
	bool IsStaticBlock(void) const;
	btRigidBody* GetRigidBody(void) const { return m_pRigidBody; }
	D3DXVECTOR3 GetColliderSize(void) const;
	void SetColliderSize(const D3DXVECTOR3& size);

private:
	char m_szPath[MAX_PATH];	// ファイルパス
	TYPE m_Type;				// 種類
	D3DXCOLOR m_col;			// アルファ値
	D3DXCOLOR m_baseCol;		// ベースのアルファ値
	bool m_bSelected;			// 選択フラグ
	int m_nIdxTexture;			// テクスチャインデックス
	btRigidBody* m_pRigidBody;	// 剛体へのポインタ
	btCollisionShape* m_pShape;	// 当たり判定の形へのポインタ
	CDebugProc3D* m_pDebug3D;	// 3Dデバッグ表示へのポインタ
	D3DXVECTOR3 m_prevSize;		// 前回のサイズ
	D3DXVECTOR3 m_colliderSize; // コライダーサイズ


};

#endif