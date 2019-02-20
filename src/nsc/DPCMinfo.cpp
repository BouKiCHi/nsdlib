/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#include "StdAfx.h"
#include "DPCMinfo.h"

//==============================================================
//		�R���X�g���N�^
//--------------------------------------------------------------
//	������
//		MMLfile*			MML			MML�t�@�C���̃I�u�W�F�N�g
//		const		_CHAR	_strName[]	���̃I�u�W�F�N�g�̖��O
//	���Ԓl
//					����
//==============================================================
DPCMinfo::DPCMinfo(MMLfile* MML, bool _bank, const _CHAR _strName[]):
	MusicItem(_strName)
{
	//----------------------
	//Local�ϐ�

	bank = _bank;

//	�萔��`
enum	Command_ID_mml {
	DPCM_C,
	DPCM_Cis,
	DPCM_D,
	DPCM_Dis,
	DPCM_E,
	DPCM_F,
	DPCM_Fis,
	DPCM_G,
	DPCM_Gis,
	DPCM_A,
	DPCM_Ais,
	DPCM_B,
	DPCM_Note
};

//	�����́AMML�\���Ŏg����R�}���h�B
const	static	Command_Info	Command[] = {
		{	"c#",	DPCM_Cis	},
		{	"d#",	DPCM_Dis	},
		{	"f#",	DPCM_Fis	},
		{	"g#",	DPCM_Gis	},
		{	"a#",	DPCM_Ais	},

		{	"c+",	DPCM_Cis	},
		{	"d+",	DPCM_Dis	},
		{	"f+",	DPCM_Fis	},
		{	"g+",	DPCM_Gis	},
		{	"a+",	DPCM_Ais	},

		{	"d-",	DPCM_Cis	},
		{	"e-",	DPCM_Dis	},
		{	"g-",	DPCM_Fis	},
		{	"a-",	DPCM_Gis	},
		{	"b-",	DPCM_Ais	},

		{	"c",	DPCM_C		},
		{	"d",	DPCM_D		},
		{	"e",	DPCM_E		},
		{	"f",	DPCM_F		},
		{	"g",	DPCM_G		},
		{	"a",	DPCM_A		},
		{	"b",	DPCM_B		},

		{	"��#",		DPCM_Cis	},
		{	"��#",		DPCM_Dis	},
		{	"�ӂ�#",	DPCM_Fis	},
		{	"��#",		DPCM_Fis	},
		{	"��#",		DPCM_Gis	},
		{	"��#",		DPCM_Ais	},

		{	"��-",	DPCM_Cis	},
		{	"��-",	DPCM_Dis	},
		{	"��-",	DPCM_Fis	},
		{	"��-",	DPCM_Gis	},
		{	"��-",	DPCM_Ais	},

		{	"��",	DPCM_C		},
		{	"��",	DPCM_D		},
		{	"��",	DPCM_E		},
		{	"�ӂ�",	DPCM_F		},
		{	"��",	DPCM_F		},
		{	"��",	DPCM_G		},
		{	"��",	DPCM_A		},
		{	"��",	DPCM_B		},

		{	"�h#",		DPCM_Cis	},
		{	"��#",		DPCM_Dis	},
		{	"�t�@#",	DPCM_Fis	},
		{	"�t#",		DPCM_Fis	},
		{	"�\#",		DPCM_Gis	},
		{	"��#",		DPCM_Ais	},

		{	"��-",	DPCM_Cis	},
		{	"�~-",	DPCM_Dis	},
		{	"�\-",	DPCM_Fis	},
		{	"��-",	DPCM_Gis	},
		{	"�V-",	DPCM_Ais	},

		{	"�h",	DPCM_C		},
		{	"��",	DPCM_D		},
		{	"�~",	DPCM_E		},
		{	"�t�@",	DPCM_F		},
		{	"�t",	DPCM_F		},
		{	"�\",	DPCM_G		},
		{	"��",	DPCM_A		},
		{	"�V",	DPCM_B		},

		{	"n",	DPCM_Note	}

};

	unsigned	char	cData;

	//------------------------------
	//�N���X�̏����ݒ�
	max_number	= 0;
	m_id		= 0;

	//------------------------------
	//�R���p�C��

	// { �̌���
	while(MML->cRead() != '{'){
		if(MML->eof()){
			MML->Err(_T("�u���b�N�̊J�n������{��������܂���B"));
		}
	}

	// } ������܂ŁA�L�q�u���b�N�����R���p�C������B
	while((cData = MML->GetChar()) != '}'){
		
		// } ������O�ɁA[EOF]��������G���[
		if( MML->eof() ){
			MML->Err(_T("�u���b�N�̏I�[������`}'������܂���B"));
		}

		//�P�߂�
		MML->Back();


		//�e�R�}���h���̏���
		switch(MML->GetCommandID(Command, sizeof(Command)/sizeof(Command_Info))){

			case(DPCM_C):
				setKey(MML, 0);
				break;

			case(DPCM_Cis):
				setKey(MML, 1);
				break;

			case(DPCM_D):
				setKey(MML, 2);
				break;

			case(DPCM_Dis):
				setKey(MML, 3);
				break;

			case(DPCM_E):
				setKey(MML, 4);
				break;

			case(DPCM_F):
				setKey(MML, 5);
				break;

			case(DPCM_Fis):
				setKey(MML, 6);
				break;

			case(DPCM_G):
				setKey(MML, 7);
				break;

			case(DPCM_Gis):
				setKey(MML, 8);
				break;

			case(DPCM_A):
				setKey(MML, 9);
				break;

			case(DPCM_Ais):
				setKey(MML, 10);
				break;

			case(DPCM_B):
				setKey(MML, 11);
				break;

			case(DPCM_Note):
				setNote(MML, MML->GetInt());
				break;

			//unknown command
			default:
				MML->Err(_T("unknown command"));
				break;
		}
	}

	//�����Ŋm�ۂ��Ă����B
	if(bank == true){
		iSize = (max_number+1)*6;
	} else {
		iSize = (max_number+1)*4;
	}
	code.resize(iSize);
}

//==============================================================
//		�f�X�g���N�^
//--------------------------------------------------------------
//	������
//				����
//	���Ԓl
//				����
//==============================================================
DPCMinfo::~DPCMinfo(void)
{
	//��PCM�́A�����Ŕj������B

	//----------------------
	//Local�ϐ�
	map<string, DPCM*>::iterator	itItem;

	//----------------------
	//Delete Class
	if(!ptcDPCM.empty()){
		itItem = ptcDPCM.begin();
		while(itItem != ptcDPCM.end()){
			delete itItem->second;
			itItem++;
		}
		ptcDPCM.clear();
	}
}
//==============================================================
//		�ݒ�
//--------------------------------------------------------------
//	������
//		MMLfile*	MML		MML�t�@�C���̃I�u�W�F�N�g
//			int	key		�L�[�ԍ��i0:C / 1:Cis / ...�j
//	���Ԓl
//				����
//==============================================================
void	DPCMinfo::setKey(MMLfile* MML, int key)
{
	setNote(MML, ((MML->GetInt()-1) * 12) + key);
}

//==============================================================
//		�ݒ�
//--------------------------------------------------------------
//	������
//		MMLfile*		MML		MML�t�@�C���̃I�u�W�F�N�g
//				int		note	�m�[�g�ԍ�
//	���Ԓl
//				����
//==============================================================
void	DPCMinfo::setNote(MMLfile* MML, int note)
{
	unsigned	char	cData;
				int		play_frequency;
				int		mode			= 0;
				int		start_volume	= 0x40;
				int		next;
				int		start_offset;
				int		size_offset;
	DPCM*		_DPCM;

	if((note<0) || (note>255)){
		MML->Err(_T("���K�͈̔͂𒴂��Ă��܂��B"));
	}

	if(max_number < note){
		max_number = (unsigned char)note;
	}

	//�t�@�C����
	cData = MML->GetChar();
	if(cData != ','){
		MML->Err(_T("��PCM��`�̃p�����[�^������܂���B"));
	}

	infoDPCM[note].file = MML->GetString();
	if(ptcDPCM.count(infoDPCM[note].file) == 0){
		//�V�����t�@�C����������ADPCM�I�u�W�F�N�g�𐶐�����B
		_DPCM = new DPCM(MML, infoDPCM[note].file.c_str(), m_id);
		ptcDPCM[infoDPCM[note].file] = _DPCM;
		m_id++;
	} else {
		_DPCM = ptcDPCM[infoDPCM[note].file];
	}

	//�Đ����g��
	cData = MML->GetChar();
	if(cData != ','){
		MML->Err(_T("��PCM��`�̃p�����[�^������܂���B"));
	}
	play_frequency = MML->GetInt();
	if((play_frequency<0) || (play_frequency>15)){
		MML->Err(_T("��PCM�̎��g����0�`15�͈̔͂Ŏw�肵�ĉ������B"));
	}

	//���[�h
	cData = MML->GetChar();
	if(cData != ','){
		MML->Err(_T("��PCM��`�̃p�����[�^������܂���B"));
	}

	mode = MML->GetInt();
	if((mode<0) || (mode>2)){
		MML->Err(_T("��PCM�̃��[�h��0�`2�͈̔͂Ŏw�肵�ĉ������B"));
	}
	if((mode==2) && (bank==false)){
		MML->Err(_T("��PCM�̃��[�h2(IRQ)�́A#Bank�R�}���h�̎w�肪�K�v�ł��B"));
	}
	infoDPCM[note].ctrl = (unsigned char)(mode<<6) + (unsigned char)play_frequency;

	//�����l
	cData = MML->GetChar();
	if(cData == ','){
		start_volume = MML->GetInt();	
		if((start_volume<-1) || (start_volume>127)){
			MML->Err(_T("��PCM�̏����l��-1�`127�͈̔͂Ŏw�肵�ĉ������B"));
		}
		infoDPCM[note].DA = (unsigned char)start_volume;
	} else {
		MML->Back();
		infoDPCM[note].DA = 0;
	}

	//���̃m�[�g
	if(mode == 2){
		cData = MML->GetChar();
		if(cData == ','){
			next = MML->GetInt();	
			if((next<-1) || (next>255)){
				MML->Err(_T("���̃m�[�g�ԍ���0�`255�͈̔͂Ŏw�肵�ĉ������B"));
			}
			infoDPCM[note].next = (unsigned char)next;
		} else {
			if(mode == 2){
				MML->Err(_T("���[�h2(IRQ)�̎��͕K�����ɔ�������m�[�g�ԍ����w�肵�Ă��������B"));
			}
			MML->Back();
			infoDPCM[note].next = 0;
		}
	} else {
		infoDPCM[note].next = 0;
	}
/*
	cData = MML->GetChar();
	if(cData == ','){
		if(mode != 2){
			MML->Err(_T("���[�h2(IRQ)�ȊO�̃��[�h�ł͕s�v�ł��B"));
		}
		next = MML->GetInt();	
		if((next<-1) || (next>255)){
			MML->Err(_T("���̃m�[�g�ԍ���0�`255�͈̔͂Ŏw�肵�ĉ������B"));
		}
		infoDPCM[note].next = (unsigned char)next;
	} else {
		if(mode == 2){
			MML->Err(_T("���[�h2(IRQ)�̎��͕K�����ɔ�������m�[�g�ԍ����w�肵�Ă��������B"));
		}
		MML->Back();
		infoDPCM[note].next = 0;
	}
*/

	//offset
	cData = MML->GetChar();
	if(cData == ','){
		start_offset = MML->GetInt();	
		if((start_offset<0) || (start_offset>255)){
			MML->Err(_T("��PCM�̔����J�n�I�t�Z�b�g��0�`255�͈̔͂Ŏw�肵�ĉ������B"));
		}
		infoDPCM[note].offset = (unsigned char)start_offset;
	} else {
		MML->Back();
		infoDPCM[note].offset = 0;
	}

	//size
	cData = MML->GetChar();
	if(cData == ','){
		size_offset = MML->GetInt();	
		if((size_offset<0) || (size_offset>255)){
			MML->Err(_T("��PCM�̔����T�C�Y��0�`255�͈̔͂Ŏw�肵�ĉ������B"));
		}
		infoDPCM[note].size = (unsigned char)size_offset;
	} else {
		MML->Back();
		infoDPCM[note].size = 0;
	}

}

//==============================================================
//		��PCMinfo�\���̂̐����i�z�u�A�h���X�̉����j
//--------------------------------------------------------------
//	������
//		unsigned	int		_offset		��PCM�z�u�A�h���X
//		unsigned	char	_MusBank	�V�[�P���X�̃o���N��
//	���Ԓl
//		unsigned	int					��PCM�z�u�I���A�h���X
//	������
//		�R�[�h���擾���Ȃ���A�e��PCM�̃o�C�i���𓾂�B
//		�i�����R�[�h�́ANSF�o�͂̎������g��Ȃ����B�B�B�j
//==============================================================
unsigned	int	DPCMinfo::setDPCMoffset(unsigned	int _offset, unsigned char _MusBank)
{
	map<string, DPCM*>::iterator	itDPCM;
	DPCM*	_DPCM;
	size_t	_size;

	unsigned	int	i=0;

	//��PCM�̔z�u�A�h���X�������B���Ȃ���NSF�o�͗p�̇�PCM���̂��쐬�B
	if(m_id > 0){
		//DPCM
		if(!ptcDPCM.empty()){
			itDPCM = ptcDPCM.begin();
			while(itDPCM != ptcDPCM.end()){
				_DPCM = itDPCM->second;
				_DPCM->SetOffset(_offset);
				_size	= (_DPCM->getSize() & 0xFFC0) + 0x0040;
				_offset	+= (unsigned int)_size;
				itDPCM++;
			}
		}
	}

	//��PCMinfo�\���̂̐���
	if(bank == false){
		while(i <= max_number){
			if(infoDPCM[i].file.empty()){
				code[i*4 + 0] = 0;
				code[i*4 + 1] = 0;
				code[i*4 + 2] = 0;
				code[i*4 + 3] = 0;
			} else {
				_DPCM = ptcDPCM[infoDPCM[i].file];
				code[i*4 + 0] = infoDPCM[i].ctrl;
				code[i*4 + 1] = infoDPCM[i].DA;
				code[i*4 + 2] = (unsigned char)((_DPCM->getOffset() - 0xC000) >> 6) + infoDPCM[i].offset;
				if(infoDPCM[i].size == 0){
					if(_DPCM->getDPCMsize() >= (infoDPCM[i].offset*4)){
                        code[i*4 + 3] = _DPCM->getDPCMsize() - (infoDPCM[i].offset*4);
					} else {
						code[i*4 + 3] = 0;
					}
				} else {
					code[i*4 + 3] = infoDPCM[i].size;
				}
			}
			i++;
		}
	} else {
		while(i <= max_number){
			if(infoDPCM[i].file.empty()){
				code[i*6 + 0] = 0;
				code[i*6 + 1] = 0;
				code[i*6 + 2] = 0;
				code[i*6 + 3] = 0;
				code[i*6 + 4] = 0;
				code[i*6 + 5] = 0;
			} else {
				_DPCM = ptcDPCM[infoDPCM[i].file];
				code[i*6 + 0] = infoDPCM[i].ctrl;
				code[i*6 + 1] = infoDPCM[i].DA;
				code[i*6 + 2] = (unsigned char)((_DPCM->getOffset() & 0x0FFF) >> 6) + infoDPCM[i].offset;
				if(infoDPCM[i].size == 0){
					if(_DPCM->getDPCMsize() >= (infoDPCM[i].offset*4)){
                        code[i*6 + 3] = _DPCM->getDPCMsize() - (infoDPCM[i].offset*4);
					} else {
						code[i*6 + 3] = 0;
					}
				} else {
					code[i*6 + 3] = infoDPCM[i].size;
				}

				code[i*6 + 4] = (unsigned char)((_DPCM->getOffset() - 0xC000) / 0x1000) + _MusBank;
				code[i*6 + 5] = infoDPCM[i].next;
			}
			i++;
		}
	}


	return(_offset);
}

//==============================================================
//		�R�[�h�̎擾
//--------------------------------------------------------------
//	������
//		string*		_str		�R�[�h������string�̃|�C���^
//	���Ԓl
//		����
//	������
//		�R�[�h���擾���Ȃ���A�e��PCM�̃o�C�i���𓾂�B
//		�i�����R�[�h�́ANSF�o�͂̎������g��Ȃ����B�B�B�j
//==============================================================
void	DPCMinfo::getDPCMCode(string* _str)
{
	map<string, DPCM*>::iterator	itDPCM;
	DPCM*	_DPCM;
	size_t	_size;
	size_t	psize;

	if(m_id > 0){
		//DPCM
		if(!ptcDPCM.empty()){
			itDPCM = ptcDPCM.begin();
			while(itDPCM != ptcDPCM.end()){
				_DPCM = itDPCM->second;
				_DPCM->getCode(_str);
				_size	= _DPCM->getSize();
				psize	= (_size & 0xFFC0) + 0x0040;
				_str->append(psize - _size,(char)0x00);
				itDPCM++;
			}
		}
	}
}
//==============================================================
//		�R�[�h�̎擾
//--------------------------------------------------------------
//	������
//		MusicFile*	MUS		�R�[�h���o�͂���ȃf�[�^�t�@�C���E�I�u�W�F�N�g
//	���Ԓl
//				����
//==============================================================
void	DPCMinfo::getAsm(MusicFile* MUS)
{
	map<string, DPCM*>::iterator	itDPCM;

	if(m_id > 0){
		//DPCMinfo
		*MUS << MUS->Header.Label.c_str() << "DPCMinfo" << ":" << endl;
		MusicItem::getAsm(MUS);

		//DPCM
		if((!ptcDPCM.empty()) && (MUS->Header.segmentPCM != "")) {
			*MUS <<	"\n\n.segment	"	<<	'"'	<<	MUS->Header.segmentPCM	<<	'"' << endl;
			itDPCM = ptcDPCM.begin();
			while(itDPCM != ptcDPCM.end()){
				itDPCM->second->getAsm(MUS);
				itDPCM++;
			}
			*MUS <<	"\n\n.segment	"	<<	'"'	<<	MUS->Header.segmentSEQ	<<	'"' << endl;
		}
	}
}