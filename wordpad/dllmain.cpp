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

// �v���O�C���̐ݒ����ۊǂ��郌�W�X�g����
#define REG_KEY			"Wordpad"
// �쐬�Җ�
#define AUTHOR			"WACOM"
// �t�@���N�V�����L�[�ɐݒ�\�ȋ@�\��
#define FUNCTION_NUM	10
// �L�[�X�g���[�N�@�\�̔ԍ�
#define KEYSTROKE_NO	9

// �O���[�o���ϐ�
HINSTANCE	ghInst=NULL;
// �ݒ���
SSP_SETTING	sspSetting;
// �f�t�H���g�ݒ���(�t�@���N�V�����L�[�P�`�W�A�z�C�[���{�^���A�쐬�҂̏��j
// �����Ƀf�t�H���g�ݒ���L�q���Ă�������
SSP_DEFAULT	sspDefault = { 0, 1, 2, 3, 4, 5, 6, 7, 8, AUTHOR };

SSP_STATUS WINAPI SspSendKeyEvent(HWND hWnd, UINT uiButton, UINT uiFunction, UINT uiSW);

///////////////////////////////////////////////////////////////////////////////////
BOOL APIENTRY
DllMain(HINSTANCE hModule, DWORD dwReason, LPVOID lpReserved )
{
	if(dwReason == DLL_PROCESS_ATTACH)
	{
		// ���W�X�g������ݒ����ǂݍ��݂܂�
		SspLoadRegSetting(REG_KEY, &sspSetting, &sspDefault);

		ghInst = hModule;
	}

    UNREFERENCED_PARAMETER(lpReserved);

    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////
// < �@�\ >
//  �v���O�C���t�@�C�����h���C�o�ɂ�胍�[�h���ꂽ���ɌĂ΂��֐��ł�
// < �p�����[�^ >
//	pParam->dwModel		: SmartScroll�̃��f���ԍ�
//	pParam->dwVersion	: SmartScroll�h���C�o�̃o�[�W����
// < �߂�l >
// SSP_TRUE		: ���̃v���O�C���͏풓����܂�
// SSP_FALSE	: ���̃v���O�C���̏풓�͉�������܂�
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
// < �@�\ >
//  �t�H�A�O���E���h�E�B���h�E�ɑ΂��āA���̃v���O�C����K�p���邩�ǂ����𔻒f����֐��ł�
// < �p�����[�^ >
// hWnd		: �t�H�A�O���E���h�E�B���h�E�̃n���h��
// < �߂�l >
// SSP_TRUE		: ���̃v���O�C�����K�p�����
// SSP_FALSE	: ���̃v���O�C���͓K�p����Ȃ�
SSP_STATUS WINAPI
SspApplyProc(HWND hWnd)
{
#ifdef _DEBUG
	char	szOut[100];
	wsprintf(szOut, "SspApplyProc : hWnd = %x", hWnd);
	SspTraceOut(szOut);
#endif

	// �E�B���h�E�N���X�����A"WordPadClass"�̏ꍇ�ɓK�p����܂�
	char szClass[100];
	GetClassName(hWnd, szClass, sizeof(szClass));
	if(!strcmp(szClass, "WordPadClass"))
		return SSP_TRUE;

	return SSP_FALSE;
}

///////////////////////////////////////////////////////////////////////////////////
// < �@�\ >
// �v���O�C�����K�p����鎞�A�ŏ��ɌĂяo�����֐��ł�
// < �p�����[�^ >
//	pParam->hwndApp		: �t�H�A�O���E���h�E�B���h�E�̃n���h��
//	pParam->dwModel		: SmartScroll�̃��f���ԍ�
//	pParam->dwVersion	: SmartScroll�h���C�o�̃o�[�W����
// < �߂�l >
// SSP_TRUE		: �ʏ�͂��̒l��Ԃ��Ă�������
// SSP_FALSE	: �v���O�C���̓K�p����������܂�
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
//	SspTimerProc�֐����g�p����ꍇ�́ASSP_TIMER��߂�l�Ƃ��ĕԂ��Ă��������B
//	(�h���C�o�o�[�W����1.20�ȍ~�őΉ��j
//	return SSP_TIMER;
}

///////////////////////////////////////////////////////////////////////////////////
// < �@�\ >
// ��100ms�Ԋu�ŌĂяo�����^�C�}�֐��ł��B
//�i�h���C�o�o�[�W����1.20�ȍ~�őΉ��j
void WINAPI
SspTimerProc(void)
{
#ifdef _DEBUG
	SspTraceOut("SspTimerProc");
#endif
}

///////////////////////////////////////////////////////////////////////////////////
// < �@�\ >
// �v���O�C���̓K�p���I�����鎞�ɌĂяo�����֐��ł�
void WINAPI
SspReleaseProc(void)
{
#ifdef _DEBUG
	SspTraceOut("SspReleaseProc()");
#endif
}

///////////////////////////////////////////////////////////////////////////////////
// < �@�\ >
// �t�@���N�V�����L�[�����삳�ꂽ�ꍇ�̏������s���֐��ł�
// < �p�����[�^ >
//	pParam->hwndMouse	: �}�E�X�J�[�\���̐^���ɂ���E�B���h�E�̃n���h��
//	pParam->dwType		: �C�x���g�^�C�v
// < �߂�l >
// SSP_TRUE		: �ʏ�͂��̒l��Ԃ��Ă�������
// SSP_SHOW_HIDE: SmartScroll�E�B���h�E�̕\���^��\�����؂�ւ��܂�
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
// < �@�\ >
// �Y�[���z�C�[���@�\�̗L���^�����𔻒f����֐��ł�
// < �߂�l >
// SSP_TRUE		: �Y�[���z�C�[���͗L���ł�
// SSP_FALSE	: �Y�[���z�C�[���͖����ł�
SSP_STATUS WINAPI
SspGetWheelEffect(void)
{
#ifdef _DEBUG
	SspTraceOut("SspGetWheelEffect");
#endif

	return SSP_TRUE;
}

///////////////////////////////////////////////////////////////////////////////////
// < �@�\ >
// �Y�[���z�C�[�������삳�ꂽ�ꍇ�̏������s���֐��ł�
// < �p�����[�^ >
//	pParam->hwndMouse	: �}�E�X�J�[�\���̐^���ɂ���E�B���h�E�̃n���h��
//	pParam->lRoll		: �Y�[���z�C�[���̉�]��
//	pParam->dwDirection	: ��]�����t���O
// < �߂�l >
// SSP_TRUE		: �f�t�H���g�̃Y�[�������͍s���܂���
// SSP_FALSE	: �f�t�H���g�̃Y�[���������s���܂�
SSP_STATUS WINAPI
SspWheelProc(PWHEELPARAM pParam)
{
#ifdef _DEBUG
	char	szOut[200];
	wsprintf(szOut, "SspWheelProc : hwndMouse = %x, lRoll = %d, dwDirection = %d",
		pParam->hwndMouse, pParam->lRoll, pParam->dwDirection);
	SspTraceOut(szOut);
#endif
	// SSP_FALSE��Ԃ��A�f�t�H���g�̃Y�[���������g�p���Ă����퓮�삵�܂��B
	// �ȉ���Ctrl+�z�C�[�����b�Z�[�W�𑗐M����T���v���ł��B
	switch(pParam->dwDirection)
	{
	case SSP_WHEEL_FW:
		keybd_event(VK_CONTROL, 0x1D, 0, 0);
		mouse_event(MOUSEEVENTF_WHEEL, 0, 0, WHEEL_DELTA, 0);
		keybd_event(VK_CONTROL, 0x1D, KEYEVENTF_KEYUP, 0);
		break;
	case SSP_WHEEL_BK:
		keybd_event(VK_CONTROL, 0x1D, 0, 0);
		mouse_event(MOUSEEVENTF_WHEEL, 0, 0, -WHEEL_DELTA, 0);
		keybd_event(VK_CONTROL, 0x1D, KEYEVENTF_KEYUP, 0);
		break;
	}
	return SSP_TRUE;
}

///////////////////////////////////////////////////////////////////////////////////
// < �@�\ >
// �X�N���[���{�[�������삳�ꂽ�ꍇ�̏������s���֐��ł�
// < �p�����[�^ >
//	pParam->hwndMouse	: �}�E�X�J�[�\���̐^���ɂ���E�B���h�E�̃n���h��
//	pParam->hwndOwner	: hwndMouse�̐e�E�B���h�E�̃n���h��
//	pParam->hwndHScroll	: ���X�N���[���o�[�R���g���[���̃n���h��
//	pParam->hwndVScroll	: �c�X�N���[���o�[�R���g���[���̃n���h��
//	pParam->lX			: �X�N���[���{�[����X�����̉�]��
//	pParam->lY			: �X�N���[���{�[����Y�����̉�]��
//	pParam->dwDirection	: ��]�����t���O
// < �߂�l >
// SSP_TRUE		: �f�t�H���g�̃X�N���[�������͍s���܂���
// SSP_FALSE	: �f�t�H���g�̃X�N���[���������s���܂�
SSP_STATUS WINAPI
SspScrollProc(PSCROLLPARAM pParam)
{
#ifdef _DEBUG
	char	szOut[200];
	wsprintf(szOut, "SspScrollProc : hwndMouse = %x, hwndOwner = %x, hwndHScroll = %x, hwndVScroll = %x, lX = %d, lY = %d, dwDirection = %d",
		pParam->hwndMouse, pParam->hwndOwner, pParam->hwndHScroll, pParam->hwndVScroll, pParam->lX, pParam->lY, pParam->dwDirection);
	SspTraceOut(szOut);
#endif
	// SSP_FALSE��Ԃ��A�f�t�H���g�̃X�N���[�������Ő��퓮�삵�܂��B
	return SSP_FALSE;

	// �ȉ��̓X�N���[�����b�Z�[�W�𑗐M����T���v���ł��B
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
// < �@�\ >
// SmartScroll�p�b�h�̃{�^���ɕ\�������A�C�R���n���h����񋟂���֐��ł�
// < �p�����[�^ >
//	dwType	: �{�^���̎��
// < �߂�l >
// �e�{�^���p�̃A�C�R���n���h����Ԃ��܂�
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
		// �L�[�X�g���[�N�̏ꍇ
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
// < �@�\ >
// �v���O�C�����������񃊃\�[�X��񋟂���֐��ł�
// < �p�����[�^ >
//	dwType	: ���\�[�X�̎��
//	lpStr	: ��������󂯎��o�b�t�@���w���|�C���^
//	uiSize	: �o�b�t�@�̃T�C�Y
// < �߂�l >
// �o�b�t�@�ɃR�s�[���ꂽ��������Ԃ��܂�
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
		// �L�[�X�g���[�N�̏ꍇ
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
// < �@�\ >
// �ݒ�_�C�A���O�p�̃_�C�A���O�v���V�[�W���ł�
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
			// �R���{�{�b�N�X�̏��������s���܂�
			for(int i=0; i<FUNCTION_NUM; i++)
			{
				char	szText[50];
				LoadString(ghInst, SSP_STRING_BASE+i, szText, sizeof(szText));

				for(int j=IDC_COMBO1; j<=IDC_COMBO_WHEEL; j++)
				{
					SendMessage(GetDlgItem(hWnd, j), CB_ADDSTRING, (WPARAM)0, (LPARAM)szText);
				}
			}
			// �����l�̐ݒ���s���܂�
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
				// �R���{�{�b�N�X�̕`����s���܂��i�A�C�R���\���t���j
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
			// ���W�X�g���ɐݒ����ۑ����܂�
			SspSaveRegSetting(REG_KEY, &sspSetting);

			EndDialog(hWnd, 0);
			return TRUE;

		// �R���{�{�b�N�X���I�����ꂽ���̏������s���܂�
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

				// �L�[�X�g���[�N�̏ꍇ�͐ݒ�p�_�C�A���O���Ăяo���܂�
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

		// �f�t�H���g�ݒ�ɂ��܂�
		case IDC_DEFAULT:
			{
				for(int i=0; i<SSP_BUTTON_NUM; i++)
				{
					sspSetting.sbiButton[i].uiFunction = sspDefault.uiButton[i];
					SendMessage(GetDlgItem(hWnd, IDC_COMBO1+i), CB_SETCURSEL, (WPARAM)sspSetting.sbiButton[i].uiFunction, (LPARAM)0);
					// �L�[�X�g���[�N�����N���A���܂�
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
// < �@�\ >
// �e�@�\�ɍ����ăA�v���P�[�V�����ɑ΂��ă��b�Z�[�W�𑗐M����֐��ł�
SSP_STATUS WINAPI
SspSendKeyEvent(HWND hWnd, UINT uiButton, UINT uiFunction, UINT uiSW)
{
	switch(uiFunction)
	{
	case 0:		//	�V�K
		if(uiSW == SSP_BUTTON_UP)
		{
			keybd_event(VK_CONTROL, 0x1D, 0, 0);
			keybd_event('N', 0x31, 0, 0);
			keybd_event('N', 0x31, KEYEVENTF_KEYUP, 0);
			keybd_event(VK_CONTROL, 0x1D, KEYEVENTF_KEYUP, 0);
		}
		break;
	case 1:		//	�J��
		if(uiSW == SSP_BUTTON_UP)
		{
			keybd_event(VK_CONTROL, 0x1D, 0, 0);
			keybd_event('O', 0x18, 0, 0);
			keybd_event('O', 0x18, KEYEVENTF_KEYUP, 0);
			keybd_event(VK_CONTROL, 0x1D, KEYEVENTF_KEYUP, 0);
		}
		break;
	case 2:		//	�ۑ�
		if(uiSW == SSP_BUTTON_UP)
		{
			keybd_event(VK_CONTROL, 0x1D, 0, 0);
			keybd_event('S', 0x1F, 0, 0);
			keybd_event('S', 0x1F, KEYEVENTF_KEYUP, 0);
			keybd_event(VK_CONTROL, 0x1D, KEYEVENTF_KEYUP, 0);
		}
		break;
	case 3:		//	����v���r���[
		if(uiSW == SSP_BUTTON_UP)
		{
			keybd_event(VK_MENU, 0x38, 0, 0);
			keybd_event('F', 0x21, 0, 0);
			keybd_event('F', 0x21, KEYEVENTF_KEYUP, 0);
			keybd_event(VK_MENU, 0x38, KEYEVENTF_KEYUP, 0);
			keybd_event('V', 0x2F, 0, 0);
			keybd_event('V', 0x2F, KEYEVENTF_KEYUP, 0);
		}
		break;
	case 4:		//	�R�s�[
		if(uiSW == SSP_BUTTON_UP)
		{
			keybd_event(VK_CONTROL, 0x1D, 0, 0);
			keybd_event('C', 0x2E, 0, 0);
			keybd_event('C', 0x2E, KEYEVENTF_KEYUP, 0);
			keybd_event(VK_CONTROL, 0x1D, KEYEVENTF_KEYUP, 0);
		}
		break;
	case 5:		//	�؂���
		if(uiSW == SSP_BUTTON_UP)
		{
			keybd_event(VK_CONTROL, 0x1D, 0, 0);
			keybd_event('X', 0x2D, 0, 0);
			keybd_event('X', 0x2D, KEYEVENTF_KEYUP, 0);
			keybd_event(VK_CONTROL, 0x1D, KEYEVENTF_KEYUP, 0);
		}
		break;
	case 6:		//	�\��t��
		if(uiSW == SSP_BUTTON_UP)
		{
			keybd_event(VK_CONTROL, 0x1D, 0, 0);
			keybd_event('V', 0x2F, 0, 0);
			keybd_event('V', 0x2F, KEYEVENTF_KEYUP, 0);
			keybd_event(VK_CONTROL, 0x1D, KEYEVENTF_KEYUP, 0);
		}
		break;
	case 7:		//	���ɖ߂�
		if(uiSW == SSP_BUTTON_UP)
		{
			keybd_event(VK_CONTROL, 0x1D, 0, 0);
			keybd_event('Z', 0x2C, 0, 0);
			keybd_event('Z', 0x2C, KEYEVENTF_KEYUP, 0);
			keybd_event(VK_CONTROL, 0x1D, KEYEVENTF_KEYUP, 0);
		}
		break;
	case 8:		// �\���^��\��
		if(uiSW == SSP_BUTTON_UP)
		{
			return SSP_SHOW_HIDE;
		}
		break;
	case KEYSTROKE_NO:	// �L�[�X�g���[�N
		SspKeyStroke(&(sspSetting.sbiButton[uiButton]), uiSW);
		break;
	}
	return SSP_TRUE;
}

/////////////////////////////////   End of File   ////////////////////////////////////
