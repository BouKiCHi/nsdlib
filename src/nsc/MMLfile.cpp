/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#include "StdAfx.h"
#include "MMLfile.h"

/****************************************************************/
/*					�O���[�o���ϐ��i�N���X�����ǁE�E�E�j		*/
/****************************************************************/
extern	OPSW*			cOptionSW;	//�I�v�V�������ւ̃|�C���^�ϐ�

//==============================================================
//		�R���X�g���N�^
//--------------------------------------------------------------
//	������
//		const	char*	strFileName		�t�@�C����
//	���Ԓl
//				����
//==============================================================
MMLfile::MMLfile(const char*	strFileName):
	offset_Ei(0),
	offset_Ev(0),
	offset_En(0),
	offset_Em(0),
	iReleaseVolume(2),
	iRepeatMode(0),
	iTieMode(0),
	timebase(24),
	octave_reverse(false),
	q_reverse(false),
	rest(2),
	wait(0),
	QMax(8),
	priority(0),
	p_macro(0),
	f_macro(false),
	f_2to1(false)
	{
	//File open
	nowFile	= new FileInput();

	nowFile->fileopen(strFileName);
	ptcFiles.push_back(nowFile);

	iFiles = 0;
}

//==============================================================
//		�f�X�g���N�^
//--------------------------------------------------------------
//	������
//				����
//	���Ԓl
//				����
//==============================================================
MMLfile::~MMLfile(void)
{

	//----------------------
	//Local�ϐ�
	vector	<FileInput*				>::iterator	itFiles;
	map		<string,		string	>::iterator	itMac;
	map		<unsigned int,	Patch*	>::iterator	itPatch;

	//----------------------
	//Delete Class

	//�J�����t�@�C����S������
	itFiles = ptcFiles.begin();
	while(itFiles != ptcFiles.end()){
		(*itFiles)->close();
		delete *itFiles;
		itFiles++;
	}
	ptcFiles.clear();

	//�}�N����S���������B
	if(!ptcMac.empty()){
		itMac = ptcMac.begin();
		while(itMac != ptcMac.end()){
			itMac->second.clear();
			itMac++;
		}
		ptcMac.clear();
	}
	lv_Mac.clear();

	//�p�b�`��S���������B
	if(!ptcPatch.empty()){
		itPatch = ptcPatch.begin();
		while(itPatch != ptcPatch.end()){
			delete	itPatch->second;
			itPatch++;
		}
		ptcPatch.clear();
	}

}

//==============================================================
//			���݃R���p�C���������̃t�@�C����[EOF]�`�F�b�N
//--------------------------------------------------------------
//	������
//			����
//	���Ԓl
//			bool		true	[EOF]�����o
//						false	�܂��I��肶��Ȃ��B
//==============================================================
bool	MMLfile::eof(void)
{
	return( nowFile->eof() );
}

//==============================================================
//			MML�t�@�C���̏I���`�F�b�N
//--------------------------------------------------------------
//	������
//			����
//	���Ԓl
//			bool		true	���t�@�C����[EOF]�����o
//						false	�܂��I��肶��Ȃ��B
//==============================================================
bool	MMLfile::eom(void)
{
	if(( iFiles == 0 ) && ( eof() )){
		return(true);
	}
	return(false);
}

//==============================================================
//		�C���N���[�h�t�@�C��
//--------------------------------------------------------------
//	������
//			����
//	���Ԓl
//			����
//	������
//			���݂̃t�@�C���|�C���^�ɏ����Ă���t�@�C�����C���N���[�h����
//==============================================================
void	MMLfile::include()
{
	//----------------------
	//Local�ϐ�
	vector	<FileInput*>::iterator	itFiles;
	string	_name = GetString();

	//----------------------
	//�����t�@�C�����J����Ă��Ȃ����`�F�b�N
	itFiles = ptcFiles.begin();
	while(itFiles != ptcFiles.end()){
		if( *(*itFiles)->GetFilename() == _name ){
			Err(_T("���ɓ����t�@�C����#include�ŊJ����Ă��܂��B"));
		}
		itFiles++;
	}

	//----------------------
	//File open
	nowFile	= new FileInput();

	nowFile->fileopen(_name.c_str(), &cOptionSW->m_pass_inc);
	ptcFiles.push_back(nowFile);

	iFiles++; 
}

//==============================================================
//		�}�N���̐ݒ�
//--------------------------------------------------------------
//	������
//		int		i_Lv	�ݒ肷�郌�x��
//	���Ԓl
//				����
//	������
//			���݂̃t�@�C���|�C���^�ɏ����Ă���}�N�����`����B
//==============================================================
void	MMLfile::SetMacro(int i_Lv)
{
	char	cData;
	string	macro_name		="";
	string	macro_contents	="";

	int		iKakko	= 0;

	//------------------
	//�}�N�����̎擾
	while((cData = cRead()) > 0x20){
		if(cData == '{'){
			break;
		}
		macro_name += cData;
	};
	Back();

	//------------------
	//�}�N�����̏d���`�F�b�N
	if(ptcMac.count(macro_name) != 0){
		Err(_T("���ɂ��̃}�N�����͑��݂��Ă��܂��B"));
	}

	//------------------
	//�}�N�����e�̎擾
	while(cRead() != '{'){
		if(eof()){
			Err(_T("������J�n������{��������܂���B"));
		}
	}

	while(('}' != (cData = cRead())) || (iKakko != 0)){
		if(eof()){
			Err(_T("������I��������}��������܂���B"));
		}
		if(cData == '{'){
			iKakko++;
		} else if(cData == '}'){
			iKakko--;
		}
		macro_contents += cData;
	}
	macro_contents += " ";

	//------------------
	//�}�N�����e�̐ݒ�
	ptcMac[macro_name] = macro_contents;
	lv_Mac[macro_name] = i_Lv;

}

//==============================================================
//		�}�N���̐ݒ�
//--------------------------------------------------------------
//	������
//		int		i_Lv	�폜���郌�x��
//	���Ԓl
//				����
//	������
//			�Y�����x���̃}�N�����폜����
//==============================================================
void	MMLfile::DeleteMacro(int i_Lv)
{

	//----------------------
	//Local�ϐ�
	map		<string,		string	>::iterator	itMac;
	string	macro_name;
	int		macro_lv;

	//----------------------
	//���YLv�̃}�N�����������B
	if(!ptcMac.empty()){
		itMac = ptcMac.begin();
		while(itMac != ptcMac.end()){
			macro_name	= itMac->first;
			macro_lv	= lv_Mac[macro_name];
			itMac++;
			if(i_Lv == macro_lv){
				ptcMac.erase(macro_name);
				lv_Mac.erase(macro_name);
			}
		}
	}

}

//==============================================================
//		�}�N���̌Ăяo��
//--------------------------------------------------------------
//	������
//				����
//	���Ԓl
//				����
//	������
//			���݂̃t�@�C���|�C���^�ɏ����Ă���}�N�����Ăяo��
//==============================================================
void	MMLfile::CallMacro(void)
{
	char							cData;
	int								i		= 0;
	int								n		= 0;
	int								iSize	= ptcMac.size();
	string							_name	= "";
	string*							strMac	= new	string[iSize];
	map<string,string>::iterator	itMac	= ptcMac.begin();

	//------------------
	//�S�}�N�����̎擾
	if(!ptcMac.empty()){
		do{
			strMac[i] = itMac->first;
			i++;
			itMac++;
		}while(itMac != ptcMac.end());
	}

	//------------------
	//�}�N�����̏ƍ�
	do{
		cData = cRead();
		_name += cData;
		i = 0;			//���[�v�p
		n = 0;			//�q�b�g��
		if(cData > 0x20){
			while(i<iSize){
				if(strMac[i].find(_name.c_str()) == 0){
					n++;		//�}�N�����擪������q�b�g
				}
				i++;
			}
		}
	} while(n>0);		//�q�b�g����0�ɂȂ�܂ŁA�J��Ԃ��B

	Back();										//�|�C���^���P�߂��B
	_name = _name.substr(0, _name.length()-1);	//�P�������炷�B

	//------------------
	//�}�N�����̑��݃`�F�b�N
	if(ptcMac.count(_name) == 0){
		Err(_T("���̃}�N�����͑��݂��Ă��܂���B"));
	}

	//------------------
	//�}�N�����̏d���`�F�b�N
	i = 0;
	while(i < p_macro){
		if(s_macro[i].name == _name){
			Err(_T("�}�N�����œ����}�N�����Ăяo���Ă��܂��B"));
		}
		i++;
	}

	//------------------
	//�l�X�g��������ۑ�
	if(p_macro > 0){
		s_macro[p_macro-1].name = nowMacro.name;
		s_macro[p_macro-1].line = nowMacro.line;
	}

	nowMacro.name = _name;
	nowMacro.line = 0;

	s_macro.push_back(nowMacro);
	p_macro++;

	delete[]	strMac;
}

//==============================================================
//		�p�b�`�̐ݒ�
//--------------------------------------------------------------
//	������
//				����
//	���Ԓl
//				����
//	������
//			���݂̃t�@�C���|�C���^�ɏ����Ă���}�N�����`����B
//==============================================================
void	MMLfile::SetPatch(void)
{

	int			i		= GetNum();

	//�d���`�F�b�N
	if(ptcPatch.count(i) != 0){
		Err(_T("Patch()�u���b�N�œ����ԍ����w�肳��܂����B"));
	}

	ptcPatch[i] =  new Patch(this, i);

}

//==============================================================
//		�p�b�`�̗L���`�F�b�N
//--------------------------------------------------------------
//	������
//			unsigned int _no	�p�b�`�ԍ�
//	���Ԓl
//			bool				���邩�ǂ���
//	������
//			
//==============================================================
bool	MMLfile::ChkPatch(unsigned int _no)
{
	bool	result;

	if(ptcPatch.count(_no) == 0){
		result = false;
	} else {
		result = true;
	}
	return(result);
}

//==============================================================
//			���݃R���p�C���������̃t�@�C���|�C���^�̎擾
//--------------------------------------------------------------
//	������
//			����
//	���Ԓl
//			int		���݂̃t�@�C���|�C���^
//==============================================================
int		MMLfile::tellg(void)
{
	int	i;

	if(p_macro > 0){
		i = nowMacro.line;
	} else {
		i = nowFile->tellg();
	}

	return(i);
}

//==============================================================
//			���݃R���p�C���������̃t�@�C���|�C���^���ΓI�Ɉړ�
//--------------------------------------------------------------
//	������
//			long	iSize	�ړ��l
//	���Ԓl
//			����
//==============================================================
void	MMLfile::StreamPointerMove(long iSize)
{
	if(p_macro > 0){
		nowMacro.line = iSize;
	} else {
		nowFile->StreamPointerMove(iSize);
	}
}

//==============================================================
//			���݃R���p�C���������̃t�@�C���|�C���^����߂�
//--------------------------------------------------------------
//	������
//			����
//	���Ԓl
//			����
//==============================================================
void	MMLfile::Back_one(void)
{
	if(f_macro == true){
		f_macro = false;
		if(p_macro > 0){
			s_macro[p_macro-1].name = nowMacro.name;
			s_macro[p_macro-1].line = nowMacro.line;
		}
		p_macro++;
		nowMacro.name = s_macro[p_macro-1].name;
		nowMacro.line = s_macro[p_macro-1].line;
		nowMacro.line--;
	} else {
		if(p_macro > 0){
			nowMacro.line--;
		} else {
			nowFile->Back();
		}
	}
}

void	MMLfile::Back(void)
{
	if(f_2to1==true){
		Back_one();
		Back_one();		//�O��̓ǂݍ��݂��}���`�o�C�g������������A�Q�߂��B
	} else {
		Back_one();
	}

}

//==============================================================
//			�PByte�ǂݍ���
//--------------------------------------------------------------
//	������
//			����
//	���Ԓl
//			char	�ǂݍ��ݒl
//==============================================================
char	MMLfile::read_char(void)
{
	char	cData;

	//���O�̓ǂݍ��݂ŁA�}�N���̏I�[�ɍs�����B
	if(f_macro == true){
		f_macro	= false;		
		s_macro.pop_back();		//�}�N���X�^�b�N���P�߂��B
	}

	if(p_macro > 0){
		//�}�N������ǂݍ��ޏꍇ
		cData = ptcMac[nowMacro.name][nowMacro.line];
		nowMacro.line++;
		//�}�N���I�[�H
		if(ptcMac[nowMacro.name].size() == nowMacro.line){
			s_macro[p_macro-1].name = nowMacro.name;
			s_macro[p_macro-1].line = nowMacro.line;
			p_macro--;
			f_macro	= true;
			if(p_macro > 0){
				nowMacro.name = s_macro[p_macro-1].name;
				nowMacro.line = s_macro[p_macro-1].line;
			}
		}
	} else {
		//�t�@�C������ǂݍ��ޏꍇ
		cData		= nowFile->cRead();
	}

	return(cData);

}

//==============================================================
//			�PByte�ǂݍ���
//--------------------------------------------------------------
//	������
//			����
//	���Ԓl
//			char	�ǂݍ��ݒl
//==============================================================
char	MMLfile::cRead(void)
{
	unsigned	char	cData = read_char();
	unsigned	char	cDataMSB;

	switch(cData){
		case(0x81):
			cDataMSB = read_char();
			f_2to1 = true;
			switch(cDataMSB){
				case(0x67):
					cData = '"';
					break;
				case(0x68):
					cData = '"';
					break;
				case(0x69):
					cData = '(';
					break;
				case(0x6A):
					cData = ')';
					break;
				case(0x6F):
					cData = '{';
					break;
				case(0x70):
					cData = '}';
					break;
				case(0x7B):
					cData = '+';
					break;
				case(0x7C):
					cData = '-';
					break;
				case(0x94):	//��
					cData = '#';
					break;
				case(0xF3):	//��
					cData = '-';
					break;
				default:
					f_2to1 = false;
					Back();
					break;
			}
			break;

		case(0x82):
			cDataMSB = read_char();
			f_2to1 = true;
			switch(cDataMSB){
				case(0x4F):
					cData = '0';
					break;
				case(0x50):
					cData = '1';
					break;
				case(0x51):
					cData = '2';
					break;
				case(0x52):
					cData = '3';
					break;
				case(0x53):
					cData = '4';
					break;
				case(0x54):
					cData = '5';
					break;
				case(0x55):
					cData = '6';
					break;
				case(0x56):
					cData = '7';
					break;
				case(0x57):
					cData = '8';
					break;
				case(0x58):
					cData = '9';
					break;
				default:
					f_2to1 = false;
					Back();
					break;
			}
			break;

		default:
			f_2to1 = false;
			break;
	}

	return(cData);
}

//==============================================================
//			�PByte�ǂݍ��݁i�`�F�b�N�t���j
//--------------------------------------------------------------
//	������
//			����
//	���Ԓl
//			char	�ǂݍ��ݒl
//	������
//		�P�����̓ǂݍ��݁B
//		�A���A�ȉ��̋L�q���������ꍇ�́A�������΂��B
//			�E0x20�ȉ��̃L�����N�^�[�R�[�h
//			�E�R�����g
//		�܂��A�C���N���[�h�t�@�C������[EOF]�����o������A�O�̃t�@�C���ɖ߂�
//==============================================================
char	MMLfile::GetChar(void)		//1Byte�̓ǂݍ���
{
	unsigned	char	cData;

	do{
		cData = cRead();

		//[EOM]�H
		if( eom() )
		{
			return(0);

		//[EOF]�H
		} else if( eof() ){
			nowFile->close();
			delete	nowFile;
			ptcFiles.pop_back();
			nowFile = ptcFiles.back();
			iFiles--;
			//continue

		//0x20�ȉ��̐���R�[�h
		} else if(cData <= 0x20){
			//continue

		//�A�Z���u������^�C�v�̃R�����g�H
		} else if(cData == ';'){
			do{
				if( eof() ){
					break;
				}
				cData = cRead();		//���̃o�C�g��ǂݍ���
			} while(cData != 0x0A);
		
		//�b����^�C�v�̃R�����g�H
		} else 	if(cData == '/'){
			cData = cRead();		//���̃o�C�g��ǂݍ���
			switch(cData){

				//�P�s�R�����g
				case('/'):
					do{
						if( eof() ){
							break;
						}
						cData = cRead();		//���̃o�C�g��ǂݍ���
					}while(cData != 0x0A);
					break;

				//�͈̓R�����g
				case('*'):
					do{
						do{
							cData = cRead();		//���̃o�C�g��ǂݍ���
							if(eof()){
								Err(_T("�R�����g�I�[ */ ������܂���B"));
							}
						}while(cData != '*');
						cData = cRead();
						Back();					//StreamPointerAdd(-1);
					} while(cData != '/');
							cRead();			//StreamPointerAdd(+1); ���������B
					break;

				//����ȊO
				default:
					Err(_T("�R�����g�ł����H"));
					break;
			}

		//[EOF]�ł��A�R�����g�ł������ꍇ�B
		} else {
			break;		//while(1) �� break;
		}

	} while(true);

	return(cData);
}

//==============================================================
//			������ǂݍ���
//--------------------------------------------------------------
//	������
//			����
//	���Ԓl
//			string	�ǂݍ��񂾕�����
//	������
//			MML����"��"�ň͂܂ꂽ��������擾����B
//==============================================================
string	MMLfile::GetString(void)
{
	char	cData;
	
	string	_str;
/*
	while(cRead() != '"'){
		if(eof()){
			Err(_T("������J�n������\"��������܂���B"));
		}
	}
*/
	cData = GetChar();
	if(cData != '"'){
			Err(_T("������J�n������\"��������܂���B"));
	}

	while('"' != (cData = cRead())){
		if(eof()){
			Err(_T("������I��������\"��������܂���B"));
		}
		_str += cData;
	}
	return(_str);
}

//==============================================================
//			���l�ǂݍ���
//--------------------------------------------------------------
//	������
//			����
//	���Ԓl
//			int		�ǂݍ��񂾐��l
//	������
//			MML����(��)�ň͂܂ꂽ��������擾����B
//==============================================================
int	MMLfile::GetNum(void)
{
	char	cData;
	int		iResult;
/*
	while(cRead() != '('){
		if(eof()){
			Err(_T("���l�J�n������(��������܂���B"));
		}
	}
*/
	cData = GetChar();
	if(cData != '('){
		Err(_T("���l�J�n������(��������܂���B"));
	}

	iResult = GetInt();
/*
	while(')' != (cData = cRead())){
		if(eof()){
			Err(_T("���l�I��������)��������܂���B"));
		}
	}
*/
	cData = GetChar();
	if(cData != ')'){
		Err(_T("���l�I��������)��������܂���B"));
	}

	return(iResult);
}

//==============================================================
//			���l�ǂݍ���
//--------------------------------------------------------------
//	������
//			����
//	���Ԓl
//			int		�ǂݍ��񂾐��l
//==============================================================
int		MMLfile::GetInt(void)
{
				bool	neg = false;			//����
				int		iResult = 0;			//���Z����
	unsigned	char	cData = GetChar();

	//16�i��
	if(cData == '$'){
		cData = cRead();
		if(cData == '+'){
			neg = false;
			cData	 = cRead();
		}
		if(cData == '-'){
			neg = true;
			cData	 = cRead();
		}
		while(((cData >= '0') && (cData <= '9')) || ((cData >= 'a') && (cData <= 'f')) || ((cData >= 'A') && (cData <= 'F'))){
			iResult <<= 4;
			if((cData >= '0') && (cData <= '9')){
				iResult += (unsigned int)cData - 0x30;
			} else if((cData >= 'A') && (cData <= 'F')){
				iResult += (unsigned int)cData - 0x41 + 10;
			} else if((cData >= 'a') && (cData <= 'f')){
				iResult += (unsigned int)cData - 0x61 + 10;
			}
			cData	 = cRead();
		}

	//�Q�i��
	} else if(cData == '%'){
		cData = cRead();
		if(cData == '-'){
			neg = true;
			cData	 = cRead();
		}
		while((cData >= '0') && (cData <= '1')){
			iResult <<= 1;
			iResult += (unsigned int)cData - 0x30;
			cData	 = cRead();
		}

	//10�i��
	} else if(((cData >= '0') && (cData <= '9')) || (cData == '-') || (cData == '+')){
		if(cData == '+'){
			neg = false;
			cData	 = cRead();
		}
		if(cData == '-'){
			neg = true;
			cData	 = cRead();
		}
		while((cData >= '0') && (cData <= '9')){
			iResult *= 10;
			iResult += (unsigned int)cData - 0x30;
			cData	 = cRead();
		}

	} else {
			Err(_T("���l�ȊO���w�肳��܂����B"));
	}

	//�|�C���^���P�߂�
	Back();							//	StreamPointerAdd(-1);

	//����
	if(neg == true){
		iResult = -iResult;
	}

	return(iResult);
}

//==============================================================
//			�����ǂݍ���
//--------------------------------------------------------------
//	������
//			int		�L�ڂ��Ȃ��ꍇ�̉���[tick]
//	���Ԓl
//			int		����[tick]
//	������
//		������ǂݍ��ށi�t�_�t���j
//==============================================================
int	MMLfile::readLength(int DefaultLength){

	char	cData;				//�ǂݍ��ݗp
	int		iLength;			//���� [tick]
	int		iDot;				//�t�_�v�Z�p [tick]
	int		iMod;
	int		i;

	//�ǂݍ���
	cData = GetChar();

	//�|�C���^���P�߂�
	Back();							//StreamPointerAdd(-1);

	//Length
	if(((cData >= '0') && (cData <= '9')) || (cData == '.')){
		if((cData >= '0') && (cData <= '9')){
			i = GetInt();
			if(i==0){
				Err(_T("������0�͎g���܂���B"));
			}
			iLength = (timebase * 4) / i;
			iMod	= (timebase * 4) % i;
			if(iMod != 0){
				Warning(_T("�����̌v�Z�Ŋ���؂�܂���ł����B�����_�͐؎̂Ă��܂��B"));
			}
		} else {
			//�t�_�����L�q�����ꍇ�B
			if(DefaultLength == -1){
				Err(_T("�������L�q���ĉ������B"));
			} else {
				iLength = DefaultLength;
			}
		}

		iDot	= iLength;
		//�t�_
		while((cData = cRead()) == '.'){
			if(iLength == -1){
				Err(_T("�����̋L�q�Ȃ��ɕt�_���g���ꍇ�́A�O������l �R�}���h���L�q���ĉ������B"));
			}
			iMod = (iDot & 0x01);
			iDot >>= 1;
			if(iMod != 0){
				Warning(_T("�t�_�̌v�Z�Ŋ���؂�܂���ł����B�����_�͐؎̂Ă��܂��B"));
			}
			iLength += iDot;
		};
		Back();							//StreamPointerAdd(-1);



	//Tick
	} else if (cData == '%'){
		GetChar();	//1�i�߂�B
		iLength = GetInt();

	} else {
		//�����������Ȃ��ꍇ
		if(DefaultLength == -1){
			Err(_T("�������L�q���ĉ������B"));
		} else {
			iLength = -1;
		}
	}

	return(iLength);
}

//==============================================================
//			�����ǂݍ���
//--------------------------------------------------------------
//	������
//			int		�L�ڂ��Ȃ��ꍇ�̉���[tick]
//	���Ԓl
//			int		�ǂݍ��񂾐��l
//					������������ĂȂ��ꍇ�́A -1 ��Ԃ��B
//==============================================================
int		MMLfile::GetLength(int DefaultLength)	//
{
	char	cData;				//�ǂݍ��ݗp
	int		iLength;			//���� [tick]
	int		iCalc;
	bool	add;

	//�����ǂݍ���
	iLength = readLength(DefaultLength);

	//�����̉����Z
	cData = cRead();
	while((cData == '-') || (cData == '~') || (cData == '+') || ((iTieMode == 1) && (cData == '^'))){
		if(iLength == -1){
			iLength = DefaultLength;
		}

		//��Ōv�Z���镄���̃`�F�b�N
		if((cData == '+') || (cData == '^')){
			add = true;
		} else {
			add = false;
		}
		iCalc = readLength(DefaultLength);

		if(iCalc == -1){
		//	Warning(_T("�����̉����Z�l�ɐ��l�ȊO���w�肳��Ă��܂��B"));
			iCalc = DefaultLength;
		}

		if(add == true){
			iLength += iCalc;
		} else {
			iLength -= iCalc;
		}
		cData = cRead();
	};
	Back();							//StreamPointerAdd(-1);

	if(iLength == -1){
		return(iLength);
	}

	if((iLength < 1) || (iLength > 255)){
		Err(_T("�����́A%1�i96�j�`%255�i1+1+2+8�j�̊ԂŎw�肵�ĉ������B255[tick]�𒴂���ꍇ�̓^�C`&', `^'���g���ĉ������B"));
	}

	return(iLength);
}

//==============================================================
//		�R�}���hID�̎擾
//--------------------------------------------------------------
//	������
//		Command_Info*			_command
//		unsigned		int		_size
//	���Ԓl
//		int			�R�}���h�R�[�h�@�i-1��unknown command�j
//==============================================================
int	MMLfile::GetCommandID(const Command_Info _command[], unsigned int _size)
{
	unsigned	int		ptCommand	= tellg();	//���݂̃t�@�C���|�C���^��ێ����Ă����B
	unsigned	int		i			= 0;		//�����p
	unsigned	int		j;						//������`�F�b�N�p

	//�R�}���h������̃`�F�b�N
	while(i < _size){
		StreamPointerMove(ptCommand);
		j = 0;
		do{
			if(_command[i].str[j] == 0){
				return(_command[i].id);
			}
		} while(cRead() == _command[i].str[j++]);
		i++;
	}
	return(-1);
}

//==============================================================
//		�G���[����
//--------------------------------------------------------------
//	������
//		const	_CHAR	msg[]	�G���[���b�Z�[�W
//	���Ԓl
//				����
//==============================================================
void	MMLfile::Err(const _CHAR msg[])
{

	//�G���[���e��\��
	if(cOptionSW->fErr == true){
		//���݂̃t�@�C�����ƁA�s����\��
		cerr << "[ ERROR ] " << nowFile->GetFilename()->c_str() << " (Line = " << nowFile->GetLine() << ") : ";
		_CERR << msg << endl;
	} else {
		//���݂̃t�@�C�����ƁA�s����\��
		cout << "[ ERROR ] " << nowFile->GetFilename()->c_str() << " (Line = " << nowFile->GetLine() << ") : ";
		_COUT << msg << endl;
	}

	//�ُ�I��
	nsc_exit(EXIT_FAILURE);
}

//==============================================================
//		���[�j���O����
//--------------------------------------------------------------
//	������
//		const	_CHAR	msg[]	���[�j���O���b�Z�[�W
//	���Ԓl
//				����
//==============================================================
void	MMLfile::Warning(const _CHAR msg[])
{

	//���[�j���O���e��\��
	if(cOptionSW->fErr == true){
		//���݂̃t�@�C�����ƁA�s����\��
		cerr << "[WARNING] " << nowFile->GetFilename()->c_str() << " (Line = " << nowFile->GetLine() << ") : ";
		_CERR << msg << endl;
	} else {
		//���݂̃t�@�C�����ƁA�s����\��
		cout << "[WARNING] " << nowFile->GetFilename()->c_str() << " (Line = " << nowFile->GetLine() << ") : ";
		_COUT << msg << endl;
	}
}
