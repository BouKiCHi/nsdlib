/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#include "StdAfx.h"
#include "MusicFile.h"

/****************************************************************/
/*					�O���[�o���ϐ��i�N���X�����ǁE�E�E�j		*/
/****************************************************************/
extern	OPSW*			cOptionSW;	//�I�v�V�������ւ̃|�C���^�ϐ�

//==============================================================
//		�R���X�g���N�^
//--------------------------------------------------------------
//	������
//		MMLfile*			MML			�l�l�k�t�@�C���̃I�u�W�F�N�g
//		string				_code		�����N����R�[�h�i*.bin�j�̃t�@�C����
//		const		_CHAR	_strName[]	�I�u�W�F�N�g��
//	���Ԓl
//				����
//==============================================================
MusicFile::MusicFile(MMLfile* MML, string _code, const _CHAR _strName[]):
	MusicItem(_strName),
	cDPCMinfo(NULL),
	Header(_code)
{
	//----------------------
	//Local�ϐ�


//	�萔��`
enum	Command_ID_MusicFile {
	//for NSF output
	id_Title,
	id_Composer,
	id_Copyright,
	id_OffsetPCM,
	id_Code,
	id_External,
	id_Bank,

	//for ASM output
	id_SegmentSEQ,
	id_SegmentPCM,
	id_Label,
	id_Priority,

	//General
	id_include,
	id_timebase,
	id_OctaveReverse,
	id_QReverse,
	id_bgm_num,
	id_se_num,
	id_releaseVolume,
	id_repeatMode,
	id_TieMode,
	id_offset_Ei,
	id_offset_Ev,
	id_offset_En,
	id_offset_Em,
	id_QMax,
	id_rest,
	id_wait,

	//Block
	id_DPCM,
	id_FDSC,
	id_FDSM,
	id_VRC7,
	id_N163,
	id_Envelop,
	id_Vibrato,
	id_Macro,
	id_Sub,
	id_BGM,
	id_SE,
	id_Patch,

	id_Null
};

//	�����́AMML�\���Ŏg����R�}���h�B
const	static	Command_Info	Command[] = {
		//for NSF output
		{	"#Title",			id_Title		},
		{	"#title",			id_Title		},
		{	"�Ȗ�",				id_Title		},
		{	"#Composer",		id_Composer		},
		{	"#composer",		id_Composer		},
		{	"��Ȏ�",			id_Composer		},
		{	"���",				id_Composer		},
		{	"#Copyright",		id_Copyright	},
		{	"#copyright",		id_Copyright	},
		{	"���",				id_Copyright	},
		{	"���쌠��",			id_Copyright	},
		{	"#OffsetPCM",		id_OffsetPCM	},	//Offset Address of ��PCM
		{	"#offsetPCM",		id_OffsetPCM	},	//Offset Address of ��PCM
		{	"#offsetPCM",		id_OffsetPCM	},	//Offset Address of ��PCM
		{	"#Code",			id_Code			},
		{	"#code",			id_Code			},
		{	"�R�[�h",			id_Code			},
		{	"#External",		id_External		},
		{	"#external",		id_External		},
		{	"#Bank",			id_Bank			},
		{	"#bank",			id_Bank			},
		{	"�o���N",			id_Bank			},
		//for ASM output
		{	"#SegmentPCM",		id_SegmentPCM	},	//Segment name for ��PCM
		{	"#segmentPCM",		id_SegmentPCM	},
		{	"#SegmentSEQ",		id_SegmentSEQ	},	//Segment name for Sequence
		{	"#segmentSEQ",		id_SegmentSEQ	},
		{	"#Segment",			id_SegmentSEQ	},	//Segment name for Sequence
		{	"#segment",			id_SegmentSEQ	},
		{	"#Label",			id_Label		},
		{	"#label",			id_Label		},

		//General
		{	"#Include",			id_include		},
		{	"#include",			id_include		},
		{	"�ǂ�",				id_include		},
		{	"#Timebase",		id_timebase		},
		{	"#timebase",		id_timebase		},
		{	"#OctaveReverse",	id_OctaveReverse},
		{	"#octaveReverse",	id_OctaveReverse},
		{	"#QReverse",		id_QReverse		},
		{	"#qReverse",		id_QReverse		},
		{	"#BGM",				id_bgm_num		},
		{	"#bgm",				id_bgm_num		},
		{	"���y��",			id_bgm_num		},
		{	"#SE",				id_se_num		},
		{	"#se",				id_se_num		},
		{	"���ʉ���",			id_se_num		},
		{	"#releaseVolume",	id_releaseVolume},
		{	"#ReleaseVolume",	id_releaseVolume},
		{	"#repeatMode",		id_repeatMode	},
		{	"#RepeatMode",		id_repeatMode	},
		{	"#TieMode",			id_TieMode		},
		{	"#tieMode",			id_TieMode		},	
		{	"#OffsetE@",		id_offset_Ei	},
		{	"#offsetE@",		id_offset_Ei	},
		{	"#OffsetEv",		id_offset_Ev	},
		{	"#offsetEv",		id_offset_Ev	},
		{	"#OffsetEm",		id_offset_Em	},
		{	"#offsetEm",		id_offset_Em	},
		{	"#OffsetEn",		id_offset_En	},
		{	"#offsetEn",		id_offset_En	},
		{	"#Priority",		id_Priority		},
		{	"#priority",		id_Priority		},
		{	"#QMax",			id_QMax			},
		{	"#Qmax",			id_QMax			},
		{	"#Rest",			id_rest			},
		{	"#rest",			id_rest			},
		{	"#Wait",			id_wait			},
		{	"#wait",			id_wait			},
		//Block
		{	"DPCM",				id_DPCM			},
		{	"FDSC",				id_FDSC			},
		{	"FDSM",				id_FDSM			},
		{	"VRC7",				id_VRC7			},
		{	"N163",				id_N163			},
		{	"Envelope",			id_Envelop		},
		{	"envelope",			id_Envelop		},
		{	"Envelop",			id_Envelop		},
		{	"envelop",			id_Envelop		},
		{	"�G���x���[�v",		id_Envelop		},
		{	"�r�u���[�g",		id_Vibrato		},
		{	"Sub",				id_Sub			},
		{	"sub",				id_Sub			},
		{	"BGM",				id_BGM			},
		{	"bgm",				id_BGM			},
		{	"���y",				id_BGM			},
		{	"SE",				id_SE			},
		{	"se",				id_SE			},
		{	"���ʉ�",			id_SE			},
		{	"Patch",			id_Patch		},

		//for 1 command
		{	"D",				id_DPCM			},
		{	"FC",				id_FDSC			},
		{	"FM",				id_FDSM			},
		{	"O",				id_VRC7			},
		{	"V",				id_VRC7			},
		{	"N",				id_N163			},
		{	"E",				id_Envelop		},
		{	"S",				id_Sub			},
		{	"P",				id_Patch		},
		{	"$",				id_Macro		},

		//for mck/ppmck
		{	"@DPCM",			id_DPCM			},
		{	"@FM",				id_FDSC			},
		{	"@MW",				id_FDSM			},
		{	"@OP",				id_VRC7			},
		{	"@N",				id_N163			},
		{	"@E",				id_Envelop		},

		{	"�@",				id_Null			},
	};

	unsigned	int			i;
	unsigned	char		cData;
				FDSC*		_fdsc;
				FDSM*		_fdsm;
				VRC7*		_vrc7;
				N163*		_n163;
				Envelop*	_env;
				BGM*		_bgm;
				SE*			_se;
				Sub*		_sub;
	string		msg;

	map<	int,	Envelop*	>::iterator	itEnvelop;
	map<	int,	BGM*		>::iterator	itBGM;
	map<	int,	SE*			>::iterator	itSE;
	map<	int,	Sub*		>::iterator	itSub;

	iSize = 0;

	do{
		
		//�P�����ǂݍ��݁i�R�����g�`�F�b�N�Ainclude�t�@�C���̏I�[�`�F�b�N������j
		cData = MML->GetChar();

		//[EOF]�`�F�b�N
		if( MML->eom() ){
			break;
		}

		//�P�߂�
		MML->Back();

		//�R�}���h������̃`�F�b�N
		switch(MML->GetCommandID(Command, sizeof(Command)/sizeof(Command_Info))){
			//for NSF output
			case(id_Code):
				Header.Set_RomCode(MML);
				break;
			case(id_Title):
				Header.Set_Title(MML);
				break;
			case(id_Composer):
				Header.Set_Composer(MML);
				break;
			case(id_Copyright):
				Header.Set_Copyright(MML);
				break;
			case(id_OffsetPCM):
				Header.Set_OffsetPCM(MML);
				break;
			case(id_External):
				Header.Set_External(MML);
				break;
			case(id_Bank):
				Header.Set_Bank();
				break;
			//for ASM output
			case(id_SegmentSEQ):
				Header.Set_SegmentSEQ(MML);
				break;
			case(id_SegmentPCM):
				Header.Set_SegmentPCM(MML);
				break;
			case(id_Label):
				Header.Set_Label(MML);
				break;
			//General
			case(id_include):
				MML->include();
				break;
			case(id_bgm_num):
				Header.Set_Number_BGM(MML);
				break;
			case(id_se_num):
				Header.Set_Number_SE(MML);
				break;
			case(id_timebase):
				MML->timebase = MML->GetInt();	//����́AMML�t�@�C���̑����B
				break;
			case(id_OctaveReverse):
				MML->octave_reverse = true;		//����́AMML�t�@�C���̑����B
				break;
			case(id_QReverse):
				MML->q_reverse = true;			//����́AMML�t�@�C���̑����B
				break;
			case(id_releaseVolume):
				MML->iReleaseVolume	=  MML->GetInt();
				if((MML->iReleaseVolume<0) || (MML->iReleaseVolume>15)){
					MML->Err(_T("#ReleaseVolume�R�}���h�́A0�`15�͈̔͂Ŏw�肵�Ă��������B"));
				}
				break;
			case(id_repeatMode):
				MML->iRepeatMode	=  MML->GetInt();
				if((MML->iRepeatMode<0) || (MML->iRepeatMode>2)){
					MML->Err(_T("#RepeatMode�R�}���h�́A0�`2�͈̔͂Ŏw�肵�Ă��������B"));
				}
				break;
			case(id_TieMode):
				MML->iTieMode	=  MML->GetInt();
				if((MML->iTieMode<0) || (MML->iTieMode>1)){
					MML->Err(_T("#TieMode�R�}���h�́A0�`1�͈̔͂Ŏw�肵�Ă��������B"));
				}
				break;
			case(id_offset_Ei):
				MML->offset_Ei = MML->GetInt();
				break;
			case(id_offset_Ev):
				MML->offset_Ev = MML->GetInt();
				break;
			case(id_offset_En):
				MML->offset_En = MML->GetInt();
				break;
			case(id_offset_Em):
				MML->offset_Em = MML->GetInt();
				break;
			case(id_Priority):
				i = MML->GetInt();
				if((i<0) || (i>3)){
					MML->Err(_T("#priority�R�}���h�́A0�`3�͈̔͂Ŏw�肵�Ă��������B"));
				} else {
					MML->priority = (unsigned char)i;
				}
				break;
			case(id_QMax):
				MML->QMax = MML->GetInt();
				break;
			case(id_rest):
				MML->rest = MML->GetInt();
				if((MML->rest<0) || (MML->rest>3)){
					MML->Err(_T("#Rest�R�}���h�́A0�`3�͈̔͂Ŏw�肵�Ă��������B"));
				}
				break;
			case(id_wait):
				MML->wait = MML->GetInt();
				if((MML->rest<0) || (MML->rest>3)){
					MML->Err(_T("#Wait�R�}���h�́A0�`3�͈̔͂Ŏw�肵�Ă��������B"));
				}
				break;
			case(id_Macro):
				MML->SetMacro(0);
				break;
			case(id_Patch):
				MML->SetPatch();
				break;
			//MML
			case(id_DPCM):
				if(cDPCMinfo != NULL){
					MML->Err(_T("DPCM�u���b�N�͂P�܂łł��B"));
				}
				cDPCMinfo = new DPCMinfo(MML, Header.bank);
				ptcItem.push_back(cDPCMinfo);
				iSize += cDPCMinfo->getSize();	//BGM�̃T�C�Y���X�V
				break;
			case(id_FDSC):
				i = MML->GetNum();

				//�d���`�F�b�N
				if(ptcFDSC.count(i) != 0){
					MML->Err(_T("FDSC()�u���b�N�œ����ԍ����w�肳��܂����B"));
				}
				_fdsc = new FDSC(MML, i);
				ptcItem.push_back(_fdsc);
				ptcFDSC[i] = _fdsc;
				iSize += _fdsc->getSize();	//BGM�̃T�C�Y���X�V
				break;
			case(id_FDSM):
				i = MML->GetNum();

				//�d���`�F�b�N
				if(ptcFDSM.count(i) != 0){
					MML->Err(_T("FDSM()�u���b�N�œ����ԍ����w�肳��܂����B"));
				}
				_fdsm = new FDSM(MML, i);
				ptcItem.push_back(_fdsm);
				ptcFDSM[i] = _fdsm;
				iSize += _fdsm->getSize();	//BGM�̃T�C�Y���X�V
				break;
			case(id_VRC7):
				i = MML->GetNum();

				//�d���`�F�b�N
				if(ptcVRC7.count(i) != 0){
					MML->Err(_T("VRC7()�u���b�N�œ����ԍ����w�肳��܂����B"));
				}
				_vrc7 = new VRC7(MML, i);
				ptcItem.push_back(_vrc7);
				ptcVRC7[i] = _vrc7;
				iSize += _vrc7->getSize();	//BGM�̃T�C�Y���X�V
				break;
			case(id_N163):
				i = MML->GetNum();

				//�d���`�F�b�N
				if(ptcN163.count(i) != 0){
					MML->Err(_T("N163()�u���b�N�œ����ԍ����w�肳��܂����B"));
				}
				_n163 = new N163(MML, i);
				ptcItem.push_back(_n163);
				ptcN163[i] = _n163;
				iSize += _n163->getSize();	//BGM�̃T�C�Y���X�V
				break;
			case(id_Envelop):
				i = MML->GetNum();
				//�d���`�F�b�N
				if(ptcEnv.count(i) != 0){
					MML->Err(_T("Envelope()�u���b�N�œ����ԍ����w�肳��܂����B"));
				}
				_env = new Envelop(MML, i);
				ptcItem.push_back(_env);
				ptcEnv[i] = _env;
				iSize += _env->getSize();	//BGM�̃T�C�Y���X�V
				break;
			case(id_Vibrato):
				MML->offset_Em = 1000000;
				i = MML->GetNum() + MML->offset_Em;
				//�d���`�F�b�N
				if(ptcEnv.count(i) != 0){
					MML->Err(_T("�r�u���[�g()�u���b�N�œ����ԍ����w�肳��܂����B"));
				}
				_env = new Envelop(MML, i);
				ptcItem.push_back(_env);
				ptcEnv[i] = _env;
				iSize += _env->getSize();	//BGM�̃T�C�Y���X�V
				break;
			case(id_Sub):
				i = MML->GetNum();
				//�d���`�F�b�N
				if(ptcSub.count(i) != 0){
					MML->Err(_T("Sub()�u���b�N�œ����ԍ����w�肳��܂����B"));
				}
				//�͈̓`�F�b�N
				_sub = new Sub(MML, i);
				ptcItem.push_back(_sub);
				ptcSub[i] = _sub;
				iSize += _sub->getSize();	//BGM�̃T�C�Y���X�V
				break;
			case(id_BGM):
				i = MML->GetNum();
				//�d���`�F�b�N
				if(ptcBGM.count(i) != 0){
					MML->Err(_T("BGM()�u���b�N�œ����ԍ����w�肳��܂����B"));
				}
				//�͈̓`�F�b�N
				if((Header.iBGM <= i) || (i<0)){
					MML->Err(_T("BGM()�u���b�N�Ŏw��ł���͈͂𒴂��Ă��܂��B\n#BGM�̐��l���m�F���Ă��������B"));
				}
				_bgm = new BGM(MML, i);
				ptcItem.push_back(_bgm);
				ptcBGM[i] = _bgm;
				iSize += _bgm->getSize();	//BGM�̃T�C�Y���X�V
				break;
			case(id_SE):
				i = MML->GetNum();
				//�d���`�F�b�N
				if(ptcSE.count(i) != 0){
					MML->Err(_T("SE()�u���b�N�œ����ԍ����w�肳��܂����B"));
				}
				//�͈̓`�F�b�N
				if((Header.iSE <= i) || (i<0)){
					MML->Err(_T("SE()�u���b�N�Ŏw��ł���͈͂𒴂��Ă��܂��B\n#SE�̐��l���m�F���Ă��������B"));
				}
				_se = new SE(MML, i);
				ptcItem.push_back(_se);
				ptcSE[i] = _se;
				iSize += _se->getSize();	//BGM�̃T�C�Y���X�V
				break;
			case(id_Null):
				break;
			default:
				MML->Err(_T("unknown command"));
				break;
		}
		
	} while( !MML->eom() );

	//Check
	if( Header.iBGM + Header.iSE > 255){
		Err(_T("BGM��SE�̐������v��255���z���܂����B"));
	}

	i = 0;
	while(i < Header.iBGM){
		if(ptcBGM.count(i) == 0){
			Err(_T("BGM�f�[�^������܂���B"));
		};
		i++;
	}

	i = 0;
	while(i < Header.iSE){
		if(ptcSE.count(i) == 0){
			Err(_T("SE �f�[�^������܂���B"));
		};
		i++;
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
MusicFile::~MusicFile(void)
{
}

//==============================================================
//		�g�p���Ȃ��I�u�W�F�N�g�̌������폜
//		���Ȃ���ATick���J�E���g
//--------------------------------------------------------------
//	������
//				����
//	���Ԓl
//				����
//==============================================================
void	MusicFile::TickCount(void)
{
	map<	int, FDSC*		>::iterator	itFDSC;		//FDS  wave table (career)
	map<	int, FDSM*		>::iterator	itFDSM;		//FDS  wave table (modulator)
	map<	int, VRC7*		>::iterator	itVRC7;		//VRC7 User Instrument
	map<	int, N163*		>::iterator	itN163;		//N163 wave table
	map<	int, Envelop*	>::iterator	itEnv;		//Envelop
	map<	int, Sub*		>::iterator	itSub;		//Subroutine

	unsigned	int			iBGM	= 0;
	unsigned	int			iSE		= 0;

	//----------------------
	//Tick Count & �œK���̂��߂̏����W

	while(iBGM < Header.iBGM){
		cout << "---- BGM(" << iBGM << ") ----" <<endl;
		ptcBGM[iBGM]->TickCount(this);				//�J�E���e�B���O���Ȃ���A�s�v�ȃR�}���h���������`�F�b�N
		ptcBGM[iBGM]->OptimizeDefineCheck(this);	//�g�Ă����`���`�F�b�N
		iBGM++;
	}

	while(iSE < Header.iSE){
		cout << "---- SE(" << iSE << ") ----" <<endl;
		ptcSE[iSE]->TickCount(this);				//�J�E���e�B���O���Ȃ���A�s�v�ȃR�}���h���������`�F�b�N
		ptcSE[iSE]->OptimizeDefineCheck(this);		//�g�Ă����`���`�F�b�N
		iSE++;
	}

	//----------------------
	//�s�v�ȃR�}���h�̍폜

	if(cOptionSW->flag_OptSeq == true){		//�R�}���h�̍œK����������������A�œK�����Ȃ��B

		//	to do	������������	�s�v�ȃR�}���h�E�I�u�W�F�N�g���폜����	������������

	}

	//----------------------
	//�g���Ă��Ȃ���`�̍폜

	if(cOptionSW->flag_OptObj == true){		//��`�̍œK����������������A�œK�����Ȃ��B

		//�T�u���[�`��
		if(!ptcSub.empty()){
			itSub = ptcSub.begin();
			while(itSub != ptcSub.end()){
				if(itSub->second->chkUse() == true){
					//�g���T�u���[�`���ł���΁A���̒��Ŏg���Ă��`���`�F�b�N
					itSub->second->OptimizeDefineCheck(this);
				} else {
					//�g��Ȃ��T�u���[�`���ł���΁A�I�u�W�F�N�g�폜�B
					itSub->second->clear(itSub->first);
				}
				itSub++;
			}
		}

		//�G���x���[�v
		if(!ptcEnv.empty()){
			itEnv = ptcEnv.begin();
			while(itEnv != ptcEnv.end()){
				if(itEnv->second->chkUse() == false){
					//�g��Ȃ��T�u���[�`���ł���΁A�I�u�W�F�N�g�폜�B
					itEnv->second->clear(itEnv->first);
				}
				itEnv++;
			}
		}

		//FDSC
		if(!ptcFDSC.empty()){
			itFDSC = ptcFDSC.begin();
			while(itFDSC != ptcFDSC.end()){
				if(itFDSC->second->chkUse() == false){
					//�g��Ȃ��T�u���[�`���ł���΁A�I�u�W�F�N�g�폜�B
					itFDSC->second->clear(itFDSC->first);
				}
				itFDSC++;
			}
		}

		//FDSM
		if(!ptcFDSM.empty()){
			itFDSM = ptcFDSM.begin();
			while(itFDSM != ptcFDSM.end()){
				if(itFDSM->second->chkUse() == false){
					//�g��Ȃ��T�u���[�`���ł���΁A�I�u�W�F�N�g�폜�B
					itFDSM->second->clear(itFDSM->first);
				}
				itFDSM++;
			}
		}

		//VRC7
		if(!ptcVRC7.empty()){
			itVRC7 = ptcVRC7.begin();
			while(itVRC7 != ptcVRC7.end()){
				if(itVRC7->second->chkUse() == false){
					//�g��Ȃ��T�u���[�`���ł���΁A�I�u�W�F�N�g�폜�B
					itVRC7->second->clear(itVRC7->first);
				}
				itVRC7++;
			}
		}

		//N163
		if(!ptcN163.empty()){
			itN163 = ptcN163.begin();
			while(itN163 != ptcN163.end()){
				if(itN163->second->chkUse() == false){
					//�g��Ȃ��T�u���[�`���ł���΁A�I�u�W�F�N�g�폜�B
					itN163->second->clear(itN163->first);
				}
				itN163++;
			}
		}
	}
}

//==============================================================
//		��PCM�̃I�t�Z�b�g�A�h���X���v�Z
//--------------------------------------------------------------
//	������
//		unsigned	int	iMusSize	�V�[�P���X�̃T�C�Y
//	���Ԓl
//		unsigned	int	��PCM�̍��v�T�C�Y
//==============================================================
unsigned	int		MusicFile::SetDPCMOffset(unsigned int iMusSize)
{
	unsigned	int		i;
	unsigned	char	mus_bank = (unsigned char)(iMusSize >> 12);

	if((iMusSize & 0x0FFF) != 0){
		mus_bank++;
	}


	dpcm_code.clear();
	if(cDPCMinfo != NULL){
		cDPCMinfo->getDPCMCode(&dpcm_code);
		i = cDPCMinfo->setDPCMoffset(Header.offsetPCM, mus_bank+3);
	} else {
		i = Header.offsetPCM;
	}
	return(i - Header.offsetPCM);
}

//==============================================================
//		�A�h���X�������肷��B
//--------------------------------------------------------------
//	������
//				����
//	���Ԓl
//				����
//==============================================================
void	MusicFile::Fix_Address(void)
{
	map<int,Sub*	>::iterator	itSub;

	unsigned	int			iBGM	= 0;
	unsigned	int			iSE		= 0;


	while(iBGM < Header.iBGM){
		ptcBGM[iBGM]->Fix_Address(this);
		iBGM++;
	}

	while(iSE < Header.iSE){
		ptcSE[iSE]->Fix_Address(this);
		iSE++;
	}

	if(!ptcSub.empty()){
		itSub = ptcSub.begin();
		while(itSub != ptcSub.end()){
			itSub->second->Fix_Address(this);
			itSub++;
		}
	}
}

/*
//==============================================================
//		�ȃo�C�i���C���[�W�̍쐬
//--------------------------------------------------------------
//	������
//				����
//	���Ԓl
//				����
//==============================================================
void	MusicFile::make_binary(void)
{

}
*/		
//==============================================================
//		�m�r�e�̍쐬
//--------------------------------------------------------------
//	������
//		size_t	rom_size	*.bin�̃T�C�Y
//	���Ԓl
//				����
//==============================================================
void	MusicFile::make_bin(size_t rom_size, int ptOffset)
{
				string		_str;
	unsigned	int			i		= 2;
	unsigned	int			iBGM	= 0;
	unsigned	int			iSE		= 0;
	unsigned	short*		pt;

				size_t		_size	= 4 + (Header.iBGM + Header.iSE)*2;


	//�ȃo�C�i���[�̍쐬
	_str.clear();
	_str.resize(_size);

	pt = (unsigned short*)_str.c_str();

	_str[0] = Header.iBGM;
	_str[1] = Header.iSE;

	if(Header.bank == false){

		if(cDPCMinfo != NULL){
			pt[1]	= (unsigned short)(ptOffset + rom_size - 0x80 + _size + cDPCMinfo->getOffset());	//��PCM info �̃A�h���X
		} else {
			pt[1]	= 0;
		}

		while(iBGM < Header.iBGM){
			pt[i] = (unsigned short)(ptOffset + rom_size - 0x80 + _size + ptcBGM[iBGM]->getOffset());
			i++;
			iBGM++;
		}
		while(iSE < Header.iSE){
			pt[i] = (unsigned short)(ptOffset + rom_size - 0x80 + _size + ptcSE[iSE]->getOffset());
			i++;
			iSE++;
		}

	} else {

		if(cDPCMinfo != NULL){
			pt[1] = (unsigned short)(ptOffset + _size + cDPCMinfo->getOffset());	//��PCM info �̃A�h���X
		} else {
			pt[1]	= 0;
		}

		while(iBGM < Header.iBGM){
			pt[i] = (unsigned short)(ptOffset + _size + ptcBGM[iBGM]->getOffset());
			i++;
			iBGM++;
		}
		while(iSE < Header.iSE){
			pt[i] = (unsigned short)(ptOffset + _size + ptcSE[iSE]->getOffset());
			i++;
			iSE++;
		}

	}

	getCode(&_str);
	code = _str;

	//��PCM�̍쐬
	dpcm_code.clear();
	if(cDPCMinfo != NULL){
		cDPCMinfo->getDPCMCode(&dpcm_code);
	}

}

//==============================================================
//		�m�r�e�`���ւ̕ۑ�
//--------------------------------------------------------------
//	������
//		const	char*	strFileName		�t�@�C����
//		bool			opt				�œK���t���O
//	���Ԓl
//				����
//==============================================================
void	MusicFile::saveNSF(const char*	strFileName)
{

	unsigned	int		i,j;
				size_t	bin_size;
				size_t	mus_size;
				size_t	pcm_size;
	unsigned	char	mus_bank;
	unsigned	char	pcm_bank;
				char*	romimg			= new char[0xC000+0x80];
	NSF_Header*			nsf				= (NSF_Header*)romimg;
	FileInput*			_romcode		= new FileInput();
				bool	dpcm_bank		= false;
				bool	flag_Optimize	= cOptionSW->flag_Optimize;

	//NSF�p�R�[�h�̓]��
	_romcode->fileopen(Header.romcode.c_str(), &(cOptionSW->m_pass_code));
	bin_size = _romcode->GetSize();
	_romcode->read(romimg, bin_size);
	_romcode->close();
	delete		_romcode;

	//NSF�w�b�_�[�̍X�V
	memcpy(&nsf->Title, Header.title.c_str(), 32);
	memcpy(&nsf->Composer, Header.composer.c_str(), 32);
	memcpy(&nsf->Copyright, Header.copyright.c_str(), 32);
	nsf->MusicNumber	= Header.iBGM + Header.iSE;
	if(Header.iExternal != -1){
		nsf->External	= (unsigned char)Header.iExternal;
	}

	_COUT << _T("----------------------------------------") << endl;
	_COUT << _T("*NSF build process") << endl;


	if((nsf->Bank[0] == 0) && (nsf->Bank[1] == 0) && (nsf->Bank[2] == 0) && (nsf->Bank[3] == 0)){

		//------------------------------
		//Bank ��Ή�bin

		//�V�[�P���X�̃o�C�i���𐶐�
		make_bin(bin_size, 0x8000);

		if(Header.bank == true){
			Err(_T("�w���.bin�t�@�C���́A��PCM�̃o���N�ɑΉ����Ă��܂���B\n��PCM�̃o���N�ɑΉ�����.bin�t�@�C�����w�肵�Ă��������B"));
		}

		mus_size = bin_size - 0x80 + code.size();
		mus_bank = (unsigned char)(mus_size >> 12);
		if((mus_size & 0x0FFF) != 0){
			mus_bank++;
		}

		//�T�C�Y�`�F�b�N
		_COUT << _T("[CODE & MUSIC]") << endl;
		_COUT << _T("  Bank = ") << (unsigned int)mus_bank << endl;
		_COUT << _T("  Size = ") << (unsigned int)mus_size << _T(" [Byte] / ") << Header.offsetPCM - 0x8000 << _T(" [Byte]") << endl;

		if((0x8000 + mus_size) > Header.offsetPCM){
			Err(_T("�R�[�h�E�V�[�P���X�̃T�C�Y�����e�l���z���܂����B"));
		}

	} else {

		//------------------------------
		//Bank�Ή�bin�H

		unsigned	int	iSizeLimit = 0x10000;	//�g��RAM�ւ̓]���L��

		//�V�[�P���X�̃o�C�i���𐶐�
		make_bin(bin_size, 0x0000);

		if(Header.bank == false){
			Err(_T("�w���.bin�t�@�C���́A��PCM�̃o���N�ɑΉ����Ă��܂��B\n#Bank�R�}���h���w�肵�Ă��������B"));
		}

		dpcm_bank = true;
		mus_size = code.size();
		mus_bank = (unsigned char)(mus_size >> 12);
		if((mus_size & 0x0FFF) != 0){
			mus_bank++;
		}

		_COUT << _T("[CODE]") << endl;
		_COUT << _T("  Bank = 3") << endl;
		_COUT << _T("  Size = 12288 [Byte]") << endl;

		_COUT << _T("[MUSIC]") << endl;
		_COUT << _T("  Bank = ") << (unsigned int)mus_bank << endl;
		_COUT << _T("  Size = ") << (unsigned int)mus_size << _T(" [Byte] / ") << iSizeLimit << _T(" [Byte]") << endl;

		//�T�C�Y�`�F�b�N
		if(mus_size > iSizeLimit){
			Err(_T("�R�[�h�E�V�[�P���X�̃T�C�Y�����e�l���z���܂����B"));
		}

	}

	//��PCM
	_COUT << _T("[DPCM]") << endl;

	pcm_size = dpcm_code.size();
	pcm_bank = (unsigned char)((pcm_size + (Header.offsetPCM & 0x0FFF)) >> 12);
	if((pcm_size & 0x0FFF) != 0){
		pcm_bank++;
	}


	if(dpcm_bank == false){
		//��PCM�T�C�Y�`�F�b�N
		_COUT << _T("  Bank = ") << (unsigned int)pcm_bank << endl;
		_COUT << _T("  Size = ") << (unsigned int)pcm_size << _T(" [Byte] / ") << 0x10000 - Header.offsetPCM << _T(" [Byte]") << endl;

		if(	(Header.offsetPCM + pcm_size) > 0x10000	){
			Err(_T("��PCM�̃T�C�Y�����e�l���z���܂����B"));
		}

	} else {
		//��PCM�T�C�Y�`�F�b�N
		_COUT << _T("  Bank = ") << (unsigned int)pcm_bank << endl;
		_COUT << _T("  Size = ") << (unsigned int)pcm_size << _T(" [Byte]") << endl;

		i = mus_bank + pcm_bank + 3;
		if(i > 255){
			Err(_T("�o���N���̍��v��255���z���܂����B"));
		}
	}

	//----------------------
	//�m�r�e��������
	fileopen(strFileName);

	if(dpcm_bank == false){
		if(cDPCMinfo == NULL){
			//��PCM���g��Ȃ��ꍇ
			write(romimg, bin_size);			//NSF�w�b�_�[ �� �R�[�h�̏�������
			write(code.c_str(), code.size());	//�V�[�P���X�̏�������

		} else {
			//��PCM���g���ꍇ
			if(flag_Optimize == true){
				//�œK�����L���ł���΁A�w�b�_�[�Ƀo���N���������B
				i = 0;
				while(i < mus_bank){
					nsf->Bank[i] = (unsigned char)i;
					i++;
				}
				while(i < ((Header.offsetPCM - 0x8000)>>12)){
					nsf->Bank[i] = 0;
					i++;
				}
				j = 0;
				while(i < 8){
					if(j < pcm_bank){
						nsf->Bank[i] = mus_bank + (unsigned char)j;
					} else {
						nsf->Bank[i] = 0;
					}
					i++;
					j++;
				}
			}

			//�R�[�h���V�[�P���X
			write(romimg, bin_size);			//NSF�w�b�_�[ �� �R�[�h�̏�������
			write(code.c_str(), code.size());	//�V�[�P���X�̏�������

			if(flag_Optimize == true){
				//GAP
				while(mus_size < ((unsigned int)mus_bank<<12)){
					put(0);		//0 padding
					mus_size++;
				}
				//GAP2
				mus_size = Header.offsetPCM & 0x0FFF;
				while(mus_size > 0 ){
					put(0);		//0 padding
					mus_size--;
				}
				//��PCM
				write(dpcm_code.c_str(), pcm_size);		//��PCM�̏�������
				while(pcm_size < ((unsigned int)pcm_bank<<12)){
					put(0);		//0 padding
					pcm_size++;
				}
			} else {
				//GAP
				while(mus_size < (Header.offsetPCM - 0x8000)){
					put(0);		//0 padding
					mus_size++;
				}
				//��PCM
				write(dpcm_code.c_str(), pcm_size);		//��PCM�̏�������
			}
		}
	} else {
		//Bank �Ή�bin
		write(&romimg[0x0000], bin_size);	//NSF�w�b�_�[ �� �R�[�h�̏�������
		write(code.c_str(), code.size());	//�V�[�P���X�̏�������
		//GAP
		while(mus_size < ((unsigned int)mus_bank<<12)){
			put(0);		//0 padding
			mus_size++;
		}

		write(dpcm_code.c_str(), pcm_size);		//��PCM�̏�������
		//GAP
		while(pcm_size < ((unsigned int)pcm_bank<<12)){
			put(0);		//0 padding
			pcm_size++;
		}

		if(flag_Optimize != true){
			//GAP�i�K���A32kByte�ȏ�ɂ���B�j
			i = (mus_bank + pcm_bank + 3) << 12;
			while(i < 0x8000){
				put(0);		//0 padding
				i++;
			}
		}

	}

	close();

	//----------------------
	//Exit
	delete[]	romimg;
}

//==============================================================
//		�A�Z���u������\�[�X�ւ̕ۑ�
//--------------------------------------------------------------
//	������
//		const	char*	strFileName		�t�@�C����
//	���Ԓl
//				����
//==============================================================
void	MusicFile::saveASM(const char*	strFileName)
{
	unsigned	int			iBGM	= 0;
	unsigned	int			iSE		= 0;

	//----------------------
	//File open
	fileopen(strFileName);

	//Header
	*this <<	";===============================================================\n"
				";		Music file for NES Sound Driver & Library\n"
				";			for assembly language (ca65.exe)\n"
				";===============================================================\n"
				<<endl;

	//Export of Sequence
	while(iBGM < Header.iBGM){
		*this	<<	"	.export		"	<<	Header.Label	<<	"BGM"	<<	iBGM	<<	endl;
		iBGM++;
	}
	while(iSE < Header.iSE){
		*this	<<	"	.export		"	<<	Header.Label	<<	"SE"	<<	iSE	<<	endl;
		iSE++;
	}

	if(cDPCMinfo != NULL){
		*this	<<	"	.export		"	<<	Header.Label	<<	"DPCMinfo"	<<	endl;
	}

	//MML
	*this <<	"\n\n.segment	"	<<	'"'	<<	Header.segmentSEQ	<<	'"' << endl;

	getAsm(this);

	//----------------------
	//Close file
	close();
}

//==============================================================
//		�G���[����
//--------------------------------------------------------------
//	������
//		const	_CHAR	msg[]	�G���[���b�Z�[�W
//	���Ԓl
//				����
//==============================================================
void	MusicFile::Err(const _CHAR msg[])
{
	if(cOptionSW->fErr == true){
		_CERR << _T("[ ERROR ] : ") << msg << endl;
	} else {
		_COUT << _T("[ ERROR ] : ") << msg << endl;
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
void	MusicFile::Warning(const _CHAR msg[])
{
	//���݂̃t�@�C�����ƁA�s����\��
	if(cOptionSW->fErr == true){
		_CERR << _T("[WARNING] : ") << msg << endl;
	} else {
		_COUT << _T("[WARNING] : ") << msg << endl;
	}
}
