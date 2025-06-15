//=============================================================================
//
// オブジェクト処理 [object.cpp]
// Author : TANEKAWA RIKU
//
//=============================================================================
#include "object.h"
#include "camera.h"
#include "manager.h"

// 静的メンバ変数宣言
CObject* CObject::m_apObject[MAX_PRIORITY_2D][MAX_OBJECT] = {};
int CObject::m_nNumAll = 0;

//=======================================
// コンストラクタ
//=======================================
CObject::CObject(int nPriority)
{
	m_nID	    = 0;
	m_nPriority = 0;
	m_type      = TYPE_NONE;

	for (int nCnt = 0; nCnt < MAX_OBJECT; nCnt++)
	{
		if (m_apObject[nPriority][nCnt] == NULL)
		{
			m_apObject[nPriority][nCnt] = this;	// 自分自身を代入
			m_nPriority = nPriority;
			m_nID = nCnt;				// 自分自身のIDを保存
			m_nNumAll++;				// 総数をカウントアップ
			break;
		}
	}
}
//=======================================
// デストラクタ
//=======================================
CObject::~CObject()
{
	// 今はなし

}
//=======================================
// すべてのオブジェクトの破棄
//=======================================
void CObject::ReleaseAll(void)
{
	for (int nPriority = 0; nPriority < MAX_PRIORITY_2D; nPriority++)
	{
		for (int nCnt = 0; nCnt < MAX_OBJECT; nCnt++)
		{
			if (m_apObject[nPriority][nCnt] != NULL)
			{
				// 終了処理
				m_apObject[nPriority][nCnt]->Uninit();
			}
		}
	}
}
//=======================================
// すべてのオブジェクトの更新処理
//=======================================
void CObject::UpdateAll(void)
{
	for (int nPriority = 0; nPriority < MAX_PRIORITY_2D; nPriority++)
	{
		for (int nCnt = 0; nCnt < MAX_OBJECT; nCnt++)
		{
			if (m_apObject[nPriority][nCnt] != NULL)
			{
				// 更新処理
				m_apObject[nPriority][nCnt]->Update();
			}
		}
	}
}
//=======================================
// すべてのオブジェクトの描画処理
//=======================================
void CObject::DrawAll(void)
{
	//カメラの取得
	CCamera* pCamera = CManager::GetCamera();

	// カメラの設定
	pCamera->SetCamera();

	for (int nPriority = 0; nPriority < MAX_PRIORITY_2D; nPriority++)
	{
		for (int nCnt = 0; nCnt < MAX_OBJECT; nCnt++)
		{
			if (m_apObject[nPriority][nCnt] != NULL)
			{
				// 描画処理
				m_apObject[nPriority][nCnt]->Draw();
			}
		}
	}
}
//=======================================
// オブジェクトの破棄
//=======================================
void CObject::Release(void)
{
	int nIdx = this->m_nID;
	int nPriority = this->m_nPriority;

	if (m_apObject[nPriority][nIdx] != NULL)
	{
		// オブジェクトの破棄(自分自身)
		delete m_apObject[nPriority][nIdx];
		m_apObject[nPriority][nIdx] = NULL;

		m_nNumAll--;	// 総数をカウントダウン
	}
}
//=======================================
// オブジェクトの総数の取得
//=======================================
int CObject::GetNumObject(void)
{
	return m_nNumAll;
}
//=======================================
// オブジェクトの取得
//=======================================
CObject* CObject::GetObject(int nPriority,int nIdx)
{
	return m_apObject[nPriority][nIdx];
}
//=======================================
// タイプの設定処理
//=======================================
void CObject::SetType(TYPE type)
{
	m_type = type;
}
//=======================================
// タイプの取得
//=======================================
CObject::TYPE CObject::GetType(void)
{
	return m_type;
}
