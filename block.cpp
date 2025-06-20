//=============================================================================
//
// �u���b�N���� [block.cpp]
// Author : TANEKAWA RIKU
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "block.h"
#include "manager.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CBlock::CBlock()
{
	// �l�̃N���A
	for (int nCnt = 0; nCnt < MAX_PATH; nCnt++)
	{
		m_szPath[nCnt] = NULL;					// �t�@�C���p�X
	}
	m_col = INIT_XCOL;							// �F
	m_baseCol = INIT_XCOL;						// �x�[�X�̐F
	m_bSelected = false;
	m_nIdxTexture = 0;
}
//=============================================================================
// �f�X�g���N�^
//=============================================================================
CBlock::~CBlock()
{
	// �Ȃ�
}
//=============================================================================
// ��������
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
//=============================================================================
// �^�C�v�ł̐�������
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
		path = "data/MODELS/wall.x";
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
// ����������
//=============================================================================
HRESULT CBlock::Init(void)
{
	// �I�u�W�F�N�gX�̏���������
	CObjectX::Init();

	// �}�e���A���F���u���b�N�̐F�ɐݒ�
	m_col = GetMaterialColor();
	m_col = m_baseCol;              // ���݂̐F�ɂ���x����Ă���

	// �e�N�X�`�����蓖��
	CTexture* pTexture = CManager::GetTexture();
	m_nIdxTexture = pTexture->Register(GetTexPathFromType(m_Type));

	return S_OK;
}
//=============================================================================
// �I������
//=============================================================================
void CBlock::Uninit(void)
{
	// �I�u�W�F�N�gX�̏I������
	CObjectX::Uninit();
}
//=============================================================================
// �X�V����
//=============================================================================
void CBlock::Update(void)
{
	// �I�u�W�F�N�gX�̍X�V����
	CObjectX::Update();
}
//=============================================================================
// �`�揈��
//=============================================================================
void CBlock::Draw(void)
{
	// �I�u�W�F�N�gX�̕`�揈��
	CObjectX::Draw();
}
//=============================================================================
// �F�̎擾
//=============================================================================
D3DXCOLOR CBlock::GetCol(void) const
{
	if (m_bSelected)
	{
		// �Ԃ�����
		return D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.6f);
	}
	else
	{
		return D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); // ���␳
	}
}
//=============================================================================
// �摜�\���p�e�N�X�`���p�X�̎擾
//=============================================================================
const char* CBlock::GetTexPathFromType(TYPE type)
{
	switch (type)
	{
	case TYPE_WOODBOX: 
		return "data/TEXTURE/woodbox.png";
	case TYPE_WALL: 
		return "data/TEXTURE/wall.png";
	case TYPE_AXE: 
		return "data/TEXTURE/Axe.png";
	case TYPE_IKADA: 
		return "data/TEXTURE/ikada.png";
	case TYPE_ROCK:
		return "data/TEXTURE/rock.png";

	default: 
		return "";
	}
}
