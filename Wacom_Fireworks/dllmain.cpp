//
// File : dllmain.cpp
//
///////////////////////////////////////////////////////////////////////////////////
#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include "resource.h"
#include "common.h"
#include "sspfunc.h"

// プラグインの設定情報を保管するレジストリ名
#define REG_KEY			"FIREWORKS_MX"
// 作成者名
#define AUTHOR			"GDF"
// ファンクションキーに設定可能な機能数
#define FUNCTION_NUM	22
// キーストローク機能の番号
#define KEYSTROKE_NO	20

// グローバル変数
HINSTANCE	ghInst=NULL;
// 設定情報
SSP_SETTING	sspSetting;
// デフォルト設定情報(ファンクションキー１～８、ホイールボタン、作成者の順）
// ここにデフォルト設定を記述してください
SSP_DEFAULT	sspDefault = { 0, 1, 2, 3, 4, 5, 6, 7, 8, AUTHOR };

SSP_STATUS WINAPI SspSendKeyEvent(HWND hWnd, UINT uiButton, UINT uiFunction, UINT uiSW);

HWND hgWnd;


unsigned char keycode = 0x6A;

void PutingMessage(char *cmdstr);


///////////////////////////////////////////////////////////////////////////////////
BOOL APIENTRY
DllMain(HINSTANCE hModule, DWORD dwReason, LPVOID lpReserved )
{
	if(dwReason == DLL_PROCESS_ATTACH)
	{
		// レジストリから設定情報を読み込みます
		SspLoadRegSetting(REG_KEY, &sspSetting, &sspDefault);

		ghInst = hModule;
	}

    UNREFERENCED_PARAMETER(lpReserved);

    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////
// < 機能 >
//  プラグインファイルがドライバによりロードされた時に呼ばれる関数です
// < パラメータ >
//	pParam->dwModel		: SmartScrollのモデル番号
//	pParam->dwVersion	: SmartScrollドライバのバージョン
// < 戻り値 >
// SSP_TRUE		: このプラグインは常駐されます
// SSP_FALSE	: このプラグインの常駐は解除されます
SSP_STATUS WINAPI
SspLoadPlugin(PLOADPARAM pParam)
{
#ifdef _DEBUG
	char	szOut[200];
	wsprintf(szOut, "SspLoadPlugin : dwModel = %x, dwVersion = %x.%x",
		pParam->dwModel, HIWORD(pParam->dwVersion), LOWORD(pParam->dwVersion));
	SspTraceOut(szOut);
#endif

	return SSP_TRUE;
}

///////////////////////////////////////////////////////////////////////////////////
// < 機能 >
//  フォアグラウンドウィンドウに対して、このプラグインを適用するかどうかを判断する関数です
// < パラメータ >
// hWnd		: フォアグラウンドウィンドウのハンドル
// < 戻り値 >
// SSP_TRUE		: このプラグインが適用される
// SSP_FALSE	: このプラグインは適用されない
SSP_STATUS WINAPI
SspApplyProc(HWND hWnd)
{
	#ifdef _DEBUG
		char	szOut[100];
		wsprintf(szOut, "SspApplyProc : hWnd = %x", hWnd);
		SspTraceOut(szOut);
	#endif

	// ウィンドウクラス名が、"Macromedia"の場合に適用されます
	char szClass[100];
	bool appFlag = false;
	//	GetClassName(hWnd, szClass, sizeof(szClass));
	GetWindowText(hWnd, szClass, sizeof(szClass));


	if(!_strnicmp(szClass, "Macromedia Fireworks", 20)) appFlag = true;

	if(appFlag == true) {
		hgWnd = hWnd;
		return SSP_TRUE;
	}
	return SSP_FALSE;
}

///////////////////////////////////////////////////////////////////////////////////
// < 機能 >
// プラグインが適用される時、最初に呼び出される関数です
// < パラメータ >
//	pParam->hwndApp		: フォアグラウンドウィンドウのハンドル
//	pParam->dwModel		: SmartScrollのモデル番号
//	pParam->dwVersion	: SmartScrollドライバのバージョン
// < 戻り値 >
// SSP_TRUE		: 通常はこの値を返してください
// SSP_FALSE	: プラグインの適用が取り消されます
SSP_STATUS WINAPI
SspInitProc(PINITPARAM pParam)
{
#ifdef _DEBUG
	char	szOut[200];
	wsprintf(szOut, "SspInitProc : hwndApp = %x, dwModel = %x, dwVersion = %x.%x",
		pParam->hwndApp, pParam->dwModel, HIWORD(pParam->dwVersion), LOWORD(pParam->dwVersion));
	SspTraceOut(szOut);
#endif

	return SSP_TRUE;
//	SspTimerProc関数を使用する場合は、SSP_TIMERを戻り値として返してください。
//	(ドライババージョン1.20以降で対応）
//	return SSP_TIMER;
}

///////////////////////////////////////////////////////////////////////////////////
// < 機能 >
// 約100ms間隔で呼び出されるタイマ関数です。
//（ドライババージョン1.20以降で対応）
void WINAPI
SspTimerProc(void)
{
#ifdef _DEBUG
	SspTraceOut("SspTimerProc");
#endif
}

///////////////////////////////////////////////////////////////////////////////////
// < 機能 >
// プラグインの適用が終了する時に呼び出される関数です
void WINAPI
SspReleaseProc(void)
{
#ifdef _DEBUG
	SspTraceOut("SspReleaseProc()");
#endif
}

///////////////////////////////////////////////////////////////////////////////////
// < 機能 >
// ファンクションキーが操作された場合の処理を行う関数です
// < パラメータ >
//	pParam->hwndMouse	: マウスカーソルの真下にあるウィンドウのハンドル
//	pParam->dwType		: イベントタイプ
// < 戻り値 >
// SSP_TRUE		: 通常はこの値を返してください
// SSP_SHOW_HIDE: SmartScrollウィンドウの表示／非表示が切り替わります
SSP_STATUS WINAPI
SspButtonProc(PBUTTONPARAM pParam)
{
#ifdef _DEBUG
	char	szOut[200];
	wsprintf(szOut, "SspButtonProc : hwndMouse = %x, dwType = %d", pParam->hwndMouse, pParam->dwType);
	SspTraceOut(szOut);
#endif

	UINT uiNo = BUTTON_NO(pParam->dwType);
	UINT uiEvent = BUTTON_EVENT(pParam->dwType);

	if(SSP_BUTTON1 <= uiNo && uiNo <= SSP_BUTTON_WHEEL)
	{
		return SspSendKeyEvent(pParam->hwndMouse, uiNo, sspSetting.sbiButton[uiNo].uiFunction, uiEvent);
	}
	return SSP_TRUE;
}

///////////////////////////////////////////////////////////////////////////////////
// < 機能 >
// ズームホイール機能の有効／無効を判断する関数です
// < 戻り値 >
// SSP_TRUE		: ズームホイールは有効です
// SSP_FALSE	: ズームホイールは無効です
SSP_STATUS WINAPI
SspGetWheelEffect(void)
{
#ifdef _DEBUG
	SspTraceOut("SspGetWheelEffect");
#endif

	return SSP_TRUE;
}

///////////////////////////////////////////////////////////////////////////////////
// < 機能 >
// ズームホイールが操作された場合の処理を行う関数です
// < パラメータ >
//	pParam->hwndMouse	: マウスカーソルの真下にあるウィンドウのハンドル
//	pParam->lRoll		: ズームホイールの回転量
//	pParam->dwDirection	: 回転方向フラグ
// < 戻り値 >
// SSP_TRUE		: デフォルトのズーム処理は行われません
// SSP_FALSE	: デフォルトのズーム処理が行われます
SSP_STATUS WINAPI
SspWheelProc(PWHEELPARAM pParam)
{

#ifdef _DEBUG
	char	szOut[200];
	wsprintf(szOut, "SspWheelProc : hwndMouse = %x, lRoll = %d, dwDirection = %d",
		pParam->hwndMouse, pParam->lRoll, pParam->dwDirection);
	SspTraceOut(szOut);
#endif
	// SSP_FALSEを返し、デフォルトのズーム処理を使用しても正常動作します。
	// 以下はCtrl+ホイールメッセージを送信するサンプルです。

	keycode++;

	switch(pParam->dwDirection)
	{
	case SSP_WHEEL_FW:
		keybd_event(VK_CONTROL, 0x1D, 0, 0);
		keybd_event(VK_ADD, 0x1D, 0, 0);
		keybd_event(VK_ADD, 0x1D, KEYEVENTF_KEYUP, 0);
		keybd_event(VK_CONTROL, 0x1D, KEYEVENTF_KEYUP, 0);
#ifdef _DEBUG
	wsprintf(szOut, "SspWheelProc : VK_CONTROL + '-' %x" ,keycode);
	SspTraceOut(szOut);
#endif
		break;
	case SSP_WHEEL_BK:
		keybd_event(VK_CONTROL, 0x1D, 0, 0);
		keybd_event(VK_SUBTRACT, 0x1D, 0, 0);
		keybd_event(VK_SUBTRACT, 0x1D, KEYEVENTF_KEYUP, 0);
		keybd_event(VK_CONTROL, 0x1D, KEYEVENTF_KEYUP, 0);
#ifdef _DEBUG
	wsprintf(szOut, "SspWheelProc : VK_CONTROL + ';'");
	SspTraceOut(szOut);
#endif
		break;
	}
	return SSP_TRUE;
}

///////////////////////////////////////////////////////////////////////////////////
// < 機能 >
// スクロールボールが操作された場合の処理を行う関数です
// < パラメータ >
//	pParam->hwndMouse	: マウスカーソルの真下にあるウィンドウのハンドル
//	pParam->hwndOwner	: hwndMouseの親ウィンドウのハンドル
//	pParam->hwndHScroll	: 横スクロールバーコントロールのハンドル
//	pParam->hwndVScroll	: 縦スクロールバーコントロールのハンドル
//	pParam->lX			: スクロールボールのX方向の回転量
//	pParam->lY			: スクロールボールのY方向の回転量
//	pParam->dwDirection	: 回転方向フラグ
// < 戻り値 >
// SSP_TRUE		: デフォルトのスクロール処理は行われません
// SSP_FALSE	: デフォルトのスクロール処理が行われます
SSP_STATUS WINAPI
SspScrollProc(PSCROLLPARAM pParam)
{
#ifdef _DEBUG
	char	szOut[200];
	wsprintf(szOut, "SspScrollProc : hwndMouse = %x, hwndOwner = %x, hwndHScroll = %x, hwndVScroll = %x, lX = %d, lY = %d, dwDirection = %d",
		pParam->hwndMouse, pParam->hwndOwner, pParam->hwndHScroll, pParam->hwndVScroll, pParam->lX, pParam->lY, pParam->dwDirection);
	SspTraceOut(szOut);
#endif
	// SSP_FALSEを返し、デフォルトのスクロール処理で正常動作します。
	return SSP_FALSE;

	// 以下はスクロールメッセージを送信するサンプルです。
/*
	if(pParam->dwDirection & SSP_SCROLL_UP)
	{
		PostMessage(pParam->hwndMouse, WM_VSCROLL, (WPARAM)SB_LINEUP, (LPARAM)NULL);
	}
	else if(pParam->dwDirection & SSP_SCROLL_DOWN)
	{
		PostMessage(pParam->hwndMouse, WM_VSCROLL, (WPARAM)SB_LINEDOWN, (LPARAM)NULL);
	}
	if(pParam->dwDirection & SSP_SCROLL_LEFT)
	{
		PostMessage(pParam->hwndMouse, WM_HSCROLL, (WPARAM)SB_LINELEFT, (LPARAM)NULL);
	}
	else if(pParam->dwDirection & SSP_SCROLL_RIGHT)
	{
		PostMessage(pParam->hwndMouse, WM_HSCROLL, (WPARAM)SB_LINERIGHT, (LPARAM)NULL);
	}	
	return SSP_TRUE;
*/
}

///////////////////////////////////////////////////////////////////////////////////
// < 機能 >
// SmartScrollパッドのボタンに表示されるアイコンハンドルを提供する関数です
// < パラメータ >
//	dwType	: ボタンの種類
// < 戻り値 >
// 各ボタン用のアイコンハンドルを返します
HICON WINAPI
SspGetIcon(DWORD dwType)
{
#ifdef _DEBUG
	char	szOut[100];
	wsprintf(szOut, "SspGetIcon : dwType = %d", dwType);
	SspTraceOut(szOut);
#endif
	
	if(SSP_BUTTON1 <= dwType && dwType <= SSP_BUTTON_WHEEL)
	{
		// キーストロークの場合
		if(sspSetting.sbiButton[dwType].uiFunction == KEYSTROKE_NO)
		{
			return SspGetKeyIcon(&(sspSetting.sbiButton[dwType]));
		}
		else
		{
			return (HICON)LoadImage(ghInst,
									MAKEINTRESOURCE(SSP_ICON_BASE+sspSetting.sbiButton[dwType].uiFunction),
									IMAGE_ICON,
									SSP_ICON_SIZE,
									SSP_ICON_SIZE,
									LR_DEFAULTCOLOR);
		}
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////
// < 機能 >
// プラグインが持つ文字列リソースを提供する関数です
// < パラメータ >
//	dwType	: リソースの種類
//	lpStr	: 文字列を受け取るバッファを指すポインタ
//	uiSize	: バッファのサイズ
// < 戻り値 >
// バッファにコピーされた文字数を返します
int WINAPI
SspGetString(DWORD dwType, LPSTR lpStr, UINT uiSize)
{
#ifdef _DEBUG
	char	szOut[100];
	wsprintf(szOut, "SspGetString : dwType = %d", dwType);
	SspTraceOut(szOut);
#endif

	if(dwType == SSP_NAME)
	{
		return LoadString(ghInst, IDS_NAME, lpStr, uiSize);
	}
	else if(dwType == SSP_NAME_EX)
	{
		return LoadString(ghInst, IDS_NAME_EX, lpStr, uiSize);
	}
	else if(SSP_BUTTON1 <= dwType && dwType <= SSP_BUTTON_WHEEL)
	{
		// キーストロークの場合
		if(sspSetting.sbiButton[dwType].uiFunction == KEYSTROKE_NO)
		{
			return SspGetKeyText(&(sspSetting.sbiButton[dwType]), lpStr, uiSize);
		}
		else
		{
			return LoadString(ghInst, SSP_STRING_BASE+sspSetting.sbiButton[dwType].uiFunction, lpStr, uiSize);
		}
	}
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////
// < 機能 >
// 設定ダイアログ用のダイアログプロシージャです
BOOL CALLBACK
SspDialogProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uiMsg)
	{
	case WM_INITDIALOG:
		{
#ifdef _DEBUG
			SspTraceOut("SspDialogProc : WM_INITDIALOG");
#endif
			// コンボボックスの初期化を行います
			for(int i=0; i<FUNCTION_NUM; i++)
			{
				char	szText[50];
				LoadString(ghInst, SSP_STRING_BASE+i, szText, sizeof(szText));

				for(int j=IDC_COMBO1; j<=IDC_COMBO_WHEEL; j++)
				{
					SendMessage(GetDlgItem(hWnd, j), CB_ADDSTRING, (WPARAM)0, (LPARAM)szText);
				}
			}
			// 初期値の設定を行います
			for(i=IDC_COMBO1; i<=IDC_COMBO_WHEEL; i++)
			{
				SendMessage(GetDlgItem(hWnd, i),
							CB_SETCURSEL,
							(WPARAM)sspSetting.sbiButton[i-IDC_COMBO1].uiFunction,
							(LPARAM)0);
			}
		}
		return TRUE;

	case WM_MEASUREITEM:
		{
			LPMEASUREITEMSTRUCT lpmis = (LPMEASUREITEMSTRUCT)lParam;
			lpmis->itemHeight = SSP_ICON_SIZE;
		}
		return TRUE;

	case WM_DRAWITEM:
		{
			LPDRAWITEMSTRUCT lpdis = (LPDRAWITEMSTRUCT)lParam;
			if(lpdis->itemID == -1)
				break;
			if(IDC_COMBO1 <= lpdis->CtlID && lpdis->CtlID <= IDC_COMBO_WHEEL)
			{
				// コンボボックスの描画を行います（アイコン表示付き）
				SspDrawComboBox(ghInst, lpdis);
				return TRUE;
			}
		}
		break;

	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDOK:
		case IDCANCEL:
			// レジストリに設定情報を保存します
			SspSaveRegSetting(REG_KEY, &sspSetting);

			EndDialog(hWnd, 0);
			return TRUE;

		// コンボボックスが選択された時の処理を行います
		case IDC_COMBO1:
		case IDC_COMBO2:
		case IDC_COMBO3:
		case IDC_COMBO4:
		case IDC_COMBO5:
		case IDC_COMBO6:
		case IDC_COMBO7:
		case IDC_COMBO8:
		case IDC_COMBO_WHEEL:
			{
				if(HIWORD(wParam) != CBN_SELCHANGE)
					break;
				int i = LOWORD(wParam)-IDC_COMBO1;
				UINT uiRet = SendMessage(GetDlgItem(hWnd, LOWORD(wParam)), CB_GETCURSEL, (WPARAM)0, (LPARAM)0);

				// キーストロークの場合は設定用ダイアログを呼び出します
				if(uiRet == KEYSTROKE_NO)
				{
					if(SspSetKeyStroke(hWnd, &(sspSetting.sbiButton[i])))
						sspSetting.sbiButton[i].uiFunction = uiRet;
					else
						SendMessage(GetDlgItem(hWnd, LOWORD(wParam)),
									CB_SETCURSEL,
									(WPARAM)sspSetting.sbiButton[i].uiFunction,
									(LPARAM)0);
				}
				else
					sspSetting.sbiButton[i].uiFunction = uiRet;
			}
			return TRUE;

		// デフォルト設定にします
		case IDC_DEFAULT:
			{
				for(int i=0; i<SSP_BUTTON_NUM; i++)
				{
					sspSetting.sbiButton[i].uiFunction = sspDefault.uiButton[i];
					SendMessage(GetDlgItem(hWnd, IDC_COMBO1+i), CB_SETCURSEL, (WPARAM)sspSetting.sbiButton[i].uiFunction, (LPARAM)0);
					// キーストローク情報をクリアします
					SspClearKeyInfo(&(sspSetting.sbiButton[i]));
				}
			}
			return TRUE;
		}
		break;
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////////
// < 機能 >
// 各機能に合せてアプリケーションに対してメッセージを送信する関数です
SSP_STATUS WINAPI
SspSendKeyEvent(HWND hWnd, UINT uiButton, UINT uiFunction, UINT uiSW)
{
#ifdef _DEBUG
	char	szOut[200];
	wsprintf(szOut, "SspSendKeyEvent : hWnd = %x", hWnd);
	SspTraceOut(szOut);
#endif


	if(uiSW == SSP_BUTTON_UP) {

		switch(uiFunction)
		{
		case 0:		//	複写
			PutingMessage("_copy\n"); break;
		case 1:		//	移動
			PutingMessage("_move\n"); break;
		case 2:		//	範囲ズーム
			PutingMessage("_zoom _e\n"); break;
		case 3:		//	窓ズーム
			PutingMessage("_zoom _w\n"); break;
		case 4:		//	線分
			PutingMessage("_line\n"); break;
		case 5:		//	円弧(3点)
			PutingMessage("_arc\n"); break;
		case 6:		//	円(中心,半径)
			PutingMessage("_circle\n"); break;
		case 7:		//	オフセット
			PutingMessage("_offset\n"); break;
		case 8:		//	延長
			PutingMessage("_extend\n"); break;
		case 9:		//	トリム
			PutingMessage("_trim\n"); break;
		case 10:		//	Enter
			keybd_event(VK_RETURN, 0x1D, 0, 0);
			keybd_event(VK_RETURN, 0x1D, KEYEVENTF_KEYUP, 0);
			break;
		case 11:		//	削除
			PutingMessage("_erase\n"); break;
		case 12:		//	キャンセル
			keybd_event(VK_ESCAPE, 0x1D, 0, 0);
			keybd_event(VK_ESCAPE, 0x1D, KEYEVENTF_KEYUP, 0);
			break;
		case 13:		//	コピー
			keybd_event(VK_CONTROL, 0x1D, 0, 0);
			keybd_event('C', 0x2E, 0, 0);
			keybd_event('C', 0x2E, KEYEVENTF_KEYUP, 0);
			keybd_event(VK_CONTROL, 0x1D, KEYEVENTF_KEYUP, 0);
			break;
		case 14:		//	切り取り
			keybd_event(VK_CONTROL, 0x1D, 0, 0);
			keybd_event('X', 0x2D, 0, 0);
			keybd_event('X', 0x2D, KEYEVENTF_KEYUP, 0);
			keybd_event(VK_CONTROL, 0x1D, KEYEVENTF_KEYUP, 0);
			break;
		case 15:		//	貼り付け
			keybd_event(VK_CONTROL, 0x1D, 0, 0);
			keybd_event('V', 0x2F, 0, 0);
			keybd_event('V', 0x2F, KEYEVENTF_KEYUP, 0);
			keybd_event(VK_CONTROL, 0x1D, KEYEVENTF_KEYUP, 0);
			break;
		case 16:		//	元に戻す
			keybd_event(VK_CONTROL, 0x1D, 0, 0);
			keybd_event('Z', 0x2C, 0, 0);
			keybd_event('Z', 0x2C, KEYEVENTF_KEYUP, 0);
			keybd_event(VK_CONTROL, 0x1D, KEYEVENTF_KEYUP, 0);
			break;
		case 17:		//	開く
			keybd_event(VK_CONTROL, 0x1D, 0, 0);
			keybd_event('O', 0x18, 0, 0);
			keybd_event('O', 0x18, KEYEVENTF_KEYUP, 0);
			keybd_event(VK_CONTROL, 0x1D, KEYEVENTF_KEYUP, 0);
			break;
		case 18:		//	閉じる
			PutingMessage("_close\n"); break;
		case 19:		// 表示／非表示
			return SSP_SHOW_HIDE;
			break;
		case 20:		//	キーストローク
			break;
		case 21:		//	Regen
			PutingMessage("_regen\n"); break;
		}
	}

	if(uiFunction == KEYSTROKE_NO) SspKeyStroke(&(sspSetting.sbiButton[uiButton]), uiSW); // キーストローク
			
	return SSP_TRUE;
}



void PutingMessage(char *cmdstr)
{
	COPYDATASTRUCT CpData;
	CpData.dwData = 1;
	CpData.cbData = strlen(cmdstr)+1;
	CpData.lpData = cmdstr;
	SendMessage(hgWnd, WM_COPYDATA, 0, (LPARAM)&CpData);
	PostMessage(hgWnd, WM_NULL, 0, 0);
}


/////////////////////////////////   End of File   ////////////////////////////////////
