//=============================================================================
//
// プレイヤー処理 [player.h]
// Author : TANEKAWA RIKU
//
//=============================================================================
#ifndef _PLAYER_H_// このマクロ定義がされていなかったら
#define _PLAYER_H_// 2重インクルード防止のマクロ定義

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "object.h"
#include "model.h"
#include "shadow.h"
#include "motion.h"
#include "imguimaneger.h"
#include "debugproc3D.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_PARTS		(32)		// 最大パーツ数
#define PLAYER_SPEED	(1.20f)		// 移動スピード
#define MAX_JUMP_POWER	(6.3f)		// ジャンプ初速
#define MAX_GRAVITY		(-0.26f)	// 重力加速度

//*****************************************************************************
// プレイヤークラス
//*****************************************************************************
class CPlayer : public CObject
{
public:
	CPlayer();
	~CPlayer();

	static CPlayer* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void UpdateInfo(void);
	void Draw(void);
	D3DXVECTOR3 GetPos(void);
	D3DXVECTOR3 GetRot(void);
	bool GetPlayerUse(void);

private:
	D3DXVECTOR3 m_pos;					// 位置
	D3DXVECTOR3 m_rot;					// 向き
	D3DXVECTOR3 m_rotDest;				// 向き
	D3DXVECTOR3 m_move;					// 移動量
	D3DXVECTOR3 m_size;					// サイズ
	D3DXMATRIX m_mtxWorld;				// ワールドマトリックス
	CModel* m_apModel[MAX_PARTS];		// モデル(パーツ)へのポインタ
	int m_nNumModel;					// モデル(パーツ)の総数
	bool m_playerUse;					// 使われているかどうか
	CShadow* m_pShadow;					// 影へのポインタ
	CMotion* m_pMotion;					// モーションへのポインタ
	CMotion::TYPE m_currentMotion;		// 現在のモーション
	bool m_isJumping;					// ジャンプ中フラグ

	btRigidBody* m_pRigidBody;			// 剛体へのポインタ
	btCollisionShape* m_pShape;			// 当たり判定の形へのポインタ
	CDebugProc3D* m_pDebug3D;			// 3Dデバッグ表示へのポインタ
	btScalar m_radius;					// カプセルコライダーの半径
	btScalar m_height;					// カプセルコライダーの高さ

};

#endif
