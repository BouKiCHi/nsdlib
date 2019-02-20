
	.export		NMI_main

	.include	"sample.inc"


.segment	"LOWCODE"
;===============================================================
;			nmi_main(void);
;---------------------------------------------------------------
;	Summary :		NMI main Routine
;	Arguments :		None
;	Return :		None
;	Modifies :		None
;	Useage :		Please write this address to interrupt vector
;---------------------------------------------------------------
;	�s�����t
;	�ENMI���荞�݂́APPU�֘A�̑�����ɂ��
;	�@PPU�ւ̃A�N�Z�X�́A���������A�Ғ��ɂ����ł��Ȃ����߁B
;	�E�L�[�̓ǂݍ��݂��ANMI���荞�݂ōs���B
;	�@�L�[�ǂݍ��ݒ��ɉ����h���C�o�[�����荞�ނƁA�L�[�ǂݍ��݂Ɏ��s���邽�߁B
;===============================================================
.proc	NMI_main

;---------------------------------------
;register push
;---------------------------------------
	pha
	tya
	pha
	txa
	pha

;---------------------------------------
;call the callback routine
;---------------------------------------

	;---------------
	;VRAM�X�V�R�[�h�̌ďo��
;	lda	_NMI_CallBack + 1
;	beq	CallBack_Ret
;	jsr	jmp_NMI_CallBack

CallBack_Ret:

;---------------------------------------
; Call sound driver main routine
;---------------------------------------

	;---------------
	;�T�E���h�h���C�o�ďo��

	jsr	_nsd_main

;---------------------------------------
; Read joy-pad
;---------------------------------------

Key:
	;---------------
	;�O��̃L�[���R�s�[
	lda	__PAD0
	sta	__PAD0_OLD
	lda	__PAD1
	sta	__PAD1_OLD

	;---------------
	;�ϐ�������
	ldx	#$01
	stx	__PAD0
	stx	__PAD1
	stx	__n0
	stx	__n1
	stx	__n2
	stx	__n3

	;---------------
	;�P��ړǂݍ���
	stx	APU_PAD1	;4021 P/S_CONTROL�[�q <= High (PARALLEL operation)
	dex
	stx	APU_PAD1	;4021 P/S_CONTROL�[�q <= Low  (SERIAL   operation)

@loop1:
	lda	APU_PAD1
	lsr	a
	rol	__n0		;PAD1
	lsr	a
	rol	__n2		;PAD3�i�g��PAD�j

	lda	APU_PAD2
	lsr	a
	rol	__n1		;PAD2
	bcc	@loop1

	;---------------
	;�Q��ړǂݍ���	�i����PCM �� �`���^�����O�ɂ��m�C�Y�����̂��߁j
	inx
	stx	APU_PAD1	;4021 P/S_CONTROL�[�q <= High (PARALLEL operation)
	dex
	stx	APU_PAD1	;4021 P/S_CONTROL�[�q <= Low  (SERIAL   operation)

@loop2:
	lda	APU_PAD1
	lsr	a
	rol	__PAD0		;PAD1
	lsr	a
	rol	__n3		;PAD3�i�g��PAD�j

	lda	APU_PAD2
	lsr	a
	rol	__PAD1		;PAD2
	bcc	@loop2

	;---------------
	;��PCM �� �`���^�����O�̃m�C�Y���� �� ���オ��E�����茟�o

	;-------
	;PAD3�i�g��PAD�j
	lda	__n3
	and	__n2		;�m�C�Y�����i�P��ځA�Q��ڋ���`H'�̏ꍇ�ɁA`H'�j
	sta	__n2

	;-------
	;PAD1 & PAD3
	lda	__n0
	and	__PAD0		;�m�C�Y�����i�P��ځA�Q��ڋ���`H'�̏ꍇ�ɁA`H'�j
	ora	__n2		;PAD3�i�g��PAD�j�̓ǂݍ��݂��APAD1�Ƀ}�[�W
	sta	__PAD0

	eor	__PAD0_OLD	;�G�b�W���o
	tax
	and	__PAD0
	sta	__PAD0_TRG	;���オ��
	txa
	and	__PAD0_OLD
	sta	__PAD0_REL	;������

	;-------
	;PAD2
	lda	__n1
	and	__PAD1		;�m�C�Y�����i�P��ځA�Q��ڋ���`H'�̏ꍇ�ɁA`H'�j
	sta	__PAD1

	eor	__PAD1_OLD	;�G�b�W���o
	tax
	and	__PAD1
	sta	__PAD1_TRG	;���オ��
	txa
	and	__PAD1_OLD
	sta	__PAD1_REL	;������



;---------------------------------------
; Count-up
;---------------------------------------
Count:
@cc:
	inc	__cc
	lda	__cc
	cmp	#60
	bne	@exit
	lda	#0
	sta	__cc

@ss:
	ldx	__ss
	cpx	#$59
	beq	@ss_1
	inx
	stx	__ss
	txa
	and	#$0F
	cmp	#$0A
	bne	@exit
	txa
	add	#6
	sta	__ss
	bne	@exit
@ss_1:
	lda	#0
	sta	__ss

@mm:
	ldx	__mm
	cpx	#$59
	beq	@mm_1
	inx
	stx	__mm
	txa
	and	#$0F
	cmp	#$0A
	bne	@exit
	txa
	add	#6
	sta	__mm
	bne	@exit
@mm_1:
	lda	#0
	sta	__mm

@hh:
	inc	__hh

@exit:

;---------------------------------------
;register pop
;---------------------------------------
	pla
	tax
	pla
	tay
	pla

	rti
.endproc

.proc	jmp_NMI_CallBack
	jmp	(_NMI_CallBack)
.endproc

