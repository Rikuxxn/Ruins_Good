//=============================================================================
//
// ブロックマネージャー処理 [blockmanager.h]
// Author : TANEKAWA RIKU
//
//=============================================================================
#ifndef _BLOCKMANAGER_H_
#define _BLOCKMANAGER_H_

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "block.h"

//*****************************************************************************
// ブロックマネージャークラス
//*****************************************************************************
class CBlockManager
{
public:
	CBlockManager();
	~CBlockManager();

    void Init(void);
    void Uninit(void);
    void UpdateInfo(void); // ImGuiでの表示

    static CBlock* CreateBlock(const char* filepath, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size);
    void SaveToJson(const char* filename);
    void LoadFromJson(const char* filename);
    //CBlock* GetBlock(int index);
    //int GetBlockCount(void);

private:
    static std::vector<CBlock*> m_blocks;   // ブロック情報
    static int m_selectedIdx;               // 選択中のインデックス
    int m_prevSelectedIdx;
};

#endif
