
// AboutDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AboutDlg.h"

using namespace Cactus;


CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
, _strMail(_T("931076774"))
, _strAuthor(_T("张昆"))
{
	static const char buffer[] = "老虎通讯录是一款免费的VCF文件编辑器。VCF是保存个人通讯信息的的文件格式，\r\n"
								"广泛用于邮件、手机等系统。但是不同的实现又有小差别，造成了一定的兼容性问\r\n"
								"题。老虎编辑器可以读入或导入安卓、苹果、诺基亚S60等系统输出的VCF文件，并\r\n"
								"能针对不同的用途保存或导出到单个或者多个VCF文件。解决常见的不同系统间的乱\r\n"
								"码、导入失败等问题。\r\n\r\n"
								"系统默认读写安卓、苹果系统的VCF文件。\r\n\r\n"
								"你可以通过菜单：文件/导入联系人/选择来源来导入不同系统的单个或多个VCF文件；\r\n"
								"文件/导出联系人/选择用途来导出不同系统的单个或多个VCF文件。\r\n\r\n"
								"你可以对联系人进行标记、查找、删除、合并等操作。\r\n\r\n"
								"你可以编辑单个联系人信息，包括姓名、头像、电话、地址、网址、IM、备注等。\r\n\r\n"
								"该工具尽可能支持各种VCF文件，但是由于各厂家、各系统的VCF格式都有略微差别，\r\n"
								"该工具可能并不支持某种特定的VCF文件。如果你遇到不支持的情况并希望该工具给\r\n"
								"予支持，请通过QQ信箱和作者联系。请简要说明你的情况并附上供测试用的VCF文件。";

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

