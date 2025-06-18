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
	if (m_blocks.empty())
	{
		return;
	}

	// GUI�X�^�C���̎擾
	ImGuiStyle& style = ImGui::GetStyle();

	style.GrabRounding		= 10.0f;		// �X���C�_�[�̂܂݂��ۂ�
	style.ScrollbarRounding = 10.0f;		// �X�N���[���o�[�̊p
	style.ChildRounding		= 10.0f;		// �q�E�B���h�E�̊p
	style.WindowRounding	= 10.0f;		// �E�B���h�E�S�̂̊p

	// �ꏊ
	CImGuiManager::SetPosImgui(ImVec2(1900.0f, 20.0f));

	// �T�C�Y
	CImGuiManager::SetSizeImgui(ImVec2(400.0f, 500.0f));

	// �ŏ���GUI
	CImGuiManager::StartImgui(u8"BlockInfo", CImGuiManager::IMGUITYPE_DEFOULT);

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
		D3DXVECTOR3 degRot = rot * (180.0f / D3DX_PI);

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
		rot = degRot * (D3DX_PI / 180.0f);

		// �ʒu�̐ݒ�
		selectedBlock->SetPos(pos);

		// �����̐ݒ�
		selectedBlock->SetRot(rot);

		// �T�C�Y�̐ݒ�
		selectedBlock->SetSize(size);
	}

	// �Ō�ɕۑ�
	m_prevSelectedIdx = m_selectedIdx;

	ImGui::Dummy(ImVec2(0.0f, 10.0f)); // �󔒂��󂯂�

	if (ImGui::Button("Save"))
	{
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
CBlock* CBlockManager::CreateBlock(const char* filepath, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size)
{
	CBlock* newBlock = CBlock::Create(filepath, pos, rot, size);

	if (newBlock)
	{
		// �ǉ�
		m_blocks.push_back(newBlock);
	}

	return newBlock;
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
		json b;
		b["filepath"] = block->GetPath(); // ���f���p�X
		b["pos"] = { block->GetPos().x, block->GetPos().y, block->GetPos().z };
		b["rot"] = { block->GetRot().x, block->GetRot().y, block->GetRot().z };
		b["size"] = { block->GetSize().x, block->GetSize().y, block->GetSize().z };

		j.push_back(b);
	}

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
			block->Uninit();
		}
	}
	m_blocks.clear();

	// �V���ɐ���
	for (const auto& b : j)
	{
		std::string path = b["filepath"];
		D3DXVECTOR3 pos(b["pos"][0], b["pos"][1], b["pos"][2]);
		D3DXVECTOR3 rot(b["rot"][0], b["rot"][1], b["rot"][2]);
		D3DXVECTOR3 size(b["size"][0], b["size"][1], b["size"][2]);

		// �u���b�N�̐���
		CreateBlock(path.c_str(), pos, rot, size);
	}
}
//=============================================================================
// �u���b�N�̎擾
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
// �u���b�N�̃T�C�Y�擾
//=============================================================================
int CBlockManager::GetBlockCount(void)
{
	return (int)m_blocks.size();
}
