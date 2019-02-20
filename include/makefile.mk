#==============================================================================
#	NES Sound Driver & library (NSD.lib)
#		Make file
#------------------------------------------------------------------------------
#
#	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
#	For conditions of distribution and use, see copyright notice
#	  in "nsd.h" or "nsd.inc".
#
#==============================================================================

########################################
#;	LOG files
########################################

LOG_FILE    =	comlog.txt		#�e�R�}���h�̕W���o�͐�ł��B
ERRLOG_FILE =	errlog.txt		#�e�R�}���h�̃G���[�o�͐�ł��B
BLDLOG_FILE =	buildlog.txt		#make.exe�̏o�͐�ł��B



########################################
#;	Define of build tools & option
########################################

#---------------
#;  MML Compiler (for Music & SE data)
MC      = $(TOOL_PASS)nsc
MCFLAGS = -a -e

#---------------
#;  C Compiler
CC      = $(TOOL_PASS)cl65
CFLAGS  = -t none -I$(INC_PASS) --listing $*.lst --list-bytes 255 -c -o

#---------------
#;  Assembler
AS      = $(TOOL_PASS)ca65
ASFLAGS = -t none -I$(INC_PASS) --listing $*.lst --list-bytes 255

#---------------
#;  Archiver
AR      = $(TOOL_PASS)ar65
ARFLAGS = a

#---------------
#;  Linker
LINKER  = $(TOOL_PASS)ld65
LDFLAGS = -C $(CFG_FILE) -L$(LIB_PASS) -v -m $(MAP_FILE) -vm -o

#---------------
#;  File delete
RM      = del

#---------------
#;  File copy
CP      = copy



#===============================
#; �ˑ��֌W�i�w�b�_�[�t�@�C�������A�ˑ��֌W�ɒǉ����邽�߂ɁA��`�j

%.o : %.s $(INC_FILES)			#;".inc"�t�@�C�����X�V���ꂽ�ꍇ�́A.s�́A�S���A�Z���u������B
	@echo [Assemble] : $< >>$(ERRLOG_FILE)
	$(AS) $(ASFLAGS) $< 1>>$(LOG_FILE) 2>>$(ERRLOG_FILE)


%.o : %.c $(H_FILES)			#;".h"�t�@�C�����X�V���ꂽ�ꍇ�́A.c�́A�S���R���p�C������B
	@echo [Compile] : $< >>$(ERRLOG_FILE)
	$(CC)  $(CFLAGS) $*.o $< 1>>$(LOG_FILE) 2>>$(ERRLOG_FILE)


%.s : %.mml $(DMC_FILES)		#;".dmc"�t�@�C�����X�V���ꂽ�ꍇ�́A.mml�́A�S���R���p�C������B
	@echo ======================================================== >>$(LOG_FILE)
	@echo [Compile] : $< >>$(LOG_FILE)
	@echo ======================================================== >>$(LOG_FILE)
	@echo [Compile] : $< >>$(ERRLOG_FILE)
	$(MC) $(MCFLAGS) $< 1>>$(LOG_FILE) 2>>$(ERRLOG_FILE)

%.s : %.bmp
	
