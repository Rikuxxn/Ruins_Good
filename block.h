//=============================================================================
//
// �u���b�N���� [block.h]
// Author : TANEKAWA RIKU
//
//=============================================================================
#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "main.h"
#include "objectX.h"

// �u���b�N�N���X
class CBlock : public CObjectX
{
public:
	CBlock();
	~CBlock();

	static CBlock* Create(const char* pFilepath, D3DXVECTOR3 pos, D3DXVECTOR3 rot,D3DXVECTOR3 size);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	static void UpdateInfo(void);
	void Draw(void);

private:
	char m_szPath[MAX_PATH];			// �t�@�C���p�X
};

#endif