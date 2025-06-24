//=============================================================================
//
// 3Dデバッグ表示処理 [debugproc3D.cpp]
// Author : TANEKAWA RIKU
//
//=============================================================================
#include "debugproc3D.h"
#include "renderer.h"
#include "manager.h"

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
LPD3DXLINE CDebugProc3D::m_pLine = NULL;

//=============================================================================
// コンストラクタ
//=============================================================================
CDebugProc3D::CDebugProc3D()
{
	// 値のクリア

}
//=============================================================================
// デストラクタ
//=============================================================================
CDebugProc3D::~CDebugProc3D()
{
	// なし
}
//=============================================================================
// 初期化処理
//=============================================================================
void CDebugProc3D::Init(void)
{
	// デバイスの取得
	CRenderer* renderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = renderer->GetDevice();

	if (pDevice != NULL)
	{
		D3DXCreateLine(pDevice, &m_pLine);
	}
}
//=============================================================================
// 終了処理
//=============================================================================
void CDebugProc3D::Uninit(void)
{
	// ライン表示の破棄
	if (m_pLine != NULL)
	{
		m_pLine->Release();
		m_pLine = NULL;
	}
}
//=============================================================================
// ライン描画処理
//=============================================================================
void CDebugProc3D::DrawLine3D(const D3DXVECTOR3& start, const D3DXVECTOR3& end, D3DXCOLOR color)
{
    // デバイスの取得
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

	// 頂点フォーマット設定
	pDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);

	// ライン描画
	pDevice->DrawPrimitiveUP(D3DPT_LINELIST, 1, v, sizeof(VERTEX));
}
//=============================================================================
// カプセルコライダー描画処理
//=============================================================================
void CDebugProc3D::DrawCapsuleCollider(btCapsuleShape* capsule, const btTransform& transform, D3DXCOLOR color)
{
    if (!capsule || !m_pLine)
    {
        return;
    }

    // デバイスの取得
    CRenderer* renderer = CManager::GetRenderer();
    LPDIRECT3DDEVICE9 pDevice = renderer->GetDevice();

    const int kNumSegments = 16;
    float radius = capsule->getRadius();
    float halfHeight = capsule->getHalfHeight();

    // Capsule の中心位置
    btVector3 base = transform.getOrigin();

    base += btVector3(0, halfHeight, 0);

    // ここで、btTransformの回転をD3DXMATRIXへ変換（傾きを含めたければこちらを使う）
    btQuaternion q = transform.getRotation();

    D3DXMATRIX matRot;
    D3DXQUATERNION dq(q.x(), q.y(), q.z(), q.w());
    D3DXMatrixRotationQuaternion(&matRot, &dq);

    // 平行移動行列
    D3DXMATRIX matTrans;
    D3DXMatrixTranslation(&matTrans, base.x(), base.y(), base.z());

    // ワールド行列 = 回転 × 平行移動
    D3DXMATRIX matWorld = matRot * matTrans;

    // デバイスにワールド行列を設定
    pDevice->SetTransform(D3DTS_WORLD, &matWorld);

    // カプセル方向はY軸固定（BulletのbtCapsuleShapeはUp軸指定できるが、ここではY軸と仮定）
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

        // 上の円
        DrawLine3D(D3DXVECTOR3((top + dir1).x(), (top + dir1).y(), (top + dir1).z()),
            D3DXVECTOR3((top + dir2).x(), (top + dir2).y(), (top + dir2).z()), color);

        // 下の円
        DrawLine3D(D3DXVECTOR3((bottom + dir1).x(), (bottom + dir1).y(), (bottom + dir1).z()),
            D3DXVECTOR3((bottom + dir2).x(), (bottom + dir2).y(), (bottom + dir2).z()), color);

        // 側面の線
        DrawLine3D(D3DXVECTOR3((top + dir1).x(), (top + dir1).y(), (top + dir1).z()),
            D3DXVECTOR3((bottom + dir1).x(), (bottom + dir1).y(), (bottom + dir1).z()), color);
    }
}