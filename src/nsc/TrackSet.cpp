/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#include "StdAfx.h"
#include "TrackSet.h"

//==============================================================
//		�R���X�g���N�^
//--------------------------------------------------------------
//	������
//		MMLfile*			MML			MML�t�@�C���̃I�u�W�F�N�g
//		unsigned	int		_id			BGM�ԍ�
//		bool				_sub		���̃I�u�W�F�N�g�́A�T�u���[�`���H
//		const		_CHAR	_strName[]	�I�u�W�F�N�g��
//	���Ԓl
//					����
//==============================================================
TrackSet::TrackSet(MMLfile* MML, unsigned int _id, bool _sub, bool _se, const _CHAR _strName[]):
	MusicItem(_id, _strName),
	iTempo(120),
	m_id(_id)
{
	//----------------------
	//Local�ϐ�

//	�萔��`
enum	Command_ID_mml {
	mml_Track,
	mml_Scale,
	mml_Major,
	mml_Minor,
	mml_HMinor,
	mml_MMinor,
	mml_KeySignature,
	mml_Macro,
	mml_MacroSet,
	mml_CallSE,
	mml_Subroutine,

	mml_Loop,
	mml_Repeat_A_Start,
	mml_Repeat_A_Branch,
	mml_Repeat_A_End,
	mml_Repeat_B_Start,
	mml_Repeat_B_Branch,
	mml_Repeat_B_End,
	mml_Repeat_C_Start,
//	mml_Repeat_C_Branch,
	mml_Repeat_C_End,

	mml_Tempo,
	mml_Tempo_Relative,
	mml_Tempo_Up,
	mml_Tempo_Down,

	mml_La,
	mml_Si,
	mml_Do,
	mml_Re,
	mml_Mi,
	mml_Fa,
	mml_Sol,
	mml_Rest,
	mml_Wait,
	mml_Tai,

	mml_Length,
	mml_Gate_Q,
	mml_Gate_q,
	mml_Gate_u,

	mml_Echo,
	mml_Echo_Off,
	mml_Echo_Reset,
	mml_Echo_C,
	mml_Echo_D,
	mml_Echo_E,
	mml_Echo_F,
	mml_Echo_G,
	mml_Echo_A,
	mml_Echo_B,
	mml_Echo_R,

	mml_Jump,
	mml_Jump_drv,

	mml_Envelop_Voice,
	mml_Envelop_Volume,
	mml_Envelop_Frequency,
	mml_Envelop_Note,

	mml_Envelop_Off_Voice,
	mml_Envelop_Off_Volume,
	mml_Envelop_Off_Frequency,
	mml_Envelop_Off_Note,

	mml_Patch,
	mml_Patch_Off,

	mml_Release_mdoe,
	mml_Release_Voice,
	mml_Release_Volume,

	mml_Voice,
	mml_FDSC,
	mml_FDSM,
	mml_FDSF,
	mml_FDSV,
	mml_VRC7,
	mml_N163,
	mml_N163_Set,
	mml_N163_Load,
	mml_N163_Channel,
	mml_FME7_frequency,

	mml_Octave,
	mml_Octave_Up,
	mml_Octave_Down,
	mml_Octave_Up1,
	mml_Octave_Down1,
	mml_Detune_Cent,
	mml_Detune_Cent_Relative,
	mml_Detune_Register,
	mml_Detune_Register_Relative,
	mml_Transpose,
	mml_Transpose_Relative,
	mml_KeyShift,
	mml_KeyShift_Relative,
	mml_Protament,
	mml_Protament2,
	mml_Sweep,

	mml_Volume,
	mml_Volume_Up,
	mml_Volume_Down,

	mml_VRC7_Write,
	mml_Memory_Write,

	mml_Priority,

	mml_Bar
};

//	�����́AMML�\���Ŏg����R�}���h�B

const	static	unsigned	char	RS_UTF8[]	=	{0x5c, 0x00};				//REVERSE SOLIDUS
const	static	unsigned	char	RS_UTF8W[]	=	{0xEF, 0xBC, 0xBC, 0x00};	//REVERSE SOLIDUS
const	static	unsigned	char	Yen_UTF8[]	=	{0xC2, 0xA5, 0x00};			//Yen
const	static	unsigned	char	Yen_UTF8W[]	=	{0xEF, 0xBF, 0xA5, 0x00};	//Yen

const	static	Command_Info	Command[] = {
		{	"TR",		mml_Track				},
		{	"�g���b�N",	mml_Track				},
		{	"Scale",	mml_Scale				},
		{	"scale",	mml_Scale				},
		{	"Mode",		mml_Scale				},
		{	"mode",		mml_Scale				},
		{	"Major",	mml_Major				},
		{	"major",	mml_Major				},
		{	"Minor",	mml_Minor				},
		{	"minor",	mml_Minor				},
		{	"HMinor",	mml_HMinor				},
		{	"hminor",	mml_HMinor				},
		{	"MMinor",	mml_MMinor				},
		{	"mminor",	mml_MMinor				},

		{	"K",		mml_KeySignature		},
		{	"��",		mml_KeySignature		},
		{	"SE",		mml_CallSE				},
		{	"S",		mml_Subroutine			},
		{	"$$",		mml_MacroSet			},
		{	"$",		mml_Macro				},

		{	"L",		mml_Loop				},
		{	"[:",		mml_Repeat_C_Start		},
	//	{	":",		mml_Repeat_C_Branch		},
		{	":]",		mml_Repeat_C_End		},
		{	"|:",		mml_Repeat_B_Start		},
		{	(char*)RS_UTF8,		mml_Repeat_B_Branch		},
		{	(char*)Yen_UTF8,	mml_Repeat_B_Branch		},
		{	(char*)RS_UTF8W,	mml_Repeat_B_Branch		},
		{	(char*)Yen_UTF8W,	mml_Repeat_B_Branch		},
		{	":|",		mml_Repeat_B_End		},
		{	"[",		mml_Repeat_A_Start		},
		{	":",		mml_Repeat_A_Branch		},
		{	"]",		mml_Repeat_A_End		},

		{	"��",		mml_Loop				},
		{	"�b�F",		mml_Repeat_B_Start		},
		{	"��",		mml_Repeat_B_Branch		},
		{	"�_",		mml_Repeat_B_Branch		},
		{	"�F�b",		mml_Repeat_B_End		},
		{	"�y",		mml_Repeat_A_Start		},
		{	"�F",		mml_Repeat_A_Branch		},
		{	"�z",		mml_Repeat_A_End		},

		{	"t_",		mml_Tempo_Relative,		},
		{	"t",		mml_Tempo,				},
		{	"�e���|",	mml_Tempo,				},
		{	"��",		mml_Tempo_Up,			},
		{	"��",		mml_Tempo_Down,			},

		{	"a",	mml_La					},
		{	"b",	mml_Si					},
		{	"c",	mml_Do					},
		{	"d",	mml_Re					},
		{	"e",	mml_Mi					},
		{	"f",	mml_Fa					},
		{	"g",	mml_Sol					},
		{	"r",	mml_Rest				},
		{	"w",	mml_Wait				},
		{	"^",	mml_Tai					},

		{	"��",	mml_La					},
		{	"��",	mml_Si					},
		{	"��",	mml_Do					},
		{	"��",	mml_Re					},
		{	"��",	mml_Mi					},
		{	"�ӂ�",	mml_Fa					},
		{	"��",	mml_Fa					},
		{	"��",	mml_Sol					},
		{	"��",	mml_Rest				},
		{	"��",	mml_Rest				},
		{	"�[",	mml_Tai					},

		{	"��",	mml_La					},
		{	"�V",	mml_Si					},
		{	"�h",	mml_Do					},
		{	"��",	mml_Re					},
		{	"�~",	mml_Mi					},
		{	"�t�@",	mml_Fa					},
		{	"�t",	mml_Fa					},
		{	"�\",	mml_Sol					},
		{	"��",	mml_Rest				},
		{	"�b",	mml_Rest				},
		{	"�|",	mml_Tai					},

		{	"�C",	mml_La					},
		{	"��",	mml_Si					},
		{	"�n",	mml_Do					},
		{	"�j",	mml_Re					},
		{	"�z",	mml_Mi					},
		{	"�w",	mml_Fa					},
		{	"�g",	mml_Sol					},

		{	"l",		mml_Length				},
		{	"����",		mml_Length				},
		{	"Q",		mml_Gate_Q				},
		{	"q",		mml_Gate_q				},
		{	"�Q�[�g",	mml_Gate_q				},
		{	"u",		mml_Gate_u				},

		{	"ECc",			mml_Echo_C				},
		{	"ECd",			mml_Echo_D				},
		{	"ECe",			mml_Echo_E				},
		{	"ECf",			mml_Echo_F				},
		{	"ECg",			mml_Echo_G				},
		{	"ECa",			mml_Echo_A				},
		{	"ECb",			mml_Echo_B				},
		{	"ECr",			mml_Echo_R				},
		{	"EC-",			mml_Echo_Reset			},
		{	"EC*",			mml_Echo_Off			},
		{	"EC",			mml_Echo				},
		{	"�G�R�[�I�t",	mml_Echo_Off			},
		{	"�G�R�[",		mml_Echo				},

		{	"J",			mml_Jump				},
		{	"j",			mml_Jump_drv			},

		{	"E@*",	mml_Envelop_Off_Voice		},
		{	"Ev*",	mml_Envelop_Off_Volume		},
		{	"Em*",	mml_Envelop_Off_Frequency	},
		{	"En*",	mml_Envelop_Off_Note		},

		{	"E@",	mml_Envelop_Voice		},
		{	"Ev",	mml_Envelop_Volume		},
		{	"Em",	mml_Envelop_Frequency	},
		{	"En",	mml_Envelop_Note		},

		{	"�G���x���[�v�I�t",	mml_Envelop_Off_Volume		},
		{	"�G���x���[�v",		mml_Envelop_Volume			},
		{	"�r�u���[�g�I�t",	mml_Envelop_Off_Frequency	},
		{	"�r�u���[�g",		mml_Envelop_Frequency		},

		{	"Rm",	mml_Release_mdoe		},
		{	"R@",	mml_Release_Voice		},
		{	"Rv",	mml_Release_Volume		},

		{	"@P*",	mml_Patch_Off			},
		{	"@P",	mml_Patch				},

		{	"@FC",	mml_FDSC				},
		{	"@FM",	mml_FDSM				},
		{	"@FF",	mml_FDSF				},
		{	"@FV",	mml_FDSV				},
		{	"@V",	mml_VRC7				},
		{	"@NS",	mml_N163_Set			},
		{	"@NL",	mml_N163_Load			},
		{	"@N",	mml_N163				},
		{	"@",	mml_Voice				},
		{	"���F",	mml_Voice				},
		{	"NC",	mml_N163_Channel		},
		{	"F",	mml_FME7_frequency		},

		{	"o",	mml_Octave				},
		{	"���K",	mml_Octave				},
		{	">",	mml_Octave_Up			},
		{	"��",	mml_Octave_Up			},
		{	"��",	mml_Octave_Up			},
		{	"<",	mml_Octave_Down			},
		{	"��",	mml_Octave_Down			},
		{	"��",	mml_Octave_Down			},
		{	"`",	mml_Octave_Up1			},
		{	"�e",	mml_Octave_Up1			},
		{	"\"",	mml_Octave_Down1		},
		{	"�h",	mml_Octave_Down1		},

		{	"D%_",			mml_Detune_Register_Relative		},
		{	"D%",			mml_Detune_Register					},
		{	"D_",			mml_Detune_Cent_Relative			},
		{	"D",			mml_Detune_Cent						},
		{	"�f�B�`���[��",	mml_Detune_Cent			},
		{	"__",			mml_Transpose_Relative	},
		{	"_",			mml_Transpose			},
		{	"k_",			mml_KeyShift_Relative	},
		{	"k",			mml_KeyShift			},
		{	"TrackKey",		mml_Transpose			},
		{	"P",			mml_Protament			},
		{	"�|���^�����g",	mml_Protament			},
		{	"{",			mml_Protament2			},
		{	"s",			mml_Sweep				},

		{	"v",		mml_Volume				},
		{	"����",		mml_Volume				},
		{	")",		mml_Volume_Up			},
		{	"�傫��",	mml_Volume_Up			},
		{	"��",		mml_Volume_Up			},
		{	"(",		mml_Volume_Down			},
		{	"������",	mml_Volume_Down			},
		{	"��",		mml_Volume_Down			},

		{	"yV",	mml_VRC7_Write			},
		{	"y",	mml_Memory_Write		},

		{	"#Priority",	mml_Priority	},
		{	"#priority",	mml_Priority	},

		{	"�@",	mml_Bar					},
		{	"|",	mml_Bar					},
		{	"�b",	mml_Bar					}
};

	unsigned	char	cData;
				int		i;

	//------------------------------
	//�N���X�̏����ݒ�
	fSub		= _sub;		//�T�u���[�`���̃t���O
	fSE			= _se;		//
	iTrack		= 0;		//�R���p�C�����̃g���b�N�iDefault = 0�j
	maxTrack	= 0;		//�ő�g���b�N�ԍ�

	if(fSE == true){
		Priority	= MML->priority;
	} else {
		Priority	= 0;
	}

	//�܂��́A�P�����g���b�N�i0�ԁj�̃I�u�W�F�N�g�����B
	nowTrack	= makeTrack(MML, iTrack);
	nowTrack->SetCompileFlag(true);


	//------------------------------
	//�R���p�C��

	// { �̌���
	while(MML->cRead() != '{'){
		if(MML->eof()){
			MML->Err(_T("�u���b�N�̊J�n������{��������܂���B"));
		}
	}

	// } ������܂ŁA�L�q�u���b�N�����R���p�C������B
	while(1){

		cData = MML->GetChar();
		if(cData == '}'){
			TrackChk(MML);
			if(nowTrack == NULL){
				break;
			}
		} else {
			// } ������O�ɁA[EOF]��������G���[
			if( MML->eof() ){
				MML->Err(_T("�u���b�N�̏I�[������`}'������܂���B"));
			}

			//�P�߂�
			MML->Back();
		}

		//�e�R�}���h���̏���
		switch(MML->GetCommandID(Command, sizeof(Command)/sizeof(Command_Info))){

			case(mml_Track):
				if(fSub == true){
					MML->Warning(_T("Sub�u���b�N���ł̓g���b�N�w��͂ł��܂���B�������܂��B"));
				} else {
					TrackProc(MML);
				}
				break;

			case(mml_Scale):
				nowTrack->SetScale(MML);
				break;

			case(mml_Major):
				nowTrack->SetMajor();
				break;

			case(mml_Minor):
				nowTrack->SetMinor();
				break;

			case(mml_HMinor):
				nowTrack->SetHMinor(MML);
				break;

			case(mml_MMinor):
				nowTrack->SetMMinor(MML);
				break;

			case(mml_KeySignature):
				nowTrack->SetKeySignature(MML);
				break;

			case(mml_Macro):
				MML->CallMacro();
				break;

			case(mml_MacroSet):
				MML->SetMacro(1);
				break;

			case(mml_CallSE):
				nowTrack->SetSE(MML);
				break;

			case(mml_Subroutine):
				nowTrack->SetSubroutine(MML->GetInt());
				break;

			case(mml_Loop):
				if(fSub == true){
					MML->Warning(_T("Sub�u���b�N���ł͖������[�v�͂ł��܂���B�������܂��B"));
				} else {
					nowTrack->SetLoop();
				}
				break;

			case(mml_Repeat_B_Start):
				nowTrack->SetRepeat_B_Start();
				break;

			case(mml_Repeat_B_Branch):
				nowTrack->SetRepeat_B_Branch(MML);
				break;

			case(mml_Repeat_B_End):
				nowTrack->SetRepeat_B_End(MML);
				break;

			case(mml_Repeat_A_Branch):
				nowTrack->SetRepeat_Branch(MML);
				break;

			case(mml_Repeat_A_Start):
				if((MML->iRepeatMode == 0) && (fSub == false)){
					nowTrack->SetRepeat_Start(MML);		//0 && main
				} else if(MML->iRepeatMode == 1){
					nowTrack->SetRepeat_A_Start(MML);	//1
				} else {
					nowTrack->SetRepeat_C_Start(MML);	//2 || (0 && sub)
				}
				break;

			case(mml_Repeat_A_End):
				if((MML->iRepeatMode == 0) && (fSub == false)){
					nowTrack->SetRepeat_End(MML);		//0 && main
				} else if(MML->iRepeatMode == 1){
					nowTrack->SetRepeat_A_End(MML);	//1
				} else {
					nowTrack->SetRepeat_C_End(MML);	//2 || (0 && sub)
				}
				break;

			case(mml_Repeat_C_Start):
				if((MML->iRepeatMode == 2) || ((MML->iRepeatMode == 0) && (fSub == true))){
					nowTrack->SetRepeat_A_Start(MML);	//2 || (0 && sub)
				} else {
					nowTrack->SetRepeat_C_Start(MML);	//1 || (0 && main)
				}
				break;

			case(mml_Repeat_C_End):
				if((MML->iRepeatMode == 2) || ((MML->iRepeatMode == 0) && (fSub == true))){
					nowTrack->SetRepeat_A_End(MML);
				} else {
					nowTrack->SetRepeat_C_End(MML);
				}
				break;

		//	case(mml_Repeat_C_Branch):
		//		nowTrack->SetRepeat_C_Branch(MML);
		//		break;

			case(mml_Tempo):
				SetTempo(MML);
				break;

			case(mml_Tempo_Relative):
				SetRelativeTempo(MML);
				break;

			case(mml_Tempo_Up):
				TempoUp();
				break;

			case(mml_Tempo_Down):
				TempoDown();
				break;

			case(mml_La):
				nowTrack->SetNote(MML, 5);
				break;

			case(mml_Si):
				nowTrack->SetNote(MML, 6);
				break;

			case(mml_Do):
				nowTrack->SetNote(MML, 0);
				break;

			case(mml_Re):
				nowTrack->SetNote(MML, 1);
				break;

			case(mml_Mi):
				nowTrack->SetNote(MML, 2);
				break;

			case(mml_Fa):
				nowTrack->SetNote(MML, 3);
				break;

			case(mml_Sol):
				nowTrack->SetNote(MML, 4);
				break;

			case(mml_Rest):
				nowTrack->SetRest(MML, MML->rest);
				break;

			case(mml_Wait):
				nowTrack->SetRest(MML, MML->wait | 0x80);
				break;

			case(mml_Tai):
				nowTrack->SetTai(MML);
				break;

			case(mml_Length):
				nowTrack->SetLength(MML);
				break;

			case(mml_Gate_Q):
				if(MML->q_reverse == true){
					nowTrack->SetGatetime(MML);
				} else {
					nowTrack->SetGatetime_Q(MML);
				}
				break;

			case(mml_Gate_q):
				if(MML->q_reverse == true){
					nowTrack->SetGatetime_Q(MML);
				} else {
					nowTrack->SetGatetime(MML);
				}
				break;

			case(mml_Gate_u):
				nowTrack->SetGatetime_u(MML);
				break;

			case(mml_Echo_C):
				nowTrack->SetEchoBuffer(MML, 0);
				break;

			case(mml_Echo_D):
				nowTrack->SetEchoBuffer(MML, 1);
				break;

			case(mml_Echo_E):
				nowTrack->SetEchoBuffer(MML, 2);
				break;

			case(mml_Echo_F):
				nowTrack->SetEchoBuffer(MML, 3);
				break;

			case(mml_Echo_G):
				nowTrack->SetEchoBuffer(MML, 4);
				break;

			case(mml_Echo_A):
				nowTrack->SetEchoBuffer(MML, 5);
				break;

			case(mml_Echo_B):
				nowTrack->SetEchoBuffer(MML, 6);
				break;

			case(mml_Echo_R):
				nowTrack->SetEchoBuffer(MML, -1);
				break;

			case(mml_Echo_Reset):
				nowTrack->ResetEcho();
				break;

			case(mml_Echo_Off):
				nowTrack->SetEcho();
				break;

			case(mml_Echo):
				nowTrack->SetEcho(MML);
				break;

			case(mml_Jump):
				nowTrack->SetJump(MML);
				break;

			case(mml_Jump_drv):
				SetJumpDrv(MML);
				break;

			case(mml_Envelop_Voice):
				nowTrack->SetEnvelop_Evoi(MML->GetInt() + MML->offset_Ei);
			//	nowTrack->SetEnvelop(nsd_Envelop_Voice, MML, MML->offset_Ei);
				break;

			case(mml_Envelop_Volume):
				nowTrack->SetEnvelop_Evol(MML->GetInt() + MML->offset_Ev);
			//	nowTrack->SetEnvelop(nsd_Envelop_Volume, MML, MML->offset_Ev);
				break;

			case(mml_Envelop_Frequency):
				nowTrack->SetEnvelop_Em(MML->GetInt() + MML->offset_Em);
			//	nowTrack->SetEnvelop(nsd_Envelop_Frequency, MML, MML->offset_Em);
				break;

			case(mml_Envelop_Note):
				nowTrack->SetEnvelop_En(MML->GetInt() + MML->offset_En);
			//	nowTrack->SetEnvelop(nsd_Envelop_Note, MML, MML->offset_En);
				break;

			case(mml_Envelop_Off_Voice):
				MML->Err(_T("���F�G���x���[�v�́A@�R�}���h�Ŗ����ɂł��܂��B"));
				break;

			case(mml_Envelop_Off_Volume):
				nowTrack->SetEnvelop_Evol();
			//	SetEvent(new mml_Address(nsd_Envelop_Volume));
				break;

			case(mml_Envelop_Off_Frequency):
				nowTrack->SetEnvelop_Em();
			//	SetEvent(new mml_Address(nsd_Envelop_Frequency));
				break;

			case(mml_Envelop_Off_Note):
				nowTrack->SetEnvelop_En();
			//	SetEvent(new mml_Address(nsd_Envelop_Note));
				break;

			case(mml_Patch):
				nowTrack->SetPatch(MML);
				break;

			case(mml_Patch_Off):
				nowTrack->SetPatch();
				break;

			case(mml_Release_mdoe):
				nowTrack->SetReleaseMode(MML);
				break;

			case(mml_Release_Voice):
				nowTrack->SetReleaseVoice(MML);
				break;

			case(mml_Release_Volume):
				nowTrack->SetReleaseVolume(MML);
				break;

			case(mml_Voice):
				nowTrack->SetVoice(MML->GetInt());
			//	SetEvent(new mml_general(nsd_Voice, MML, _T("Voice")));
				break;

			case(mml_FDSC):
				nowTrack->SetFDSC(MML);
				break;

			case(mml_FDSM):
				nowTrack->SetFDSM(MML);
				break;

			case(mml_FDSF):
				Set_FDS_Frequency(MML);
				break;

			case(mml_FDSV):
				Set_FDS_Volume(MML);
				break;

			case(mml_VRC7):
				nowTrack->SetVRC7(MML);
				break;

			case(mml_N163):
				nowTrack->SetN163(MML);
				break;

			case(mml_N163_Set):
				nowTrack->SetN163_Set(MML);
				break;

			case(mml_N163_Load):
				nowTrack->SetN163_Load(MML);
				break;

			case(mml_N163_Channel):
				Set_N163_Channel(MML);
				break;

			case(mml_FME7_frequency):
				Set_FME7_Frequency(MML);
				break;

			case(mml_Octave):
				nowTrack->SetOctave(MML);
				break;

			case(mml_Octave_Up):
				if(MML->octave_reverse == true){
					nowTrack->SetOctaveDec();
				} else {
					nowTrack->SetOctaveInc();
				}
				break;

			case(mml_Octave_Down):
				if(MML->octave_reverse == true){
					nowTrack->SetOctaveInc();
				} else {
					nowTrack->SetOctaveDec();
				}
				break;

			case(mml_Octave_Up1):
				nowTrack->SetOctaveOne_Inc();
				break;

			case(mml_Octave_Down1):
				nowTrack->SetOctaveOne_Dec();
				break;

			case(mml_Detune_Cent):
				SetEvent(new mml_general(nsd_Detune_Cent, MML, _T("Detune Cent")));
				break;

			case(mml_Detune_Register):
				SetEvent(new mml_general(nsd_Derune_Register, MML, _T("Derune Register")));
				break;

			case(mml_Detune_Cent_Relative):
				SetEvent(new mml_general(nsd_SubCommand, (const char)nsd_sub_Detune_Cent, (char)MML->GetInt(), _T("Relative Detune Cent")));
				break;

			case(mml_Detune_Register_Relative):
				SetEvent(new mml_general(nsd_SubCommand, (const char)nsd_sub_Derune_Register, (char)MML->GetInt(), _T("Relative Detune Register")));
				break;
	
			case(mml_Transpose):
				i = MML->GetInt();
				if( (i < -128) || (i > 127) ){
					MML->Err(_T("�ڒ���-127�`128�͈̔͂Ŏw�肵�Ă��������B"));
				}
				nowTrack->SetTranspose(i);
				//SetEvent(new mml_general(nsd_Transpose, MML, _T("Transpose")));
				break;

			case(mml_Transpose_Relative):
				nowTrack->SetTranspose_Relative(MML->GetInt());
				//SetEvent(new mml_general(nsd_Relative_Transpose, MML, _T("Relative Transpose")));
				break;

			case(mml_KeyShift):
				nowTrack->SetKeyShift(MML);
				break;

			case(mml_KeyShift_Relative):
				nowTrack->SetKeyShift_Relative(MML);
				break;

			case(mml_Protament):
				nowTrack->SetProtament(MML);
				break;

			case(mml_Protament2):
				nowTrack->SetProtament(MML, (unsigned char)iTempo);
				break;

			case(mml_Sweep):
				nowTrack->SetSweep(MML);
				break;

			case(mml_Volume):
				nowTrack->SetVolume(MML);
				break;

			case(mml_Volume_Up):
				nowTrack->SetVolumeInc(MML);
				break;

			case(mml_Volume_Down):
				nowTrack->SetVolumeDec(MML);
				break;

			case(mml_VRC7_Write):
				Set_VRC7_Write(MML);
				break;

			case(mml_Memory_Write):
				Set_Poke(MML);
				break;

			case(mml_Priority):
				Set_Priority(MML);
				break;

			case(mml_Bar):
				break;

			//unknown command
			default:
				MML->Err(_T("unknown command"));
				break;
		}
	}

	//Local Macro�̉��
	MML->DeleteMacro(1);

	if(fSub == true){
		//�T�u���[�`���u���b�N�̏ꍇ
		code.resize(0);
		i = (int)ptcTrack[iTrack]->SetEnd(MML);

	} else {
		//����ȊO�̏ꍇ
		i = 2 + ((maxTrack + 1) * 2);		//�g���b�N���������w�b�_�[�̃T�C�Y���v�Z�B
		code.resize(i);						//�w�b�_�p�ɃR�[�h�T�C�Y���m��

		code[0] = (unsigned char)maxTrack + 1;				//�g���b�N��
		code[1] = Priority;									//�e��t���O

		//�e�g���b�N�ɏI�[�������āA�ȃf�[�^�̃A�h���X�����쐬
		iTrack = 0;
		while(iTrack <= maxTrack){
			unsigned	int	n	= (int)ptcTrack[iTrack]->SetEnd(MML);
			if(n==0){
				code[iTrack *2 + 2]	= 0;
				code[iTrack *2 + 3]	= 0;
			} else {
				code[iTrack *2 + 2]	= (unsigned char)((i   ) & 0xFF);
				code[iTrack *2 + 3]	= (unsigned char)((i>>8) & 0xFF);
			}
			i += n;
			iTrack++;
		}
	}
	iSize = i;

}

//==============================================================
//		�f�X�g���N�^
//--------------------------------------------------------------
//	������
//				����
//	���Ԓl
//				����
//==============================================================
TrackSet::~TrackSet(void)
{
}

//==============================================================
//		�N���A
//--------------------------------------------------------------
//	������
//				����
//	���Ԓl
//				����
//==============================================================
void	TrackSet::clear(int _id)
{
	maxTrack = -1;
	MusicItem::clear(_id);
}

//==============================================================
//		�R�[�h�̎擾
//--------------------------------------------------------------
//	������
//		MusicFile*	MUS		�R�[�h���o�͂���ȃf�[�^�t�@�C���E�I�u�W�F�N�g
//	���Ԓl
//				����
//==============================================================
void	TrackSet::getAsm(MusicFile* MUS)
{
	//----------------------
	//Local�ϐ�
	int	i = 0;
	list<	MusicItem*>::iterator	itItem;

	if(fSub == false){
		*MUS << "	.byte	$" << hex << setw(2) << setfill('0') << (int)(code[0] & 0xFF) << ", $" << (int)(code[1] & 0xFF) << endl;
		while(i <= maxTrack){
			if(i==0){
				*MUS << "	.word	$";
			} else {
				*MUS << " ,$";
			}
			*MUS << hex << setw(4) << setfill('0') << (int)((code[i*2+2] & 0xFF) | ((code[i*2+3] & 0xFF)<<8));
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

//==============================================================
//		�J�E���g���Ȃ���A�œK�������W
//--------------------------------------------------------------
//	������
//				MusicFile*	MUS
//				int			iStart
//				int			iEnd
//	���Ԓl
//				����
//==============================================================
void	TrackSet::TickCountPrint(MusicFile* MUS, int iStart, int iEnd)
{
	int		i		= iStart;
	int		iTick;

	cout	<< "Track ";
	while(i < iEnd){
		i++;
		cout << "| TR(" << setfill(' ')  << setw(2) << i << ") ";
	}

	i = iStart;
	cout << endl <<	"Loop  ";
	while(i < iEnd){
		ptcTrack[i]->nsd.init();		//�G�~�����[�^�E�p�����[�^������
		ptcTrack[i]->TickCount(MUS);	//�G�~�����[�g
		iTick = ptcTrack[i]->GetTickLoop();
		if(iTick == -1){
			cout << "| no-loop";
		} else {
			cout << "|" << setw(8) << setfill(' ') << iTick;
		}
		i++;
	}

	i = iStart;
	cout << endl <<	"Total ";;
	while(i < iEnd){
		cout << "|" << setw(8) << setfill(' ') << ptcTrack[i]->GetTickTotal();
		i++;
	}
	cout << endl;
	
}

//==============================================================
//		�J�E���g���Ȃ���A�œK�������W
//--------------------------------------------------------------
//	������
//				MusicFile*	MUS
//	���Ԓl
//				����
//==============================================================
void	TrackSet::TickCount(MusicFile* MUS)
{

	int		iTrack = 0;
	int		j;
	int		_maxTrack = maxTrack + 1;

	while(iTrack < _maxTrack){
		j = iTrack + 8;
		if(j >= _maxTrack){
			j = _maxTrack;
		}
		TickCountPrint(MUS,iTrack,j);
		cout << endl;
		iTrack = j;
	}
}

//==============================================================
//		�s�v�Ȓ�`�����邩����
//--------------------------------------------------------------
//	������
//		MusicFile*	MUS		�ȃf�[�^�t�@�C���E�I�u�W�F�N�g
//	���Ԓl
//				����
//==============================================================
void	TrackSet::OptimizeDefineCheck(MusicFile* MUS)
{
	iTrack = 0;
	while(iTrack <= maxTrack){
		ptcTrack[iTrack]->OptimizeDefineCheck(MUS);
		iTrack++;
	}

}

//==============================================================
//		�A�h���X�������肷��B
//--------------------------------------------------------------
//	������
//		MusicFile*	MUS		�ȃf�[�^�t�@�C���E�I�u�W�F�N�g
//	���Ԓl
//				����
//==============================================================
void	TrackSet::Fix_Address(MusicFile* MUS)
{
	iTrack = 0;
	while(iTrack <= maxTrack){
		ptcTrack[iTrack]->Fix_Address(MUS);
		iTrack++;
	}

}

//==============================================================
//		�g���b�N
//--------------------------------------------------------------
//	������
//		MMLfile*			MML			MML�t�@�C���̃I�u�W�F�N�g
//	���Ԓl
//		����
//==============================================================
void	TrackSet::TrackChk(MMLfile* MML)
{
	int		i = iTrack + 1;

	if(MML->GetMacroNest() > 0){
		MML->Err(_T("�}�N�����Ńg���b�N�̎w��͂ł��܂���B"));
	}

	//------------------
	//�����̃g���b�N�̃`�F�b�N
//	i		= iTrack + 1;
	iTrack	= 0;
	while(i <= maxTrack){
		if(ptcTrack[i]->GetCompileFlag() == true){
			iTrack = i;
			break;
		}
		i++;
	}

	if(iTrack != 0){
		//------------------
		//�����̃g���b�N������ꍇ

		//�|�C���^�ƍs�ԍ��𕜋A
		MML->StreamPointerMove(TrackPt);
		MML->SetLine(TrackLine);
		nowTrack = getTrack(MML, iTrack);
	} else {
		nowTrack = NULL;
	}
}

//==============================================================
//		�g���b�N�̏���
//--------------------------------------------------------------
//	������
//		MMLfile*			MML			MML�t�@�C���̃I�u�W�F�N�g
//	���Ԓl
//		����
//==============================================================
void	TrackSet::TrackProc(MMLfile* MML)
{
					int		i;
	unsigned		char	cData;

	//------------------
	//�����̃g���b�N�̃`�F�b�N

	TrackChk(MML);

	if(nowTrack == NULL){
		//------------------
		//�����̃g���b�N���Ȃ��ꍇ

		//�t���O��S�ă��Z�b�g
		i = 0;
		while(i <= maxTrack){
			ptcTrack[i]->SetCompileFlag(false);
			i++;
		}
		
		//�R���p�C������g���b�N���
		do{
			iTrack = MML->GetInt() - 1;
			if( (iTrack <= -1) ){
				MML->Err(_T("�g���b�N�ԍ��Ŏw��ł���͈͂𒴂��Ă��܂��B"));
			}
			nowTrack = getTrack(MML, iTrack);
			nowTrack->SetCompileFlag(true);
			cData = MML->GetChar();
		} while(cData == ',');
		MML->Back();

		//�|�C���^�ƍs�ԍ���ޔ�
		TrackPt		= MML->tellg();
		TrackLine	= MML->GetLine();

		//�ŏ���
		i = 0;
		while(i <= maxTrack){
			if(ptcTrack[i]->GetCompileFlag() == true){
				iTrack = i;
				nowTrack = getTrack(MML, iTrack);
				break;
			}
			i++;
		}

	}

}

//==============================================================
//		�g���b�N�̍쐬
//--------------------------------------------------------------
//	������
//		unsigned	int	_track		�g���b�N�ԍ�
//	���Ԓl
//		MusicTrack*					������g���b�N�E�I�u�W�F�N�g�̃|�C���^
//==============================================================
MusicTrack*	TrackSet::makeTrack(MMLfile* MML, int _track)
{
	//�g���b�N�̃I�u�W�F�N�g�𐶐��B
	MusicTrack*	newTrack	= new MusicTrack(MML);

	//���������A�C�e���͕ۑ�
	ptcItem.push_back(newTrack);		//���N���X"MusicItem"���ŊJ������B
	ptcTrack[_track] = newTrack;

	if(MML->iReleaseVolume != 2){		//�h���C�o�[���̃f�t�H���g�͂Q
		newTrack->SetEvent(new mml_general(nsd_Release_Volume + (unsigned char)MML->iReleaseVolume, _T("Release Volume")));
	}

	//�|�C���^��n��
	return(newTrack);
}

//==============================================================
//		�g���b�N�E�I�u�W�F�N�g�̃|�C���^���擾
//--------------------------------------------------------------
//	������
//		unsigned	int	_track	�g���b�N�ԍ�
//	���Ԓl
//		MusicTrack*				�g���b�N�E�I�u�W�F�N�g�̃|�C���^
//	���Ԓl
//		�w�肳�ꂽ�ԍ��̃g���b�N�E�I�u�W�F�N�g�̃|�C���^���擾����
//		���������ꍇ�͐V���Ƀg���b�N������āA
//		�g���b�N�ԍ����ő�l�𒴂��Ă�����ő�l���X�V����B
//==============================================================
MusicTrack*	TrackSet::getTrack(MMLfile* MML, int _track)
{
			int	i			 = maxTrack;	// i = ������A�ŏI�g���b�N�̔ԍ�
	MusicTrack*	_getTrack;

	//�ŏI�g���b�N�ԍ����w��l������������A�J��Ԃ��B
	while(i < _track){
		i++;
		//�g���b�N��������������
		if(ptcTrack.count(i) == 0){
			_getTrack	= makeTrack(MML, i);
		} else {
			_CERR << _T("MusicTrack* TrackSet::getTrack()�֐��ŃG���[���������܂����B") << endl;
			nsc_exit(EXIT_FAILURE);
		}
	}
	maxTrack = i;	//�g���b�N�̍ő�l���L���B

	//�|�C���^���擾����B
	_getTrack = ptcTrack[_track];

	return(_getTrack);
}

//==============================================================
//		�C�x���g
//--------------------------------------------------------------
//	������
//		MusicItem* _item
//	���Ԓl
//		����
//==============================================================
void	TrackSet::SetEvent(MusicItem* _item)
{
	nowTrack->SetEvent(_item);
}

//==============================================================
//			���ʉ��D��x�ݒ�
//--------------------------------------------------------------
//	������
//		MMLfile*	MML		MML�t�@�C���̃I�u�W�F�N�g
//	���Ԓl
//				����
//==============================================================
void	TrackSet::Set_Priority(MMLfile* MML)
{
	int	i = MML->GetInt();

	//se?
	if(fSE == false){
		MML->Warning(_T("SE�u���b�N�ȊO�ł͗D��x�w��͂ł��܂���B�������܂��B"));
	} else {
		if( (i <= 3) && (i >=0) ){
			Priority = (char)i;
		} else {
			MML->Err(_T("���ʉ��̗D��x�́A��0�`3�͈̔͂Ŏw�肵�ĉ������B"));
		}
	}

}

//==============================================================
//		�W�����v
//--------------------------------------------------------------
//	������
//		MusicItem* _item
//	���Ԓl
//		����
//==============================================================
void	TrackSet::SetJumpDrv(MMLfile* MML)
{
	int	i	= MML->GetInt();

	switch(i){
		case(0):
			SetEvent(new mml_general(nsd_SubCommand, (const char)nsd_sub_Jump_off, _T("Jump Off")));
			break;
		case(1):
			SetEvent(new mml_general(nsd_SubCommand, (const char)nsd_sub_Jump_on, _T("Jump On")));
			break;
		default:
			MML->Err(_T("j�R�}���h���w��\�Ȕ͈͂𒴂��܂����B"));
	}
}

//==============================================================
//		�e���|
//--------------------------------------------------------------
//	������
//		MMLfile*			MML			MML�t�@�C���̃I�u�W�F�N�g
//	���Ԓl
//		����
//==============================================================
void	TrackSet::SetTempo(MMLfile* MML)
{
	int	iValue	= MML->GetInt();

	iTempo = ((iValue * MML->timebase) / 24);
	
	if((iTempo<0) || (iTempo>255)){
		MML->Err(_T("�e���|���w��\�Ȕ͈͂𒴂��܂����B"));
	}
	SetEvent(new mml_general(nsd_Tempo, (unsigned char)iTempo, _T("Tempo")));
}

//==============================================================
//		���΃e���|
//--------------------------------------------------------------
//	������
//		MMLfile*			MML			MML�t�@�C���̃I�u�W�F�N�g
//	���Ԓl
//		����
//==============================================================
void	TrackSet::SetRelativeTempo(MMLfile* MML)
{
	int	iValue	= MML->GetInt();

	iTempo += iValue;

	SetEvent(new mml_general(nsd_Relative_Tempo, (char)iValue, _T("Relative Tempo")));

}

//==============================================================
//		�e���|�A�b�v
//--------------------------------------------------------------
//	������
//		MMLfile*			MML			MML�t�@�C���̃I�u�W�F�N�g
//	���Ԓl
//		����
//==============================================================
void	TrackSet::TempoUp()
{
	iTempo += 4;

	SetEvent(new mml_general(nsd_Relative_Tempo, (char)4, _T("Relative Tempo")));

}

//==============================================================
//		�e���|�_�E��
//--------------------------------------------------------------
//	������
//		MMLfile*			MML			MML�t�@�C���̃I�u�W�F�N�g
//	���Ԓl
//		����
//==============================================================
void	TrackSet::TempoDown()
{
	iTempo -= 4;

	SetEvent(new mml_general(nsd_Relative_Tempo, (char)-4, _T("Relative Tempo")));

}

//==============================================================
//		�A�h���X�w��̏�����
//--------------------------------------------------------------
//	������
//		MMLfile*	MML		MML�t�@�C���̃I�u�W�F�N�g
//	���Ԓl
//		����
//==============================================================
void	TrackSet::Set_Poke(MMLfile* MML)
{
	unsigned	int		addr;
	unsigned	int		data;
	unsigned	char	cData;

	addr = MML->GetInt();
	if( (addr < 0x0000) || (addr > 0xFFFF) ){
		MML->Err(_T("y�R�}���h�̃A�h���X��$0000�`$FFFF�͈̔͂Ŏw�肵�ĉ������B"));
	}

	cData = MML->GetChar();
	if(cData != ','){
		MML->Err(_T("y�R�}���h�̃p�����[�^������܂���B�Q�w�肵�Ă��������B"));
	}

	data = MML->GetInt();
	if(data > 255){
		MML->Err(_T("y�R�}���h�̃f�[�^��$00�`$FF�͈̔͂Ŏw�肵�ĉ������B"));
	}
	SetEvent(new mml_poke(addr, (unsigned char)data));
}

//==============================================================
//			FDS	�L�����A���g���ݒ�
//--------------------------------------------------------------
//	������
//		MMLfile*	MML		MML�t�@�C���̃I�u�W�F�N�g
//	���Ԓl
//				����
//==============================================================
void	TrackSet::Set_FDS_Frequency(MMLfile* MML)
{
	int	i = MML->GetInt();

	if( (i <= 0x0FFF) && (i >=0) ){
		unsigned	char	c0 = (unsigned char)( i       & 0xFF);
		unsigned	char	c1 = (unsigned char)((i >> 8) & 0xFF);
		SetEvent(new mml_general(nsd_FDS_Frequency,c0,c1,_T("FDS career frequency")));
	} else {
		MML->Err(_T("FDS�̃L�����A���g����0�`4095�͈̔͂Ŏw�肵�ĉ������B"));
	}
}

//==============================================================
//			FDS	Master volume
//--------------------------------------------------------------
//	������
//		MMLfile*	MML		MML�t�@�C���̃I�u�W�F�N�g
//	���Ԓl
//				����
//==============================================================
void	TrackSet::Set_FDS_Volume(MMLfile* MML)
{
	int		i = MML->GetInt();

	if((i<=3 ) && (i>=0)){
		SetEvent(new mml_general(nsd_FDS_Volume,(unsigned char)i,_T("FDS Master volume")));
	} else {
		MML->Err(_T("FDS�̃}�X�^�[���ʂ�0�`3�͈̔͂Ŏw�肵�ĉ������B"));
	}
}

//==============================================================
//		VRC7	���W�X�^��������
//--------------------------------------------------------------
//	������
//		MMLfile*	MML		MML�t�@�C���̃I�u�W�F�N�g
//	���Ԓl
//		����
//==============================================================
void	TrackSet::Set_VRC7_Write(MMLfile* MML)
{
	unsigned	char	cData;

				int		_Reg;
				int		_Dat;

	_Reg = MML->GetInt();
	if( (_Reg < 0) || (_Reg > 0x40) ){
		MML->Err(_T("VRC7���W�X�^����̑�1�p�����[�^��0�`63�͈̔͂Ŏw�肵�Ă��������B"));
	}

	cData = MML->GetChar();
	if(cData != ','){
		MML->Err(_T("yV �R�}���h�̃p�����[�^������܂���B�Q�w�肵�Ă��������B"));
	}

	_Dat = MML->GetInt();
	if( (_Dat < 0) || (_Dat > 255) ){
		MML->Err(_T("VRC7���W�X�^����̑�2�p�����[�^��0�`255�͈̔͂Ŏw�肵�Ă��������B"));
	}

	SetEvent(new mml_general(nsc_VRC7_reg,(unsigned char)_Reg,(unsigned char)_Dat, _T("VRC7 Register Write")));
}

//==============================================================
//		NC	n163�`�����l�����ݒ�
//--------------------------------------------------------------
//	������
//		MMLfile*	MML		MML�t�@�C���̃I�u�W�F�N�g
//	���Ԓl
//				����
//==============================================================
void	TrackSet::Set_N163_Channel(MMLfile* MML)
{
	int	i = MML->GetInt();

	if( (i <= 8) && (i >=1) ){
		SetEvent(new mml_general(nsc_N163_Channel,(unsigned char)(i-1),_T("n163 channel")));
	} else {
		MML->Err(_T("n163�̃`�����l������1�`8�͈̔͂Ŏw�肵�Ă��������B"));
	}

}

//==============================================================
//			SUNSOFT 5b �G���x���[�v���g���ݒ�
//--------------------------------------------------------------
//	������
//		MMLfile*	MML		MML�t�@�C���̃I�u�W�F�N�g
//	���Ԓl
//				����
//==============================================================
void	TrackSet::Set_FME7_Frequency(MMLfile* MML)
{
	int	i = MML->GetInt();

	if( (i <= 0xFFFF) && (i >=0) ){
		unsigned	char	c0 = (unsigned char)( i       & 0xFF);
		unsigned	char	c1 = (unsigned char)((i >> 8) & 0xFF);
		SetEvent(new mml_general(nsc_FME7_frequency,c0,c1,_T("FME7 envelop frequency")));
	} else {
		MML->Err(_T("SUNSOFT 5b�̃n�[�h�E�F�A�G���x���[�v���g����0�`65535�͈̔͂Ŏw�肵�ĉ������B"));
	}
}
