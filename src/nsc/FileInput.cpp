/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#include "StdAfx.h"
#include "FileInput.h"

/****************************************************************/
/*					�O���[�o���ϐ��i�N���X�����ǁE�E�E�j		*/
/****************************************************************/
extern	OPSW*			cOptionSW;	//�I�v�V�������ւ̃|�C���^�ϐ�

//==============================================================
//		�f�X�g���N�^
//--------------------------------------------------------------
//	������
//				����
//	���Ԓl
//				����
//==============================================================
FileInput::FileInput(void):
	readData(0),
	iLine(1)
{
}

//==============================================================
//		�f�X�g���N�^
//--------------------------------------------------------------
//	������
//				����
//	���Ԓl
//				����
//==============================================================
FileInput::~FileInput(void)
{
}

//--------------------------------
//�t�@�C�����J���@�G���[�����t��
//--------------------------------
void	FileInput::fileopen(const char*	_strFileName){

	errno = 0;	//�O���[�o���ϐ� errno ���O�ɏ�����
	clear();	//�t���O�̃N���A

	open(_strFileName,ios_base::in | ios_base::binary);
	if(good()==false){
		perror(_strFileName);
		nsc_exit(EXIT_FAILURE);
	};
	strFilename = _strFileName;
};

//--------------------------------
//�t�@�C�����J���@�G���[�����t��
//--------------------------------
void	FileInput::fileopen(const char*	_strFileName,SearchPass* _pass)
{
	bool	success	= false;

	//�悸�́A���̂܂�
	errno = 0;	//�O���[�o���ϐ� errno ���O�ɏ�����
	clear();	//�t���O�̃N���A
	open(_strFileName,ios_base::in | ios_base::binary);
	if(cOptionSW->flag_SearchPass){
		perror(_strFileName);
	}

	if(good()==true){
		success = true;
	} else {

		//�����p�X
		int		i		= 0;
		int		iSize	= _pass->count();
		string	name;
		string	workName= string(_strFileName);
		int		loc		= workName.rfind('/');

		//�w��̃t�@�C���Ƀp�X��������Ă�����A�����B
		if(loc != string::npos){
			workName.erase(0, loc);		//�t�@�C�����̂�
		}

		while(i < iSize){

#ifdef _WIN32
			//Windows�̏ꍇ�́A���΃p�X���܂߂Č�������iUNIX�n�͕s�j
			//�w��̌����p�X����Ƃ������΃p�X����������B
			errno = 0;	//�O���[�o���ϐ� errno ���O�ɏ�����
			clear();	//�t���O�̃N���A

			name.assign(_pass->get(i));
			name.append(_strFileName);
			open(name.c_str(),ios_base::in | ios_base::binary);
			if(cOptionSW->flag_SearchPass){
				perror(name.c_str());
			}
			if(good()==true){
				success = true;
				break;
			};
#endif
			//WINDOWS, UNIX�n����
			//�����p�X�{�t�@�C�����݂̂Ō����B
			errno = 0;	//�O���[�o���ϐ� errno ���O�ɏ�����
			clear();	//�t���O�̃N���A

			name.assign(_pass->get(i));
			name.append(workName);
			open(name.c_str(),ios_base::in | ios_base::binary);
			if(cOptionSW->flag_SearchPass){
				perror(name.c_str());
			}
			if(good()==true){
				success = true;
				break;
			};

			i++;
		}

	};

	if(success == false){
		_CERR << _T("�S�Ă̌����p�X�ŁA�t�@�C����������܂���ł����B") << endl;
		if(cOptionSW->flag_SearchPass == false){
			perror(_strFileName);
		}
		nsc_exit(EXIT_FAILURE);
	}
}

//--------------------------------
//���΃V�[�N
//--------------------------------
void	FileInput::StreamPointerAdd(long iSize){
	seekg((long)iSize,ios::cur);
};

//--------------------------------
//��΃V�[�N
//--------------------------------
void	FileInput::StreamPointerMove(long iSize){
		seekg((long)iSize,ios::beg);
};

void	FileInput::Back(void)
{
	StreamPointerAdd(-1);
	if(readData == 0x0A){
		iLine--;
	}

	//�X�V
	read((char*)&readData, sizeof(unsigned char));
	StreamPointerAdd(-1);
}
//--------------------------------
//1Byte�ǂݍ���
//--------------------------------
unsigned	char	FileInput::cRead()
{
	read((char*)&readData, sizeof(unsigned char));
	if(readData == 0x0A){
		iLine++;
	}

	return(readData);
};
//--------------------------------
//�T�C�Y
//--------------------------------
unsigned	int	FileInput::GetSize(){

	unsigned	int	iData;
	unsigned	int	iDataT = tellg();

	seekg(0		,ios::end);
	iData = tellg();
	seekg(iDataT,ios::beg);

	return(iData);
};
