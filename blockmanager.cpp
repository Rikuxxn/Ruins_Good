//=============================================================================
//
// �u���b�N�}�l�[�W���[���� [blockmanager.cpp]
// Author : TANEKAWA RIKU
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "blockmanager.h"
#include "json.hpp"
#include "FileDialogUtils.h"
#include "manager.h"

using json = nlohmann::json;

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
std::vector<CBlock*> CBlockManager::m_blocks;	// �u���b�N�̏��
int CBlockManager::m_selectedIdx = 0;			// �I�𒆂̃C���f�b�N�X

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CBlockManager::CBlockManager()
{
	// �l�̃N���A
	m_prevSelectedIdx = -1;
}
//=============================================================================
// �f�X�g���N�^
//=============================================================================
CBlockManager::~CBlockManager()
{
	// �Ȃ�
}
//=============================================================================
// ����������
//=============================================================================
void CBlockManager::Init(void)
{
	// ���I�z�����ɂ��� (�T�C�Y��0�ɂ���)
	m_blocks.clear();
}
//=============================================================================
// �I������
//=============================================================================
void CBlockManager::Uninit(void)
{
	// ���I�z�����ɂ��� (�T�C�Y��0�ɂ���)
	m_blocks.clear();
}
//=============================================================================
// ���̍X�V����
//=============================================================================
void CBlockManager::UpdateInfo(void)
{
	// ImGui�}�l�[�W���[�̎擾
	CImGuiManager* pImGuiManager = CManager::GetImGuiManager();

	// GUI�X�^�C���̎擾
	ImGuiStyle& style = ImGui::GetStyle();

	style.GrabRounding		= 10.0f;		// �X���C�_�[�̂܂݂��ۂ�
	style.ScrollbarRounding = 10.0f;		// �X�N���[���o�[�̊p
	style.ChildRounding		= 10.0f;		// �q�E�B���h�E�̊p
	style.WindowRounding	= 10.0f;		// �E�B���h�E�S�̂̊p

	// �ꏊ
	pImGuiManager->SetPosImgui(ImVec2(1900.0f, 20.0f));

	// �T�C�Y
	pImGuiManager->SetSizeImgui(ImVec2(400.0f, 500.0f));

	// �ŏ���GUI
	pImGuiManager->StartImgui(u8"BlockInfo", CImGuiManager::IMGUITYPE_DEFOULT);

	// �u���b�N���Ȃ��ꍇ
	if (m_blocks.empty())
	{
		ImGui::Text("No blocks placed yet.");
	}
	else
	{
		// �͈͊O�΍�
		if (m_selectedIdx >= (int)m_blocks.size())
		{
			m_selectedIdx = (int)m_blocks.size() - 1;
		}

		// �C���f�b�N�X�I��
		ImGui::SliderInt("Block Index", &m_selectedIdx, 0, (int)m_blocks.size() - 1);

		// �O��I��ł��u���b�N������
		if (m_prevSelectedIdx != -1 && m_prevSelectedIdx != m_selectedIdx)
		{
			m_blocks[m_prevSelectedIdx]->SetSelected(false);
		}

		// �Ώۃu���b�N�̎擾
		CBlock* selectedBlock = m_blocks[m_selectedIdx];

		if (selectedBlock)
		{
			// �I�𒆂̃u���b�N�̐F��ς���
			selectedBlock->SetSelected(true);

			D3DXVECTOR3 pos = selectedBlock->GetPos();	// �I�𒆂̃u���b�N�̈ʒu�̎擾
			D3DXVECTOR3 rot = selectedBlock->GetRot();	// �I�𒆂̃u���b�N�̌����̎擾
			D3DXVECTOR3 size = selectedBlock->GetSize();// �I�𒆂̃u���b�N�̃T�C�Y�̎擾

			// ���W�A�����p�x�Ɉꎞ�ϊ�
			D3DXVECTOR3 degRot = D3DXToDegree(rot);

			//*********************************************************************
			// POS �̒���
			//*********************************************************************

			ImGui::Dummy(ImVec2(0.0f, 10.0f)); // �󔒂��󂯂�

			// ���x��
			ImGui::Text("POS"); ImGui::SameLine(60); // ���x���̈ʒu������ƒ���

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
			// ROT �̒���
			//*********************************************************************

			ImGui::Dummy(ImVec2(0.0f, 10.0f)); // �󔒂��󂯂�

			// ���x��
			ImGui::Text("ROT"); ImGui::SameLine(60); // ���x���̈ʒu������ƒ���

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
			// SIZE �̒���
			//*********************************************************************

			ImGui::Dummy(ImVec2(0.0f, 10.0f)); // �󔒂��󂯂�

			// ���x��
			ImGui::Text("SIZE"); ImGui::SameLine(60); // ���x���̈ʒu������ƒ���

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

			// �p�x�����W�A���ɖ߂�
			rot = D3DXToRadian(degRot);

			// �ʒu�̐ݒ�
			selectedBlock->SetPos(pos);

			// �����̐ݒ�
			selectedBlock->SetRot(rot);

			// �T�C�Y�̐ݒ�
			selectedBlock->SetSize(size);

			// ��������폜�{�^����ǉ�
			ImGui::Dummy(ImVec2(0.0f, 10.0f));

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.1f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.3f, 0.3f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.6f, 0.0f, 0.0f, 1.0f));

			if (ImGui::Button("Delete"))
			{
				m_blocks[m_selectedIdx]->Uninit();
				m_blocks.erase(m_blocks.begin() + m_selectedIdx);

				// �I���C���f�b�N�X�𒲐�
				if (m_selectedIdx >= (int)m_blocks.size())
				{
					m_selectedIdx = (int)m_blocks.size() - 1;
				}

				m_prevSelectedIdx = -1;
			}

			ImGui::PopStyleColor(3);
		}

		// �Ō�ɕۑ�
		m_prevSelectedIdx = m_selectedIdx;
	}

	ImGui::Dummy(ImVec2(0.0f, 10.0f)); // �󔒂��󂯂�

	// �u���b�N�^�C�v�ꗗ
	if (ImGui::TreeNode("Block Types"))
	{
		for (int typeInt = 0; typeInt < (int)CBlock::TYPE_MAX; typeInt++)
		{
			CBlock::TYPE type = static_cast<CBlock::TYPE>(typeInt);

			// �e�N�X�`���擾
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

				// 4���Ƃɉ��s
				if (typeInt % 4 != 3)
				{
					ImGui::SameLine();
				}
			}
		}

		ImGui::NewLine(); // ���̍s��
		ImGui::TreePop(); // �c���[����
	}

	ImVec2 panelSize = ImVec2(400, 300); // �K���ȃT�C�Y

	// �h���b�v�ł���͈�
	ImGui::InvisibleButton("DropTargetZone", panelSize);

	// �h���b�v�󂯎��i���_�ɔz�u
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("BLOCK_TYPE"))
		{
			CBlock::TYPE droppedType = *(CBlock::TYPE*)payload->Data;

			// ���_�Ƀu���b�N�𐶐�
			D3DXVECTOR3 pos(0.0f, 0.0f, 0.0f);
			CBlockManager::CreateBlock(droppedType, pos);
		}
		ImGui::EndDragDropTarget();
	}

	ImGui::Dummy(ImVec2(0.0f, 10.0f)); // �󔒂��󂯂�

	if (ImGui::Button("Save"))
	{
		// �_�C�A���O���J���ăt�@�C���ɕۑ�
		std::string path = OpenWindowsSaveFileDialog();

		if (!path.empty())
		{
			// �f�[�^�̕ۑ�
			CBlockManager::SaveToJson(path.c_str());
		}
	}

	ImGui::SameLine();

	if (ImGui::Button("Load"))
	{
		// �_�C�A���O���J���ăt�@�C�����J��
		std::string path = OpenWindowsOpenFileDialog();

		if (!path.empty())
		{
			// �f�[�^�̓ǂݍ���
			CBlockManager::LoadFromJson(path.c_str());
		}
	}

	ImGui::End();
}
//=============================================================================
// ��������
//=============================================================================
CBlock* CBlockManager::CreateBlock(CBlock::TYPE type, D3DXVECTOR3 pos)
{
	const char* path = CBlockManager::GetFilePathFromType(type);

	CBlock* newBlock = CBlock::Create(path, pos, D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(1, 1, 1));

	if (newBlock)
	{
		newBlock->SetType(type); // �^�C�v�𖾎��I�ɐݒ�
		m_blocks.push_back(newBlock);
	}
	return newBlock;
}
//=============================================================================
// �^�C�v����t�@�C���p�X���擾
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
// �u���b�N���̕ۑ�����
//=============================================================================
void CBlockManager::SaveToJson(const char* filename)
{
	// JSON�I�u�W�F�N�g
	json j;

	// 1�Â�JSON��
	for (const auto& block : m_blocks)
	{
		// ���W�A�����p�x�Ɉꎞ�ϊ�
		D3DXVECTOR3 degRot = D3DXToDegree(block->GetRot());
		
		json b;
		b["type"] = block->GetType();												// �u���b�N�̃^�C�v
		b["pos"] = { block->GetPos().x, block->GetPos().y, block->GetPos().z };		// �ʒu
		b["rot"] = { degRot.x, degRot.y, degRot.z };								// ����
		b["size"] = { block->GetSize().x, block->GetSize().y, block->GetSize().z };	// �T�C�Y

		// �ǉ�
		j.push_back(b);
	}

	// �o�̓t�@�C���X�g���[��
	std::ofstream file(filename);

	if (file.is_open())
	{
		file << std::setw(4) << j;

		// �t�@�C�������
		file.close();
	}
}
//=============================================================================
// �u���b�N���̓ǂݍ��ݏ���
//=============================================================================
void CBlockManager::LoadFromJson(const char* filename)
{
	std::ifstream file(filename);

	if (!file.is_open())
	{// �J���Ȃ�����
		return;
	}

	json j;
	file >> j;

	// �t�@�C�������
	file.close();

	// �����̃u���b�N������
	for (auto block : m_blocks)
	{
		if (block != NULL)
		{
			// �u���b�N�̏I������
			block->Uninit();
		}
	}

	// ���I�z�����ɂ��� (�T�C�Y��0�ɂ���)
	m_blocks.clear();

	// �V���ɐ���
	for (const auto& b : j)
	{
		// �^�C�v�����擾���ė񋓌^�ɃL���X�g
		int typeInt = b["type"];
		CBlock::TYPE type = static_cast<CBlock::TYPE>(typeInt);

		D3DXVECTOR3 pos(b["pos"][0], b["pos"][1], b["pos"][2]);
		D3DXVECTOR3 degRot(b["rot"][0], b["rot"][1], b["rot"][2]);
		D3DXVECTOR3 rot = D3DXToRadian(degRot); // �x�����W�A���ɕϊ�
		D3DXVECTOR3 size(b["size"][0], b["size"][1], b["size"][2]);

		// �^�C�v����u���b�N����
		CBlock* block = CreateBlock(type, pos);

		if (block)
		{
			block->SetRot(rot);
			block->SetSize(size);
		}
	}
}
////=============================================================================
//// �u���b�N�̎擾
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
//// �u���b�N�̃T�C�Y�擾
////=============================================================================
//int CBlockManager::GetBlockCount(void)
//{
//	return (int)m_blocks.size();
//}
