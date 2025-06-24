//=============================================================================
//
// プレイヤー処理 [player.cpp]
// Author : TANEKAWA RIKU
//
//=============================================================================

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "player.h"
#include "texture.h"
#include "manager.h"
#include "model.h"
#include "shadow.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CPlayer::CPlayer()
{
	// 値のクリア
	m_pos			= INIT_VEC3;					// 位置
	m_rot			= INIT_VEC3;					// 向き
	m_rotDest		= INIT_VEC3;					// 向き
	m_move			= INIT_VEC3;					// 移動量
	m_size          = D3DXVECTOR3(1.0f, 1.0f, 1.0f);// サイズ
	m_mtxWorld		= {};							// ワールドマトリックス
	m_nNumModel		= 0;							// モデル(パーツ)の総数
	m_playerUse		= true;							// 使われているかどうか
	m_pShadow		= NULL;							// 影へのポインタ
	m_pMotion		= NULL;							// モーションへのポインタ
	m_currentMotion = CMotion::TYPE_NEUTRAL;		// 現在のモーション
	m_isJumping		= false;						// ジャンプ中フラグ
	m_pRigidBody	= NULL;							// 剛体へのポインタ
	m_pShape		= NULL;							// 当たり判定の形へのポインタ
	m_pDebug3D		= NULL;							// 3Dデバッグ表示へのポインタ
	m_radius        = 0.0f;							// カプセルコライダーの半径
	m_height		= 0.0f;							// カプセルコライダーの高さ
	for (int nCnt = 0; nCnt < MAX_PARTS; nCnt++)
	{
		m_apModel[nCnt] = {};						// モデル(パーツ)へのポインタ
	}
}
//=============================================================================
// デストラクタ
//=============================================================================
CPlayer::~CPlayer()
{
	// なし
}
//=============================================================================
// 生成処理
//=============================================================================
CPlayer* CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CPlayer* pPlayer;

	if (CObject::GetNumObject() < MAX_OBJECT - 1)
	{
		pPlayer = new CPlayer;

		// 初期化処理
		pPlayer->Init();

		pPlayer->m_pos = pos;
		pPlayer->m_rot = D3DXToRadian(rot);

		return pPlayer;
	}
	else
	{// エラー
		return NULL;
	}
}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CPlayer::Init(void)
{
	CModel* pModels[MAX_PARTS];
	int nNumModels = 0;

	// パーツの読み込み
	m_pMotion = CMotion::Load("data/motion.txt", pModels, nNumModels);

	for (int nCnt = 0; nCnt < nNumModels && nCnt < MAX_PARTS; nCnt++)
	{
		m_apModel[nCnt] = pModels[nCnt];

		// オフセット考慮
		m_apModel[nCnt]->SetOffsetPos(m_apModel[nCnt]->GetPos());
		m_apModel[nCnt]->SetOffsetRot(m_apModel[nCnt]->GetRot());
	}

	// パーツ数を代入
	m_nNumModel = nNumModels;

	// プレイヤーが使われている
	m_playerUse = true;

	// Bullet Physics 用カプセルコライダーの作成
	m_radius = 20.5f;
	m_height = 140.8f;

	m_pShape = new btCapsuleShape(m_radius, m_height - 2.0f * m_radius);

	// プレイヤーの位置を元にTransform作成
	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btVector3(m_pos.x - 5.5f, m_pos.y + m_height / 2.0f, m_pos.z));

	// 質量0にして完全制御（キネマティックにする）
	btScalar mass = 0.0f;
	btVector3 inertia(0, 0, 0);  // 質量0だと慣性は0でOK

	btDefaultMotionState* motionState = new btDefaultMotionState(transform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, m_pShape, inertia);

	m_pRigidBody = new btRigidBody(rbInfo);

	m_pRigidBody->setAngularFactor(btVector3(0, 0, 0)); // これで角運動量の影響をゼロに

	// キネマティック設定：重力・物理演算無視、自前で位置制御
	m_pRigidBody->setCollisionFlags(m_pRigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
	m_pRigidBody->setActivationState(DISABLE_DEACTIVATION);

	// 物理ワールドに追加
	btDiscreteDynamicsWorld* pWorld = CManager::GetPhysicsWorld();

	if (pWorld != NULL)
	{
		pWorld->addRigidBody(m_pRigidBody);
	}

	//// 影の生成
	//m_pShadow = CShadow::Create(m_pos, 100, 25.0f, 0.1f, 25.0f);

	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void CPlayer::Uninit(void)
{
	for (int nCnt = 0; nCnt < MAX_PARTS; nCnt++)
	{
		if (m_apModel[nCnt] != NULL)
		{
			m_apModel[nCnt]->Uninit();
			delete m_apModel[nCnt];
			m_apModel[nCnt] = NULL;
		}
	}

	if (m_pMotion != NULL)
	{
		delete m_pMotion;
		m_pMotion = NULL;
	}

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

	// オブジェクトの破棄(自分自身)
	this->Release();
}
//=============================================================================
// 更新処理
//=============================================================================
void CPlayer::Update(void)
{
	// キーボードの取得
	CInputKeyboard* pInputKeyboard = CManager::GetInputKeyboard();

	// ジョイパッドの取得
	CInputJoypad* pInputJoypad = CManager::GetInputJoypad();

	// ジョイスティック情報の取得
	XINPUT_STATE* pStick = CInputJoypad::GetStickAngle();

	// カメラの取得
	CCamera* pCamera = CManager::GetCamera();

	// カメラの角度の取得
	D3DXVECTOR3 CamRot = pCamera->GetRot();

	// 移動入力があるか判定するフラグ
	bool bIsMoving = false;

	// ゲームパッドの移動処理
	if (pInputJoypad->GetStick() == true && pStick != NULL)
	{
		// 左スティックの入力を取得
		float stickX = pStick->Gamepad.sThumbLX;
		float stickY = pStick->Gamepad.sThumbLY;

		// スティックのベクトル長を計算
		float magnitude = sqrtf(stickX * stickX + stickY * stickY);

		// デッドゾーン処理
		const float DEADZONE = 10922.0f;
		if (magnitude < DEADZONE)
		{
			stickX = 0.0f;
			stickY = 0.0f;
		}
		else
		{
			// 正規化
			stickX /= magnitude;
			stickY /= magnitude;

			// 倒し具合を 0.0～1.0 にマッピング
			float normalizedMagnitude = (magnitude - DEADZONE) / (32767.0f - DEADZONE);
			normalizedMagnitude = min(normalizedMagnitude, 1.0f);

			// スティックの方向ベクトルに倒し具合を掛ける
			stickX *= normalizedMagnitude;
			stickY *= normalizedMagnitude;

			// カメラの回転を取得
			float cameraYaw = CamRot.y;

			// 移動ベクトル計算
			float moveX = -(stickX * cosf(cameraYaw) + stickY * sinf(cameraYaw));
			float moveZ = stickX * sinf(-cameraYaw) + stickY * cosf(cameraYaw);

			// 移動方向反転
			moveZ = -moveZ;

			// プレイヤーの移動更新（通常速度）
			m_move.x += moveX * PLAYER_SPEED;
			m_move.z += moveZ * PLAYER_SPEED;

			// プレイヤーの向きを更新
			m_rotDest.y = atan2f(-moveX, -moveZ);

			bIsMoving = true;
		}
	}

	if (pInputKeyboard->GetPress(DIK_A) == true)
	{
		bIsMoving = true;

		if (pInputKeyboard->GetPress(DIK_W) == true)
		{
			// 移動量を更新(増加させる)
			m_move.x -= sinf(CamRot.y + -D3DX_PI * 0.25f) * PLAYER_SPEED;
			m_move.z -= cosf(CamRot.y + -D3DX_PI * 0.25f) * PLAYER_SPEED;

			m_rotDest.y = CamRot.y - D3DX_PI * 0.25f;
		}
		else if (pInputKeyboard->GetPress(DIK_S) == true)
		{
			// 移動量を更新(増加させる)
			m_move.x -= sinf(CamRot.y + -D3DX_PI * 0.75f) * PLAYER_SPEED;
			m_move.z -= cosf(CamRot.y + -D3DX_PI * 0.75f) * PLAYER_SPEED;

			m_rotDest.y = CamRot.y - D3DX_PI * 0.75f;
		}
		else
		{
			// 移動量を更新(増加させる)
			m_move.z -= sinf(CamRot.y) * PLAYER_SPEED;
			m_move.x += cosf(CamRot.y) * PLAYER_SPEED;

			m_rotDest.y = CamRot.y - D3DX_PI * 0.5f;
		}
	}
	else if (pInputKeyboard->GetPress(DIK_D) == true)
	{
		bIsMoving = true;

		if (pInputKeyboard->GetPress(DIK_W) == true)
		{
			// 移動量を更新(増加させる)
			m_move.x -= sinf(CamRot.y + D3DX_PI * 0.25f) * PLAYER_SPEED;
			m_move.z -= cosf(CamRot.y + D3DX_PI * 0.25f) * PLAYER_SPEED;

			m_rotDest.y = CamRot.y + (D3DX_PI * 0.25f);
		}
		else if (pInputKeyboard->GetPress(DIK_S) == true)
		{
			// 移動量を更新(増加させる)
			m_move.x -= sinf(CamRot.y + D3DX_PI * 0.75f) * PLAYER_SPEED;
			m_move.z -= cosf(CamRot.y + D3DX_PI * 0.75f) * PLAYER_SPEED;

			m_rotDest.y = CamRot.y + (D3DX_PI * 0.75f);
		}
		else
		{
			// 移動量を更新(増加させる)
			m_move.z += sinf(CamRot.y) * PLAYER_SPEED;
			m_move.x -= cosf(CamRot.y) * PLAYER_SPEED;

			m_rotDest.y = CamRot.y + D3DX_PI * 0.5f;
		}

	}
	else if (pInputKeyboard->GetPress(DIK_W) == true)
	{
		bIsMoving = true;

		// 移動量を更新(増加させる)
		m_move.x -= sinf(CamRot.y) * PLAYER_SPEED;
		m_move.z -= cosf(CamRot.y) * PLAYER_SPEED;

		m_rotDest.y = CamRot.y;
	}
	else if (pInputKeyboard->GetPress(DIK_S) == true)
	{
		bIsMoving = true;

		// 移動量を更新(増加させる)
		m_move.x += sinf(CamRot.y) * PLAYER_SPEED;
		m_move.z += cosf(CamRot.y) * PLAYER_SPEED;

		m_rotDest.y = CamRot.y + D3DX_PI;
	}

	// ジャンプ
	if (!m_isJumping && (pInputKeyboard->GetTrigger(DIK_SPACE) || pInputJoypad->GetTrigger(CInputJoypad::JOYKEY_A)))
	{
		m_isJumping = true;
		m_move.y = MAX_JUMP_POWER;

		m_pMotion->StartBlendMotion(CMotion::TYPE_JUMP, 10);
		m_currentMotion = CMotion::TYPE_JUMP;
	}


	// 接地していないなら常に重力をかける
	if (m_pos.y > 0.0f)
	{
		m_move.y += MAX_GRAVITY;  // 重力
	}
	else if (m_pos.y <= 0.0f && m_move.y < 0.0f)
	{
		// 着地処理
		m_pos.y = 0.0f;
		m_move.y = 0.0f;

		if (m_isJumping) // すでにジャンプしていたときだけ着地処理
		{
			m_pMotion->StartBlendMotion(CMotion::TYPE_NEUTRAL, 15);
			m_currentMotion = CMotion::TYPE_NEUTRAL;
		}

		m_isJumping = false;
	}

	// モーション切り替え
	if (!m_isJumping)  // ジャンプ中はモーションを変えない
	{
		if (bIsMoving)
		{
			if (m_currentMotion == CMotion::TYPE_NEUTRAL)
			{// 移動モーションへ
				m_pMotion->StartBlendMotion(CMotion::TYPE_MOVE, 10);
				m_currentMotion = CMotion::TYPE_MOVE;
			}
		}
		else
		{
			if (m_currentMotion == CMotion::TYPE_MOVE)
			{// 待機モーションへ
				m_pMotion->StartBlendMotion(CMotion::TYPE_NEUTRAL, 10);
				m_currentMotion = CMotion::TYPE_NEUTRAL;
			}
		}
	}

	// 向きの正規化
	if (m_rotDest.y - m_rot.y > D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2.0f;
	}
	else if (m_rotDest.y - m_rot.y < -D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2.0f;
	}

	m_rot.y += (m_rotDest.y - m_rot.y) * 0.2f;

	btTransform trans;
	trans.setIdentity();
	trans.setOrigin(btVector3(m_pos.x, m_pos.y, m_pos.z));
	m_pRigidBody->setWorldTransform(trans);
	m_pRigidBody->getMotionState()->setWorldTransform(trans);
	// 位置を更新
	m_pos.x += m_move.x;
	m_pos.z += m_move.z;
	m_pos.y += m_move.y;

	// 移動量を更新(減衰させる)
	m_move.x += (0.0f - m_move.x) * 0.3f;
	m_move.z += (0.0f - m_move.z) * 0.3f;

	int nNumModels = 10;

	// モーションの更新処理
	m_pMotion->Update(m_apModel, nNumModels);
}
//=============================================================================
// 情報の更新処理
//=============================================================================
void CPlayer::UpdateInfo(void)
{
	if (ImGui::TreeNode("Player Info"))
	{
		ImGui::Dummy(ImVec2(0.0f, 10.0f)); // 空白を空ける

		//*********************************************************************
		// POS の調整
		//*********************************************************************

		// 位置
		ImGui::Text("POS"); ImGui::SameLine(70); // ラベル位置微調整

		ImGui::Text("X:"); ImGui::SameLine();
		ImGui::SetNextItemWidth(80); // 幅を狭く
		ImGui::DragFloat("##Player_pos_x", &m_pos.x, 1.0f, -9000.0f, 9000.0f, "%.1f");

		ImGui::SameLine();
		ImGui::Text("Y:"); ImGui::SameLine();
		ImGui::SetNextItemWidth(80);
		ImGui::DragFloat("##Player_pos_y", &m_pos.y, 1.0f, -9000.0f, 9000.0f, "%.1f");

		ImGui::SameLine();
		ImGui::Text("Z:"); ImGui::SameLine();
		ImGui::SetNextItemWidth(80);
		ImGui::DragFloat("##Player_pos_z", &m_pos.z, 1.0f, -9000.0f, 9000.0f, "%.1f");

		//*********************************************************************
		// ROT の調整
		//*********************************************************************

		ImGui::Dummy(ImVec2(0.0f, 10.0f)); // 空白を空ける

		// 向き
		ImGui::Text("ROT"); ImGui::SameLine(70); // ラベル位置微調整

		ImGui::Text("X:"); ImGui::SameLine();
		ImGui::SetNextItemWidth(80); // 幅を狭く
		ImGui::DragFloat("##Player_rot_x", &m_rot.x, 0.1f, -D3DX_PI, D3DX_PI, "%.1f");

		ImGui::SameLine();
		ImGui::Text("Y:"); ImGui::SameLine();
		ImGui::SetNextItemWidth(80);
		ImGui::DragFloat("##Player_rot_y", &m_rot.y, 0.1f, -D3DX_PI, D3DX_PI, "%.1f");

		ImGui::SameLine();
		ImGui::Text("Z:"); ImGui::SameLine();
		ImGui::SetNextItemWidth(80);
		ImGui::DragFloat("##Player_rot_z", &m_rot.z, 0.1f, -D3DX_PI, D3DX_PI, "%.1f");

		//*********************************************************************
		// SIZE の調整
		//*********************************************************************

		ImGui::Dummy(ImVec2(0.0f, 10.0f)); // 空白を空ける

		// ラベル
		ImGui::Text("SIZE"); ImGui::SameLine(70); // ラベルの位置ちょっと調整

		// X
		ImGui::Text("X:"); ImGui::SameLine();
		ImGui::SetNextItemWidth(80);
		ImGui::DragFloat("##Player_size_x", &m_size.x, 0.1f, -100.0f, 100.0f, "%.1f");

		// Y
		ImGui::SameLine();
		ImGui::Text("Y:"); ImGui::SameLine();
		ImGui::SetNextItemWidth(80);
		ImGui::DragFloat("##Player_size_y", &m_size.y, 0.1f, -100.0f, 100.0f, "%.1f");

		// Z
		ImGui::SameLine();
		ImGui::Text("Z:"); ImGui::SameLine();
		ImGui::SetNextItemWidth(80);
		ImGui::DragFloat("##Player_size_z", &m_size.z, 0.1f, -100.0f, 100.0f, "%.1f");

		ImGui::TreePop(); // 閉じる
	}
}
//=============================================================================
// 描画処理
//=============================================================================
void CPlayer::Draw(void)
{
	// デバイスの取得
	CRenderer* renderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = renderer->GetDevice();

	// 計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans, mtxSize;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// サイズを反映
	D3DXMatrixScaling(&mtxSize, m_size.x, m_size.y, m_size.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxSize);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスを設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	for (int nCntMat = 0; nCntMat < m_nNumModel; nCntMat++)
	{
		// モデル(パーツ)の描画
		if (m_apModel[nCntMat])
		{
			m_apModel[nCntMat]->Draw();
		}
	}

#ifdef _DEBUG

	// カプセルコライダーの描画
	if (m_pRigidBody && m_pShape)
	{
		btTransform transform;
		m_pRigidBody->getMotionState()->getWorldTransform(transform);

		m_pDebug3D->DrawCapsuleCollider((btCapsuleShape*)m_pShape, transform, D3DXCOLOR(1, 1, 1, 1));
	}

#endif

}
//=============================================================================
// 位置の取得
//=============================================================================
D3DXVECTOR3 CPlayer::GetPos(void)
{
	return m_pos;
}
//=============================================================================
// 向きの取得
//=============================================================================
D3DXVECTOR3 CPlayer::GetRot(void)
{
	return m_rot;
}
//=============================================================================
// プレイヤーの判定取得
//=============================================================================
bool CPlayer::GetPlayerUse(void)
{
	return m_playerUse;
}