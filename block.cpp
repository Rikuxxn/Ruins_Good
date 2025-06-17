//=============================================================================
//
// �u���b�N���� [block.cpp]
// Author : TANEKAWA RIKU
//
//=============================================================================
#include "block.h"

//=======================================
// �R���X�g���N�^
//=======================================
CBlock::CBlock()
{
	// �l�̃N���A
	for (int nCnt = 0; nCnt < MAX_PATH; nCnt++)
	{
		m_szPath[nCnt] = NULL;					// �t�@�C���p�X
	}
}
//=======================================
// �f�X�g���N�^
//=======================================
CBlock::~CBlock()
{
	// �Ȃ�
}
//=======================================
// ��������
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
		pBlock->SetPath(pFilepath);	// �p�X�ۑ�

		// ����������
		pBlock->Init();

		return pBlock;
	}
	else
	{// �G���[
		return NULL;
	}
}
//=======================================
// ����������
//=======================================
HRESULT CBlock::Init(void)
{
	// �I�u�W�F�N�gX�̏���������
	CObjectX::Init();

	return S_OK;
}
//=======================================
// �I������
//=======================================
void CBlock::Uninit(void)
{
	// �I�u�W�F�N�gX�̏I������
	CObjectX::Uninit();
}
//=======================================
// �X�V����
//=======================================
void CBlock::Update(void)
{
	// �I�u�W�F�N�gX�̍X�V����
	CObjectX::Update();
}
//=======================================
// ���̍X�V����
//=======================================
void CBlock::UpdateInfo(void)
{
	// �ꏊ
	SetPosImgui(ImVec2(0.0f, 0.0f));

	// �T�C�Y
	SetSizeImgui(ImVec2(180.0f, 200.0f));

	StartImgui(u8"BlockInfo", IMGUITYPE_DEFOULT);

	ImGui::Text("Editor Mode Active");
	ImGui::End();
}
//=======================================
// �`�揈��
//=======================================
void CBlock::Draw(void)
{
	// �I�u�W�F�N�gX�̕`�揈��
	CObjectX::Draw();
}
