//=============================================================================
//
// マネージャー処理 [manager.h]
// Author : TANEKAWA RIKU
//
//=============================================================================
#ifndef _MANAGER_H_// このマクロ定義がされていなかったら
#define _MANAGER_H_// 2重インクルード防止のマクロ定義

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "main.h"
#include "renderer.h"
#include "object2D.h"
#include "input.h"
#include "sound.h"
#include "player.h"
#include "effect.h"
#include "score.h"
#include "time.h"
#include "pause.h"
#include "texture.h"
#include "camera.h"
#include "light.h"
#include "motion.h"
#include "block.h"
#include "blockmanager.h"


//*****************************************************************************
// マネージャークラス
//*****************************************************************************
class CManager
{
public:
	CManager();
	~CManager();

	HRESULT Init(HINSTANCE HInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CRenderer* GetRenderer(void);
	static CObject2D* GetObject2D(void);
	static CInputKeyboard* GetInputKeyboard(void);
	static CInputJoypad* GetInputJoypad(void);
	static CInputMouse* GetInputMouse(void);
	static CSound* GetSound(void);
	static CPlayer* GetPlayer(void);
	static CEffect* GetEffect(void);
	static CScore* GetScore(void);
	static CTime* GetTime(void);
	static CColon* GetColon(void);
	static CPause* GetPause(void);
	static CTexture* GetTexture(void);
	static CCamera* GetCamera(void);
	static CLight* GetLight(void);
	static CModel* GetModel(void);
	static CMotion* GetMotion(void);
	static CObjectX* GetObjectX(void);
	static CBlock* GetBlock(void);
	static CBlockManager* GetBlockManager(void);

	int GetFPS(int fps) { return m_fps = fps; };
	int GetFPSCnt(void) { return m_fps; }
	static bool GetisPaused(void);

private:
	static CRenderer* m_pRenderer;				// レンダラーへのポインタ
	static CObject2D* m_pObject2D;				// 2Dオブジェクトへのポインタ
	static CInputKeyboard* m_pInputKeyboard;	// キーボードへのポインタ
	static CInputJoypad* m_pInputJoypad;		// ジョイパッドへのポインタ
	static CInputMouse* m_pInputMouse;			// マウスへのポインタ
	static CSound* m_pSound;					// マウスへのポインタ
	static CPlayer* m_pPlayer;					// プレイヤーへのポインタ
	static CEffect* m_pEffect;					// エフェクトへのポインタ
	static CScore* m_pScore;					// スコアへのポインタ
	static CTime* m_pTime;						// タイムへのポインタ
	static CColon* m_pColon;					// コロンへのポインタ
	static CPause* m_pPause;					// ポーズへのポインタ
	static CTexture* m_pTexture;				// テクスチャへのポインタ
	static CCamera* m_pCamera;					// カメラへのポインタ
	static CLight* m_pLight;					// ライトへのポインタ
	static CModel* m_pModel;					// モデルへのポインタ
	static CMotion* m_pMotion;					// モーションへのポインタ
	static CBlock* m_pBlock;					// ブロックへのポインタ
	static CObjectX* m_pObjectX;				// Xファイルオブジェクトへのポインタ
	static CBlockManager* m_pBlockManager;		// ブロックマネージャーへのポインタ
	static std::vector<CBlock*> m_blocks;  // 複数のブロックを保持

	int m_fps;
	static bool m_isPaused; // trueならポーズ中

	//btBroadphaseInterface* m_pBroadphase;						// おおまか衝突判定のクラスへのポインタ
	//btDefaultCollisionConfiguration* m_pCollisionConfiguration ;// 衝突検出の設定を管理するクラスへのポインタ
	//btCollisionDispatcher* m_pDispatcher;						// 実際に衝突判定処理を実行するクラスへのポインタ
	//btSequentialImpulseConstraintSolver* m_pSolver;				// 物理シミュレーションの制約ソルバーへのポインタ
	//btDiscreteDynamicsWorld* m_pDynamicsWorld;					// 物理世界を管理するクラスへのポインタ
};

#endif