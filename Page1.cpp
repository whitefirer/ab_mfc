// Page1.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ab_mfc.h"
#include "Page1.h"


// CPage1 �Ի���

IMPLEMENT_DYNAMIC(CPage1, CDialog)

CPage1::CPage1(CWnd* pParent /*=NULL*/)
: CDialog(CPage1::IDD, pParent)
, m_comb_add(_T(""))
, m_list_add(_T(""))
, m_list_ctrl_add(_T(""))
, m_spinval(_T("spinval"))
{

}

CPage1::~CPage1()
{
}

void CPage1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_comb_add);
	DDX_Text(pDX, IDC_EDIT2, m_list_add);
	DDX_Control(pDX, IDC_COMBO1, m_comb);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Control(pDX, IDC_LIST2, m_list_ctrl);
	DDX_Text(pDX, IDC_EDIT3, m_list_ctrl_add);
	DDX_Control(pDX, IDC_PROGRESS1, m_progress);
	DDX_Control(pDX, IDC_SLIDER1, m_slider);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_datetime);
	DDX_Control(pDX, IDC_SPIN1, m_spin);
	DDX_Control(pDX, IDC_IPADDRESS1, m_ipaddress);
	DDX_Text(pDX, IDC_SPINVAL, m_spinval);
}


BEGIN_MESSAGE_MAP(CPage1, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_COMB_ADD, &CPage1::OnBnClickedButtonCombAdd)
	ON_BN_CLICKED(IDC_BUTTON_COMB_DEL, &CPage1::OnBnClickedButtonCombDel)
	ON_BN_CLICKED(IDC_IDC_BUTTON_LIST_ADD, &CPage1::OnBnClickedIdcButtonListAdd)
	ON_BN_CLICKED(IDC_IDC_BUTTON_LIST_DEL, &CPage1::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_IDC_BUTTON_LIST_CTRL_ADD, &CPage1::OnBnClickedIdcButtonListCtrlAdd)
	ON_NOTIFY(NM_RCLICK, IDC_LIST2, &CPage1::OnNMRclickList2)
	ON_BN_CLICKED(IDC_IDC_BUTTON_LIST_CTRL_DEL, &CPage1::OnBnClickedIdcButtonListCtrlDel)
	ON_COMMAND(ID_ADD, &CPage1::OnAdd)
	ON_COMMAND(ID_DEL, &CPage1::OnDel)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CPage1::OnNMCustomdrawSlider1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CPage1::OnDeltaposSpin1)
	ON_BN_CLICKED(IDC_BUTTON_GETTIME, &CPage1::OnBnClickedButtonGettime)
	ON_BN_CLICKED(IDC_BUTTON_GETIP, &CPage1::OnBnClickedButtonGetip)
	ON_WM_HSCROLL()
//	ON_NOTIFY(NM_THEMECHANGED, IDC_SCROLLBAR1, &CPage1::OnThemechangedScrollbar1)
ON_WM_VSCROLL()
END_MESSAGE_MAP()


// CPage2 ��Ϣ�������
BOOL CPage1::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_comb.AddString("superKiki");
	m_comb.SetCurSel(0);

	//����list_ctrl���
	DWORD dwStyle = ::GetWindowLong(m_list_ctrl.m_hWnd, GWL_STYLE); 
	dwStyle &= ~(LVS_TYPEMASK);
	dwStyle &= ~(LVS_EDITLABELS);
	//�����·��
	SetWindowLong(m_list_ctrl.m_hWnd, GWL_STYLE,dwStyle|LVS_REPORT|LVS_NOLABELWRAP|LVS_SHOWSELALWAYS);
	//������չ���
	DWORD styles = LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES;
	ListView_SetExtendedListViewStyleEx(m_list_ctrl.m_hWnd,styles,styles);
	ListView_SetBkColor(m_list_ctrl,RGB(254,240,69));
	ListView_SetTextBkColor(m_list_ctrl,RGB(254,240,69));

	LV_COLUMN lvcol;
	lvcol.mask =LVCF_TEXT; 

	lvcol.pszText = "Name"; 
	ListView_InsertColumn(m_list_ctrl, 0, &lvcol); 
	ListView_SetColumnWidth(m_list_ctrl, 0, 100); 


	lvcol.mask =LVCF_TEXT; 
	lvcol.pszText = "Password"; 
	ListView_InsertColumn(m_list_ctrl, 1, &lvcol); 
	ListView_SetColumnWidth(m_list_ctrl, 1, 100); 
	ListView_SetExtendedListViewStyle(m_list_ctrl,LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);//������ʾ��ʽ
	/*	����LVS_EX_FULLROWSELECT������ǰ��˵������ѡ��
	LVS_EX_GRIDLINES�������ߣ�ֻ������report����listctrl��
	LVS_EX_CHECKBOXES��ǰ��Ӹ�checkbox
	pListCtrl->SetExtendedStyle( m_listctrl.GetExtendedStyle()|LVS_EX_SUBITEMIMAGES);
	��Ҳ��һ������Ҫ�����ԣ������Ļ����������б��м�ICON���ǵ�windows��
	��������������������������������ҲҪ��Ŷ��������Ժ�ὲ�ġ�
	*/
	//����MFC������еķ���
	//	m_list.InsertColumn( 0, "ID", LVCFMT_LEFT, 40 );//������
	//	m_list.InsertColumn( 1, "NAME", LVCFMT_LEFT, 50 );

	//slider + progress
	m_slider.SetRange(0,100);
	m_progress.SetRange(0,100);

	CString strNum;
	strNum.Format("%d", m_slider.GetPos());
	SetDlgItemText(IDC_NUM, strNum);

	//spin
	m_spin.SetBuddy(GetDlgItem(IDC_NUM));
	m_spin.SetRange(0, 5);
	m_spin.SetPos(0);
	return TRUE;
}

void CPage1::OnBnClickedButtonCombAdd()
{
	UpdateData(TRUE);
	if(m_comb_add == "")
	{
		MessageBox("û�����ݣ�", "oops!", MB_OK);
		return;
	}
	m_comb.AddString(m_comb_add);
	m_comb.SetCurSel(m_comb.GetCount()-1);
	m_comb_add = "";
	UpdateData(FALSE);
}

void CPage1::OnBnClickedButtonCombDel()
{
	if(m_comb.GetCount() == 0)
	{
		MessageBox("û�����ˣ�", "oops!", MB_OK);
	}

	UpdateData(TRUE);
	int curSel = m_comb.GetCurSel();
	m_comb.DeleteString(curSel);
	if(m_comb.GetCount() == 0)
	{
		SetDlgItemText(IDC_COMBO1,"");
	}
	else
		m_comb.SetCurSel(m_comb.GetCount()-1);
	UpdateData(FALSE);
}

void CPage1::OnBnClickedIdcButtonListAdd()
{
	UpdateData(TRUE);
	if(m_list_add == "")
	{
		MessageBox("û�����ݣ�","oops!",MB_OK);
		return;
	}
	m_list.AddString(m_list_add);
	m_list_add = "";
	UpdateData(FALSE);
}

void CPage1::OnBnClickedButton4()
{
	int curSel = m_list.GetCurSel();
	if (LB_ERR != curSel)
	{
		m_list.DeleteString(curSel); //ɾ�� m_mlist ѡ�е���Ŀ��
	}
	else
	{
		MessageBox("û��Ҫɾ������Ŀ��", "oops!", MB_OK);
	}
}

void CPage1::OnBnClickedIdcButtonListCtrlAdd()
{
	UpdateData(TRUE);
	if (m_list_ctrl_add == "")
	{
		MessageBox("û�����ݣ�", "oops!", MB_OK);
		return;
	}
    int nRow = m_list_ctrl.GetItemCount();//�Ӻ���ǰ����
	nRow = m_list_ctrl.InsertItem(nRow, m_list_ctrl_add);//������
	m_list_ctrl.SetItemText(nRow,1, m_list_ctrl_add);//��������
	UpdateData(FALSE);
}

void CPage1::OnNMRclickList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if(pNMListView->iItem != -1)
	{
		DWORD dwPos = GetMessagePos();
		CPoint point( LOWORD(dwPos), HIWORD(dwPos) );

		CMenu menu;
		VERIFY( menu.LoadMenu( IDR_MENU_LIST) );     
		CMenu* popup = menu.GetSubMenu(0);
		ASSERT( popup != NULL );
		popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this );
	}

	*pResult = 0;
}

void CPage1::OnBnClickedIdcButtonListCtrlDel()
{
	int nChoice = m_list_ctrl.GetNextItem(-1, LVNI_SELECTED);// ���ѡ����. 
	if (nChoice != -1)						// ������ѡ����ʱ
	{
		m_list_ctrl.DeleteItem(nChoice);	// ɾ����.
	}
	else
	{
		AfxMessageBox(_T("û��ѡ������ڣ����Ƚ���ѡ��"));
	}
}

void CPage1::OnAdd()
{
	OnBnClickedIdcButtonListCtrlAdd();
}

void CPage1::OnDel()
{
	OnBnClickedIdcButtonListCtrlDel();
}

void CPage1::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	int curPos = m_slider.GetPos();
	m_progress.SetPos(curPos);

	CString strNum;
	strNum.Format("%d", curPos);
	SetDlgItemText(IDC_NUM, strNum);
	*pResult = 0;
}

void CPage1::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)// ���޸�
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR); 
	pNMUpDown->iDelta = pNMUpDown->iDelta;			// Spin Contrl���÷����ܼ򵥰�~��
	m_spinval.Format("%d", pNMUpDown->iDelta);		// ת�����ݸ�ʽ��int -> CString
	SetDlgItemText(IDC_SPINVAL,m_spinval);			// ��������ʾ�ھ�̬�ؼ���

	*pResult = 0;
}

void CPage1::OnBnClickedButtonGettime()// ���޸�
{
	SYSTEMTIME st;
 	m_datetime.GetTime(&st);
 	CString strTime;

	CString strGmt = CTime::GetCurrentTime().FormatGmt("��ǰ���ڣ�%Y/%m/%d\n ��ǰʱ�䣺%H:%M:%S\n");   // �õ���ǰʱ��
	strTime.Format("�ӿؼ��ϵõ���ʱ�䣺%d��%d��%d��", st.wYear, st.wMonth, st.wDay);
 	MessageBox(strGmt + strTime, "����", MB_OK);
}

void CPage1::OnBnClickedButtonGetip()
{
	int b1, b2, b3, b4;								// IP���Ķ�,��һ��Ϊʲô��int�Ͷ�����BYTE���أ�
	CString strIP;
	WSADATA wsd;
	PHOSTENT hostinfo;

	if (WSAStartup(MAKEWORD(2,2), &wsd) !=  0) 
	{ 
		AfxMessageBox( "�����ʼ��ʧ�� "); 
	} 
	else 
	{
		if (gethostname(strIP.GetBuffer(128), 128) == 0) 
		{ 
			//��ȡ���������Ϣ
			MessageBox(strIP, "��һ�����õ����Ļ�������", MB_OK);
			if ((hostinfo = gethostbyname(strIP))  !=  NULL) 
			{ 
				strIP = inet_ntoa(*(struct in_addr*)*hostinfo->h_addr_list);// �õ�IP�ַ��� 
			} 
			MessageBox(strIP, "�ڶ������õ�����IP��", MB_OK);
		}
	}
	WSACleanup(); 

	MessageBox("ȷ�����", "��������������IP�ַ���ת��IP��ʽ��", MB_OK);
	sscanf(strIP, "%d.%d.%d.%d", &b1, &b2, &b3, &b4); // ���ַ���ת��IP
	m_ipaddress.SetAddress(b1, b2, b3, b4);
}


void CPage1::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	int minpos;
	int maxpos;

	if (pScrollBar != GetDlgItem(IDC_SCROLLBAR1))	  // �ж��ǲ��Ǳ��ؼ���������ǣ���ֱ�ӷ���
		return ;									  // $$$$���Ǳ�Ҫ��, Ϊ��ֹ�����ؼ�����WM_HSCROLL��Ϣ����

	pScrollBar->SetScrollRange(0, 100);				  // ���ù�������Χ
	pScrollBar->GetScrollRange(&minpos, &maxpos); 
	maxpos = pScrollBar->GetScrollLimit();			  // �õ�ʵ�ʹ�������Χ

	// �õ��������ĵ�ǰλ��
	int curpos = pScrollBar->GetScrollPos();		  // �õ���ǰ��������λ��
	CWnd *HwndHstr = GetDlgItem(IDC_Hstr);			  // �õ���ʾ�������ǩ�ľ��

	RECT rect;										  
	pScrollBar->GetWindowRect(&rect);				  // �õ����ؼ�������Χ
	int per = (rect.right - rect.left) / maxpos;	  // �õ�������һ��ľ��� $$����Ҫ��
	HwndHstr->GetWindowRect(&rect); 				  // �õ���ʾ�������ǩ������
	ScreenToClient(&rect);							  // �����������

	// ������Ϣ�Ի����λ���Լ���ǩ��λ�ý��м���
	switch (nSBCode)
	{
		case SB_LEFT:		
			curpos = minpos;						  // ������
		break;

		case SB_RIGHT:      						  // �����Ҵ�
			curpos = maxpos;
		break;
													  // �ͷŻ���
		case SB_ENDSCROLL:		
		break;

		case SB_LINELEFT:							  // ������������ҳʱ���鼰��ǩ�ƶ�
		case SB_PAGELEFT:
			if (curpos > minpos)
			{
				curpos--;
				rect.left -= per;
				rect.right -= per;
			}
		break;

		case SB_LINERIGHT:   						 // �����������ҷ�ҳʱ���鼰��ǩ�ƶ�
		case SB_PAGERIGHT:
			if (curpos < maxpos - 1)				 // ��һ��ΪʲôҪ��1��
			{
				curpos++;
				rect.left += per;
				rect.right += per;
			}
		break;

		case SB_THUMBPOSITION: 						//������������ҷʱ���鼰��ǩͬʱ�ƶ�
		case SB_THUMBTRACK:   
			rect.left += (nPos - curpos) * per;
			rect.right += (nPos - curpos) * per;
			curpos = nPos;     
		break;
	}

	// �����鼰��ǩ�ƶ��µ�λ��
	pScrollBar->SetScrollPos(curpos); 
	HwndHstr->MoveWindow(&rect);
	
	CString strNum;
	strNum.Format("%d", curpos);
	SetDlgItemText(IDC_Hstr, strNum);					 // ������ʾ��ǩ��ǰλ����ֵ

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CPage1::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	int minpos;
	int maxpos;

	if (pScrollBar != GetDlgItem(IDC_SCROLLBAR2))		 // �ж��ǲ��Ǳ��ؼ���������ǣ���ֱ�ӷ���
		return ;

	pScrollBar->SetScrollRange(0, 100);					 // ���ù�������Χ
	pScrollBar->GetScrollRange(&minpos, &maxpos); 
	maxpos = pScrollBar->GetScrollLimit();				 // �õ�ʵ�ʹ�������Χ

	// Get the current position of scroll box.
	int curpos = pScrollBar->GetScrollPos();			 // �õ���ǰ��������λ��
	CWnd *HwndHstr = GetDlgItem(IDC_Vstr);				 // �õ���ʾ�������ǩ�ľ��

	RECT rect;
	pScrollBar->GetWindowRect(&rect);					 // �õ�����������
	int per = (rect.bottom - rect.top) / maxpos;		 // �õ�������һ��ľ��� 
	HwndHstr->GetWindowRect(&rect); 					 // �õ���ʾ�������ǩ������
	ScreenToClient(&rect);								 // �����������

	// ������Ϣ�Ի��鼰��ǩλ�ý��ж�λ
	switch (nSBCode)
	{
		case SB_TOP:      								 // �����
			curpos = minpos;
		break;
														 // ����״�
		case SB_BOTTOM:      
			curpos = maxpos;
		break;
														// �ͷŻ���
		case SB_ENDSCROLL:   
		break;
														// ���������������Ϸ�ҳ��ť���鼰��ǩ����仯
		case SB_LINEUP:      
		case SB_PAGEUP:
			if (curpos > minpos)
			{
				curpos--;
				rect.top -= per;
				rect.bottom -= per;
			}
		break;

		case SB_LINEDOWN:   							// ���������������·�ҳ��ť���鼰��ǩ����仯
		case SB_PAGEDOWN:
			if (curpos < maxpos - 1)
			{
				curpos++;
				rect.top += per;
				rect.bottom += per;
			}
		break;

		case SB_THUMBPOSITION: 							//������������ҷʱ���鼰��ǩͬʱ�ƶ�
		case SB_THUMBTRACK:   
			rect.top += (nPos - curpos) * per;
			rect.bottom += (nPos - curpos) * per;
			curpos = nPos;     
		break;
	}

	// �����鼰��ǩ�ƶ�����λ��
	pScrollBar->SetScrollPos(curpos); 
	HwndHstr->MoveWindow(&rect);
	
	CString strNum;
	strNum.Format("%d", curpos);
	SetDlgItemText(IDC_Vstr, strNum);				   // ��ʾ��ǰ������

	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CPage1::OnOK()
{
	return;
}
