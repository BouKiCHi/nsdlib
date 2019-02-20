/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#include "StdAfx.h"
#include "MusicItem.h"

/****************************************************************/
/*					�O���[�o���ϐ��i�N���X�����ǁE�E�E�j		*/
/****************************************************************/
extern	OPSW*			cOptionSW;	//�I�v�V�������ւ̃|�C���^�ϐ�

//==============================================================
//		�R���X�g���N�^
//--------------------------------------------------------------
//	������
//		const		_CHAR	_strName[]	�I�u�W�F�N�g��
//	���Ԓl
//				����
//==============================================================
MusicItem::MusicItem(const _CHAR _strName[]):
	iOffset(0),
	iSize(0),
	f_Optimize(false),
	strName(_strName)
{
	//Debug message�@�i���������o�͂���̂Œ��ӁB�j
	if(cOptionSW->cDebug & 0x01){
		_COUT << _T("Create Music Object : ") << strName << endl;
	}
}

MusicItem::MusicItem(int _id, const _CHAR _strName[]):
	iOffset(0),
	iSize(0),
	f_Optimize(false),
	strName(_strName)
{
	//Debug message�@�i���������o�͂���̂Œ��ӁB�j
	if(cOptionSW->cDebug & 0x01){
		_COUT << _T("Create Music Object : ====[ ") << strName << _T("(");
		cout << _id;
		_COUT << _T(") ]====") << endl;
	}
}

//==============================================================
//		�f�X�g���N�^
//--------------------------------------------------------------
//	������
//				����
//	���Ԓl
//				����
//==============================================================
MusicItem::~MusicItem(void)
{
	clear();

	//Debug message�@�i���������o�͂���̂Œ��ӁB�j
	if(cOptionSW->cDebug & 0x80){
		_COUT << _T("Delete Music Object : ") << strName << endl;
	}
}

//==============================================================
//		�N���A
//--------------------------------------------------------------
//	������
//				����
//	���Ԓl
//				����
//==============================================================
void	MusicItem::clear(void)
{
	//----------------------
	//Local�ϐ�
	list<	MusicItem*>::iterator	itItem;

	//----------------------
	//clear
	code.clear();

	//----------------------
	//Delete Class
	if(!ptcItem.empty()){
		itItem = ptcItem.begin();
		while(itItem != ptcItem.end()){
			delete *itItem;
			itItem++;
		}
		ptcItem.clear();
	}
}

void	MusicItem::clear(int _id)
{

	//Debug message�@�i���������o�͂���̂Œ��ӁB�j
	if(cOptionSW->cDebug & 0x40){
		_COUT << _T("Clear Music Object : ====[ ") << strName << _T("(") << _id << _T(") ]====") << endl;
	}

	clear();
}

//==============================================================
//		�R�[�h�T�C�Y�̎擾
//--------------------------------------------------------------
//	������
//				����
//	���Ԓl
//		size_t
//==============================================================
size_t		MusicItem::getSize()
{
	return(iSize);
}

//==============================================================
//		�R�[�h�̃I�t�Z�b�g�A�h���X�̎擾
//--------------------------------------------------------------
//	������
//				����
//	���Ԓl
//		unsigned	int	
//==============================================================
unsigned	int		MusicItem::getOffset()
{
	return(iOffset);
}

//==============================================================
//		�I�t�Z�b�g�A�h���X�̐ݒ�
//--------------------------------------------------------------
//	������
//		unsigned	int		_offset
//	���Ԓl
//				����
//==============================================================
unsigned	int		MusicItem::SetOffset(unsigned	int _offset)
{
	//----------------------
	//Local�ϐ�
	list<	MusicItem*>::iterator	itItem;
	unsigned	int		i	=	0;

	//Debug message�@�i���������o�͂���̂Œ��ӁB�j
	if(cOptionSW->cDebug & 0x02){
		_COUT << _T("Object Address [0x") << hex << setw(4) << setfill(_T('0')) << _offset << _T("]: ");
		while(i < code.size()){
			_COUT	<<	hex	<<	setw(2)	<<	setfill(_T('0'))	<<	(unsigned int)(code[i] & 0xFF)	<<	_T(" ");
			i++;
		}
		_COUT  << dec	<< _T(": ") << strName << endl;
	}

	iOffset = _offset;
	_offset	+= (unsigned int)code.size();

	if(!ptcItem.empty()){
		itItem = ptcItem.begin();
		while(itItem != ptcItem.end()){
			_offset = (*itItem)->SetOffset(_offset);
			itItem++;
		}
	}

	return(_offset);
}

//==============================================================
//		�R�[�h�̎擾
//--------------------------------------------------------------
//	������
//		unsigned	int	n	�Y����
//	���Ԓl
//		unsigned	char	���e
//==============================================================
unsigned	char	MusicItem::getCode(unsigned int n)
{
	unsigned	char	iCode;
	
	if((n<0) || (n>iSize)){
		iCode = 0xFF;
	} else {
		iCode = code[n];
	}

	return(iCode);
}

//==============================================================
//		�R�[�h�̎擾
//--------------------------------------------------------------
//	������
//		string*		_str
//	���Ԓl
//				����
//==============================================================
void	MusicItem::getCode(string* _str)
{
	//----------------------
	//Local�ϐ�
	list<	MusicItem*>::iterator	itItem;

	_str->append(code);

	if(!ptcItem.empty()){
		itItem = ptcItem.begin();
		while(itItem != ptcItem.end()){
			(*itItem)->getCode(_str);
			itItem++;
		}
	}
}

//==============================================================
//		�R�[�h�̐ݒ�
//--------------------------------------------------------------
//	������
//		string*		_str
//	���Ԓl
//				����
//==============================================================
void	MusicItem::setCode(string* _str)
{
	code.clear();
	code.assign(*_str);
	iSize = code.size();
}

//==============================================================
//		�R�[�h�̎擾
//--------------------------------------------------------------
//	������
//		MusicFile*	MUS		�R�[�h���o�͂���ȃf�[�^�t�@�C���E�I�u�W�F�N�g
//	���Ԓl
//				����
//==============================================================
void	MusicItem::getAsm(MusicFile* MUS)
{
	//----------------------
	//Local�ϐ�
	unsigned	int	i = 0;
	list<	MusicItem*>::iterator	itItem;

	if(code.size() > 0){
		while(i < code.size()){
			if(i==0){
				*MUS << "	.byte	$";
			} else {
				*MUS << " ,$";
			}
			*MUS << hex << setw(2) << setfill('0') << (int)(code[i] & 0xFF);
			i++;
		}
		*MUS << dec << endl;
	}

	if(!ptcItem.empty()){
		itItem = ptcItem.begin();
		while(itItem != ptcItem.end()){
			(*itItem)->getAsm(MUS);
			itItem++;
		}
	}
}
