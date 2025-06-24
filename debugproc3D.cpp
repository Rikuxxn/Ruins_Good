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

    const int kNumSegments = 12;  // ������
    const int kNumRings = 4;      // �c����

    float radius = capsule->getRadius();
    float halfHeight = capsule->getHalfHeight();

    // Capsule �̒��S�ʒu
    btVector3 base = transform.getOrigin();

    base += btVector3(0, halfHeight + 20.0f, 0);

    // btTransform�̉�]��D3DXMATRIX�֕ϊ�
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

    // �J�v�Z��������Y���Œ�
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

        // ���ʂ̐�
        DrawLine3D(D3DXVECTOR3((top + dir1).x(), (top + dir1).y(), (top + dir1).z()),
            D3DXVECTOR3((bottom + dir1).x(), (bottom + dir1).y(), (bottom + dir1).z()), color);
    }

    // --- �㔼����`�� ---
    for (int ring = 0; ring < kNumRings; ring++)
    {
        float theta1 = (D3DX_PI / 2) * (ring / (float)kNumRings);       // 0 �` ��/2
        float theta2 = (D3DX_PI / 2) * ((ring + 1) / (float)kNumRings);

        for (int seg = 0; seg < kNumSegments; seg++)
        {
            float phi1 = (2 * D3DX_PI * seg) / kNumSegments;
            float phi2 = (2 * D3DX_PI * (seg + 1)) / kNumSegments;

            // ���ʍ��W �� 3D�ʒu (���a1�̒P�ʋ��x�[�X)
            btVector3 p1 = btVector3(
                cosf(phi1) * sinf(theta1),
                cosf(theta1),
                sinf(phi1) * sinf(theta1)
            );
            btVector3 p2 = btVector3(
                cosf(phi2) * sinf(theta1),
                cosf(theta1),
                sinf(phi2) * sinf(theta1)
            );
            btVector3 p3 = btVector3(
                cosf(phi2) * sinf(theta2),
                cosf(theta2),
                sinf(phi2) * sinf(theta2)
            );
            btVector3 p4 = btVector3(
                cosf(phi1) * sinf(theta2),
                cosf(theta2),
                sinf(phi1) * sinf(theta2)
            );

            // �����Ȃ̂ŁAY������up���� �� top�ɃI�t�Z�b�g�A���a�X�P�[��
            p1 = top + p1 * radius;
            p2 = top + p2 * radius;
            p3 = top + p3 * radius;
            p4 = top + p4 * radius;

            // 4�_�Ŏl�p�`���\�� �� 2�{�̐��Ń��C���[�t���[����
            DrawLine3D(D3DXVECTOR3(p1.x(), p1.y(), p1.z()), D3DXVECTOR3(p2.x(), p2.y(), p2.z()), color);
            DrawLine3D(D3DXVECTOR3(p2.x(), p2.y(), p2.z()), D3DXVECTOR3(p3.x(), p3.y(), p3.z()), color);
            DrawLine3D(D3DXVECTOR3(p3.x(), p3.y(), p3.z()), D3DXVECTOR3(p4.x(), p4.y(), p4.z()), color);
            DrawLine3D(D3DXVECTOR3(p4.x(), p4.y(), p4.z()), D3DXVECTOR3(p1.x(), p1.y(), p1.z()), color);
        }
    }

    // --- ��������`�� ---
    for (int ring = 0; ring < kNumRings; ring++)
    {
        float theta1 = (D3DX_PI / 2) * (ring / (float)kNumRings);
        float theta2 = (D3DX_PI / 2) * ((ring + 1) / (float)kNumRings);

        for (int seg = 0; seg < kNumSegments; seg++)
        {
            float phi1 = (2 * D3DX_PI * seg) / kNumSegments;
            float phi2 = (2 * D3DX_PI * (seg + 1)) / kNumSegments;

            btVector3 p1 = btVector3(
                cosf(phi1) * sinf(theta1),
                -cosf(theta1),  // Y���]
                sinf(phi1) * sinf(theta1)
            );
            btVector3 p2 = btVector3(
                cosf(phi2) * sinf(theta1),
                -cosf(theta1),
                sinf(phi2) * sinf(theta1)
            );
            btVector3 p3 = btVector3(
                cosf(phi2) * sinf(theta2),
                -cosf(theta2),
                sinf(phi2) * sinf(theta2)
            );
            btVector3 p4 = btVector3(
                cosf(phi1) * sinf(theta2),
                -cosf(theta2),
                sinf(phi1) * sinf(theta2)
            );

            p1 = bottom + p1 * radius;
            p2 = bottom + p2 * radius;
            p3 = bottom + p3 * radius;
            p4 = bottom + p4 * radius;

            DrawLine3D(D3DXVECTOR3(p1.x(), p1.y(), p1.z()), D3DXVECTOR3(p2.x(), p2.y(), p2.z()), color);
            DrawLine3D(D3DXVECTOR3(p2.x(), p2.y(), p2.z()), D3DXVECTOR3(p3.x(), p3.y(), p3.z()), color);
            DrawLine3D(D3DXVECTOR3(p3.x(), p3.y(), p3.z()), D3DXVECTOR3(p4.x(), p4.y(), p4.z()), color);
            DrawLine3D(D3DXVECTOR3(p4.x(), p4.y(), p4.z()), D3DXVECTOR3(p1.x(), p1.y(), p1.z()), color);
        }
    }
}