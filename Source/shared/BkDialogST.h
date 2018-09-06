//
//	Class:		CBkDialogST
//
//	Compiler:	Visual C++
//	Tested on:	Visual C++ 6.0
//
//	Version:	See GetVersionC() or GetVersionI()
//
//	Created:	01/September/2000
//	Updated:	21/July/2002
//
//	Author:		Davide Calabro'		davide_calabro@yahoo.com
//									http://www.softechsoftware.it
//
//	Disclaimer
//	----------
//	THIS SOFTWARE AND THE ACCOMPANYING FILES ARE DISTRIBUTED "AS IS" AND WITHOUT
//	ANY WARRANTIES WHETHER EXPRESSED OR IMPLIED. NO REPONSIBILITIES FOR POSSIBLE
//	DAMAGES OR EVEN FUNCTIONALITY CAN BE TAKEN. THE USER MUST ASSUME THE ENTIRE
//	RISK OF USING THIS SOFTWARE.
//
//	Terms of use
//	------------
//	THIS SOFTWARE IS FREE FOR PERSONAL USE OR FREEWARE APPLICATIONS.
//	IF YOU USE THIS SOFTWARE IN COMMERCIAL OR SHAREWARE APPLICATIONS YOU
//	ARE GENTLY ASKED TO DONATE 1$ (ONE U.S. DOLLAR) TO THE AUTHOR:
//
//		Davide Calabro'
//		P.O. Box 65
//		21019 Somma Lombardo (VA)
//		Italy
//
#ifndef _BKDIALOGST_H_
#define _BKDIALOGST_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Return values
#ifndef	BKDLGST_OK
#define	BKDLGST_OK						0
#endif
#ifndef	BKDLGST_INVALIDRESOURCE
#define	BKDLGST_INVALIDRESOURCE			1
#endif
#ifndef	BKDLGST_INVALIDMODE
#define	BKDLGST_INVALIDMODE				2
#endif

class CBkDialogST : public CDialog
{
public:
	CBkDialogST(CWnd* pParent = NULL);
	CBkDialogST(UINT uResourceID, CWnd* pParent = NULL);
	CBkDialogST(LPCTSTR pszResourceID, CWnd* pParent = NULL);

	virtual ~CBkDialogST();

	DWORD SetMode(BYTE byMode, BOOL bRepaint = TRUE);

	DWORD SetBitmap(HBITMAP hBitmap);
	DWORD SetBitmap(int nBitmap);
    DWORD CBkDialogST::SetBitmap(TCHAR *ws_filename); //added by Seth
#ifndef UNDER_CE
	DWORD ActivateEasyMoveMode(BOOL bActivate);
	DWORD ShrinkToFit(BOOL bRepaint = TRUE);
#endif

	enum	{
				BKDLGST_MODE_TILE = 0,
				BKDLGST_MODE_CENTER,
				BKDLGST_MODE_STRETCH,
				BKDLGST_MODE_TILETOP,
				BKDLGST_MODE_TILEBOTTOM,
				BKDLGST_MODE_TILELEFT,
				BKDLGST_MODE_TILERIGHT,
				BKDLGST_MODE_TOPLEFT,
				BKDLGST_MODE_TOPRIGHT,
				BKDLGST_MODE_TOPCENTER,
				BKDLGST_MODE_BOTTOMLEFT,
				BKDLGST_MODE_BOTTOMRIGHT,
				BKDLGST_MODE_BOTTOMCENTER,

				BKDLGST_MAX_MODES
			};

	//{{AFX_DATA(CBkDialogST)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBkDialogST)
	//}}AFX_VIRTUAL

protected:
	// Generated message map functions
	//{{AFX_MSG(CBkDialogST)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnPostEraseBkgnd(CDC* pDC);
#ifndef UNDER_CE
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
#endif

	static short GetVersionI()		{return 11;}
	static LPCTSTR GetVersionC()	{return (LPCTSTR)_T("1.1");}

protected:
	void Init();
	void FreeResources(BOOL bCheckForNULL = TRUE);
    HBRUSH m_hbrush; //added by Seth, for my dialog box to use for things
	HBITMAP		m_hBitmap;			// Handle to bitmap
	DWORD		m_dwWidth;			// Width of bitmap
	DWORD		m_dwHeight;			// Height of bitmap
	BYTE		m_byMode;			// Current drawing bitmap mode
#ifndef UNDER_CE
	BOOL		m_bEasyMoveMode;	// Indicates if EasyMove mode must be used
#endif

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif
