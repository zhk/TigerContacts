# TigerContacts
A VCF editor on Windows using MFC/C++.

一、简介
老虎通讯录是一款免费的VCF文件编辑器。VCF是保存个人通讯信息的的文件格式，广泛用于邮件、手机等系统。但是不同的实现又有小差别，成了一定的兼容性问题。老虎编辑器可以读入或导入安卓、苹果、诺基亚S60等系统输出的VCF文件，并能针对不同的用途保存或导出到单个或者多个VCF文件。解决常见的不同系统间的乱码、导入失败问题。

系统默认读写安卓、苹果系统的VCF文件。

你可以通过菜单：文件/导入联系人/选择来源来导入不同系统的单个或多个VCF文件；
文件/导出联系人/选择用途来导出不同系统的单个或多个VCF文件。

你可以对联系人进行标记、查找、删除、合并等操作。

你可以编辑单个联系人信息，包括姓名、头像、电话、地址、网址、IM、备注等。

该工具尽可能支持各种VCF文件，但是由于各厂家、各系统的VCF格式都有略微差别，该工具可能并不支持某种特定的VCF文件。如果你遇到不支持的情况并希望该工具给予支持，请通过QQ信箱和作者联系。请简要说明你的情况并附上供测试用的VCF文件。

二、使用协议
◇该软件是免费软件，作者不收取任何费用。
◇欢迎推广该软件，但不要修改、重新包装该软件。用户在推广中必须遵守免费原则。
◇作者保证该软件不含任何恶意代码。
◇请用户注意备份自己的原始数据，作者不对使用该软件造成用户的任何软硬件损失承担任何形式的责任。作者不接受任何形式的指责、侮辱等。
◇如果用户在使用中发现任何bug、要求或建议，请通过QQ信箱和作者联系，作者将尽快给予答复。

三、版本历史
	1.0版本		2012.8
	发布初始版本。

	1.1版本		2013.3
	增加对格式不太规范的VCF文件的支持；
	文件读入时去掉电话号码内部的空格和连字符'-'(否则诺基亚S60系统会出错)；
	页面详细视图中N字段旁边增加备注；
	改进网格视图中地址信息的显示。

	1.2版本		2013.3
	增强对格式不太规范的VCF文件的支持；
	更新了详细视图中地址备注的显示；
	去掉“文件读入时去掉电话号码内部的空格和连字符'-'的功能”，此功能现在在操作菜单下面进行；
	操作菜单下面增加名字整理功能。

	1.21版本	2013.3
	删除重复联系人时，改进界面的不响应，增加提示和删除结果界面；

	1.31版本	2013.9
	修正合并联系人时，允许重复的项目的数据丢失问题；
	增加了REV信息的更新；

	1.50版本	2014.2
	增加了分组信息的显示和编辑；
	增强了大小写混乱的VCF文件的支持；

四、感谢下面网友提供的问题反馈、建议和测试文件
	生活、twelve、云

五、联系方式
	作者：张昆(zhk_tiger)
	QQ：931076774
	eMail：931076774@qq.com
