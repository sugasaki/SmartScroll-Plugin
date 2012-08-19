/*-----------------------------------------------------------------------------
*  File : sspfunc.h
*
*  Comment : SmartScroll Plugin用インクルードファイル
*            このファイルは変更しないでください
*
*  Copyright WACOM Co., Ltd. 2002
*
--------------------------------------------------------------------------------*/
#ifndef _SSPFUNC_H_
#define _SSPFUNC_H_

#define SSP_AUTHOR_LENGTH		20
#define SSP_ICON_SIZE			16
#define SSP_ICON_BASE			100
#define SSP_STRING_BASE			100

#define SSP_BUTTON_DOWN			0
#define SSP_BUTTON_UP			1

#define SSP_BUFFER_LENGTH		256
#define SSP_KEY_MAX				20
#define SSP_KEY_TEXT_MAX		SSP_KEY_MAX*10

#define SSP_BUTTON_NUM			9

/* Model */
#define SSP_SMART_SCROLL		0118

#ifndef WHEEL_DELTA
#define WHEEL_DELTA				120
#endif

enum PLATFORM
{
	eUNKNOWN,
	eWIN95,
	eWIN95OSR2,
	eWIN98,
	eWINME,
	eWINNT40,
	eWIN2000,
	eWINXP
};

/*----------------------------   Structure   ----------------------------*/
/* SSP_DEFAULT */
typedef struct _SSP_DEFAULT
{
	UINT	uiButton[SSP_BUTTON_NUM];
	char	szAuthor[SSP_AUTHOR_LENGTH];
} SSP_DEFAULT, *PSSP_DEFAULT;


/* SSP_BUTTONINFO */
typedef struct _SSP_BUTTONINFO
{
	UINT	uiFunction;
	unsigned char	szKeys[SSP_KEY_MAX];
	DWORD	dwKeyIcon;
} SSP_BUTTONINFO, *PSSP_BUTTONINFO;

/* SSP_SETTING */
typedef struct _SSP_SETTING
{
	SSP_BUTTONINFO	sbiButton[SSP_BUTTON_NUM];
	char	szAuthor[SSP_AUTHOR_LENGTH];
} SSP_SETTING, *PSSP_SETTING;


/*-----------------------------   Function   -----------------------------*/
/* Registry */
BOOL WINAPI SspLoadRegSetting(const char* pszKey, PSSP_SETTING pSet, PSSP_DEFAULT pDefault);
BOOL WINAPI SspLoadRegDWord(const char* pszKey, const char* pszName, PDWORD pdwValue);
BOOL WINAPI SspLoadRegString(const char* pszKey, const char* pszaName, char* pszStr, UINT uiSize);
BOOL WINAPI SspLoadRegBinary(const char* pszKey, const char* pszName, unsigned char* pszBuff, UINT uiSize);
BOOL WINAPI SspSaveRegSetting(const char* pszKey, PSSP_SETTING pSet);
BOOL WINAPI SspSaveRegDWord(const char* pszKey, const char* pszName, DWORD dwValue);
BOOL WINAPI SspSaveRegString(const char* pszKey, const char* pszName, char* pszStr);
BOOL WINAPI SspSaveRegBinary(const char* pszKey, const char* pszName, unsigned char* pszBuff, UINT uiSize);
BOOL WINAPI SspDeleteRegKey(const char* pszKey);
BOOL WINAPI SspDeleteRegValue(const char* pszKey, const char* pszValue);

/* Dialog */
void WINAPI SspDrawComboBox(HINSTANCE hInst, LPDRAWITEMSTRUCT lpdis);

/* KeyStroke */
void WINAPI SspKeyStroke(PSSP_BUTTONINFO pInfo, UINT uiSW);
BOOL WINAPI SspSetKeyStroke(HWND hWnd, PSSP_BUTTONINFO pInfo);
int WINAPI SspGetKeyText(PSSP_BUTTONINFO pInfo, char* pszText, UINT uiSize);
HICON WINAPI SspGetKeyIcon(PSSP_BUTTONINFO pInfo);
void WINAPI SspClearKeyInfo(PSSP_BUTTONINFO pInfo);

/* Debug */
void WINAPI SspTraceOut(char* pszOut);

/* etc */
PLATFORM WINAPI SspGetPlatform(void);
UINT WINAPI SspGetFileName(HWND hWnd, LPSTR pszFile, UINT uiSize);
BOOL WINAPI SspGetFileVersion(LPSTR pFile, PDWORD pdwFileVersionMS, PDWORD pdwFileVersionLS);
void WINAPI SspLoadPluginResource(void);

#endif
/*-----------------------------   End of File   -----------------------------*/
