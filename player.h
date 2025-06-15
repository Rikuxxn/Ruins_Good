//=============================================================================
//
// プレイヤー処理 [player.h]
// Author : TANEKAWA RIKU
//
//=============================================================================
#ifndef _PLAYER_H_// このマクロ定義がされていなかったら
#define _PLAYER_H_// 2重インクルード防止のマクロ定義

#include "main.h"
#include "object.h"
#include "model.h"
#include "shadow.h"
#include "motion.h"

#define MAX_PARTS (32)		// 最大パーツ数
#define PLAYER_SPEED (0.90f)// 移動スピード

// プレイヤークラス
class CPlayer : public CObject
{
public:
	CPlayer();
	~CPlayer();

	static CPlayer* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	D3DXVECTOR3 GetPos(void);
	D3DXVECTOR3 GetRot(void);
	bool GetPlayerUse(void);

private:
	D3DXVECTOR3 m_pos;					// 位置
	D3DXVECTOR3 m_rot;					// 向き
	D3DXVECTOR3 m_rotDest;				// 向き
	D3DXVECTOR3 m_move;					// 移動量
	D3DXMATRIX m_mtxWorld;				// ワールドマトリックス
	CModel* m_apModel[MAX_PARTS];		// モデル(パーツ)へのポインタ
	int m_nNumModel;					// モデル(パーツ)の総数
	bool m_playerUse;					// 使われているかどうか
	CShadow* m_pShadow;					// 影へのポインタ
	CMotion* m_pMotion;					// モーションへのポインタ
	CMotion::TYPE m_currentMotion;		// 現在のモーション
};

#endif
