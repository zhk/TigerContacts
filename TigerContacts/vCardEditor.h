#pragma once

#include "vcard.h"

//----------------------------------------------------------------
// �ؼ���
typedef Cactus::list<CStatic*>::type		CStatic_ListType;
typedef Cactus::list<CEdit*>::type			CEdit_ListType;
typedef Cactus::list<CButton*>::type		CButton_ListType;
typedef Cactus::list<CComboBox*>::type		CComboBox_ListType;

typedef Cactus::list<CWnd*>::type			CWnd_ListType;	// ComboBox<0..1> + CEdit<N> + CButton<0..1>

//----------------------------------------------------------------
// ������
struct SPropLine : public Cactus::Object
{
	SPropLine(int iEditorIndex);
	virtual ~SPropLine();

	CWnd_ListType	_controlsInLine;
	int				_iEditorIndex;		//Same with vCardPropoty's
	int				_iLineHeight;
};
typedef Cactus::list<SPropLine*>::type	PropLines_ListType;


//----------------------------------------------------------------
struct SLabelGroup : public Cactus::Object
{
	Cactus::GStringVector	_strPropNames;			//����������
	Cactus::GString			_strLabelSection;		//���Ǹ������ȡ��ǩ
};
typedef Cactus::map<Cactus::GString, SLabelGroup>::type	NameLabelGroupMapType;
/*
SLabelGroup lg;
lg._strLabelSection = "X-ABDATE";
lg._strPropNames.push_back("BDAY");
lg._strPropNames.push_back("X-ABDATE");
Controls::getSingleton()._LabelsGroup[lg._strParamForAdd] = lg;
*/

//----------------------------------------------------------------
//WORK;FAX = ��˾����,false,false,TYPE=WORK;TYPE=FAX
//X-MSN = MSN,false,true,
struct SPropInfo : public Cactus::Object
{
	Cactus::GString			_strPropGroup;			//����������Section��
	Cactus::GString			_strComboKey;			//Section Item��Key						//WORK;FAX
	Cactus::GString			_strComboCap;			//Caption						[0]		//��˾����
	bool					_bExtend;				//�Ƿ�����չ����				[1]		//false
	bool					_bReplaceName;			//�Ƿ�ʹ��Item Key�滻������	[2]		//false
	Cactus::GString			_strParam;				//���ԵĲ�������׼���Բ���	[3]		//TYPE=WORK;TYPE=FAX
	bool					_bUsed;					//��������Ƿ��Ѿ�����
};
typedef Cactus::map<Cactus::GString, SPropInfo>::type				GroupPropInfoMapType;
typedef Cactus::map<Cactus::GString, GroupPropInfoMapType>::type	NameGroupPropInfoMapType;

//----------------------------------------------------------------

// ������
class PropGroup : public Cactus::Object
{
public:
	//TEL, ADR
	PropGroup(vCard* pCard, const Cactus::GString& strLabelSection
		, int iIndexH, bool bHasTitle, bool bCombo, bool bPhotoRight);

	//BDAY, X-ABDATE, RELATES
	PropGroup(vCard* pCard, const SLabelGroup& labelGroup
		, int iIndexH, bool bHasTitle, bool bCombo, bool bPhotoRight);

	virtual ~PropGroup();

	//�������ӵ��и�
	int						AddPropLine(const vCardProperty& prop, int iScrollPos, bool bEnsureVisible);

	int						GetNextLineHeightPos() const { return _iNextLinePosH; }
	void					SetNextLineHeightPos(int i) { _iNextLinePosH = i; }

	PropLines_ListType&		GetLines() { return _lines; }
	SPropLine*				GetLine(int iIndex);

	void					MoveDownControls(CWnd* pWnd, int iHeight);
	//���ؼ��ٵ��и�
	int						RemoveOneLine(CWnd* pWnd, int iIndex);
	void					MoveUpControls(CWnd* pWnd, int iHeight);


protected:

	Cactus::GString			_strLabelSection;
	CString					_strCaption;

	CStatic*				_pLaber;
	CButton*				_pButtonBmp;
	PropLines_ListType		_lines;

	int						_iNextLinePosH;
	bool					_bHasTitle;
	bool					_bPhotoRight;
	vCard*					_pCard;
	bool					_bCombo;
};
typedef Cactus::list<PropGroup*>::type		NamePropGroups_ListType;

//----------------------------------------------------------------
// �û�����
class UserData : public Cactus::Object 
{
public:
	UserData(const Cactus::GString& key, int i1, int i2, const Cactus::String& str)
		: _strKey(key)
		, _intParam1(i1)
		, _intParam2(i2)
		, _strParam(str)
	{
	}
	virtual ~UserData(){}

	Cactus::GString			_strKey;
	int						_intParam1;		// -1 Title, 0 prop1, 1 prop2...
	int						_intParam2;		// 0 value0, 1 value1
	Cactus::String			_strParam;
};
typedef Cactus::map<CWnd*, UserData*>::type		CWndUserData_MapType;


//----------------------------------------------------------------
class Controls : public Cactus::Singleton<Controls>
	, public Cactus::Object
{
public:
	Controls(CScrollView* pWnd);
	virtual ~Controls();

	CStatic_ListType	_statics;
	CStatic_ListType	_staticsBmp;
	CEdit_ListType		_edits;
	CButton_ListType	_buttons;
	CButton_ListType	_buttonsBmp;
	CComboBox_ListType	_comboes;

	CStatic_ListType	_staticsLib;
	CStatic_ListType	_staticsBmpLib;
	CEdit_ListType		_editsLib;
	CButton_ListType	_buttonsLib;
	CButton_ListType	_buttonsBmpLib;
	CComboBox_ListType	_comboesLib;

	CStatic*	GetStatic(int iType);
	CStatic*	GetStaticBmp();
	CButton*	GetButton();
	CButton*	GetButtonBmp(int iType);
	CComboBox*	GetCombo();
	CEdit*		GetEdit();

	void	ReturnAllUsedControls();

	int		_iCurCmdID_Button;
	int		_iCurCmdID_Edit;
	int		_iCurCmdID_Combo;

	CFont	_fontDefault;
	CFont	_fontLarge;
	HICON	_iconAdd;
	HICON	_iconDel;

	CScrollView*	_pWnd;

	CWndUserData_MapType		_UserData;

	Cactus::FileConfig			_FileConfig;

	NameLabelGroupMapType		_PropLabelsGroup;
	NameGroupPropInfoMapType	_PropInfoGroup;

	Cactus::map<Cactus::GString, UINT>::type	_CmdButtons;

	Cactus::GStringVector		_supportPropNames;

	void					LoadPropInfoGroup(const Cactus::GString& strSectionName);
	Cactus::GStringVector	GetAllLabels(const Cactus::GString& strSectionName);
	SPropInfo*				GetAvaliablePropInfo(const Cactus::GString& strSectionName);
	SPropInfo*				GetPropInfoFromCaption(const Cactus::GString& strSectionName, const Cactus::GString& strCap);
	void					ResetLabelUsage();
	Cactus::GString			GetLabel(const Cactus::GString& strSectionName, const vCardProperty& prop);
};

//------------------------------------------------------------------------------------
#define		M_Top_1				180		//��̬�п�ʼλ��
#define		M_LineH				22		//���и�
#define		M_LineH_M			13		//���м��

//------------------------------------------------------------------------------------
#define		M_Cmd_Button_Start		WM_USER + 100
#define		M_Cmd_Button_END		WM_USER + 179

#define		M_Cmd_Edit_Start		WM_USER + 180
#define		M_Cmd_Edit_END			WM_USER + 299

#define		M_Cmd_Combo_Start		WM_USER + 300
#define		M_Cmd_Combo_END			WM_USER + 379
