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
    void UpdateDraggingBlock(void);

    static CBlock* CreateBlock(CBlock::TYPE type, D3DXVECTOR3 pos);
    static const char* GetFilePathFromType(CBlock::TYPE type);
    void SaveToJson(const char* filename);
    void LoadFromJson(const char* filename);
    //CBlock* GetBlock(int index);
    //int GetBlockCount(void);

private:
    static std::vector<CBlock*> m_blocks;   // �u���b�N���
    static CBlock* m_draggingBlock;         // �h���b�O���̃u���b�N���
    static int m_nNumAll;                   // ����
    static int m_selectedIdx;               // �I�𒆂̃C���f�b�N�X
    int m_prevSelectedIdx;
    bool m_hasConsumedPayload ;      // �y�C���[�h�����ς݃t���O
};

#endif
