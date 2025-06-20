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
#include "manager.h"

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
	m_prevSelectedIdx = -1;
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
	// ImGuiマネージャーの取得
	CImGuiManager* pImGuiManager = CManager::GetImGuiManager();

	// GUIスタイルの取得
	ImGuiStyle& style = ImGui::GetStyle();

	style.GrabRounding		= 10.0f;		// スライダーのつまみを丸く
	style.ScrollbarRounding = 10.0f;		// スクロールバーの角
	style.ChildRounding		= 10.0f;		// 子ウィンドウの角
	style.WindowRounding	= 10.0f;		// ウィンドウ全体の角

	// 場所
	pImGuiManager->SetPosImgui(ImVec2(1900.0f, 20.0f));

	// サイズ
	pImGuiManager->SetSizeImgui(ImVec2(400.0f, 500.0f));

	// 最初のGUI
	pImGuiManager->StartImgui(u8"BlockInfo", CImGuiManager::IMGUITYPE_DEFOULT);

	// ブロックがない場合
	if (m_blocks.empty())
	{
		ImGui::Text("No blocks placed yet.");
	}
	else
	{
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
			D3DXVECTOR3 degRot = D3DXToDegree(rot);

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
			rot = D3DXToRadian(degRot);

			// 位置の設定
			selectedBlock->SetPos(pos);

			// 向きの設定
			selectedBlock->SetRot(rot);

			// サイズの設定
			selectedBlock->SetSize(size);

			// ここから削除ボタンを追加
			ImGui::Dummy(ImVec2(0.0f, 10.0f));

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.1f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.3f, 0.3f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.6f, 0.0f, 0.0f, 1.0f));

			if (ImGui::Button("Delete"))
			{
				m_blocks[m_selectedIdx]->Uninit();
				m_blocks.erase(m_blocks.begin() + m_selectedIdx);

				// 選択インデックスを調整
				if (m_selectedIdx >= (int)m_blocks.size())
				{
					m_selectedIdx = (int)m_blocks.size() - 1;
				}

				m_prevSelectedIdx = -1;
			}

			ImGui::PopStyleColor(3);
		}

		// 最後に保存
		m_prevSelectedIdx = m_selectedIdx;
	}

	ImGui::Dummy(ImVec2(0.0f, 10.0f)); // 空白を空ける

	// ブロックタイプ一覧
	if (ImGui::TreeNode("Block Types"))
	{
		for (int typeInt = 0; typeInt < (int)CBlock::TYPE_MAX; typeInt++)
		{
			CBlock::TYPE type = static_cast<CBlock::TYPE>(typeInt);

			// テクスチャ取得
			const char* texPath = CBlock::GetTexPathFromType(type);
			int texIdx = CManager::GetTexture()->Register(texPath);

			LPDIRECT3DTEXTURE9 tex = CManager::GetTexture()->GetAddress(texIdx);

			if (tex)
			{
				ImGui::PushID(typeInt);
				ImGui::Image((ImTextureID)tex, ImVec2(74, 74));

				if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
				{
					ImGui::SetDragDropPayload("BLOCK_TYPE", &type, sizeof(type));
					ImGui::Text("Block Type %d", typeInt);
					ImGui::Image((ImTextureID)tex, ImVec2(64, 64));
					ImGui::EndDragDropSource();
				}

				ImGui::PopID();

				// 4つごとに改行
				if (typeInt % 4 != 3)
				{
					ImGui::SameLine();
				}
			}
		}

		ImGui::NewLine(); // 次の行へ
		ImGui::TreePop(); // ツリー閉じる
	}

	ImVec2 panelSize = ImVec2(400, 300); // 適当なサイズ

	// ドロップできる範囲
	ImGui::InvisibleButton("DropTargetZone", panelSize);

	// ドロップ受け取り（原点に配置
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("BLOCK_TYPE"))
		{
			CBlock::TYPE droppedType = *(CBlock::TYPE*)payload->Data;

			// 原点にブロックを生成
			D3DXVECTOR3 pos(0.0f, 0.0f, 0.0f);
			CBlockManager::CreateBlock(droppedType, pos);
		}
		ImGui::EndDragDropTarget();
	}

	ImGui::Dummy(ImVec2(0.0f, 10.0f)); // 空白を空ける

	if (ImGui::Button("Save"))
	{
		// ダイアログを開いてファイルに保存
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
		// ダイアログを開いてファイルを開く
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
CBlock* CBlockManager::CreateBlock(CBlock::TYPE type, D3DXVECTOR3 pos)
{
	const char* path = CBlockManager::GetFilePathFromType(type);

	CBlock* newBlock = CBlock::Create(path, pos, D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(1, 1, 1));

	if (newBlock)
	{
		newBlock->SetType(type); // タイプを明示的に設定
		m_blocks.push_back(newBlock);
	}
	return newBlock;
}
//=============================================================================
// タイプからファイルパスを取得
//=============================================================================
const char* CBlockManager::GetFilePathFromType(CBlock::TYPE type)
{
	switch (type)
	{
	case CBlock::TYPE_WOODBOX:
		return "data/MODELS/woodbox_001.x";
	case CBlock::TYPE_WALL:
		return "data/MODELS/wall_001.x";
	case CBlock::TYPE_AXE:
		return "data/MODELS/Axe_01.x";
	case CBlock::TYPE_IKADA:
		return "data/MODELS/ikada.x";
	case CBlock::TYPE_ROCK:
		return "data/MODELS/Rock_001.x";

	default: 
		return "";
	}
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
		// ラジアン→角度に一時変換
		D3DXVECTOR3 degRot = D3DXToDegree(block->GetRot());
		
		json b;
		b["type"] = block->GetType();												// ブロックのタイプ
		b["pos"] = { block->GetPos().x, block->GetPos().y, block->GetPos().z };		// 位置
		b["rot"] = { degRot.x, degRot.y, degRot.z };								// 向き
		b["size"] = { block->GetSize().x, block->GetSize().y, block->GetSize().z };	// サイズ

		// 追加
		j.push_back(b);
	}

	// 出力ファイルストリーム
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
			// ブロックの終了処理
			block->Uninit();
		}
	}

	// 動的配列を空にする (サイズを0にする)
	m_blocks.clear();

	// 新たに生成
	for (const auto& b : j)
	{
		// タイプ情報を取得して列挙型にキャスト
		int typeInt = b["type"];
		CBlock::TYPE type = static_cast<CBlock::TYPE>(typeInt);

		D3DXVECTOR3 pos(b["pos"][0], b["pos"][1], b["pos"][2]);
		D3DXVECTOR3 degRot(b["rot"][0], b["rot"][1], b["rot"][2]);
		D3DXVECTOR3 rot = D3DXToRadian(degRot); // 度→ラジアンに変換
		D3DXVECTOR3 size(b["size"][0], b["size"][1], b["size"][2]);

		// タイプからブロック生成
		CBlock* block = CreateBlock(type, pos);

		if (block)
		{
			block->SetRot(rot);
			block->SetSize(size);
		}
	}
}
////=============================================================================
//// ブロックの取得
////=============================================================================
//CBlock* CBlockManager::GetBlock(int index)
//{
//	if (index >= 0 && index < (int)m_blocks.size())
//	{
//		return m_blocks[index];
//	}
//
//	return NULL;
//}
////=============================================================================
//// ブロックのサイズ取得
////=============================================================================
//int CBlockManager::GetBlockCount(void)
//{
//	return (int)m_blocks.size();
//}
