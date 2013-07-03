// PackToolDlg.h : ͷ�ļ�
//

#pragma once


// CPackToolDlg �Ի���
class CPackToolDlg : public CDialog
{
// ����
public:
	CPackToolDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_PACKTOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

	std::vector<std::string> pngs;
	std::string getPngName(CString filePath);
// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonOutPut();
	afx_msg void OnBnClickedButtonUpAck();
	afx_msg void OnBnClickedButtonEncode();
};
