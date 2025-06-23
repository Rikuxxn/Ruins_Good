//=============================================================================
//
// マネージャー処理 [manager.cpp]
// Author : TANEKAWA RIKU
//
//=============================================================================

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "manager.h"
#include "renderer.h"
#include "object2D.h"
#include "sound.h"
#include "object3D.h"
#include "objectX.h"
#include "objectBillboard.h"
#include "meshfield.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
CRenderer* CManager::m_pRenderer = NULL;
CObject2D* CManager::m_pObject2D = NULL;
CInputKeyboard* CManager::m_pInputKeyboard = NULL;
CInputJoypad* CManager::m_pInputJoypad = NULL;
CInputMouse* CManager::m_pInputMouse = NULL;
CSound* CManager::m_pSound = NULL;
CPlayer* CManager::m_pPlayer = NULL;
CEffect* CManager::m_pEffect = NULL;
CScore* CManager::m_pScore = NULL;
CTime* CManager::m_pTime = NULL;
CColon* CManager::m_pColon = NULL;
CPause* CManager::m_pPause = NULL;
CTexture* CManager::m_pTexture = NULL;
CCamera* CManager::m_pCamera = NULL;
CLight* CManager::m_pLight = NULL;
CModel* CManager::m_pModel = NULL;
CMotion* CManager::m_pMotion = NULL;
CObjectX* CManager::m_pObjectX = NULL;
CBlockManager* CManager::m_pBlockManager = NULL;
CBlock* CManager::m_pBlock = NULL;
CImGuiManager* CManager::m_pImGuiManager = NULL;

bool CManager::m_isPaused = false;

//=============================================================================
// コンストラクタ
//=============================================================================
CManager::CManager()
{
	// 値のクリア
	m_fps = 0;
	m_isPaused = false;
}
//=============================================================================
// デストラクタ
//=============================================================================
CManager::~CManager()
{

}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd)
{
	// レンダラーの生成
	m_pRenderer = new CRenderer;

	// レンダラーの初期化処理
	if (FAILED(m_pRenderer->Init(hWnd, TRUE)))
	{
		return -1;
	}

	// キーボードの生成
	m_pInputKeyboard = new CInputKeyboard;

	// キーボードの初期化処理
	if (FAILED(m_pInputKeyboard->Init(hInstance, hWnd)))
	{
		return -1;
	}

	// ジョイパッドの生成
	m_pInputJoypad = new CInputJoypad;

	// ジョイパッドの初期化処理
	if (FAILED(m_pInputJoypad->Init()))
	{
		return E_FAIL;
	}

	// マウスの生成
	m_pInputMouse = new CInputMouse;

	// マウスの初期化処理
	if (FAILED(m_pInputMouse->Init(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	// サウンドの生成
	m_pSound = new CSound;

	// サウンドの初期化処理
	if (FAILED(m_pSound->Init(hWnd)))
	{
		return E_FAIL;
	}

	// カメラの生成
	m_pCamera = new CCamera;

	// カメラの初期化処理
	m_pCamera->Init();

	// ライトの生成
	m_pLight = new CLight;

	// ライトの初期化処理
	m_pLight->Init();

	// テクスチャの生成
	m_pTexture = new CTexture;

	// テクスチャの読み込み
	m_pTexture->Load();

	// ブロックマネージャーの生成
	m_pBlockManager = new CBlockManager;

	// ブロックマネージャーの初期化
	m_pBlockManager->Init();

	// ライトの設定処理
	CLight::AddLight(D3DLIGHT_DIRECTIONAL, D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), D3DXVECTOR3(0.0f, 100.0f, 0.0f));
	//CLight::AddLight(D3DLIGHT_DIRECTIONAL, D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	CLight::AddLight(D3DLIGHT_DIRECTIONAL, D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f), D3DXVECTOR3(1.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	CLight::AddLight(D3DLIGHT_DIRECTIONAL, D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f), D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	CLight::AddLight(D3DLIGHT_DIRECTIONAL, D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	CLight::AddLight(D3DLIGHT_DIRECTIONAL, D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));


	//// メッシュフィールドの生成
	//CMeshfield::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1200.0f, 1200.0f);

	// プレイヤーの生成
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f, 0.0f, -300.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	////// ブロックの生成
	//m_pBlock = CBlockManager::CreateBlock(CBlock::TYPE_WOODBOX, D3DXVECTOR3(-300.0f, 110.0f, 0.0f));
	//m_pBlock = CBlockManager::CreateBlock("data/MODELS/test_block.x", D3DXVECTOR3(-100.0f, 110.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(2.0f, 1.0f, 3.0f));
	//m_pBlock = CBlockManager::CreateBlock("data/MODELS/woodbox_001.x", D3DXVECTOR3(200.0f, 18.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	//m_pBlock = CBlockManager::CreateBlock("data/MODELS/Axe_01.x", D3DXVECTOR3(200.0f, 18.0f, -400.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f));

	// JSONの読み込み
	m_pBlockManager->LoadFromJson("data/block_info.json");

	//// ビルボードの生成
	//CObjectBillboard::Create(CObjectBillboard::TYPE_ONE, D3DXVECTOR3(0.0f, 20.0f, -350.0f), 120.0f, 60.0f);

	//// スコアの生成
	//m_pScore = CScore::Create(920.0f, 10.0f, 42.0f, 58.0f);

	float fTimePosX = 760.0f;
	float fTimeWidth = 42.0f;
	float fTimeHeight = 58.0f;

	// タイムの生成
	m_pTime = CTime::Create(12, 15, fTimePosX, 10.0f, fTimeWidth, fTimeHeight);

	// コロンの生成
	m_pColon = CColon::Create(D3DXVECTOR3(fTimePosX + 2 * fTimeWidth, 10.0f, 0.0f), fTimeWidth / 2, fTimeHeight);

	// ポーズの生成
	m_pPause = CPause::Create(D3DXVECTOR3(490.0f, 340.0f, 0.0f), 400.0f, 90.0f);

	//m_pSound->Play(CSound::SOUND_LABEL_GAMEBGM);

	//// Bullet物理ワールドの生成
	//m_pBroadphase = new btDbvtBroadphase();
	//m_pCollisionConfiguration = new btDefaultCollisionConfiguration();
	//m_pDispatcher = new btCollisionDispatcher(m_pCollisionConfiguration);
	//m_pSolver = new btSequentialImpulseConstraintSolver();

	//m_pDynamicsWorld = new btDiscreteDynamicsWorld(m_pDispatcher, m_pBroadphase, m_pSolver, m_pCollisionConfiguration);

	//// 重力を設定
	//m_pDynamicsWorld->setGravity(btVector3(0, -9.8f, 0));

	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void CManager::Uninit(void)
{
	// すべてのオブジェクトの破棄
	CObject::ReleaseAll();

	// テクスチャの破棄
	if (m_pTexture != NULL)
	{
		// 全てのテクスチャの破棄
		m_pTexture->Unload();

		delete m_pTexture;
		m_pTexture = NULL;
	}

	// キーボードの終了処理
	m_pInputKeyboard->Uninit();

	// ジョイパッドの終了処理
	m_pInputJoypad->Uninit();

	// マウスの終了処理
	m_pInputMouse->Uninit();

	// サウンドの終了処理
	m_pSound->Uninit();

	//// Bullet Physicsの破棄
	//if (m_pDynamicsWorld)
	//{
	//	delete m_pDynamicsWorld;
	//	m_pDynamicsWorld = NULL;
	//}

	//if (m_pSolver)
	//{
	//	delete m_pSolver;
	//	m_pSolver = NULL;
	//}

	//if (m_pDispatcher)
	//{
	//	delete m_pDispatcher;
	//	m_pDispatcher = NULL;
	//}

	//if (m_pCollisionConfiguration)
	//{
	//	delete m_pCollisionConfiguration;
	//	m_pCollisionConfiguration = NULL;
	//}

	//if (m_pBroadphase)
	//{
	//	delete m_pBroadphase;
	//	m_pBroadphase = NULL;
	//}

	// ポーズの破棄
	if (m_pPause != NULL)
	{
		// ポーズの終了処理
		m_pPause->Uninit();

		delete m_pPause;
		m_pPause = NULL;
	}

	// キーボードの破棄
	if (m_pInputKeyboard != NULL)
	{
		// レンダラーの終了処理
		m_pInputKeyboard->Uninit();

		delete m_pInputKeyboard;
		m_pInputKeyboard = NULL;
	}

	// ジョイパッドの破棄
	if (m_pInputJoypad != NULL)
	{
		// ジョイパッドの終了処理
		m_pInputJoypad->Uninit();

		delete m_pInputJoypad;
		m_pInputJoypad = NULL;
	}

	// マウスの破棄
	if (m_pInputMouse != NULL)
	{
		// マウスの終了処理
		m_pInputMouse->Uninit();

		delete m_pInputMouse;
		m_pInputMouse = NULL;
	}

	// サウンドの破棄
	if (m_pSound != NULL)
	{
		// マウスの終了処理
		m_pSound->Uninit();

		delete m_pSound;
		m_pSound = NULL;
	}

	// カメラの破棄
	if (m_pCamera != NULL)
	{
		delete m_pCamera;
		m_pCamera = NULL;
	}

	// ライトの破棄
	if (m_pLight != NULL)
	{
		delete m_pLight;
		m_pLight = NULL;
	}

	// ブロックマネージャーの破棄
	if (m_pBlockManager != NULL)
	{
		m_pBlockManager->Uninit();

		delete m_pBlockManager;
		m_pBlockManager = NULL;
	}

	// レンダラーの破棄
	if (m_pRenderer != NULL)
	{
		// レンダラーの終了処理
		m_pRenderer->Uninit();

		delete m_pRenderer;
		m_pRenderer = NULL;
	}
}
//=============================================================================
// 更新処理
//=============================================================================
void CManager::Update(void)
{
	// キーボードの更新
	m_pInputKeyboard->Update();

	// PキーでポーズON/OFF
	if (m_pInputKeyboard->GetTrigger(DIK_P))
	{
		m_isPaused = !m_isPaused;
	}

	// ポーズ中はゲーム更新しない
	if (m_isPaused == true)
	{
		// ポーズの更新処理
		m_pPause->Update();

		return;
	}

	//m_pDynamicsWorld->stepSimulation(1/60);

	// ジョイパッドの更新
	m_pInputJoypad->Update();

	// マウスの更新
	m_pInputMouse->Update();

	// カメラの更新
	m_pCamera->Update();

	// ライトの更新
	m_pLight->Update();

	// ブロックマネージャーの情報の更新
	m_pBlockManager->UpdateInfo();

	// ブロックマネージャーのドラッグ処理の更新
	m_pBlockManager->UpdateDraggingBlock();

	// レンダラーの更新
	m_pRenderer->Update();
}
//=============================================================================
// 描画処理
//=============================================================================
void CManager::Draw(void)
{
	// レンダラーの描画
	m_pRenderer->Draw(m_fps);

	// ポーズ中だったら
	if (m_isPaused)
	{
		// ポーズの描画処理
		m_pPause->Draw();
	}
}
//=============================================================================
// レンダラーの取得
//=============================================================================
CRenderer* CManager::GetRenderer(void)
{
	return m_pRenderer;
}
//=============================================================================
// 2Dオブジェクトの取得
//=============================================================================
CObject2D* CManager::GetObject2D(void)
{
	return m_pObject2D;
}
//=============================================================================
// キーボードの取得
//=============================================================================
CInputKeyboard* CManager::GetInputKeyboard(void)
{
	return m_pInputKeyboard;
}
//=============================================================================
// ジョイパッドの取得
//=============================================================================
CInputJoypad* CManager::GetInputJoypad(void)
{
	return m_pInputJoypad;
}
//=============================================================================
// マウスの取得
//=============================================================================
CInputMouse* CManager::GetInputMouse(void)
{
	return m_pInputMouse;
}
//=============================================================================
// サウンドの取得
//=============================================================================
CSound* CManager::GetSound(void)
{
	return m_pSound;
}
//=============================================================================
// プレイヤーの取得
//=============================================================================
CPlayer* CManager::GetPlayer(void)
{
	return m_pPlayer;
}
//=============================================================================
// エフェクトの取得
//=============================================================================
CEffect* CManager::GetEffect(void)
{
	return m_pEffect;
}
//=============================================================================
// スコアの取得
//=============================================================================
CScore* CManager::GetScore(void)
{
	return m_pScore;
}
//=============================================================================
// タイムの取得
//=============================================================================
CTime* CManager::GetTime(void)
{
	return m_pTime;
}
//=============================================================================
// コロンの取得
//=============================================================================
CColon* CManager::GetColon(void)
{
	return m_pColon;
}
//=============================================================================
// ポーズの取得
//=============================================================================
CPause* CManager::GetPause(void)
{
	return m_pPause;
}
//=============================================================================
// テクスチャの取得
//=============================================================================
CTexture* CManager::GetTexture(void)
{
	return m_pTexture;
}
//=============================================================================
// カメラの取得
//=============================================================================
CCamera* CManager::GetCamera(void)
{
	return m_pCamera;
}
//=============================================================================
// ライトの取得
//=============================================================================
CLight* CManager::GetLight(void)
{
	return m_pLight;
}
//=============================================================================
// モデルの取得
//=============================================================================
CModel* CManager::GetModel(void)
{
	return m_pModel;
}
//=============================================================================
// モーションの取得
//=============================================================================
CMotion* CManager::GetMotion(void)
{
	return m_pMotion;
}
//=============================================================================
// Xファイルオブジェクトの取得
//=============================================================================
CObjectX* CManager::GetObjectX(void)
{
	return m_pObjectX;
}
//=============================================================================
// ブロックの取得
//=============================================================================
CBlock* CManager::GetBlock(void)
{
	return m_pBlock;
}
//=============================================================================
// ブロックマネージャーの取得
//=============================================================================
CBlockManager* CManager::GetBlockManager(void)
{
	return m_pBlockManager;
}
//=============================================================================
// ImGuiマネージャーの取得
//=============================================================================
CImGuiManager* CManager::GetImGuiManager(void)
{
	return m_pImGuiManager;
}
//=============================================================================
// ポーズ中かどうかの取得
//=============================================================================
bool CManager::GetisPaused(void)
{
	return m_isPaused;
}
