
// AboutDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AboutDlg.h"

using namespace Cactus;


CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
, _strMail(_T("931076774"))
, _strAuthor(_T("����"))
{
	static const char buffer[] = "�ϻ�ͨѶ¼��һ����ѵ�VCF�ļ��༭����VCF�Ǳ������ͨѶ��Ϣ�ĵ��ļ���ʽ��\r\n"
								"�㷺�����ʼ����ֻ���ϵͳ�����ǲ�ͬ��ʵ������С��������һ���ļ�������\r\n"
								"�⡣�ϻ��༭�����Զ�����밲׿��ƻ����ŵ����S60��ϵͳ�����VCF�ļ�����\r\n"
								"����Բ�ͬ����;����򵼳����������߶��VCF�ļ�����������Ĳ�ͬϵͳ�����\r\n"
								"�롢����ʧ�ܵ����⡣\r\n\r\n"
								"ϵͳĬ�϶�д��׿��ƻ��ϵͳ��VCF�ļ���\r\n\r\n"
								"�����ͨ���˵����ļ�/������ϵ��/ѡ����Դ�����벻ͬϵͳ�ĵ�������VCF�ļ���\r\n"
								"�ļ�/������ϵ��/ѡ����;��������ͬϵͳ�ĵ�������VCF�ļ���\r\n\r\n"
								"����Զ���ϵ�˽��б�ǡ����ҡ�ɾ�����ϲ��Ȳ�����\r\n\r\n"
								"����Ա༭������ϵ����Ϣ������������ͷ�񡢵绰����ַ����ַ��IM����ע�ȡ�\r\n\r\n"
								"�ù��߾�����֧�ָ���VCF�ļ����������ڸ����ҡ���ϵͳ��VCF��ʽ������΢���\r\n"
								"�ù��߿��ܲ���֧��ĳ���ض���VCF�ļ��������������֧�ֵ������ϣ���ù��߸�\r\n"
								"��֧�֣���ͨ��QQ�����������ϵ�����Ҫ˵�������������Ϲ������õ�VCF�ļ���";

	_strHelp = GString(buffer).Str().c_str();
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_MAIL, _strMail);
	DDX_Text(pDX, IDC_EDIT_QQ, _strAuthor);
	DDX_Text(pDX, IDC_EDIT_HELP, _strHelp);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

