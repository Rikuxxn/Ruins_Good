//=============================================================================
//
// ブロック処理 [block.cpp]
// Author : TANEKAWA RIKU
//
//=============================================================================

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "block.h"
#include "manager.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CBlock::CBlock()
{
	// 値のクリア
	for (int nCnt = 0; nCnt < MAX_PATH; nCnt++)
	{
		m_szPath[nCnt] = NULL;					// ファイルパス
	}
	m_col = INIT_XCOL;							// 色
	m_baseCol = INIT_XCOL;						// ベースの色
	m_bSelected = false;
	m_nIdxTexture = 0;
}
//=============================================================================
// デストラクタ
//=============================================================================
CBlock::~CBlock()
{
	// なし
}
//=============================================================================
// 生成処理
//=============================================================================
CBlock* CBlock::Create(const char* pFilepath, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size)
{
	CBlock* pBlock;

	if (CObject::GetNumObject() < MAX_OBJECT - 1)
	{
		pBlock = new CBlock;

		pBlock->SetPos(pos);
		pBlock->SetRot(rot);
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
//=============================================================================
// タイプでの生成処理
//=============================================================================
CBlock* CBlock::CreateFromType(TYPE type, D3DXVECTOR3 pos)
{
	const char* path = "";
	D3DXVECTOR3 size = { 1.0f, 1.0f, 1.0f };
	D3DXVECTOR3 rot = { 0.0f, 0.0f, 0.0f };

	switch (type)
	{
	case TYPE_WOODBOX:
		path = "data/MODELS/woodbox_001.x";
		size = { 1.5f, 1.5f, 1.5f };
		break;

	case TYPE_WALL:
		path = "data/MODELS/wall_01.x";
		size = { 1.0f, 1.0f, 1.0f };
		break;

	case TYPE_WALL2:
		path = "data/MODELS/wall_02.x";
		size = { 1.0f, 1.0f, 1.0f };
		break;

	case TYPE_WALL3:
		path = "data/MODELS/wall_03.x";
		size = { 1.0f, 1.0f, 1.0f };
		break;

	case TYPE_WALL4:
		path = "data/MODELS/wall_04.x";
		size = { 1.0f, 1.0f, 1.0f };
		break;

	case TYPE_AXE:
		path = "data/MODELS/Axe_01.x";
		size = { 2.0f, 2.0f, 2.0f };
		break;

	case TYPE_IKADA:
		path = "data/MODELS/ikada.x";
		size = { 1.0f, 1.0f, 1.0f };
		break;

	case TYPE_ROCK:
		path = "data/MODELS/Rock_001.x";
		size = { 1.0f, 1.0f, 1.0f };
		break;

	case TYPE_DOOR:
		path = "data/MODELS/wall_door_01.x";
		size = { 1.0f, 1.0f, 1.0f };
		break;

	default:
		return NULL;
	}

	CBlock* block = CBlock::Create(path, pos, rot, size);
	if (block)
	{
		block->SetType(type);
	}
	return block;
}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CBlock::Init(void)
{
	// オブジェクトXの初期化処理
	CObjectX::Init();

	// マテリアル色をブロックの色に設定
	m_col = GetMaterialColor();
	m_col = m_baseCol;              // 現在の色にも一度入れておく

	// テクスチャ割り当て
	CTexture* pTexture = CManager::GetTexture();
	m_nIdxTexture = pTexture->Register(GetTexPathFromType(m_Type));

	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void CBlock::Uninit(void)
{
	// オブジェクトXの終了処理
	CObjectX::Uninit();
}
//=============================================================================
// 更新処理
//=============================================================================
void CBlock::Update(void)
{
	// オブジェクトXの更新処理
	CObjectX::Update();
}
//=============================================================================
// 描画処理
//=============================================================================
void CBlock::Draw(void)
{
	// オブジェクトXの描画処理
	CObjectX::Draw();
}
//=============================================================================
// 色の取得
//=============================================================================
D3DXCOLOR CBlock::GetCol(void) const
{
	if (m_bSelected)
	{
		// 赤くする
		return D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.6f);
	}
	else
	{
		return D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); // 無補正
	}
}
//=============================================================================
// 画像表示用テクスチャパスの取得
//=============================================================================
const char* CBlock::GetTexPathFromType(TYPE type)
{
	switch (type)
	{
	case TYPE_WOODBOX: 
		return "data/TEXTURE/woodbox.png";

	case TYPE_WALL: 
		return "data/TEXTURE/wall1.png";

	case TYPE_WALL2:
		return "data/TEXTURE/wall2.png";

	case TYPE_WALL3:
		return "data/TEXTURE/wall3.png";

	case TYPE_WALL4:
		return "data/TEXTURE/wall4.png";

	case TYPE_AXE: 
		return "data/TEXTURE/Axe.png";

	case TYPE_IKADA: 
		return "data/TEXTURE/ikada.png";

	case TYPE_ROCK:
		return "data/TEXTURE/rock.png";

	case TYPE_TORCH:
		return "data/TEXTURE/torch1.png";

	case TYPE_TORCH2:
		return "data/TEXTURE/torch2.png";

	case TYPE_FLOOR:
		return "data/TEXTURE/floor1.png";

	case TYPE_FLOOR2:
		return "data/TEXTURE/floor2.png";

	case TYPE_DOOR:
		return "data/TEXTURE/wall_door1.png";

	case TYPE_CEILING:
		return "data/TEXTURE/ceiling1.png";

	case TYPE_CEILING2:
		return "data/TEXTURE/ceiling2.png";

	case TYPE_DOOR_BODY:
		return "data/TEXTURE/door1.png";

	default: 
		return "";
	}
}
