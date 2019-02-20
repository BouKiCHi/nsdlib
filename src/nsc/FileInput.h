/*******************************************************************************

			NES Sound Driver & Library	(NSD.lib)	MML Compiler

	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
	 For conditions of distribution and use, see copyright notice in "nsc.cpp".

*******************************************************************************/

#pragma once

class FileInput :
	public ifstream
{
protected:
//�����o�[�ϐ�
				string		strFilename;
	unsigned	int			iLine;		//���݂̃��C��
	unsigned	char		readData;

//�����o�[�֐�
public:
					FileInput(void);
					~FileInput(void);
			void	fileopen(const char*	_strFileName);
			void	fileopen(const char*	_strFileName,SearchPass* _pass);
			void	StreamPointerAdd(long iSize);
			void	StreamPointerMove(long iSize);
			void	Back(void);
			string*	GetFilename(void){return(&strFilename);};
unsigned	char	cRead();
unsigned	int		GetSize();
unsigned	int		GetLine(void){return(iLine);};
			void	SetLine(unsigned int i){iLine = i;};

};
