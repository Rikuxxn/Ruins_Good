//=============================================================================
//
// オブジェクト処理 [object.h]
// Author : TANEKAWA RIKU
//
//=============================================================================
#ifndef _OBJECT_H_// このマクロ定義がされていなかったら
#define _OBJECT_H_// 2重インクルード防止のマクロ定義

#include "main.h"

#define MAX_OBJECT (1024)	// 2Dオブジェクトの最大数
#define MAX_PRIORITY_2D (8)

// オブジェクトクラス
class CObject
{
public:
	CObject(int nPriority = 3);
	virtual ~CObject();

	// オブジェクトの種類
	typedef enum
	{
		TYPE_NONE = 0,
		TYPE_PLAYER,
		TYPE_ENEMY,
		TYPE_BULLET,
		TYPE_MAX
	}TYPE;

	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;
	virtual D3DXVECTOR3 GetPos(void) = 0;
	static void ReleaseAll(void);
	static void UpdateAll(void);
	static void DrawAll(void);
	static int GetNumObject(void);
	static CObject* GetObject(int nPriority,int nIdx);
	void SetType(TYPE type);
	TYPE GetType(void);
protected:
	void Release(void);

private:
	static CObject* m_apObject[MAX_PRIORITY_2D][MAX_OBJECT];
	static int m_nNumAll;				// オブジェクトの総数
	int m_nID;							// 自分自身の総数
	TYPE m_type;
	int m_nPriority;					// 優先順位の位置
};

#endif
