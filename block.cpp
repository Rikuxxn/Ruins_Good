//=============================================================================
//
// ブロック処理 [block.cpp]
// Author : TANEKAWA RIKU
//
//=============================================================================
#include "block.h"

//=======================================
// コンストラクタ
//=======================================
CBlock::CBlock()
{
	// 値のクリア
	for (int nCnt = 0; nCnt < MAX_PATH; nCnt++)
	{
		m_szPath[nCnt] = NULL;					// ファイルパス
	}
}
//=======================================
// デストラクタ
//=======================================
CBlock::~CBlock()
{
	// なし
}
//=======================================
// 生成処理
//=======================================
CBlock* CBlock::Create(const char* pFilepath, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size)
{
	CBlock* pBlock;

	if (CObject::GetNumObject() < MAX_OBJECT - 1)
	{
		pBlock = new CBlock;

		pBlock->SetPos(pos);
		pBlock->SetRot(D3DXToRadian(rot));
		pBlock->SetSize(size);
		pBlock->SetPath(pFilepath);	// パス保存

		// 初期化処理
		pBlock->Init();

		return pBlock;
	}
	else
	{// エラー
		return NULL;
	}
}
//=======================================
// 初期化処理
//=======================================
HRESULT CBlock::Init(void)
{
	// オブジェクトXの初期化処理
	CObjectX::Init();

	return S_OK;
}
//=======================================
// 終了処理
//=======================================
void CBlock::Uninit(void)
{
	// オブジェクトXの終了処理
	CObjectX::Uninit();
}
//=======================================
// 更新処理
//=======================================
void CBlock::Update(void)
{
	// オブジェクトXの更新処理
	CObjectX::Update();
}
//=======================================
// 情報の更新処理
//=======================================
void CBlock::UpdateInfo(void)
{
	// 場所
	SetPosImgui(ImVec2(0.0f, 0.0f));

	// サイズ
	SetSizeImgui(ImVec2(180.0f, 200.0f));

	StartImgui(u8"BlockInfo", IMGUITYPE_DEFOULT);

	ImGui::Text("Editor Mode Active");
	ImGui::End();
}
//=======================================
// 描画処理
//=======================================
void CBlock::Draw(void)
{
	// オブジェクトXの描画処理
	CObjectX::Draw();
}
