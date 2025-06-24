//=============================================================================
//
// 3D�f�o�b�O�\������ [debugproc3D.cpp]
// Author : TANEKAWA RIKU
//
//=============================================================================
#include "debugproc3D.h"
#include "renderer.h"
#include "manager.h"

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
LPD3DXLINE CDebugProc3D::m_pLine = NULL;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CDebugProc3D::CDebugProc3D()
{
	// �l�̃N���A

}
//=============================================================================
// �f�X�g���N�^
//=============================================================================
CDebugProc3D::~CDebugProc3D()
{
	// �Ȃ�
}
//=============================================================================
// ����������
//=============================================================================
void CDebugProc3D::Init(void)
{
	// �f�o�C�X�̎擾
	CRenderer* renderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = renderer->GetDevice();

	if (pDevice != NULL)
	{
		D3DXCreateLine(pDevice, &m_pLine);
	}
}
//=============================================================================
// �I������
//=============================================================================
void CDebugProc3D::Uninit(void)
{
	// ���C���\���̔j��
	if (m_pLine != NULL)
	{
		m_pLine->Release();
		m_pLine = NULL;
	}
}
//=============================================================================
// ���C���`�揈��
//=============================================================================
void CDebugProc3D::DrawLine3D(const D3DXVECTOR3& start, const D3DXVECTOR3& end, D3DXCOLOR color)
{
    // �f�o�C�X�̎擾
    CRenderer* renderer = CManager::GetRenderer();
    LPDIRECT3DDEVICE9 pDevice = renderer->GetDevice();

	struct VERTEX
	{
		D3DXVECTOR3 pos;
		D3DXCOLOR color;
	};

	VERTEX v[2] =
    {
		{ start, color },
		{ end,   color }
	};

	// ���_�t�H�[�}�b�g�ݒ�
	pDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);

	// ���C���`��
	pDevice->DrawPrimitiveUP(D3DPT_LINELIST, 1, v, sizeof(VERTEX));
}
//=============================================================================
// �J�v�Z���R���C�_�[�`�揈��
//=============================================================================
void CDebugProc3D::DrawCapsuleCollider(btCapsuleShape* capsule, const btTransform& transform, D3DXCOLOR color)
{
    if (!capsule || !m_pLine)
    {
        return;
    }

    // �f�o�C�X�̎擾
    CRenderer* renderer = CManager::GetRenderer();
    LPDIRECT3DDEVICE9 pDevice = renderer->GetDevice();

    const int kNumSegments = 16;
    float radius = capsule->getRadius();
    float halfHeight = capsule->getHalfHeight();

    // Capsule �̒��S�ʒu
    btVector3 base = transform.getOrigin();

    base += btVector3(0, halfHeight, 0);

    // �����ŁAbtTransform�̉�]��D3DXMATRIX�֕ϊ��i�X�����܂߂�����΂�������g���j
    btQuaternion q = transform.getRotation();

    D3DXMATRIX matRot;
    D3DXQUATERNION dq(q.x(), q.y(), q.z(), q.w());
    D3DXMatrixRotationQuaternion(&matRot, &dq);

    // ���s�ړ��s��
    D3DXMATRIX matTrans;
    D3DXMatrixTranslation(&matTrans, base.x(), base.y(), base.z());

    // ���[���h�s�� = ��] �~ ���s�ړ�
    D3DXMATRIX matWorld = matRot * matTrans;

    // �f�o�C�X�Ƀ��[���h�s���ݒ�
    pDevice->SetTransform(D3DTS_WORLD, &matWorld);

    // �J�v�Z��������Y���Œ�iBullet��btCapsuleShape��Up���w��ł��邪�A�����ł�Y���Ɖ���j
    btVector3 up(0, 1, 0);
    btVector3 side1(1, 0, 0);
    btVector3 side2(0, 0, 1);

    btVector3 top = up * halfHeight;
    btVector3 bottom = -up * halfHeight;

    for (int i = 0; i < kNumSegments; i++)
    {
        float theta1 = (2.0f * D3DX_PI * i) / kNumSegments;
        float theta2 = (2.0f * D3DX_PI * (i + 1)) / kNumSegments;

        btVector3 dir1 = cosf(theta1) * side1 + sinf(theta1) * side2;
        btVector3 dir2 = cosf(theta2) * side1 + sinf(theta2) * side2;

        dir1 *= radius;
        dir2 *= radius;

        // ��̉~
        DrawLine3D(D3DXVECTOR3((top + dir1).x(), (top + dir1).y(), (top + dir1).z()),
            D3DXVECTOR3((top + dir2).x(), (top + dir2).y(), (top + dir2).z()), color);

        // ���̉~
        DrawLine3D(D3DXVECTOR3((bottom + dir1).x(), (bottom + dir1).y(), (bottom + dir1).z()),
            D3DXVECTOR3((bottom + dir2).x(), (bottom + dir2).y(), (bottom + dir2).z()), color);

        // ���ʂ̐�
        DrawLine3D(D3DXVECTOR3((top + dir1).x(), (top + dir1).y(), (top + dir1).z()),
            D3DXVECTOR3((bottom + dir1).x(), (bottom + dir1).y(), (bottom + dir1).z()), color);
    }
}