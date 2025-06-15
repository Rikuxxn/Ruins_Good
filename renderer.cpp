//=============================================================================
//
// レンダリング処理 [renderer.cpp]
// Author : TANEKAWA RIKU
//
//=============================================================================
#include "renderer.h"
#include "object.h"
#include "object2D.h"
#include "manager.h"

// 静的メンバ変数宣言
CDebugProc* CRenderer::m_pDebug = NULL;

//=======================================
// コンストラクタ
//=======================================
CRenderer::CRenderer()
{
	// 値のクリア
	m_pD3D       = NULL;
	m_pD3DDevice = NULL;
}
//=======================================
// デストラクタ
//=======================================
CRenderer::~CRenderer()
{
	// 今はなし

}
//=======================================
// 初期化処理
//=======================================
HRESULT CRenderer::Init(HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;			// ディスプレイモード
	D3DPRESENT_PARAMETERS d3dpp;	// プレゼンテーションパラメータ

	// DirectX3Dオブジェクトの生成
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (m_pD3D == NULL)
	{
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));							// パラメータのゼロクリア

	d3dpp.BackBufferWidth = SCREEN_WIDTH;						// ゲーム画面サイズ（幅）
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;						// ゲーム画面サイズ（高さ）
	d3dpp.BackBufferFormat = d3ddm.Format;						// バックバッファの形式
	d3dpp.BackBufferCount = 1;									// バックバッファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					// ダブルバッファの切り替え
	d3dpp.EnableAutoDepthStencil = TRUE;						// デプスバッファとステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					// デプスバッファとして16bitを使う
	d3dpp.Windowed = bWindow;									// ウインドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	// リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// インターバル

	// DirectX3Dデバイスの生成
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&m_pD3DDevice)))
	{
		// DirectX3Dデバイスの生成
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&m_pD3DDevice)))
		{
			//DirectX3Dデバイスの生成
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_HAL,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&m_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	// レンダーステートの設定
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// サンプラーステートの設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	// テクスチャステージステートの設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	if (m_pDebug == NULL)
	{
		// デバッグフォントの初期化
		m_pDebug->Init();
	}

	return S_OK;
}
//=======================================
// 終了処理
//=======================================
void CRenderer::Uninit(void)
{
	// デバッグフォントの終了処理
	m_pDebug->Uninit();

	// Direct3Dデバイスの破棄
	if (m_pD3DDevice != NULL)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = NULL;
	}

	// Direct3Dオブジェクトの破棄
	if (m_pD3D != NULL)
	{
		m_pD3D->Release();
		m_pD3D = NULL;
	}
}
//=======================================
// 更新処理
//=======================================
void CRenderer::Update(void)
{

	// すべてのオブジェクトの更新処理
	CObject::UpdateAll();

}
//=======================================
// 描画処理
//=======================================
void CRenderer::Draw(int fps)
{
	// 画面クリア
	m_pD3DDevice->Clear(0, NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 255), 1.0f, 0);

	// 描画開始
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{// 描画開始が成功した場合

		// すべてのオブジェクトの描画処理
		CObject::DrawAll();

//#ifdef _DEBUG

		CPlayer* pPlayer = CManager::GetPlayer();	// プレイヤーの取得
		CCamera* pCamera = CManager::GetCamera();	// カメラの取得

		// FPS値のデバッグ表示
		m_pDebug->Print("FPS:%d", fps);
		m_pDebug->Draw(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0, 0);

		if (pPlayer && pPlayer->GetPlayerUse() == true)
		{
			D3DXVECTOR3 Pos = pPlayer->GetPos();

			m_pDebug->Print("プレイヤーの位置 : (X %.1f,Y %.1f,Z %.1f)", Pos.x, Pos.y, Pos.z);
			m_pDebug->Draw(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0, 20);
		}
		else
		{
			m_pDebug->Print("プレイヤーなし（死亡 or 未生成）");
			m_pDebug->Draw(D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f), 0, 20);
		}

		// カメラの角度の取得
		D3DXVECTOR3 rot = pCamera->GetRot();

		// カメラのデバッグ表示
		m_pDebug->Print("カメラの角度 : (X:%.2f Y:%.2f)", rot.x,rot.y);
		m_pDebug->Draw(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0, 60);

		//// 操作のデバッグ表示
		//m_pDebug->Print("通常弾 [SPACE]");
		//m_pDebug->Draw(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0, 60);
		//m_pDebug->Print("ホーミング弾 [H]");
		//m_pDebug->Draw(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0, 80);
		//m_pDebug->Print("ポーズ [P]");
		//m_pDebug->Draw(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0, 100);
		//m_pDebug->Print("再配置 [R]");
		//m_pDebug->Draw(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0, 120);

		//m_pDebug->Print("敵の総数 : (%d)", CEnemy::GetNum());
		//m_pDebug->Draw(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 0, 140);
		//m_pDebug->Print("敵(1)の数 : (%d)", CEnemy::GetNumType1());
		//m_pDebug->Draw(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 0, 160);
		//m_pDebug->Print("敵(2)の数 : (%d)", CEnemy::GetNumType2());
		//m_pDebug->Draw(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 0, 180);
		//m_pDebug->Print("敵(3)の数 : (%d)", CEnemy::GetNumType3());
		//m_pDebug->Draw(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 0, 200);

//#endif
		// 描画終了
		m_pD3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}
//=======================================
// デバッグ取得
//=======================================
CDebugProc* CRenderer::GetDebug(void)
{
	return m_pDebug;
}
