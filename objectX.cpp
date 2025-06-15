//=============================================================================
//
// Xファイル処理 [objectX.cpp]
// Author : TANEKAWA RIKU
//
//=============================================================================
#include "objectX.h"
#include "renderer.h"
#include "manager.h"

//=======================================
// コンストラクタ
//=======================================
CObjectX::CObjectX()
{
	// 値のクリア
	m_nIdxTexture = 0;
	m_pos		  = INIT_VEC3;					// 位置
	m_rot		  = INIT_VEC3;					// 向き
	m_pMesh		  = NULL;						// メッシュへのポインタ
	m_pBuffMat	  = NULL;						// マテリアルへのポインタ
	m_dwNumMat	  = NULL;						// マテリアル数
	m_mtxWorld	  = {};							// ワールドマトリックス
	m_nType		  = 0;							// 種類
	m_pShadow	  = NULL;						// 影へのポインタ

	for (int nCnt = 0; nCnt < MAX_PATH; nCnt++)
	{
		m_szPath[nCnt] = NULL;					// ファイルパス
	}
}
//=======================================
// デストラクタ
//=======================================
CObjectX::~CObjectX()
{
	// なし
}
//=======================================
// 生成処理
//=======================================
CObjectX* CObjectX::Create(const char* pFilepath,D3DXVECTOR3 pos,D3DXVECTOR3 rot)
{
	CObjectX* pObjectX;

	if (CObject::GetNumObject() < MAX_OBJECT - 1)
	{
		pObjectX = new CObjectX;

		pObjectX->m_pos = pos;
		pObjectX->m_rot = D3DXToRadian(rot);
		pObjectX->SetPath(pFilepath);	// パス保存

		// 初期化処理
		pObjectX->Init();

		return pObjectX;
	}
	else
	{// エラー
		return NULL;
	}
}
//=======================================
// 初期化処理
//=======================================
HRESULT CObjectX::Init(void)
{
	// テクスチャの取得
	CTexture* pTexture = CManager::GetTexture();

	// デバイスの取得
	CRenderer* renderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = renderer->GetDevice();

	// Xファイルの読み込み
	D3DXLoadMeshFromX(m_szPath,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_dwNumMat,
		&m_pMesh);

	int nNumVtx;		// 頂点数
	DWORD sizeFVF;		// 頂点フォーマットのサイズ
	BYTE* pVtxBuff;		// 頂点バッファへのポインタ

	// メッシュが読み込まれているか確認
	if (m_pMesh == NULL)
	{
		MessageBox(NULL, "Xファイルの読み込みに失敗しました（メッシュがNULLです）", "エラー", MB_OK | MB_ICONERROR);
		return 0;
	}

	// 頂点数の取得
	nNumVtx = m_pMesh->GetNumVertices();

	// 頂点数が0なら、モデルが空とみなす
	if (nNumVtx == 0)
	{
		MessageBox(NULL, "Xファイルの読み込みに失敗しました（頂点数が0です）", "エラー", MB_OK | MB_ICONERROR);
		return 0;
	}
	else
	{// モデルあり

		// 頂点フォーマットの取得
		sizeFVF = D3DXGetFVFVertexSize(m_pMesh->GetFVF());

		// 頂点バッファのロック
		m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);


		// 最大値とかはいったんなし


		// 頂点バッファのアンロック
		m_pMesh->UnlockVertexBuffer();

		D3DXMATERIAL* pMat;// マテリアルへのポインタ

		// マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

		m_nIdxTexture = new int[m_dwNumMat];

		for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{// テクスチャファイルが存在する
				// テクスチャの登録
				m_nIdxTexture[nCntMat] = pTexture->RegisterX(pMat[nCntMat].pTextureFilename);
			}
			else
			{// テクスチャが存在しない
				m_nIdxTexture[nCntMat] = -1;
			}
		}
	}

	// 影の生成
	m_pShadow = CShadow::Create(m_pos, 200, 55.0f, 0.1f, 55.0f);

	return S_OK;
}
//=======================================
// 終了処理
//=======================================
void CObjectX::Uninit(void)
{
	// テクスチャインデックスの破棄
	if (m_nIdxTexture != NULL)
	{
		delete[] m_nIdxTexture;
		m_nIdxTexture = NULL;
	}

	//メッシュの破棄
	if (m_pMesh != NULL)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}

	//マテリアルの破棄
	if (m_pBuffMat != NULL)
	{
		m_pBuffMat->Release();
		m_pBuffMat = NULL;
	}

	// オブジェクトの破棄(自分自身)
	this->Release();
}
//=======================================
// 更新処理
//=======================================
void CObjectX::Update(void)
{
	if (m_pShadow != NULL)
	{
		D3DXVECTOR3 shadowPos = m_pos;
		shadowPos.y = 0.05f;

		// 影の位置設定
		m_pShadow->SetPosition(shadowPos);
	}
}
//=======================================
// 描画処理
//=======================================
void CObjectX::Draw(void)
{
	// テクスチャの取得
	CTexture* pTexture = CManager::GetTexture();

	// デバイスの取得
	CRenderer* renderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = renderer->GetDevice();

	// 計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	D3DMATERIAL9 matDef;	// 現在のマテリアル保存用

	D3DXMATERIAL* pMat;		// マテリアルデータへのポインタ

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスを設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 現在のマテリアルの取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		if (m_nIdxTexture[nCntMat] == -1)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, NULL);
		}
		else
		{
			// テクスチャの設定
			pDevice->SetTexture(0, pTexture->GetAddress(m_nIdxTexture[nCntMat]));
		}

		// モデル(パーツ)の描画
		m_pMesh->DrawSubset(nCntMat);
	}

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}
//=======================================
// 位置の取得
//=======================================
D3DXVECTOR3 CObjectX::GetPos(void)
{
	return m_pos;
}
//=======================================
// Xファイルパスの設定処理
//=======================================
void CObjectX::SetPath(const char* path)
{
	strcpy_s(m_szPath, path);
}
