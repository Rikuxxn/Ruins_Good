//=============================================================================
//
// �u���b�N�}�l�[�W���[���� [blockmanager.h]
// Author : TANEKAWA RIKU
//
//=============================================================================
#ifndef _BLOCKMANAGER_H_
#define _BLOCKMANAGER_H_

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "block.h"

//*****************************************************************************
// �u���b�N�}�l�[�W���[�N���X
//*****************************************************************************
class CBlockManager
{
public:
	CBlockManager();
	~CBlockManager();

    void Init(void);
    void Uninit(void);
    void UpdateInfo(void); // ImGui�ł̕\��

    static CBlock* CreateBlock(const char* filepath, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size);
    void SaveToJson(const char* filename);
    void LoadFromJson(const char* filename);
    //CBlock* GetBlock(int index);
    //int GetBlockCount(void);

private:
    static std::vector<CBlock*> m_blocks;   // �u���b�N���
    static int m_selectedIdx;               // �I�𒆂̃C���f�b�N�X
    int m_prevSelectedIdx;
};

#endif
