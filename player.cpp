//=============================================================================
//
// プレイヤー処理 [player.cpp]
// Author : TANEKAWA RIKU
//
//=============================================================================
#include "player.h"
#include "texture.h"
#include "manager.h"
#include "model.h"
#include "shadow.h"

//=======================================
// コンストラクタ
//=======================================
CPlayer::CPlayer()
{
	// 値のクリア
	m_pos			= INIT_VEC3;					// 位置
	m_rot			= INIT_VEC3;					// 向き
	m_rotDest		= INIT_VEC3;					// 向き
	m_move			= INIT_VEC3;					// 移動量
	m_mtxWorld		= {};							// ワールドマトリックス
	m_nNumModel		= 0;							// モデル(パーツ)の総数
	m_playerUse		= true;							// 使われているかどうか
	m_pShadow		= NULL;							// 影へのポインタ
	m_pMotion		= NULL;							// モーションへのポインタ
	m_currentMotion = CMotion::TYPE_NEUTRAL;

	for (int nCnt = 0; nCnt < MAX_PARTS; nCnt++)
	{
		m_apModel[nCnt] = {};						// モデル(パーツ)へのポインタ
	}
}
//=======================================
// デストラクタ
//=======================================
CPlayer::~CPlayer()
{
	// なし
}
//=======================================
// 生成処理
//=======================================
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
//=======================================
// 初期化処理
//=======================================
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

	// 影の生成
	m_pShadow = CShadow::Create(m_pos, 100, 25.0f, 0.1f, 25.0f);

	return S_OK;
}
//=======================================
// 終了処理
//=======================================
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

	// オブジェクトの破棄(自分自身)
	this->Release();
}
//=======================================
// 更新処理
//=======================================
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
	if (pInputKeyboard->GetTrigger(DIK_SPACE) == true)
	{



	}

	// モーション切り替え
	if (bIsMoving)
	{
		if (m_currentMotion == CMotion::TYPE_NEUTRAL)
		{
			m_pMotion->StartBlendMotion(CMotion::TYPE_MOVE, 10);
			m_currentMotion = CMotion::TYPE_MOVE;
		}
	}
	else
	{
		if (m_currentMotion == CMotion::TYPE_MOVE)
		{
			m_pMotion->StartBlendMotion(CMotion::TYPE_NEUTRAL, 10);
			m_currentMotion = CMotion::TYPE_NEUTRAL;
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

	// 位置を更新
	m_pos.x += m_move.x;
	m_pos.z += m_move.z;
	m_pos.y += m_move.y;

	if (m_pShadow != NULL)
	{
		D3DXVECTOR3 shadowPos = m_pos;
		shadowPos.y = 0.05f;

		// 影の位置設定
		m_pShadow->SetPosition(shadowPos);
	}

	// 移動量を更新(減衰させる)
	m_move.x += (0.0f - m_move.x) * 0.3f;
	m_move.z += (0.0f - m_move.z) * 0.3f;
	m_move.y += (0.0f - m_move.y) * 0.1f;

	int nNumModels = 10;

	// モーションの更新処理
	m_pMotion->Update(m_apModel, nNumModels);
}
//=======================================
// 描画処理
//=======================================
void CPlayer::Draw(void)
{
	// デバイスの取得
	CRenderer* renderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = renderer->GetDevice();

	// 計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

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
}
//=======================================
// 位置の取得
//=======================================
D3DXVECTOR3 CPlayer::GetPos(void)
{
	return m_pos;
}
//=======================================
// 向きの取得
//=======================================
D3DXVECTOR3 CPlayer::GetRot(void)
{
	return m_rot;
}
//=======================================
// プレイヤーの判定取得
//=======================================
bool CPlayer::GetPlayerUse(void)
{
	return m_playerUse;
}
