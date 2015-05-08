// Page1.cpp : 实现文件
//

#include "stdafx.h"
#include "ab_mfc.h"
#include "Page1.h"


// CPage1 对话框

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


// CPage2 消息处理程序
BOOL CPage1::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_comb.AddString("superKiki");
	m_comb.SetCurSel(0);

	//设置list_ctrl风格
	DWORD dwStyle = ::GetWindowLong(m_list_ctrl.m_hWnd, GWL_STYLE); 
	dwStyle &= ~(LVS_TYPEMASK);
	dwStyle &= ~(LVS_EDITLABELS);
	//设置新风格
	SetWindowLong(m_list_ctrl.m_hWnd, GWL_STYLE,dwStyle|LVS_REPORT|LVS_NOLABELWRAP|LVS_SHOWSELALWAYS);
	//设置扩展风格
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
	ListView_SetExtendedListViewStyle(m_list_ctrl,LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);//设置显示样式
	/*	其中LVS_EX_FULLROWSELECT　就是前面说得整行选中
	LVS_EX_GRIDLINES　网格线（只适用与report风格的listctrl）
	LVS_EX_CHECKBOXES　前面加个checkbox
	pListCtrl->SetExtendedStyle( m_listctrl.GetExtendedStyle()|LVS_EX_SUBITEMIMAGES);
	这也是一个很重要的属性，这样的话，可以在列表中加ICON，记得windows的
	任务管理器吗，你想做得那样，这个属性也要加哦，这个我以后会讲的～
	*/
	//这是MFC种添加列的方法
	//	m_list.InsertColumn( 0, "ID", LVCFMT_LEFT, 40 );//插入列
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
		MessageBox("没有内容！", "oops!", MB_OK);
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
		MessageBox("没东西了！", "oops!", MB_OK);
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
		MessageBox("没有内容！","oops!",MB_OK);
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
		m_list.DeleteString(curSel); //删除 m_mlist 选中的项目。
	}
	else
	{
		MessageBox("没有要删除的项目！", "oops!", MB_OK);
	}
}

void CPage1::OnBnClickedIdcButtonListCtrlAdd()
{
	UpdateData(TRUE);
	if (m_list_ctrl_add == "")
	{
		MessageBox("没有内容！", "oops!", MB_OK);
		return;
	}
    int nRow = m_list_ctrl.GetItemCount();//从后往前插入
	nRow = m_list_ctrl.InsertItem(nRow, m_list_ctrl_add);//插入行
	m_list_ctrl.SetItemText(nRow,1, m_list_ctrl_add);//设置数据
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
	int nChoice = m_list_ctrl.GetNextItem(-1, LVNI_SELECTED);// 获得选择项. 
	if (nChoice != -1)						// 当存在选择项时
	{
		m_list_ctrl.DeleteItem(nChoice);	// 删除项.
	}
	else
	{
		AfxMessageBox(_T("没有选择项存在，请先进行选择！"));
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

void CPage1::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)// 已修改
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR); 
	pNMUpDown->iDelta = pNMUpDown->iDelta;			// Spin Contrl的用法（很简单吧~）
	m_spinval.Format("%d", pNMUpDown->iDelta);		// 转换数据格式，int -> CString
	SetDlgItemText(IDC_SPINVAL,m_spinval);			// 将数据显示在静态控件上

	*pResult = 0;
}

void CPage1::OnBnClickedButtonGettime()// 已修改
{
	SYSTEMTIME st;
 	m_datetime.GetTime(&st);
 	CString strTime;

	CString strGmt = CTime::GetCurrentTime().FormatGmt("当前日期：%Y/%m/%d\n 当前时间：%H:%M:%S\n");   // 得到当前时间
	strTime.Format("从控件上得到的时间：%d年%d月%d日", st.wYear, st.wMonth, st.wDay);
 	MessageBox(strGmt + strTime, "日期", MB_OK);
}

void CPage1::OnBnClickedButtonGetip()
{
	int b1, b2, b3, b4;								// IP的四段,想一想为什么是int型而不是BYTE型呢？
	CString strIP;
	WSADATA wsd;
	PHOSTENT hostinfo;

	if (WSAStartup(MAKEWORD(2,2), &wsd) !=  0) 
	{ 
		AfxMessageBox( "网络初始化失败 "); 
	} 
	else 
	{
		if (gethostname(strIP.GetBuffer(128), 128) == 0) 
		{ 
			//获取主机相关信息
			MessageBox(strIP, "第一步，得到您的机器名：", MB_OK);
			if ((hostinfo = gethostbyname(strIP))  !=  NULL) 
			{ 
				strIP = inet_ntoa(*(struct in_addr*)*hostinfo->h_addr_list);// 得到IP字符串 
			} 
			MessageBox(strIP, "第二步，得到您的IP：", MB_OK);
		}
	}
	WSACleanup(); 

	MessageBox("确定完成", "第三步，将您的IP字符串转成IP格式：", MB_OK);
	sscanf(strIP, "%d.%d.%d.%d", &b1, &b2, &b3, &b4); // 将字符串转成IP
	m_ipaddress.SetAddress(b1, b2, b3, b4);
}


void CPage1::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	int minpos;
	int maxpos;

	if (pScrollBar != GetDlgItem(IDC_SCROLLBAR1))	  // 判断是不是本控件，如果不是，则直接返回
		return ;									  // $$$$这是必要的, 为防止其它控件产生WM_HSCROLL消息干扰

	pScrollBar->SetScrollRange(0, 100);				  // 设置滚动条范围
	pScrollBar->GetScrollRange(&minpos, &maxpos); 
	maxpos = pScrollBar->GetScrollLimit();			  // 得到实际滚动条范围

	// 得到滚动条的当前位置
	int curpos = pScrollBar->GetScrollPos();		  // 得到当前滑块所在位置
	CWnd *HwndHstr = GetDlgItem(IDC_Hstr);			  // 得到显示横坐标标签的句柄

	RECT rect;										  
	pScrollBar->GetWindowRect(&rect);				  // 得到本控件的区域范围
	int per = (rect.right - rect.left) / maxpos;	  // 得到它滚动一格的距离 $$：必要的
	HwndHstr->GetWindowRect(&rect); 				  // 得到显示横坐标标签的区域
	ScreenToClient(&rect);							  // 计算相对坐标

	// 根据消息对滑块的位置以及标签的位置进行计算
	switch (nSBCode)
	{
		case SB_LEFT:		
			curpos = minpos;						  // 到最左处
		break;

		case SB_RIGHT:      						  // 到最右处
			curpos = maxpos;
		break;
													  // 释放滑块
		case SB_ENDSCROLL:		
		break;

		case SB_LINELEFT:							  // 点击左区域或左翻页时滑块及标签移动
		case SB_PAGELEFT:
			if (curpos > minpos)
			{
				curpos--;
				rect.left -= per;
				rect.right -= per;
			}
		break;

		case SB_LINERIGHT:   						 // 点击右区域或右翻页时滑块及标签移动
		case SB_PAGERIGHT:
			if (curpos < maxpos - 1)				 // 想一想为什么要减1？
			{
				curpos++;
				rect.left += per;
				rect.right += per;
			}
		break;

		case SB_THUMBPOSITION: 						//点击滑块进行拖曳时滑块及标签同时移动
		case SB_THUMBTRACK:   
			rect.left += (nPos - curpos) * per;
			rect.right += (nPos - curpos) * per;
			curpos = nPos;     
		break;
	}

	// 将滑块及标签移动新的位置
	pScrollBar->SetScrollPos(curpos); 
	HwndHstr->MoveWindow(&rect);
	
	CString strNum;
	strNum.Format("%d", curpos);
	SetDlgItemText(IDC_Hstr, strNum);					 // 发给显示标签当前位置数值

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CPage1::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	int minpos;
	int maxpos;

	if (pScrollBar != GetDlgItem(IDC_SCROLLBAR2))		 // 判断是不是本控件，如果不是，则直接返回
		return ;

	pScrollBar->SetScrollRange(0, 100);					 // 设置滚动条范围
	pScrollBar->GetScrollRange(&minpos, &maxpos); 
	maxpos = pScrollBar->GetScrollLimit();				 // 得到实际滚动条范围

	// Get the current position of scroll box.
	int curpos = pScrollBar->GetScrollPos();			 // 得到当前滑块所在位置
	CWnd *HwndHstr = GetDlgItem(IDC_Vstr);				 // 得到显示横坐标标签的句柄

	RECT rect;
	pScrollBar->GetWindowRect(&rect);					 // 得到滚动条区域
	int per = (rect.bottom - rect.top) / maxpos;		 // 得到它滚动一格的距离 
	HwndHstr->GetWindowRect(&rect); 					 // 得到显示横坐标标签的区域
	ScreenToClient(&rect);								 // 计算相对坐标

	// 根据消息对滑块及标签位置进行定位
	switch (nSBCode)
	{
		case SB_TOP:      								 // 到最顶处
			curpos = minpos;
		break;
														 // 到最底处
		case SB_BOTTOM:      
			curpos = maxpos;
		break;
														// 释放滑块
		case SB_ENDSCROLL:   
		break;
														// 点击上区域或者向上翻页按钮滑块及标签坐标变化
		case SB_LINEUP:      
		case SB_PAGEUP:
			if (curpos > minpos)
			{
				curpos--;
				rect.top -= per;
				rect.bottom -= per;
			}
		break;

		case SB_LINEDOWN:   							// 点击下区域或者向下翻页按钮滑块及标签坐标变化
		case SB_PAGEDOWN:
			if (curpos < maxpos - 1)
			{
				curpos++;
				rect.top += per;
				rect.bottom += per;
			}
		break;

		case SB_THUMBPOSITION: 							//点击滑块进行拖曳时滑块及标签同时移动
		case SB_THUMBTRACK:   
			rect.top += (nPos - curpos) * per;
			rect.bottom += (nPos - curpos) * per;
			curpos = nPos;     
		break;
	}

	// 将滑块及标签移动最新位置
	pScrollBar->SetScrollPos(curpos); 
	HwndHstr->MoveWindow(&rect);
	
	CString strNum;
	strNum.Format("%d", curpos);
	SetDlgItemText(IDC_Vstr, strNum);				   // 显示当前纵坐标

	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CPage1::OnOK()
{
	return;
}
