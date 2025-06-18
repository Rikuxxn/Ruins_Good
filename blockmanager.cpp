//=============================================================================
//
// ブロックマネージャー処理 [blockmanager.cpp]
// Author : TANEKAWA RIKU
//
//=============================================================================

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "blockmanager.h"
#include "json.hpp"
#include "FileDialogUtils.h"

using json = nlohmann::json;

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
std::vector<CBlock*> CBlockManager::m_blocks;	// ブロックの情報
int CBlockManager::m_selectedIdx = 0;			// 選択中のインデックス


//=============================================================================
// コンストラクタ
//=============================================================================
CBlockManager::CBlockManager()
{
	// 値のクリア
}
//=============================================================================
// デストラクタ
//=============================================================================
CBlockManager::~CBlockManager()
{
	// なし
}
//=============================================================================
// 初期化処理
//=============================================================================
void CBlockManager::Init(void)
{
	// 動的配列を空にする (サイズを0にする)
	m_blocks.clear();
}
//=============================================================================
// 終了処理
//=============================================================================
void CBlockManager::Uninit(void)
{
	// 動的配列を空にする (サイズを0にする)
	m_blocks.clear();
}
//=============================================================================
// 情報の更新処理
//=============================================================================
void CBlockManager::UpdateInfo(void)
{
	if (m_blocks.empty())
	{
		return;
	}

	// GUIスタイルの取得
	ImGuiStyle& style = ImGui::GetStyle();

	style.GrabRounding		= 10.0f;		// スライダーのつまみを丸く
	style.ScrollbarRounding = 10.0f;		// スクロールバーの角
	style.ChildRounding		= 10.0f;		// 子ウィンドウの角
	style.WindowRounding	= 10.0f;		// ウィンドウ全体の角

	// 場所
	CImGuiManager::SetPosImgui(ImVec2(1900.0f, 20.0f));

	// サイズ
	CImGuiManager::SetSizeImgui(ImVec2(400.0f, 500.0f));

	// 最初のGUI
	CImGuiManager::StartImgui(u8"BlockInfo", CImGuiManager::IMGUITYPE_DEFOULT);

	// 範囲外対策
	if (m_selectedIdx >= (int)m_blocks.size())
	{
		m_selectedIdx = (int)m_blocks.size() - 1;
	}

	// インデックス選択
	ImGui::SliderInt("Block Index", &m_selectedIdx, 0, (int)m_blocks.size() - 1);

	// 前回選んでたブロックを解除
	if (m_prevSelectedIdx != -1 && m_prevSelectedIdx != m_selectedIdx)
	{
		m_blocks[m_prevSelectedIdx]->SetSelected(false);
	}

	// 対象ブロックの取得
	CBlock* selectedBlock = m_blocks[m_selectedIdx];

	if (selectedBlock)
	{
		// 選択中のブロックの色を変える
		selectedBlock->SetSelected(true);

		D3DXVECTOR3 pos = selectedBlock->GetPos();	// 選択中のブロックの位置の取得
		D3DXVECTOR3 rot = selectedBlock->GetRot();	// 選択中のブロックの向きの取得
		D3DXVECTOR3 size = selectedBlock->GetSize();// 選択中のブロックのサイズの取得

		// ラジアン→角度に一時変換
		D3DXVECTOR3 degRot = rot * (180.0f / D3DX_PI);

		//*********************************************************************
		// POS の調整
		//*********************************************************************

		ImGui::Dummy(ImVec2(0.0f, 10.0f)); // 空白を空ける

		// ラベル
		ImGui::Text("POS"); ImGui::SameLine(60); // ラベルの位置ちょっと調整

		// X
		ImGui::Text("X:"); ImGui::SameLine();
		ImGui::SetNextItemWidth(80);
		ImGui::DragFloat("##Block_pos_x", &pos.x, 1.0f, -9000.0f, 9000.0f, "%.1f");

		// Y
		ImGui::SameLine();
		ImGui::Text("Y:"); ImGui::SameLine();
		ImGui::SetNextItemWidth(80);
		ImGui::DragFloat("##Block_pos_y", &pos.y, 1.0f, -9000.0f, 9000.0f, "%.1f");

		// Z
		ImGui::SameLine();
		ImGui::Text("Z:"); ImGui::SameLine();
		ImGui::SetNextItemWidth(80);
		ImGui::DragFloat("##Block_pos_z", &pos.z, 1.0f, -9000.0f, 9000.0f, "%.1f");

		//*********************************************************************
		// ROT の調整
		//*********************************************************************

		ImGui::Dummy(ImVec2(0.0f, 10.0f)); // 空白を空ける

		// ラベル
		ImGui::Text("ROT"); ImGui::SameLine(60); // ラベルの位置ちょっと調整

		// X
		ImGui::Text("X:"); ImGui::SameLine();
		ImGui::SetNextItemWidth(80);
		ImGui::DragFloat("##Block_rot_x", &degRot.x, 0.1f, -180.0f, 180.0f, "%.2f");

		// Y
		ImGui::SameLine();
		ImGui::Text("Y:"); ImGui::SameLine();
		ImGui::SetNextItemWidth(80);
		ImGui::DragFloat("##Block_rot_y", &degRot.y, 0.1f, -180.0f, 180.0f, "%.2f");

		// Z
		ImGui::SameLine();
		ImGui::Text("Z:"); ImGui::SameLine();
		ImGui::SetNextItemWidth(80);
		ImGui::DragFloat("##Block_rot_z", &degRot.z, 0.1f, -180.0f, 180.0f, "%.2f");

		//*********************************************************************
		// SIZE の調整
		//*********************************************************************

		ImGui::Dummy(ImVec2(0.0f, 10.0f)); // 空白を空ける

		// ラベル
		ImGui::Text("SIZE"); ImGui::SameLine(60); // ラベルの位置ちょっと調整

		// X
		ImGui::Text("X:"); ImGui::SameLine();
		ImGui::SetNextItemWidth(80);
		ImGui::DragFloat("##Block_size_x", &size.x, 0.1f, -100.0f, 100.0f, "%.1f");

		// Y
		ImGui::SameLine();
		ImGui::Text("Y:"); ImGui::SameLine();
		ImGui::SetNextItemWidth(80);
		ImGui::DragFloat("##Block_size_y", &size.y, 0.1f, -100.0f, 100.0f, "%.1f");

		// Z
		ImGui::SameLine();
		ImGui::Text("Z:"); ImGui::SameLine();
		ImGui::SetNextItemWidth(80);
		ImGui::DragFloat("##Block_size_z", &size.z, 0.1f, -100.0f, 100.0f, "%.1f");

		// 角度→ラジアンに戻す
		rot = degRot * (D3DX_PI / 180.0f);

		// 位置の設定
		selectedBlock->SetPos(pos);

		// 向きの設定
		selectedBlock->SetRot(rot);

		// サイズの設定
		selectedBlock->SetSize(size);
	}

	// 最後に保存
	m_prevSelectedIdx = m_selectedIdx;

	ImGui::Dummy(ImVec2(0.0f, 10.0f)); // 空白を空ける

	if (ImGui::Button("Save"))
	{
		std::string path = OpenWindowsSaveFileDialog();
		if (!path.empty())
		{
			// データの保存
			CBlockManager::SaveToJson(path.c_str());
		}
	}

	ImGui::SameLine();

	if (ImGui::Button("Load"))
	{
		std::string path = OpenWindowsOpenFileDialog();
		if (!path.empty())
		{
			// データの読み込み
			CBlockManager::LoadFromJson(path.c_str());
		}
	}

	ImGui::End();
}
//=============================================================================
// 生成処理
//=============================================================================
CBlock* CBlockManager::CreateBlock(const char* filepath, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size)
{
	CBlock* newBlock = CBlock::Create(filepath, pos, rot, size);

	if (newBlock)
	{
		// 追加
		m_blocks.push_back(newBlock);
	}

	return newBlock;
}
//=============================================================================
// ブロック情報の保存処理
//=============================================================================
void CBlockManager::SaveToJson(const char* filename)
{
	// JSONオブジェクト
	json j;

	// 1つづつJSON化
	for (const auto& block : m_blocks)
	{
		json b;
		b["filepath"] = block->GetPath(); // モデルパス
		b["pos"] = { block->GetPos().x, block->GetPos().y, block->GetPos().z };
		b["rot"] = { block->GetRot().x, block->GetRot().y, block->GetRot().z };
		b["size"] = { block->GetSize().x, block->GetSize().y, block->GetSize().z };

		j.push_back(b);
	}

	std::ofstream file(filename);

	if (file.is_open())
	{
		file << std::setw(4) << j;

		// ファイルを閉じる
		file.close();
	}
}
//=============================================================================
// ブロック情報の読み込み処理
//=============================================================================
void CBlockManager::LoadFromJson(const char* filename)
{
	std::ifstream file(filename);

	if (!file.is_open())
	{// 開けなかった
		return;
	}

	json j;
	file >> j;

	// ファイルを閉じる
	file.close();

	// 既存のブロックを消す
	for (auto block : m_blocks)
	{
		if (block != NULL)
		{
			block->Uninit();
		}
	}
	m_blocks.clear();

	// 新たに生成
	for (const auto& b : j)
	{
		std::string path = b["filepath"];
		D3DXVECTOR3 pos(b["pos"][0], b["pos"][1], b["pos"][2]);
		D3DXVECTOR3 rot(b["rot"][0], b["rot"][1], b["rot"][2]);
		D3DXVECTOR3 size(b["size"][0], b["size"][1], b["size"][2]);

		// ブロックの生成
		CreateBlock(path.c_str(), pos, rot, size);
	}
}
//=============================================================================
// ブロックの取得
//=============================================================================
CBlock* CBlockManager::GetBlock(int index)
{
	if (index >= 0 && index < (int)m_blocks.size())
	{
		return m_blocks[index];
	}

	return NULL;
}
//=============================================================================
// ブロックのサイズ取得
//=============================================================================
int CBlockManager::GetBlockCount(void)
{
	return (int)m_blocks.size();
}
