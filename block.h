//=============================================================================
//
// �u���b�N���� [block.h]
// Author : TANEKAWA RIKU
//
//=============================================================================
#ifndef _BLOCK_H_
#define _BLOCK_H_

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "objectX.h"
#include "debugproc3D.h"

//*****************************************************************************
// �u���b�N�N���X
//*****************************************************************************
class CBlock : public CObjectX
{
public:
	CBlock();
	~CBlock();

	//*****************************************************************************
	// �u���b�N�̎��
	//*****************************************************************************
	typedef enum
	{
		TYPE_WOODBOX = 0,
		TYPE_WALL,
		TYPE_WALL2,
		TYPE_WALL3,
		TYPE_WALL4,
		TYPE_AXE,
		TYPE_IKADA,
		TYPE_ROCK,
		TYPE_TORCH,
		TYPE_TORCH2,
		TYPE_FLOOR,
		TYPE_FLOOR2,
		TYPE_DOOR,
		TYPE_CEILING,
		TYPE_CEILING2,
		TYPE_DOOR_BODY,
		TYPE_MAX
	}TYPE;

	static CBlock* Create(const char* pFilepath, D3DXVECTOR3 pos, D3DXVECTOR3 rot,D3DXVECTOR3 size);
	static CBlock* CreateFromType(TYPE type, D3DXVECTOR3 pos);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void UpdateCollider(void);
	void Draw(void);
	void SetType(TYPE type) { m_Type = type; }
	TYPE GetType(void) const { return m_Type; }
	virtual D3DXCOLOR GetCol(void) const override;
	void SetSelected(bool flag) { m_bSelected = flag; }
	bool IsSelected(void) const { return m_bSelected; }
	int GetTextureIndex() const { return m_nIdxTexture; }// �摜�\���̎��Ɏg��
	static const char* GetTexPathFromType(TYPE type);
	void CreatePhysics(const D3DXVECTOR3& pos, const D3DXVECTOR3& size);
	void ReleasePhysics(void);  // �j���p
	bool IsStaticBlock(void) const;
	btRigidBody* GetRigidBody(void) const { return m_pRigidBody; }
	D3DXVECTOR3 GetColliderSize(void) const;
	void SetColliderSize(const D3DXVECTOR3& size);

private:
	char m_szPath[MAX_PATH];	// �t�@�C���p�X
	TYPE m_Type;				// ���
	D3DXCOLOR m_col;			// �A���t�@�l
	D3DXCOLOR m_baseCol;		// �x�[�X�̃A���t�@�l
	bool m_bSelected;			// �I���t���O
	int m_nIdxTexture;			// �e�N�X�`���C���f�b�N�X
	btRigidBody* m_pRigidBody;	// ���̂ւ̃|�C���^
	btCollisionShape* m_pShape;	// �����蔻��̌`�ւ̃|�C���^
	CDebugProc3D* m_pDebug3D;	// 3D�f�o�b�O�\���ւ̃|�C���^
	D3DXVECTOR3 m_prevSize;		// �O��̃T�C�Y
	D3DXVECTOR3 m_colliderSize; // �R���C�_�[�T�C�Y


};

#endif