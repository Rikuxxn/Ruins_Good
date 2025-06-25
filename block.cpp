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

using namespace std;

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
	m_pDebug3D = NULL;							// 3D�f�o�b�O�\���ւ̃|�C���^
	m_prevSize = INIT_VEC3;
	m_colliderSize = INIT_VEC3;					// �R���C�_�[�T�C�Y
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

		pBlock->CreatePhysics(pos, size);

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
		path = "data/MODELS/wall_01.x";
		size = { 1.0f, 1.0f, 1.0f };
		break;

	case TYPE_WALL2:
		path = "data/MODELS/wall_02.x";
		size = { 1.0f, 1.0f, 1.0f };
		break;

	case TYPE_WALL3:
		path = "data/MODELS/wall_03.x";
		size = { 1.0f, 1.0f, 1.0f };
		break;

	case TYPE_WALL4:
		path = "data/MODELS/wall_04.x";
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

	case TYPE_DOOR:
		path = "data/MODELS/wall_door_01.x";
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
	// �ʒu�̎擾
	D3DXVECTOR3 Pos = GetPos();

	// �����̎擾
	D3DXVECTOR3 Rot = GetRot();

	// �T�C�Y(�g�嗦)�̎擾
	D3DXVECTOR3 size = GetSize();

	// �T�C�Y���ς���Ă�����Đ���
	if (size != m_prevSize)
	{
		CreatePhysics(Pos, size); // �R���C�_�[��蒼��
		m_prevSize = size;
	}

	btTransform trans;
	trans.setIdentity();

	D3DXMATRIX matRot;
	D3DXMatrixRotationYawPitchRoll(&matRot, Rot.y, Rot.x, Rot.z);

	D3DXQUATERNION dq;
	D3DXQuaternionRotationMatrix(&dq, &matRot);

	// Bullet�p�ɕϊ�
	btQuaternion q(dq.x, dq.y, dq.z, dq.w);

	trans.setOrigin(btVector3(Pos.x, Pos.y, Pos.z));// �ʒu�𔽉f
	trans.setRotation(q);							// ��]�𔽉f

	// �������ɔ��f
	m_pRigidBody->setWorldTransform(trans);
	m_pRigidBody->getMotionState()->setWorldTransform(trans);

	// �I�u�W�F�N�gX�̍X�V����
	CObjectX::Update();
}
//=============================================================================
// �R���C�_�[�̍X�V����
//=============================================================================
void CBlock::UpdateCollider(void)
{
	if (!m_pRigidBody)
	{
		return;
	}

	// �ʒu�̎擾
	D3DXVECTOR3 Pos = GetPos();

	// �폜���čĐ���
	ReleasePhysics();

	CreatePhysics(Pos, m_colliderSize);
}
//=============================================================================
// �`�揈��
//=============================================================================
void CBlock::Draw(void)
{
	// �I�u�W�F�N�gX�̕`�揈��
	CObjectX::Draw();

	// �u���b�N�}�l�[�W���[�̎擾
	CBlockManager* pBlockManager = CManager::GetBlockManager();

	// �u���b�N�}�l�[�W���[�̕`��
	pBlockManager->Draw();
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
		return "data/TEXTURE/wall1.png";

	case TYPE_WALL2:
		return "data/TEXTURE/wall2.png";

	case TYPE_WALL3:
		return "data/TEXTURE/wall3.png";

	case TYPE_WALL4:
		return "data/TEXTURE/wall4.png";

	case TYPE_AXE: 
		return "data/TEXTURE/Axe.png";

	case TYPE_IKADA: 
		return "data/TEXTURE/ikada.png";

	case TYPE_ROCK:
		return "data/TEXTURE/rock.png";

	case TYPE_TORCH:
		return "data/TEXTURE/torch1.png";

	case TYPE_TORCH2:
		return "data/TEXTURE/torch2.png";

	case TYPE_FLOOR:
		return "data/TEXTURE/floor1.png";

	case TYPE_FLOOR2:
		return "data/TEXTURE/floor2.png";

	case TYPE_DOOR:
		return "data/TEXTURE/wall_door1.png";

	case TYPE_CEILING:
		return "data/TEXTURE/ceiling1.png";

	case TYPE_CEILING2:
		return "data/TEXTURE/ceiling2.png";

	case TYPE_DOOR_BODY:
		return "data/TEXTURE/door1.png";

	default: 
		return "";
	}
}
//=============================================================================
// �����蔻��̐�������
//=============================================================================
void CBlock::CreatePhysics(const D3DXVECTOR3& pos, const D3DXVECTOR3& size)
{
	// ���łɍ���Ă����x�폜
	ReleasePhysics();

	// �����T�C�Y���v�Z�i�g�嗦 �~ ���f���T�C�Y�j
	D3DXVECTOR3 modelSize = GetModelSize();

	m_colliderSize.x = modelSize.x * size.x;
	m_colliderSize.y = modelSize.y * size.y;
	m_colliderSize.z = modelSize.z * size.z;

	// �`����^�C�v���Ƃɐ؂�ւ�
	if (m_Type == TYPE_ROCK)
	{
		// ���R���C�_�[
		float radius = std::max(std::max(m_colliderSize.x, m_colliderSize.y), m_colliderSize.z) * 0.5f;
		m_pShape = new btSphereShape(radius);
	}
	else
	{// ���̑�
		// �{�b�N�X�R���C�_�[
		btVector3 halfExtents(m_colliderSize.x * 0.5f, m_colliderSize.y * 0.5f, m_colliderSize.z * 0.5f);
		m_pShape = new btBoxShape(halfExtents);
	}

	// ���[���h�ϊ��i�ʒu�j
	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btVector3(pos.x, pos.y, pos.z));

	// �ÓI�u���b�N�͎��ʃ[��
	btScalar mass = IsStaticBlock() ? 0.0f : 1.0f;

	btVector3 inertia(0, 0, 0);

	if (mass != 0.0f)
	{
		m_pShape->calculateLocalInertia(mass, inertia);
	}

	// MotionState
	btDefaultMotionState* motionState = new btDefaultMotionState(transform);

	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, m_pShape, inertia);

	m_pRigidBody = new btRigidBody(rbInfo);

	// �L�l�}�e�B�b�N�ݒ�F�d�́E�������Z�����A���O�ňʒu����
	m_pRigidBody->setCollisionFlags(m_pRigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
	m_pRigidBody->setActivationState(DISABLE_DEACTIVATION);

	// �������[���h�ɒǉ�
	btDiscreteDynamicsWorld* pWorld = CManager::GetPhysicsWorld();

	if (pWorld != NULL)
	{
		pWorld->addRigidBody(m_pRigidBody);
	}

	// �O��̃T�C�Y�ɕۑ�
	m_prevSize = size;
}
//=============================================================================
// Physics�̔j��
//=============================================================================
void CBlock::ReleasePhysics(void)
{
	if (m_pRigidBody)
	{
		auto world = CManager::GetPhysicsWorld();

		if (world)
		{
			world->removeRigidBody(m_pRigidBody);
			delete m_pRigidBody->getMotionState();
			delete m_pRigidBody;
			delete m_pShape;
			m_pRigidBody = NULL;
			m_pShape = NULL;
		}
	}
}
//=============================================================================
// �ÓI�ȃu���b�N�����肷�鏈��
//=============================================================================
bool CBlock::IsStaticBlock(void) const
{
	switch (m_Type)
	{
	case TYPE_WALL:
	case TYPE_WALL2:
	case TYPE_WALL3:
	case TYPE_WALL4:
	case TYPE_TORCH:
	case TYPE_FLOOR:
	case TYPE_FLOOR2:
	case TYPE_CEILING:
	case TYPE_CEILING2:
	case TYPE_DOOR:
		return true; // �ÓI�i�����Ȃ��j

	default:
		return false; // �����\������
	}
}
//=============================================================================
// �R���C�_�[�T�C�Y�̎擾
//=============================================================================
D3DXVECTOR3 CBlock::GetColliderSize(void) const
{
	return m_colliderSize;
}
//=============================================================================
// �R���C�_�[�T�C�Y�̐ݒ菈��
//=============================================================================
void CBlock::SetColliderSize(const D3DXVECTOR3& size)
{
	m_colliderSize = size;
}

