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
#include "debugproc3D.h"

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
    void UpdateTransform(CBlock* selectedBlock);
    void UpdateDraggingBlock(void);
    void UpdateColliderSize(CBlock* selectedBlock);
    void Draw(void);
    static CBlock* CreateBlock(CBlock::TYPE type, D3DXVECTOR3 pos);
    static const char* GetFilePathFromType(CBlock::TYPE type);
    void SaveToJson(const char* filename);
    void LoadFromJson(const char* filename);
    //CBlock* GetBlock(int index);
    //int GetBlockCount(void);

private:
    static std::vector<CBlock*> m_blocks;   // ブロック情報
    static CBlock* m_draggingBlock;         // ドラッグ中のブロック情報
    static int m_nNumAll;                   // 総数
    static int m_selectedIdx;               // 選択中のインデックス
    int m_prevSelectedIdx;
    bool m_hasConsumedPayload ;             // ペイロード生成済みフラグ
    CDebugProc3D* m_pDebug3D;			    // 3Dデバッグ表示へのポインタ

};

#endif
