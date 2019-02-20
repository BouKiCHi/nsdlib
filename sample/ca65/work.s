
	;Zeropage works
	.exportzp	__r0
	.exportzp	__r1
	.exportzp	__r2
	.exportzp	__r3
	.exportzp	__r4
	.exportzp	__r5
	.exportzp	__r6
	.exportzp	__r7
	
	.exportzp	__n0
	.exportzp	__n1
	.exportzp	__n2
	.exportzp	__n3
	
	.exportzp	__i0
	.exportzp	__i1
	.exportzp	__i2
	.exportzp	__i3
	
	.exportzp	__Flag_2000
	.exportzp	__Flag_2001
		
	.exportzp	__cc
	.exportzp	__ss
	.exportzp	__mm
	.exportzp	__hh
	
	.exportzp	__PAD0
	.exportzp	__PAD1
	.exportzp	__PAD0_OLD
	.exportzp	__PAD1_OLD
	.exportzp	__PAD0_REL
	.exportzp	__PAD1_REL
	.exportzp	__PAD0_TRG
	.exportzp	__PAD1_TRG
	
	.exportzp	__MMC3_Pbank0
	.exportzp	__MMC3_Pbank1

	.exportzp	__MusBank
	.exportzp	__SeBank

	;Non Zeropage works
	.export		_NMI_CallBack
	.export		_IRQ_CallBack
	.export		_JMP_Address

;=======================================================================
;	Zeropage works
;-----------------------------------------------------------------------
.zeropage

__r0:		.byte	0		;= $00	;
__r1:		.byte	0		;= $01	;
__r2:		.byte	0		;= $02	;
__r3:		.byte	0		;= $03	;
__r4:		.byte	0		;= $04	;���֐��K��F�Ăяo���ꑤ�ŕێ�
__r5:		.byte	0		;= $05	;���֐��K��F�Ăяo���ꑤ�ŕێ�
__r6:		.byte	0		;= $06	;���֐��K��F�Ăяo���ꑤ�ŕێ�
__r7:		.byte	0		;= $07	;���֐��K��F�Ăяo���ꑤ�ŕێ�

;�ėp���W�X�^�iNMI���荞�ݗp�j
__n0:		.byte	0		;= $08	;
__n1:		.byte	0		;= $09	;
__n2:		.byte	0		;= $0A	;
__n3:		.byte	0		;= $0B	;

;�ėp���W�X�^�iIRQ���荞�ݗp�j
__i0:		.byte	0		;= $0C	;
__i1:		.byte	0		;= $0D	;
__i2:		.byte	0		;= $0E	;
__i3:		.byte	0		;= $0F	;

;-----------------------------------------------------------------------
;�V�X�e���֘A
__Flag_2000:	.byte	0		;= $10	;
__Flag_2001:	.byte	0		;= $11	;

;-----------------------------------------------------------------------
;�}�b�p�[�֘A
__MMC3_Pbank0:	.byte	0		;= $12	;
__MMC3_Pbank1:	.byte	0		;= $13	;

;-----------------------------------------------------------------------
;���Ԋ֘A
__cc:		.byte	0		;= $14	; 1/60�P�� (HEX)
__ss:		.byte	0		;= $15	; �b�yBCD�z
__mm:		.byte	0		;= $16	; ���yBCD�z
__hh:		.byte	0		;= $17	; �� (HEX)

;-----------------------------------------------------------------------
;�L�[�֘A
__PAD0:		.byte	0		;= $18	;
__PAD1:		.byte	0		;= $19	;
__PAD0_OLD:	.byte	0		;= $1A	;
__PAD1_OLD:	.byte	0		;= $1B	;
__PAD0_REL:	.byte	0		;= $1C	;
__PAD1_REL:	.byte	0		;= $1D	;
__PAD0_TRG:	.byte	0		;= $1E	;
__PAD1_TRG:	.byte	0		;= $1F	;

;-----------------------------------------------------------------------
;�T�E���h�֘A

__MusBank:	.byte	0		;= $20
__SeBank:	.byte	0		;= $21

;
;	$20 - $4F �c nsd.lib sound work
;


;=======================================================================
;	Non Zeropage works
;-----------------------------------------------------------------------
.bss

_NMI_CallBack:	.word	0		;= $0300
_IRQ_CallBack:	.word	0		;= $0302
_JMP_Address:	.word	0		;= $0304

