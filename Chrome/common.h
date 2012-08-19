//
// File : common.h
// 
// Comment : SmartScroll Plugin用インクルードファイル
//           このファイルは変更しないでください
// 
// Copyright Wacom Co., Ltd. 2001
//
/////////////////////////////////////////////////////////////////////
#ifndef _COMMON_
#define _COMMON_

/////////////////////////////////////////////////////////////////////
#define SSP_NAME				100
#define SSP_NAME_EX				101

#define SSP_BUTTON1				0
#define SSP_BUTTON2				1
#define SSP_BUTTON3				2
#define SSP_BUTTON4				3
#define SSP_BUTTON5				4
#define SSP_BUTTON6				5
#define SSP_BUTTON7				6
#define SSP_BUTTON8				7
#define SSP_BUTTON_WHEEL		8

#define SSP_BUTTON1_DOWN		0x00
#define SSP_BUTTON1_UP			0x01
#define SSP_BUTTON2_DOWN		0x10
#define SSP_BUTTON2_UP			0x11
#define SSP_BUTTON3_DOWN		0x20
#define SSP_BUTTON3_UP			0x21
#define SSP_BUTTON4_DOWN		0x30
#define SSP_BUTTON4_UP			0x31
#define SSP_BUTTON5_DOWN		0x40
#define SSP_BUTTON5_UP			0x41
#define SSP_BUTTON6_DOWN		0x50
#define SSP_BUTTON6_UP			0x51
#define SSP_BUTTON7_DOWN		0x60
#define SSP_BUTTON7_UP			0x61
#define SSP_BUTTON8_DOWN		0x70
#define SSP_BUTTON8_UP			0x71
#define SSP_BUTTON_WHEEL_DOWN	0x80
#define SSP_BUTTON_WHEEL_UP		0x81

#define BUTTON_NO(a)			a >> 4
#define BUTTON_EVENT(a)			a & 0x0F

#define SSP_WHEEL_FW			1
#define SSP_WHEEL_BK			2

#define SSP_SCROLL_NONE			0x00000000
#define SSP_SCROLL_UP			0x00000001
#define SSP_SCROLL_DOWN			0x00000002
#define SSP_SCROLL_LEFT			0x00000004
#define SSP_SCROLL_RIGHT		0x00000008

enum SSP_STATUS
{
	SSP_FALSE,
	SSP_TRUE,
	SSP_SHOW_HIDE,
	SSP_SWITCH_PROTOCOL,
	SSP_TIMER
};

/////////////////////////////////////////////////////////////////////
typedef struct _BUTTONPARAM
{
	HWND		hwndMouse;
	DWORD		dwType;
} BUTTONPARAM, *PBUTTONPARAM;

typedef struct _WHEELPARAM
{
	HWND		hwndMouse;
	LONG		lRoll;
	DWORD		dwDirection;
} WHEELPARAM, *PWHEELPARAM;

typedef struct _SCROLLPARAM
{
	HWND		hwndMouse;
	HWND		hwndOwner;
	HWND		hwndHScroll;
	HWND		hwndVScroll;
	long		lX;
	long		lY;
	DWORD		dwDirection;
} SCROLLPARAM, *PSCROLLPARAM;

typedef struct _INITPARAM
{
	HWND		hwndApp;
	DWORD		dwModel;
	DWORD		dwVersion;
} INITPARAM, *PINITPARAM;

typedef struct _LOADPARAM
{
	DWORD		dwModel;
	DWORD		dwVersion;
} LOADPARAM, *PLOADPARAM;

#endif
/////////////////////////////////////////////////////////////////////
