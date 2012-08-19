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
#define REG_KEY			"Chrome"
// 作成者名
#define AUTHOR			"SUGASAKI"
// ファンクションキーに設定可能な機能数
#define FUNCTION_NUM	20
// キーストローク機能の番号
#define KEYSTROKE_NO	19

// グローバル変数
HINSTANCE	ghInst=NULL;
// 設定情報
SSP_SETTING	sspSetting;
// デフォルト設定情報(ファンクションキー１～８、ホイールボタン、作成者の順）
// ここにデフォルト設定を記述してください
SSP_DEFAULT	sspDefault = { 0, 1, 2, 3, 4, 5, 6, 7, 18, AUTHOR };

SSP_STATUS WINAPI SspSendKeyEvent(HWND hWnd, UINT uiButton, UINT uiFunction, UINT uiSW);


int updown_flag = SSP_SCROLL_NONE;
int cnt = 0;
HWND hwndMouse;

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

	// ウィンドウクラス名が、"WordPadClass"の場合に適用されます
	char szClass[100];
	GetClassName(hWnd, szClass, sizeof(szClass));
	//if(!strcmp(szClass, "Chrome_WidgetWin_0")) return SSP_TRUE;
	if(!_strnicmp(szClass, "Chrome_Widget", 13)) return SSP_TRUE;

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

	//return SSP_TRUE;
//	SspTimerProc関数を使用する場合は、SSP_TIMERを戻り値として返してください。
//	(ドライババージョン1.20以降で対応）
	return SSP_TIMER;
}

///////////////////////////////////////////////////////////////////////////////////
// < 機能 >
// 約100ms間隔で呼び出されるタイマ関数です。
//（ドライババージョン1.20以降で対応）
void WINAPI
SspTimerProc(void)
{
#ifdef _DEBUG
//	SspTraceOut("SspTimerProc");
#endif

	if(updown_flag == SSP_SCROLL_UP)
	{
		PostMessage(hwndMouse, WM_VSCROLL, (WPARAM)SB_LINEUP, (LPARAM)NULL);
	cnt --;
#ifdef _DEBUG
	SspTraceOut("SSP_SCROLL_UP");
#endif
	}
	else if(updown_flag == SSP_SCROLL_DOWN)
	{
		PostMessage(hwndMouse, WM_VSCROLL, (WPARAM)SB_LINEDOWN, (LPARAM)NULL);
	cnt --;
#ifdef _DEBUG
	SspTraceOut("SSP_SCROLL_NONE");
#endif
	}
	if (cnt==0) updown_flag = SSP_SCROLL_NONE; 

//	char buf[10];
//	sprintf(buf, "%d", cnt);
//	SspTraceOut(buf);
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
	wsprintf(szOut, "SspButtonProc : hwndMouse = %x, dwType = %d",
		pParam->hwndMouse, pParam->dwType);
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



	switch(pParam->dwDirection)
	{
	case SSP_WHEEL_FW:
		//keybd_event(VK_CONTROL, 0x1D, 0, 0);
		//mouse_event(MOUSEEVENTF_WHEEL, 0, 0, WHEEL_DELTA, 0);
		//keybd_event(VK_CONTROL, 0x1D, KEYEVENTF_KEYUP, 0);

		return SspSendKeyEvent(pParam->hwndMouse, 10, sspSetting.sbiButton[10].uiFunction, SSP_BUTTON_UP);

		break;
	case SSP_WHEEL_BK:
		//keybd_event(VK_CONTROL, 0x1D, 0, 0);
		//mouse_event(MOUSEEVENTF_WHEEL, 0, 0, -WHEEL_DELTA, 0);
		//keybd_event(VK_CONTROL, 0x1D, KEYEVENTF_KEYUP, 0);

		return SspSendKeyEvent(pParam->hwndMouse, 10, sspSetting.sbiButton[11].uiFunction, SSP_BUTTON_UP);

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


	if(pParam->dwDirection & SSP_SCROLL_UP)
	{
		updown_flag = SSP_SCROLL_UP;
		cnt = 5;
		hwndMouse = pParam->hwndMouse;
	}
	else if(pParam->dwDirection & SSP_SCROLL_DOWN)
	{
		updown_flag = SSP_SCROLL_DOWN;
		cnt = 5;
		hwndMouse = pParam->hwndMouse;
	}
	if(pParam->dwDirection & SSP_SCROLL_LEFT)
	{

	}
	else if(pParam->dwDirection & SSP_SCROLL_RIGHT)
	{
	}	

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
			for(int i=IDC_COMBO1; i<=IDC_COMBO_WHEEL+1; i++)
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
		case IDC_COMBO_WHEEL2:
		case IDC_COMBO_WHEEL3:
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
	switch(uiFunction)
	{
	case 0:		//	新しいウィンドウを開く: Ctrl+N
		if(uiSW == SSP_BUTTON_UP)
		{
			keybd_event(VK_CONTROL, 0x1D, 0, 0);
			keybd_event('N', 0x31, 0, 0);
			keybd_event('N', 0x31, KEYEVENTF_KEYUP, 0);
			keybd_event(VK_CONTROL, 0x1D, KEYEVENTF_KEYUP, 0);
		}
		break;
	case 1:		//新しいタブを開く: Ctrl+T
		if(uiSW == SSP_BUTTON_UP)
		{
			keybd_event(VK_CONTROL, 0x1D, 0, 0);
			keybd_event('T', 0x18, 0, 0);
			keybd_event('T', 0x18, KEYEVENTF_KEYUP, 0);
			keybd_event(VK_CONTROL, 0x1D, KEYEVENTF_KEYUP, 0);
		}
		break;
	case 2:		//最近閉じたタブをもう一度開く: Ctrl+Shift+T
		if(uiSW == SSP_BUTTON_UP)
		{
			keybd_event(VK_CONTROL, 0x1D, 0, 0);
			keybd_event(VK_SHIFT, 0x1D, 0, 0);
			keybd_event('T', 0x1F, 0, 0);
			keybd_event('T', 0x1F, KEYEVENTF_KEYUP, 0);
			keybd_event(VK_SHIFT, 0x1D, KEYEVENTF_KEYUP, 0);
			keybd_event(VK_CONTROL, 0x1D, KEYEVENTF_KEYUP, 0);
		}
		break;
	case 3:		//最後のタブに切り替える: Ctrl+9
		if(uiSW == SSP_BUTTON_UP)
		{
			keybd_event(VK_CONTROL, 0x1D, 0, 0);
			keybd_event('9', 0x1F, 0, 0);
			keybd_event('9', 0x1F, KEYEVENTF_KEYUP, 0);
			keybd_event(VK_CONTROL, 0x1D, KEYEVENTF_KEYUP, 0);
		}
		break;
	case 4:		//次のタブに切り替える: Ctrl+Tab または Ctrl+PageDown
		if(uiSW == SSP_BUTTON_UP)
		{
			keybd_event(VK_CONTROL, 0x1D, 0, 0);
			keybd_event(VK_TAB, 0x2E, 0, 0);
			keybd_event(VK_TAB, 0x2E, KEYEVENTF_KEYUP, 0);
			keybd_event(VK_CONTROL, 0x1D, KEYEVENTF_KEYUP, 0);
		}
		break;
	case 5:		//前のタブに切り替える: Ctrl+Shift+Tab または Ctrl+PageUp
		if(uiSW == SSP_BUTTON_UP)
		{
			keybd_event(VK_CONTROL, 0x1D, 0, 0);
			keybd_event(VK_SHIFT, 0x1D, 0, 0);
			keybd_event(VK_TAB, 0x2E, 0, 0);
			keybd_event(VK_TAB, 0x2E, KEYEVENTF_KEYUP, 0);
			keybd_event(VK_SHIFT, 0x1D, KEYEVENTF_KEYUP, 0);
			keybd_event(VK_CONTROL, 0x1D, KEYEVENTF_KEYUP, 0);
		}
		break;
	case 6:		//現在のウィンドウを閉じる: Alt+F4
		if(uiSW == SSP_BUTTON_UP)
		{
			keybd_event(VK_LMENU, 0x1D, 0, 0);
			keybd_event(VK_F4, 0x2F, 0, 0);
			keybd_event(VK_F4, 0x2F, KEYEVENTF_KEYUP, 0);
			keybd_event(VK_LMENU, 0x1D, KEYEVENTF_KEYUP, 0);
		}
		break;
	case 7:		//現在のタブまたはポップアップを閉じる: Ctrl+W または Ctrl+F4
		if(uiSW == SSP_BUTTON_UP)
		{
			keybd_event(VK_CONTROL, 0x1D, 0, 0);
			keybd_event('W', 0x2C, 0, 0);
			keybd_event('W', 0x2C, KEYEVENTF_KEYUP, 0);
			keybd_event(VK_CONTROL, 0x1D, KEYEVENTF_KEYUP, 0);
		}
		break;
	case 8:		//ブックマーク バーの表示/非表示を切り替える: Ctrl+Shift+B
		if(uiSW == SSP_BUTTON_UP)
		{
			keybd_event(VK_CONTROL, 0x1D, 0, 0);
			keybd_event(VK_SHIFT, 0x1D, 0, 0);
			keybd_event('B', 0x1F, 0, 0);
			keybd_event('B', 0x1F, KEYEVENTF_KEYUP, 0);
			keybd_event(VK_SHIFT, 0x1D, KEYEVENTF_KEYUP, 0);
			keybd_event(VK_CONTROL, 0x1D, KEYEVENTF_KEYUP, 0);
		}
		break;
	case 9:		//履歴ページを開く: Ctrl+H
		if(uiSW == SSP_BUTTON_UP)
		{
			keybd_event(VK_CONTROL, 0x1D, 0, 0);
			keybd_event('H', 0x2C, 0, 0);
			keybd_event('H', 0x2C, KEYEVENTF_KEYUP, 0);
			keybd_event(VK_CONTROL, 0x1D, KEYEVENTF_KEYUP, 0);
		}
		break;
	case 10:	//ダウンロード ページを開く: Ctrl+J
		if(uiSW == SSP_BUTTON_UP)
		{
			keybd_event(VK_CONTROL, 0x1D, 0, 0);
			keybd_event('J', 0x2C, 0, 0);
			keybd_event('J', 0x2C, KEYEVENTF_KEYUP, 0);
			keybd_event(VK_CONTROL, 0x1D, KEYEVENTF_KEYUP, 0);
		}
		break;
	case 11:	//デベロッパー ツールを開く: Ctrl+Shift+J
		if(uiSW == SSP_BUTTON_UP)
		{
			keybd_event(VK_CONTROL, 0x1D, 0, 0);
			keybd_event(VK_SHIFT, 0x1D, 0, 0);
			keybd_event('J', 0x1F, 0, 0);
			keybd_event('J', 0x1F, KEYEVENTF_KEYUP, 0);
			keybd_event(VK_SHIFT, 0x1D, KEYEVENTF_KEYUP, 0);
			keybd_event(VK_CONTROL, 0x1D, KEYEVENTF_KEYUP, 0);
		}
		break;
	case 12:	//[閲覧履歴の消去] ダイアログを開く: Ctrl+Shift+Del
		if(uiSW == SSP_BUTTON_UP)
		{
			keybd_event(VK_CONTROL, 0x1D, 0, 0);
			keybd_event(VK_SHIFT, 0x1D, 0, 0);
			keybd_event(VK_DELETE, 0x1F, 0, 0);
			keybd_event(VK_DELETE, 0x1F, KEYEVENTF_KEYUP, 0);
			keybd_event(VK_SHIFT, 0x1D, KEYEVENTF_KEYUP, 0);
			keybd_event(VK_CONTROL, 0x1D, KEYEVENTF_KEYUP, 0);
		}
		break;
	case 13:	//ページ全体を拡大表示: Ctrl 押しながら + を押すか、Ctrl を押しながらマウスホイールを上方向に動かす
		if(uiSW == SSP_BUTTON_UP)
		{
			keybd_event(VK_CONTROL, 0x1D, 0, 0);
			keybd_event('+', 0x2C, 0, 0);
			keybd_event('+', 0x2C, KEYEVENTF_KEYUP, 0);
			keybd_event(VK_CONTROL, 0x1D, KEYEVENTF_KEYUP, 0);
		}
		break;
	case 14:	//ページ全体を縮小表示: Ctrl を押しながら - を押すか、Ctrl を押しながらマウスホイールを下方向に動かす
		if(uiSW == SSP_BUTTON_UP)
		{
			keybd_event(VK_CONTROL, 0x1D, 0, 0);
			keybd_event('-', 0x2C, 0, 0);
			keybd_event('-', 0x2C, KEYEVENTF_KEYUP, 0);
			keybd_event(VK_CONTROL, 0x1D, KEYEVENTF_KEYUP, 0);
		}
		break;
	case 15:	//ページ全体を通常のサイズに戻す: Ctrl+0
		if(uiSW == SSP_BUTTON_UP)
		{
			keybd_event(VK_CONTROL, 0x1D, 0, 0);
			keybd_event('0', 0x2C, 0, 0);
			keybd_event('0', 0x2C, KEYEVENTF_KEYUP, 0);
			keybd_event(VK_CONTROL, 0x1D, KEYEVENTF_KEYUP, 0);
		}
		break;
	case 16:	//ページの先頭に移動: Home
		if(uiSW == SSP_BUTTON_UP)
		{
			keybd_event(VK_CONTROL, 0x1D, 0, 0);
			keybd_event(VK_HOME, 0x2C, 0, 0);
			keybd_event(VK_HOME, 0x2C, KEYEVENTF_KEYUP, 0);
			keybd_event(VK_CONTROL, 0x1D, KEYEVENTF_KEYUP, 0);
		}
		break;
	case 17:	//ページの最後に移動: End
		if(uiSW == SSP_BUTTON_UP)
		{
			keybd_event(VK_CONTROL, 0x1D, 0, 0);
			keybd_event(VK_END, 0x2C, 0, 0);
			keybd_event(VK_END, 0x2C, KEYEVENTF_KEYUP, 0);
			keybd_event(VK_CONTROL, 0x1D, KEYEVENTF_KEYUP, 0);
		}
		break;
	case 18:		// 表示／非表示
		if(uiSW == SSP_BUTTON_UP)
		{
			return SSP_SHOW_HIDE;
		}
		break;
	case KEYSTROKE_NO:	// キーストローク
		SspKeyStroke(&(sspSetting.sbiButton[uiButton]), uiSW);
		break;
	}
	return SSP_TRUE;
}

/////////////////////////////////   End of File   ////////////////////////////////////
