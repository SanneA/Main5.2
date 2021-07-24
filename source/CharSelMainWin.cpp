//*****************************************************************************
// File: CharSelMainWin.cpp
// producer: Ahn Sang-Kyu
//*****************************************************************************

#include "stdafx.h"
#include "CharSelMainWin.h"
#include "Input.h"
#include "UIMng.h"
#include "ZzzBMD.h"
#include "ZzzInfomation.h"
#include "ZzzObject.h"
#include "ZzzCharacter.h"
#include "ZzzInterface.h"
#include "UIGuildInfo.h"
#include "ZzzOpenData.h"
#include "ZzzOpenglUtil.h"

#ifdef KJH_ADD_SERVER_LIST_SYSTEM
#include "ServerListManager.h"
#endif // KJH_ADD_SERVER_LIST_SYSTEM
#ifdef PBG_ADD_CHARACTERSLOT
#include "Slotlimit.h"
#endif //PBG_ADD_CHARACTERSLOT

extern float g_fScreenRate_x;
extern float g_fScreenRate_y;
#ifndef KJH_ADD_SERVER_LIST_SYSTEM			// #ifndef
extern int ServerSelectHi;
#endif // KJH_ADD_SERVER_LIST_SYSTEM

#ifdef KJH_MOD_NATION_LANGUAGE_REDEFINE
#ifdef TEENAGER_REGULATION
#define	CSMW_WARNING_TEXT_MAX	3
#endif // TEENAGER_REGULATION
#else // KJH_MOD_NATION_LANGUAGE_REDEFINE
#if (SELECTED_LANGUAGE == LANGUAGE_KOREAN) && defined TEENAGER_REGULATION
#define	CSMW_WARNING_TEXT_MAX	3	// ��� ���� ����.
#endif	// (SELECTED_LANGUAGE == LANGUAGE_KOREAN) && defined TEENAGER_REGULATION
#endif // KJH_MOD_NATION_LANGUAGE_REDEFINE

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCharSelMainWin::CCharSelMainWin()
{

}

CCharSelMainWin::~CCharSelMainWin()
{

}

void CCharSelMainWin::Create()
{
	m_asprBack[CSMW_SPR_DECO].Create(189, 103, BITMAP_LOG_IN+2);
	m_asprBack[CSMW_SPR_INFO].Create(
	CInput::Instance().GetScreenWidth() - 266, 21);
	m_asprBack[CSMW_SPR_INFO].SetColor(0, 0, 0);
	m_asprBack[CSMW_SPR_INFO].SetAlpha(143);


	m_aBtn[CSMW_BTN_CREATE].Create(54, 30, BITMAP_LOG_IN+3, 4, 2, 1, 3);
	m_aBtn[CSMW_BTN_MENU].Create(54, 30, BITMAP_LOG_IN+4, 3, 2, 1);
	m_aBtn[CSMW_BTN_CONNECT].Create(54, 30, BITMAP_LOG_IN+5, 4, 2, 1, 3);
	m_aBtn[CSMW_BTN_DELETE].Create(54, 30, BITMAP_LOG_IN+6, 4, 2, 1, 3);

	CWin::Create(
		m_aBtn[0].GetWidth() * CSMW_BTN_MAX + m_asprBack[CSMW_SPR_INFO].GetWidth() + 6,
		m_aBtn[0].GetHeight(), -2);

	for (int i = 0; i < CSMW_BTN_MAX; ++i)
		CWin::RegisterButton(&m_aBtn[i]);

	m_bAccountBlockItem = false;

	for (int i = 0; i < 5; ++i)
	{
		if (CharactersClient[i].Object.Live)
		{
			if (CharactersClient[i].CtlCode & CTLCODE_10ACCOUNT_BLOCKITEM)
			{
				m_bAccountBlockItem = true;
				break;
			}
		}
	}

#ifdef KJH_MOD_NATION_LANGUAGE_REDEFINE
#ifdef TEENAGER_REGULATION
	// ��� ������ CSMW_WARNING_TEXT_MAX�� �߿� �ϳ��� �������� ������.
	short anWarningText[CSMW_WARNING_TEXT_MAX] = { 1710, 1739, 1740 };
	m_nWarningText = anWarningText[::rand() % CSMW_WARNING_TEXT_MAX];
#endif // TEENAGER_REGULATION
#else // KJH_MOD_NATION_LANGUAGE_REDEFINE
#if (SELECTED_LANGUAGE == LANGUAGE_KOREAN) && defined TEENAGER_REGULATION
	// ��� ������ CSMW_WARNING_TEXT_MAX�� �߿� �ϳ��� �������� ������.
	short anWarningText[CSMW_WARNING_TEXT_MAX] = { 1710, 1739, 1740 };
	m_nWarningText = anWarningText[::rand() % CSMW_WARNING_TEXT_MAX];
#endif	// (SELECTED_LANGUAGE == LANGUAGE_KOREAN) && defined TEENAGER_REGULATION
#endif // KJH_MOD_NATION_LANGUAGE_REDEFINE
}

void CCharSelMainWin::PreRelease()
{
	for (int i = 0; i < CSMW_SPR_MAX; ++i)
		m_asprBack[i].Release();
}

void CCharSelMainWin::SetPosition(int nXCoord, int nYCoord)
{
	CWin::SetPosition(nXCoord, nYCoord);

	int nBtnWidth = m_aBtn[0].GetWidth();
#ifndef KWAK_FIX_COMPILE_LEVEL4_WARNING
	int nBtnHeight = m_aBtn[0].GetHeight();
#endif // KWAK_FIX_COMPILE_LEVEL4_WARNING

// ���� ��ġ.
	m_aBtn[CSMW_BTN_CREATE].SetPosition(nXCoord, nYCoord);
	m_aBtn[CSMW_BTN_MENU].SetPosition(nXCoord + nBtnWidth + 1, nYCoord);
	m_asprBack[CSMW_SPR_INFO].SetPosition(
		m_aBtn[CSMW_BTN_MENU].GetXPos() + nBtnWidth + 2, nYCoord + 5);

// ���� ��ġ.
	int nWinRPosX = nXCoord + CWin::GetWidth();
	m_asprBack[CSMW_SPR_DECO].SetPosition(
		nWinRPosX - (m_asprBack[CSMW_SPR_DECO].GetWidth() - 22), nYCoord - 59);
	m_aBtn[CSMW_BTN_DELETE].SetPosition(nWinRPosX - nBtnWidth, nYCoord);
	m_aBtn[CSMW_BTN_CONNECT].SetPosition(
		nWinRPosX - (nBtnWidth * 2 + 1), nYCoord);
}

void CCharSelMainWin::Show(bool bShow)
{
	CWin::Show(bShow);

	int i;
	for (i = 0; i < CSMW_SPR_MAX; ++i)
		m_asprBack[i].Show(bShow);
	for (i = 0; i < CSMW_BTN_MAX; ++i)
		m_aBtn[i].Show(bShow);
}

bool CCharSelMainWin::CursorInWin(int nArea)
{
	if (!CWin::m_bShow)
		return false;

	switch (nArea)
	{
	case WA_MOVE:
		return false;
	}

	return CWin::CursorInWin(nArea);
}

void CCharSelMainWin::UpdateDisplay()
{
	m_aBtn[CSMW_BTN_CREATE].SetEnable(false);
	int i=0;
#ifdef PBG_ADD_CHARACTERSLOT
	if(BLUE_MU::IsBlueMuServer())
	{
		for(i = 0; i < g_SlotLimit->GetCharacterCnt(); ++i)
		{
			if (!CharactersClient[i].Object.Live)
			{
				m_aBtn[CSMW_BTN_CREATE].SetEnable(true);
				break;
			}
		}
	}
	else
#endif //PBG_ADD_CHARACTERSLOT
	for (i = 0; i < 5; ++i)
	{
		if (!CharactersClient[i].Object.Live)
		{
			m_aBtn[CSMW_BTN_CREATE].SetEnable(true);
			break;
		}
	}

	if (SelectedHero > -1)
	{
		m_aBtn[CSMW_BTN_CONNECT].SetEnable(true);
		m_aBtn[CSMW_BTN_DELETE].SetEnable(true);
	}
	else
	{
		m_aBtn[CSMW_BTN_CONNECT].SetEnable(false);
		m_aBtn[CSMW_BTN_DELETE].SetEnable(false);
	}


	bool bNobodyCharacter = true;

	for (int i = 0; i < 5; ++i)
	{
		if (CharactersClient[i].Object.Live == true)
		{
			bNobodyCharacter = false;
			break;
		}
	}
	
	if (bNobodyCharacter == true)
	{
		CUIMng& rUIMng = CUIMng::Instance();
		rUIMng.ShowWin(&rUIMng.m_CharMakeWin);
#ifdef PJH_CHARACTER_RENAME
		rUIMng.m_CharMakeWin.Set_State();
#endif //PJH_CHARACTER_RENAME
	}
}

#ifdef KWAK_FIX_COMPILE_LEVEL4_WARNING
void CCharSelMainWin::UpdateWhileActive()
#else // KWAK_FIX_COMPILE_LEVEL4_WARNING
void CCharSelMainWin::UpdateWhileActive(double dDeltaTick)
#endif // KWAK_FIX_COMPILE_LEVEL4_WARNING
{
	//PJH_DEBUG_SERVER_ADD
	if (m_aBtn[CSMW_BTN_CONNECT].IsClick())
		::StartGame();
	else if (m_aBtn[CSMW_BTN_MENU].IsClick())
	{
		CUIMng& rUIMng = CUIMng::Instance();
		rUIMng.ShowWin(&rUIMng.m_SysMenuWin);
		rUIMng.SetSysMenuWinShow(true);
	}
	else if (m_aBtn[CSMW_BTN_CREATE].IsClick())
	{
		CUIMng& rUIMng = CUIMng::Instance();
		rUIMng.ShowWin(&rUIMng.m_CharMakeWin);
#ifdef PJH_CHARACTER_RENAME
		rUIMng.m_CharMakeWin.Set_State();
#endif //PJH_CHARACTER_RENAME

	}
	else if (m_aBtn[CSMW_BTN_DELETE].IsClick())
		DeleteCharacter();
}

void CCharSelMainWin::RenderControls()
{
	for (int i = 0; i < CSMW_SPR_MAX; ++i)
		m_asprBack[i].Render();

	::EnableAlphaTest();
	::glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	
	g_pRenderText->SetFont(g_hFixFont);
	g_pRenderText->SetTextColor(CLRDW_WHITE);
	g_pRenderText->SetBgColor(0);

#ifdef KJH_MOD_NATION_LANGUAGE_REDEFINE
#ifdef _LANGUAGE_KOR
	// ��� ���� �ؽ�Ʈ.
	g_pRenderText->SetTextColor(CLRDW_BR_ORANGE);
	g_pRenderText->RenderText(
		int(m_asprBack[CSMW_SPR_INFO].GetXPos() / g_fScreenRate_x),
		int((m_asprBack[CSMW_SPR_INFO].GetYPos() + 7) / g_fScreenRate_y),
		GlobalText[m_nWarningText],
		int(m_asprBack[CSMW_SPR_INFO].GetWidth() / g_fScreenRate_x), 0,
		RT3_SORT_CENTER);
#endif // _LANGUAGE_KOR
#else // KJH_MOD_NATION_LANGUAGE_REDEFINE
#if SELECTED_LANGUAGE == LANGUAGE_KOREAN

	// ��� ���� �ؽ�Ʈ.
	g_pRenderText->SetTextColor(CLRDW_BR_ORANGE);
	g_pRenderText->RenderText(
		int(m_asprBack[CSMW_SPR_INFO].GetXPos() / g_fScreenRate_x),
		int((m_asprBack[CSMW_SPR_INFO].GetYPos() + 7) / g_fScreenRate_y),
		GlobalText[m_nWarningText],
		int(m_asprBack[CSMW_SPR_INFO].GetWidth() / g_fScreenRate_x), 0,
		RT3_SORT_CENTER);

#endif	// SELECTED_LANGUAGE == LANGUAGE_KOREAN
#endif // KJH_MOD_NATION_LANGUAGE_REDEFINE

	if (m_bAccountBlockItem)
	{
		g_pRenderText->SetTextColor(0, 0, 0, 255);
		g_pRenderText->SetBgColor(255, 255, 0, 128);
		g_pRenderText->RenderText(320, 330, GlobalText[436], 0, 0,
			RT3_WRITE_CENTER);
		g_pRenderText->RenderText(320, 348, GlobalText[437], 0, 0,
			RT3_WRITE_CENTER);
	}

	CWin::RenderButtons();
}

void CCharSelMainWin::DeleteCharacter()
{
    if (CharactersClient[SelectedHero].GuildStatus != G_NONE)
		CUIMng::Instance().PopUpMsgWin(MESSAGE_DELETE_CHARACTER_GUILDWARNING);
#if !defined FOR_WORK && !defined EXP_SERVER_PATCH
#ifdef KJH_ADD_SERVER_LIST_SYSTEM
	else if((g_ServerListManager->IsTestServer() == false)
#else // KJH_ADD_SERVER_LIST_SYSTEM
	else if (!::IsTestServer()
#endif // KJH_ADD_SERVER_LIST_SYSTEM
		&& CharactersClient[SelectedHero].Level >= CHAR_DEL_LIMIT_LV)
		CUIMng::Instance().PopUpMsgWin(MESSAGE_DELETE_CHARACTER_WARNING);
#endif	// FOR_WORK
	else if (CharactersClient[SelectedHero].CtlCode
		& (CTLCODE_02BLOCKITEM | CTLCODE_10ACCOUNT_BLOCKITEM))
		CUIMng::Instance().PopUpMsgWin(MESSAGE_DELETE_CHARACTER_ID_BLOCK);
	else
		CUIMng::Instance().PopUpMsgWin(MESSAGE_DELETE_CHARACTER_CONFIRM);
}