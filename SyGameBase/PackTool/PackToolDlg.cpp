// PackToolDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PackTool.h"
#include "PackToolDlg.h"
#include "PngPack.h"
#include "des.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CPackToolDlg �Ի���




CPackToolDlg::CPackToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPackToolDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPackToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPackToolDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CPackToolDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_OUT_PUT, &CPackToolDlg::OnBnClickedButtonOutPut)
	ON_BN_CLICKED(IDC_BUTTON_UP_ACK, &CPackToolDlg::OnBnClickedButtonUpAck)
	ON_BN_CLICKED(IDC_BUTTON_ENCODE, &CPackToolDlg::OnBnClickedButtonEncode)
END_MESSAGE_MAP()


// CPackToolDlg ��Ϣ�������

BOOL CPackToolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CPackToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CPackToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CPackToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

CString * SplitString(CString str, char split, int& iSubStrs)
{
    int iPos = 0; //�ָ��λ��
    int iNums = 0; //�ָ��������
    CString strTemp = str;
    CString strRight;
    //�ȼ������ַ���������
    while (iPos != -1)
    {
        iPos = strTemp.Find(split);
        if (iPos == -1)
        {
            break;
        }
        strRight = strTemp.Mid(iPos + 1, str.GetLength());
        strTemp = strRight;
        iNums++;
    }
    if (iNums == 0) //û���ҵ��ָ��
    {
        //���ַ����������ַ�������
        CString* pStrSplit = new CString[1];
		pStrSplit[1] = str;
        return pStrSplit;
    }
    //���ַ�������
    iSubStrs = iNums + 1; //�Ӵ������� = �ָ������ + 1
    CString* pStrSplit;
    pStrSplit = new CString[iSubStrs];
    strTemp = str;
    CString strLeft;
    for (int i = 0; i < iNums; i++)
    {
        iPos = strTemp.Find(split);
        //���Ӵ�
        strLeft = strTemp.Left(iPos);
        //���Ӵ�
        strRight = strTemp.Mid(iPos + 1, strTemp.GetLength());
        strTemp = strRight;
        pStrSplit[i] = strLeft;
    }
    pStrSplit[iNums] = strTemp;
    return pStrSplit;
}
std::string CPackToolDlg::getPngName(CString filePath)
{
	CString* pStr = NULL;
	int iSubStrs = 0;
	pStr = SplitString(filePath, '\\', iSubStrs);
	//����������ַ���
	for (int i = 0; i < iSubStrs; i++)
	{
		//Convert CString to char
		const char* pCh = pStr[i].GetString();
		printf("%s\n", pCh);
	}
	if (iSubStrs >= 1)
	{
		filePath = pStr[iSubStrs - 1].GetString();
	}
	if (pStr)
		delete []pStr;
	return filePath.GetString();
}
void CPackToolDlg::OnBnClickedOk()
{
	
	CTreeCtrl* pNMTreeView = reinterpret_cast<CTreeCtrl*>(GetDlgItem(IDC_TREE_FILES));
	CString strText=_T("");
	pNMTreeView->DeleteAllItems();
    HTREEITEM hRoot=pNMTreeView->InsertItem(_T("Root"));//��������
	
	TCHAR szFilter[] = _T("ͼƬ(*.png)|*.png|�����ļ�(*.*)|*.*||");   
    // ������ļ��Ի���   
    CFileDialog openDlg(TRUE, _T("png"), NULL, OFN_ALLOWMULTISELECT, szFilter, this);   
    const int nMaxFiles = 1000;  
	const int nMaxPathBuffer = (nMaxFiles * (MAX_PATH + 1)) + 1;  
	char* pc = (char*)malloc(nMaxPathBuffer * sizeof(CHAR));  
	if( pc )  
	{
		openDlg.GetOFN().lpstrFile = pc;                            
		openDlg.GetOFN().lpstrFile[0] = NULL;
		openDlg.GetOFN().nMaxFile = nMaxPathBuffer;
	}

	if( openDlg.DoModal() == IDOK )  
	{  
		POSITION posStart = openDlg.GetStartPosition();  
		while( posStart )  
		{ 
			CString filePath = openDlg.GetNextPathName(posStart);  
			pngs.push_back(filePath.GetString());
			filePath = getPngName(filePath.GetString()).c_str();
			pNMTreeView->InsertItem(filePath,hRoot);
		}  
	}  
	if (pc)
		free(pc); 
}
void CPackToolDlg::OnBnClickedButtonOutPut()
{
	TCHAR szFilter[] = _T("����ļ�(*.pack)|�����ļ�(*.*)|*.*||");   
    // ���챣���ļ��Ի���   
    CFileDialog fileDlg(FALSE, _T("pack"), _T("temp"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);   
    CString strFilePath;   
  
    // ��ʾ�����ļ��Ի���   
    if (IDOK == fileDlg.DoModal())   
    {   
        // ���������ļ��Ի����ϵġ����桱��ť����ѡ����ļ�·����ʾ���༭����   
        strFilePath = fileDlg.GetPathName();   
		GetDlgItem(IDC_STATIC_PACK_NAME)->SetWindowTextA(getPngName(strFilePath.GetString()).c_str());
		PngPack pack;
		for (unsigned int i = 0; i < pngs.size();i++)
		{
			std::string &tempname = pngs[i];
			std::string pngName = getPngName(tempname.c_str());
			pack.addPng(tempname.c_str(),pngName.c_str());
		}
		if (pngs.size())
			pack.save(strFilePath.GetString());
    }   
}

void CPackToolDlg::OnBnClickedButtonUpAck()
{
	TCHAR szFilter[] = _T("ͼƬ(*.pack)|*.pack|�����ļ�(*.*)|*.*||");   
    // ������ļ��Ի���   
    CFileDialog openDlg(TRUE, _T("pack"), NULL, NULL, szFilter, this);   
    CTreeCtrl* pNMTreeView = reinterpret_cast<CTreeCtrl*>(GetDlgItem(IDC_TREE_FILES));
	CString strText=_T("");
	pNMTreeView->DeleteAllItems();
    HTREEITEM hRoot=pNMTreeView->InsertItem(_T("Root"));//��������

	if( openDlg.DoModal() == IDOK )  
	{  
		CString strFilePath = openDlg.GetPathName(); 
		PngPack pack;
		pack.load(strFilePath.GetString());
	//	pack.saveAllTextures();
		for (std::map<std::string,ObjectData>::iterator iter = pack.objects.begin(); iter != pack.objects.end();++iter)
		{
			pNMTreeView->InsertItem(iter->first.c_str(),hRoot);
		}
		pack.unpack();
		GetDlgItem(IDC_STATIC_PACK_NAME)->SetWindowTextA("����ɹ�");
	}  
}

void CPackToolDlg::OnBnClickedButtonEncode()
{
	for (unsigned int i = 0; i < pngs.size();i++)
	{
		std::string &tempname = pngs[i];
		std::string pngName = getPngName(tempname.c_str());
		FILE *fp = fopen(pngName.c_str(), "rb");

        fseek(fp,0,SEEK_END);
        unsigned long pSize = ftell(fp);
        fseek(fp,0,SEEK_SET);
        unsigned char *pBuffer = new unsigned char[pSize];
        pSize = fread(pBuffer,sizeof(unsigned char), pSize,fp);
        fclose(fp);

		CEncrypt encode;
		encode.setEncMethod(CEncrypt::ENCDEC_DES);
		const_ZES_cblock key = {1,0,0,1,0,1,1};
		encode.set_key_des(&key);
		encode.encdec(pBuffer,pSize,true);

		fp = fopen(pngName.c_str(),"wb");
		fwrite(pBuffer,pSize,1,fp);
		fclose(fp);
	}
}
