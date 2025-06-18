//=============================================================================
//
// �}�l�[�W���[���� [manager.h]
// Author : TANEKAWA RIKU
//
//=============================================================================
#ifndef _MANAGER_H_// ���̃}�N����`������Ă��Ȃ�������
#define _MANAGER_H_// 2�d�C���N���[�h�h�~�̃}�N����`

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "renderer.h"
#include "object2D.h"
#include "input.h"
#include "sound.h"
#include "player.h"
#include "effect.h"
#include "score.h"
#include "time.h"
#include "pause.h"
#include "texture.h"
#include "camera.h"
#include "light.h"
#include "motion.h"
#include "block.h"
#include "blockmanager.h"


//*****************************************************************************
// �}�l�[�W���[�N���X
//*****************************************************************************
class CManager
{
public:
	CManager();
	~CManager();

	HRESULT Init(HINSTANCE HInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CRenderer* GetRenderer(void);
	static CObject2D* GetObject2D(void);
	static CInputKeyboard* GetInputKeyboard(void);
	static CInputJoypad* GetInputJoypad(void);
	static CInputMouse* GetInputMouse(void);
	static CSound* GetSound(void);
	static CPlayer* GetPlayer(void);
	static CEffect* GetEffect(void);
	static CScore* GetScore(void);
	static CTime* GetTime(void);
	static CColon* GetColon(void);
	static CPause* GetPause(void);
	static CTexture* GetTexture(void);
	static CCamera* GetCamera(void);
	static CLight* GetLight(void);
	static CModel* GetModel(void);
	static CMotion* GetMotion(void);
	static CObjectX* GetObjectX(void);
	static CBlock* GetBlock(void);
	static CBlockManager* GetBlockManager(void);

	int GetFPS(int fps) { return m_fps = fps; };
	int GetFPSCnt(void) { return m_fps; }
	static bool GetisPaused(void);

private:
	static CRenderer* m_pRenderer;				// �����_���[�ւ̃|�C���^
	static CObject2D* m_pObject2D;				// 2D�I�u�W�F�N�g�ւ̃|�C���^
	static CInputKeyboard* m_pInputKeyboard;	// �L�[�{�[�h�ւ̃|�C���^
	static CInputJoypad* m_pInputJoypad;		// �W���C�p�b�h�ւ̃|�C���^
	static CInputMouse* m_pInputMouse;			// �}�E�X�ւ̃|�C���^
	static CSound* m_pSound;					// �}�E�X�ւ̃|�C���^
	static CPlayer* m_pPlayer;					// �v���C���[�ւ̃|�C���^
	static CEffect* m_pEffect;					// �G�t�F�N�g�ւ̃|�C���^
	static CScore* m_pScore;					// �X�R�A�ւ̃|�C���^
	static CTime* m_pTime;						// �^�C���ւ̃|�C���^
	static CColon* m_pColon;					// �R�����ւ̃|�C���^
	static CPause* m_pPause;					// �|�[�Y�ւ̃|�C���^
	static CTexture* m_pTexture;				// �e�N�X�`���ւ̃|�C���^
	static CCamera* m_pCamera;					// �J�����ւ̃|�C���^
	static CLight* m_pLight;					// ���C�g�ւ̃|�C���^
	static CModel* m_pModel;					// ���f���ւ̃|�C���^
	static CMotion* m_pMotion;					// ���[�V�����ւ̃|�C���^
	static CBlock* m_pBlock;					// �u���b�N�ւ̃|�C���^
	static CObjectX* m_pObjectX;				// X�t�@�C���I�u�W�F�N�g�ւ̃|�C���^
	static CBlockManager* m_pBlockManager;		// �u���b�N�}�l�[�W���[�ւ̃|�C���^
	static std::vector<CBlock*> m_blocks;  // �����̃u���b�N��ێ�

	int m_fps;
	static bool m_isPaused; // true�Ȃ�|�[�Y��

	//btBroadphaseInterface* m_pBroadphase;						// �����܂��Փ˔���̃N���X�ւ̃|�C���^
	//btDefaultCollisionConfiguration* m_pCollisionConfiguration ;// �Փˌ��o�̐ݒ���Ǘ�����N���X�ւ̃|�C���^
	//btCollisionDispatcher* m_pDispatcher;						// ���ۂɏՓ˔��菈�������s����N���X�ւ̃|�C���^
	//btSequentialImpulseConstraintSolver* m_pSolver;				// �����V�~�����[�V�����̐���\���o�[�ւ̃|�C���^
	//btDiscreteDynamicsWorld* m_pDynamicsWorld;					// �������E���Ǘ�����N���X�ւ̃|�C���^
};

#endif