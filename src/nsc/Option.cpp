/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#include "StdAfx.h"
#include "Option.h"


#ifdef _WIN32
// WINDOWS�̂݁A'/'�ɑΉ�����B
#define OPTCHK (argv[iCount][0]=='/')||(argv[iCount][0]=='-')
#else
// unix�n�ł̓X���b�V���̓f�B���N�g���̋�؂�Ɏg����ׁA���s���鎖������
#define OPTCHK (argv[iCount][0]=='-')
#endif

//==============================================================
//		�I�v�V��������
//--------------------------------------------------------------
//	������
//			int		argc	�I�v�V����������̐�
//			_TCHAR*	argv[]	�I�v�V����������
//	���Ԓl
//			����
//	�����l
//			�I�v�V�����Ƀt�@�C�������w�肳��Ȃ��ꍇ�́A�w���v�\�����ďI��
//==============================================================
OPSW::OPSW(int argc, char* argv[]):
	//�������ݒ�
	fHelp(0),		//�w���v�́A�f�t�H���g�͕\�����Ȃ��B
	saveNSF(false),
	saveASM(false),
	flag_Optimize(false),
	flag_OptObj(true),
	flag_OptSeq(true),
	fErr(false),
	flag_SearchPass(false),
	cDebug(0)
{

	//----------------------------------
	//��Local �ϐ�
	
	int		iCount;				//while�̃J�E���g�p
	int		iResult;			//�ėp

	//Option�����p
	int		iOptionChk;			//�I�v�V�����`�F�b�N�p�@�|�C���^
	char	cOption;			//�I�v�V�����`�F�b�N�p�@����
	char	iFlagFilnameExt;	//�g���q���������̃t���O

	//�����p�X
	string	str_code	= "";
	string	str_dmc		= "";
	string	str_inc		= "";

	//----------------------------------
	//���I�v�V��������
	iCount=1;	//�R�}���h���͔�΂�
	while(iCount!=argc)
	{
		//--------------
		//�I�v�V�����X�C�b�`�Ƀn�C�t��(or�X���b�V��)�����邩�m�F
		if(OPTCHK){

			//--------------
			//��Option Switch	�i�X���b�V�����������ꍇ�̏����j
			switch(argv[iCount][1]){
				//--------
				//Help�\��
				case 'h' :
				case 'H' :
				case '?' :
					fHelp=1;
					break;
				//--------
				//�A�Z���u�������
				case 'o' :
				case 'O' :
					{
						bool fTemp;
						switch(argv[iCount][3]){
							case '0' :
							case '+' :
								fTemp = true;
								break;
							case '1' :
							case '-' :
								fTemp = false;
								break;
							default :
								opError(_T("-o"));
								break;
						}
						switch(argv[iCount][2]){
							case 'd' :
							case 'D' :
								flag_Optimize = fTemp;
								break;
							case 'o' :
							case 'O' :
								flag_OptObj = fTemp;
								break;
							case 's' :
							case 'S' :
								flag_OptSeq = fTemp;
								break;
							default :
								opError(_T("-o"));
								break;
						}
						break;
					}
				//--------
				//�A�Z���u�������
				case 'a' :
				case 'A' :
					saveASM = true;
					break;
				//--------
				//�m�r�e��
				case 'n' :
				case 'N' :
					saveNSF = true;
					break;
				//--------
				//�W���G���[�o�͂�
				case 'e' :
				case 'E' :
					fErr = true;
					break;
				//--------
				//Tick Count
		//		case 't' :
		//		case 'T' :
		//			flag_TickCount = false;
		//			break;
				//--------
				//Search Pass
				case 's' :
				case 'S' :
					flag_SearchPass = true;
					break;
				//--------
				//Debug�̎w��
				case 'D' :
					//0x01:	Class Object Creat
					//0x02:	Address settlement
					//0x04:	Music File Outputing
					//0x40:	Class Object Clear	
					//0x80:	Class Object Delete
					int	i;
					iResult=sscanf(argv[iCount],"-D%d",&i);
					cDebug = (char)i;
					if((iResult==NULL)||(iResult==EOF)){
						opError(_T("-D"));
						break;
					};
					break;
				//--------
				//Debug�̎w��
				case 'l' :
				case 'L' :
					//���Ɏw�肳��Ă���H
					if(strCodeName.empty()){
						iFlagFilnameExt=0;		//�g���q�̗L���@Reset
						iOptionChk=0;
						while((cOption=argv[iCount][iOptionChk+2])!=0)
						{
							strCodeName+=cOption;
							if(cOption=='.'){iFlagFilnameExt=1;};
							iOptionChk++;
						};
						if(iFlagFilnameExt==0){
							strCodeName+=".bin";
						};
					} else {
						opError(_T("-l Code �t�@�C����2��ȏ�w�肳��܂����B"));
						break;
					};
					break;
				//--------
				//ROM�R�[�h�̌����p�X
				case 'c' :
				case 'C' :
					str_code += &argv[iCount][2];
					str_code.append(1, _PATH_SPLIT);
					break;
				//--------
				//��PCM�̌����p�X
				case 'p' :
				case 'P' :
					str_dmc += &argv[iCount][2];
					str_dmc.append(1, _PATH_SPLIT);
					break;
				//--------
				//�C���N���[�h�t�@�C���̌����p�X
				case 'i' :
				case 'I' :
					str_inc += &argv[iCount][2];
					str_inc.append(1, _PATH_SPLIT);
					break;
				//--------
				//�t�@�C���̎w��
				case 'f' :
				case 'F' :
					//��ɁA�t�@�C�����������Ă��邩�`�F�b�N�B
					if(argv[iCount][3]==0){
						opError(_T("/F �t�@�C�����������Ă���܂���B"));
						break;
					};
					switch(argv[iCount][2]){
					//--------
					//MML�t�@�C���̎w��
					case 'A' :
					case 'a' :
						//���Ɏw�肳��Ă���H
						if(strASMname.empty()){
							iFlagFilnameExt=0;		//�g���q�̗L���@Reset
							iOptionChk=0;
							while((cOption=argv[iCount][iOptionChk+3])!=0)
							{
								strASMname+=cOption;
								if(cOption=='.'){iFlagFilnameExt=1;};
								iOptionChk++;
							};
							if(iFlagFilnameExt==0){
								strASMname+=".s";
							};
						} else {
							opError(_T("-fa ASM �t�@�C����2��ȏ�w�肳��܂����B"));
							break;
						};
						break;
					case 'N' :
					case 'n' :
						//���Ɏw�肳��Ă���H
						if(strNSFname.empty()){
							iFlagFilnameExt=0;		//�g���q�̗L���@Reset
							iOptionChk=0;
							while((cOption=argv[iCount][iOptionChk+3])!=0)
							{
								strNSFname+=cOption;
								if(cOption=='.'){iFlagFilnameExt=1;};
								iOptionChk++;
							};
							if(iFlagFilnameExt==0){
								strNSFname+=".nsf";
							};
						} else {
							opError(_T("-fn NSF �t�@�C����2��ȏ�w�肳��܂����B"));
							break;
						};
						break;
					default :
						opError(_T("-f"));
						break;
					};
				break;
				//--------
				//�f�t�H���g
				default :
					opError(_T(""));
					break;
			};

		} else{

			//--------------
			//���t�@�C����	�i�X���b�V�������������ꍇ�̏����j
			//���Ɏw�肳��Ă���H
			if(strMMLname.empty()){
				iFlagFilnameExt=0;		//�g���q�̗L���@Reset
				iOptionChk=0;		
				while((cOption=argv[iCount][iOptionChk])!=0)
				{
					strMMLname+=cOption;
					if(cOption=='.'){iFlagFilnameExt=1;};
					iOptionChk++;
				};
				if(iFlagFilnameExt==0){
					strMMLname+=".mml";
				};
			} else {
				opError(_T("MML�t�@�C����2��ȏ�w�肳��܂����B"));
				break;
			};

		};

		//--------------
		//�����̃I�v�V����
		iCount++;
	};

	//----------------------------------
	//���I�v�V�����Ŏw�肳�ꂽ������������B

	//--------------
	//�w���v�\��
	//�t�@�C������������Ȃ������ꍇ���A�w���v��\������B
	if((fHelp==1)||(strMMLname.empty())){print_help();};

	if(strASMname.empty()){
		iOptionChk=0;		
		while((cOption=strMMLname[iOptionChk])!='.')
		{
			strASMname+=cOption;
			iOptionChk++;
		};
		strASMname+=".s";
	};

	if(strNSFname.empty()){
		iOptionChk=0;		
		while((cOption=strMMLname[iOptionChk])!='.')
		{
			strNSFname+=cOption;
			iOptionChk++;
		};
		strNSFname+=".nsf";
	};

	//----------------------------------
	//�������p�X�̐ݒ�

	//MML�̃f�B���N�g�����擾
	iCount	= 0;
	iResult	= 0;
	while(iCount < strMMLname.size()){
		if((strMMLname[iCount] == '\\') || (strMMLname[iCount] == '/')){
			iResult = iCount;
		}
		iCount++;
	}

	//�D�揇�ʂP�@�J�����g�p�X
	//�D�揇�ʂQ�@MML�t�@�C�������݂���p�X
	if(iResult > 0){
		string	str_mmldir = "";
		iCount	= 0;
		while(iCount < iResult){
			str_mmldir += strMMLname[iCount];
			iCount++;
		}
		m_pass_code.add(&str_mmldir);
		m_pass_dmc.add(&str_mmldir);
		m_pass_inc.add(&str_mmldir);
	}

	//�D�揇�ʂR�@���ϐ��Ŏw��̃p�X
	m_pass_code.add(getenv("PATH"));
	m_pass_dmc.add(getenv("DMC_INCLUDE"));

	//�D�揇�ʂS�@�I�v�V�����Ŏw��̃p�X
	m_pass_code.add(&str_code);
	m_pass_dmc.add(&str_dmc);
	m_pass_inc.add(&str_inc);



	//----------------------------------
	//��

	//	to do	���̑��̃I�v�V������ǉ������Ƃ��́A���̕ӂɒǋL����B

	//----------
	//Debug�p �\��
//	cout << "MML = " << strMMLname << endl;
//	cout << "NSF = " << strNSFname << endl;
//	cout << "BIN = " << strBINname << endl;
//	cout << "ASM = " << strASMname << endl;
//	cout << "C   = " << strCname << endl;

/*
	cout << "code:" << endl;
	m_pass_code.debug();

	cout << "dmc:" << endl;
	m_pass_dmc.debug();

	cout << "inc:" << endl;
	m_pass_inc.debug();
*/

};
//==============================================================
//		�w���v���b�Z�[�W
//--------------------------------------------------------------
//	������
//			�Ȃ�
//	���Ԓl
//			����
//==============================================================
void	OPSW::print_help(){

	_COUT	<<	_T("MML Compiler for NES Sound Driver & Library\n")
				_T("\n")
				_T("  Usage : nsc [ -options ] [file(.mml)]\n")
				_T("\n")
				_T("  -A			Compile to assembly langage.\n")
				_T("  -N			Compile to NSF music format.\n")
				_T("  -E			Error/Warning messages out the stadard error.\n")
			//	_T("  -T			Disable to output the tick counting result.\n")
				_T("  -Od[+/-]		Optimize the NSF bank struct of the delta-PCM.\n")
				_T("  -Oo[+/-]		Optimize the object data.\n")
			//	_T("  -Os[+/-]		Optimize the sequence data.\n")
				_T("  -S			Enable outout the search pass result.\n")
				_T("  -L[file(.bin)]	Filename of the rom code for NSF.\n")
				_T("  -FA[file(.s  )]	Filename of the output assembly langage file.\n")
				_T("  -FN[file(.nsf)]	Filename of the output NSF music format.\n")
				_T("  -C[dir]		Search pass of the rom code for NSF.\n")
				_T("  -P[dir]		Search pass of the delta-pcm.\n")
				_T("  -I[dir]		Search pass of the include file.\n")
				_T("  -H			Print the this help.")	<<	endl;

	nsc_exit(EXIT_SUCCESS);

};
//==============================================================
//		�f�X�g���N�g
//--------------------------------------------------------------
//	������
//			�Ȃ�
//	���Ԓl
//			����
//==============================================================
OPSW::~OPSW(){


};
//==============================================================
//		�G���[����	�i�v���Z�X���I������j
//--------------------------------------------------------------
//	������
//			const	_CHAR	*stErrMsg	�G���[���b�Z�[�W
//	���Ԓl
//			����
//==============================================================
void OPSW::opError(const _CHAR *stErrMsg){

	if(fErr == true){
		_CERR << _T("�I�v�V�������s���ł��B�F") << stErrMsg << endl;
	} else {
		_COUT << _T("�I�v�V�������s���ł��B�F") << stErrMsg << endl;
	}
	nsc_exit(EXIT_FAILURE);

};
