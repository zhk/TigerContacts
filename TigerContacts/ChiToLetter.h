//ʵ�ֺ�����ƴ����ת��
//-----------------------------------------------------�����:ţ��ƽ
// Download by http://www.codefans.net

#include <CommonUtil/CommonUtil.h>

class CMyChiToLetter
{
public:
	CMyChiToLetter();
	~CMyChiToLetter();

	//�ָ���
	BOOL _bAddSeparater;

	//TRUE:�õ�����ĸ��д
	//FALSE:�õ�����ĸСд
	BOOL m_blnFirstBig;

	//TRUE:�õ�ȫ����д
	//FALSE:�õ���ȥ��ƴ��������Сд
	BOOL m_blnAllBiG;

	//True:�õ�ȫ��ƴ��
	//FALSE:�õ���ƴ��
	BOOL _bFullPinYin;

	//����ƴ��������MBCS��
	Cactus::String		GetLetter(const Cactus::String& strText);

private:
	Cactus::String		FindLetter(int nCode);
};