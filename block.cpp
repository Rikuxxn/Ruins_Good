//=============================================================================
//
// ブロック処理 [block.cpp]
// Author : TANEKAWA RIKU
//
//=============================================================================

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "block.h"
#include "manager.h"

using namespace std;

//=============================================================================
// コンストラクタ
//=============================================================================
CBlock::CBlock()
{
	// 値のクリア
	for (int nCnt = 0; nCnt < MAX_PATH; nCnt++)
	{
		m_szPath[nCnt] = NULL;					// ファイルパス
	}
	m_col = INIT_XCOL;							// 色
	m_baseCol = INIT_XCOL;						// ベースの色
	m_bSelected = false;
	m_nIdxTexture = 0;
	m_pDebug3D = NULL;							// 3Dデバッグ表示へのポインタ
	m_prevSize = INIT_VEC3;
	m_colliderSize = INIT_VEC3;					// コライダーサイズ
}
//=============================================================================
// デストラクタ
//=============================================================================
CBlock::~CBlock()
{
	// なし
}
//=============================================================================
// 生成処理
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
		pBlock->SetPath(pFilepath);	// パス保存

		// 初期化処理
		pBlock->Init();

		pBlock->CreatePhysics(pos, size);

		return pBlock;
	}
	else
	{// エラー
		return NULL;
	}
}
//=============================================================================
// タイプでの生成処理
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
// 初期化処理
//=============================================================================
HRESULT CBlock::Init(void)
{
	// オブジェクトXの初期化処理
	CObjectX::Init();

	// マテリアル色をブロックの色に設定
	m_col = GetMaterialColor();
	m_col = m_baseCol;              // 現在の色にも一度入れておく

	// テクスチャ割り当て
	CTexture* pTexture = CManager::GetTexture();
	m_nIdxTexture = pTexture->Register(GetTexPathFromType(m_Type));

	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void CBlock::Uninit(void)
{
	// オブジェクトXの終了処理
	CObjectX::Uninit();
}
//=============================================================================
// 更新処理
//=============================================================================
void CBlock::Update(void)
{
	// 位置の取得
	D3DXVECTOR3 Pos = GetPos();

	// 向きの取得
	D3DXVECTOR3 Rot = GetRot();

	// サイズ(拡大率)の取得
	D3DXVECTOR3 size = GetSize();

	// サイズが変わっていたら再生成
	if (size != m_prevSize)
	{
		CreatePhysics(Pos, size); // コライダー作り直し
		m_prevSize = size;
	}

	btTransform trans;
	trans.setIdentity();

	D3DXMATRIX matRot;
	D3DXMatrixRotationYawPitchRoll(&matRot, Rot.y, Rot.x, Rot.z);

	D3DXQUATERNION dq;
	D3DXQuaternionRotationMatrix(&dq, &matRot);

	// Bullet用に変換
	btQuaternion q(dq.x, dq.y, dq.z, dq.w);

	trans.setOrigin(btVector3(Pos.x, Pos.y, Pos.z));// 位置を反映
	trans.setRotation(q);							// 回転を反映

	// 物理側に反映
	m_pRigidBody->setWorldTransform(trans);
	m_pRigidBody->getMotionState()->setWorldTransform(trans);

	// オブジェクトXの更新処理
	CObjectX::Update();
}
//=============================================================================
// コライダーの更新処理
//=============================================================================
void CBlock::UpdateCollider(void)
{
	if (!m_pRigidBody)
	{
		return;
	}

	// 位置の取得
	D3DXVECTOR3 Pos = GetPos();

	// 削除して再生成
	ReleasePhysics();

	CreatePhysics(Pos, m_colliderSize);
}
//=============================================================================
// 描画処理
//=============================================================================
void CBlock::Draw(void)
{
	// オブジェクトXの描画処理
	CObjectX::Draw();

	// ブロックマネージャーの取得
	CBlockManager* pBlockManager = CManager::GetBlockManager();

	// ブロックマネージャーの描画
	pBlockManager->Draw();
}
//=============================================================================
// 色の取得
//=============================================================================
D3DXCOLOR CBlock::GetCol(void) const
{
	if (m_bSelected)
	{
		// 赤くする
		return D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.6f);
	}
	else
	{
		return D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); // 無補正
	}
}
//=============================================================================
// 画像表示用テクスチャパスの取得
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
// 当たり判定の生成処理
//=============================================================================
void CBlock::CreatePhysics(const D3DXVECTOR3& pos, const D3DXVECTOR3& size)
{
	// すでに作ってたら一度削除
	ReleasePhysics();

	// 物理サイズを計算（拡大率 × モデルサイズ）
	D3DXVECTOR3 modelSize = GetModelSize();

	m_colliderSize.x = modelSize.x * size.x;
	m_colliderSize.y = modelSize.y * size.y;
	m_colliderSize.z = modelSize.z * size.z;

	// 形状をタイプごとに切り替え
	if (m_Type == TYPE_ROCK)
	{
		// 球コライダー
		float radius = std::max(std::max(m_colliderSize.x, m_colliderSize.y), m_colliderSize.z) * 0.5f;
		m_pShape = new btSphereShape(radius);
	}
	else
	{// その他
		// ボックスコライダー
		btVector3 halfExtents(m_colliderSize.x * 0.5f, m_colliderSize.y * 0.5f, m_colliderSize.z * 0.5f);
		m_pShape = new btBoxShape(halfExtents);
	}

	// ワールド変換（位置）
	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btVector3(pos.x, pos.y, pos.z));

	// 静的ブロックは質量ゼロ
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

	// キネマティック設定：重力・物理演算無視、自前で位置制御
	m_pRigidBody->setCollisionFlags(m_pRigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
	m_pRigidBody->setActivationState(DISABLE_DEACTIVATION);

	// 物理ワールドに追加
	btDiscreteDynamicsWorld* pWorld = CManager::GetPhysicsWorld();

	if (pWorld != NULL)
	{
		pWorld->addRigidBody(m_pRigidBody);
	}

	// 前回のサイズに保存
	m_prevSize = size;
}
//=============================================================================
// Physicsの破棄
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
// 静的なブロックか判定する処理
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
		return true; // 静的（動かない）

	default:
		return false; // 動く可能性あり
	}
}
//=============================================================================
// コライダーサイズの取得
//=============================================================================
D3DXVECTOR3 CBlock::GetColliderSize(void) const
{
	return m_colliderSize;
}
//=============================================================================
// コライダーサイズの設定処理
//=============================================================================
void CBlock::SetColliderSize(const D3DXVECTOR3& size)
{
	m_colliderSize = size;
}

