//实现汉字向拼音的转化
//-----------------------------------------------------设计人:牛文平
// Download by http://www.codefans.net

#include <CommonUtil/CommonUtil.h>

class CMyChiToLetter
{
public:
	CMyChiToLetter();
	~CMyChiToLetter();

	//分隔符
	BOOL _bAddSeparater;

	//TRUE:得到首字母大写
	//FALSE:得到首字母小写
	BOOL m_blnFirstBig;

	//TRUE:得到全部大写
	//FALSE:得到除去首拼音的所有小写
	BOOL m_blnAllBiG;

	//True:得到全部拼音
	//FALSE:得到首拼音
	BOOL _bFullPinYin;

	//返回拼音，传入MBCS码
	Cactus::String		GetLetter(const Cactus::String& strText);

private:
	Cactus::String		FindLetter(int nCode);
};