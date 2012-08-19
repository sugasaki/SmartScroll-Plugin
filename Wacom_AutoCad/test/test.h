// test.h : TEST アプリケーションのメイン ヘッダー ファイルです。
//

#if !defined(AFX_TEST_H__A4665A3E_51C4_424F_ABB7_3E7A53FCAA53__INCLUDED_)
#define AFX_TEST_H__A4665A3E_51C4_424F_ABB7_3E7A53FCAA53__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// メイン シンボル

/////////////////////////////////////////////////////////////////////////////
// CTestApp:
// このクラスの動作の定義に関しては test.cpp ファイルを参照してください。
//

class CTestApp : public CWinApp
{
public:
	CTestApp();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CTestApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// インプリメンテーション

	//{{AFX_MSG(CTestApp)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_TEST_H__A4665A3E_51C4_424F_ABB7_3E7A53FCAA53__INCLUDED_)
