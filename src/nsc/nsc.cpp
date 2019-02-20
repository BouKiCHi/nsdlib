/***************************************************************************

				NES Sound Driver & Library	(NSD.lib)
					Music Macro Language (MML) Compiler

****************************************************************************

   Copyright (c) 2012 A.Watanabe (S.W.)
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

     1. Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.

     2. Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER
   OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

***************************************************************************/

// nsc.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "StdAfx.h"

/****************************************************************/
/*					�O���[�o��									*/
/****************************************************************/
		OPSW*			cOptionSW = NULL;	//�ǂ�����ł��A�N�Z�X����B

//==============================================================
//		�G���[
//--------------------------------------------------------------
//	������
//		int			�G���[�R�[�h
//	���Ԓl
//					����
//==============================================================
void nsc_exit(int no)
{
	throw no;
}
//==============================================================
//		���C���֐�
//--------------------------------------------------------------
//	������
//		int		argc	�R�}���h���C������	
//		_TCHAR*	argv[]	�R�}���h���C������
//	���Ԓl
//		int			�G���[�R�[�h
//==============================================================
int	main(int argc, char* argv[])
{
	unsigned	int	i;
				int	iResult	= EXIT_SUCCESS;
	MMLfile			*cMML	= NULL;
    MusicFile		*cSND	= NULL;

	try {

#ifdef	_WIN32
		locale::global(std::locale(""));
#else
//		setlocale(LC_ALL, "ja_JP.UTF-8");
		setlocale(LC_ALL, "");
#endif


//		locale::global(std::locale(""));	//g++ ���ƁA�����^�C�� �G���[�ɂȂ�B


		//==================================
		_COUT	<<	_T("MML Compiler for NES Sound Driver & Library (NSD.Lib)\n")
					_T("    Version 1.28\n")
					_T("        Copyright (c) 2012-2018 S.W.\n")	<<	endl;


		//==================================
		//�N���X�̍쐬
		cOptionSW	= new OPSW(argc,argv);							//�I�v�V��������
		_COUT << _T("------------------------------------------------------------") << endl;
		_COUT << _T("*Object creating process") << endl;

		cMML = new MMLfile(cOptionSW->strMMLname.c_str());
		cSND = new MusicFile(cMML, cOptionSW->strCodeName);

		_COUT << endl;



		//==================================
		//Optimize & Tick Count
		_COUT << _T("------------------------------------------------------------") << endl;
		_COUT << _T("*Optimize & Tick counting process") << endl;

		cSND->TickCount();

		_COUT << endl;



		//==================================
		//�A�h���X�̉���
		_COUT << _T("------------------------------------------------------------") << endl;
		_COUT << _T("*Address settlement process") << endl;

		//�A�h���X�̌v�Z �� �T�C�Y�̏o��
		i = cSND->SetOffset(0);
		cout << "  Music Size = " << setfill(' ')  << setw(5) << i << " [Byte]" << endl;

		i = cSND->SetDPCMOffset(i);
		cout << "  DPCM Size  = " << setfill(' ')  << setw(5) << i << " [Byte]" << endl;

		//�A�h���X�������ɂ��I�y�R�[�h�̃A�h���X����
		cSND->Fix_Address();

		_COUT << endl;



		//==================================
		//�ۑ�
		if((cOptionSW->saveNSF == true) || ((cOptionSW->saveNSF == false)&&(cOptionSW->saveASM == false))){
			cSND->saveNSF(cOptionSW->strNSFname.c_str());
		}

		if(cOptionSW->saveASM == true){
			cSND->saveASM(cOptionSW->strASMname.c_str());
		}

		_COUT << endl;



		//==================================

	} catch (int no) {
		if (no != EXIT_SUCCESS){
            _COUT	<<	_T("Error!:") << no << endl;
			iResult	= EXIT_FAILURE;
		}
	}

	//==================================
	//�N���X�̍폜
	if (cSND)
		delete	cSND;
	if (cMML)
		delete	cMML;
	if (cOptionSW)
		delete	cOptionSW;

	return(iResult);
}
