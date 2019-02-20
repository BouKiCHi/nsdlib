;=======================================================================
;
;	NES Sound Driver & library (NSD.lib)	Library Functions
;
;-----------------------------------------------------------------------
;
;	Copyright (c) 2012 A.Watanabe (S.W.), All rights reserved.
;	For conditions of distribution and use, see copyright notice
;	  in "nsd.h" or "nsd.inc".
;
;=======================================================================

	.setcpu		"6502"

	.export		_nsd_init

	.import		_nsd_stop_bgm
	.import		_nsd_stop_se

	.import		_nsd_snd_init

	.import		nsd_work
	.importzp	nsd_work_zp

	.include	"nes.inc"
	.include	"nsddef.inc"
	.include	"macro.inc"


.code

;=======================================================================
;	void	__fastcall__	nsd_init(void );
;-----------------------------------------------------------------------
;<<Contents>>
;	Initialize the sound driver
;<<Input>>
;	nothing
;<<Output>>
;	nothing
;=======================================================================
.proc	_nsd_init: near

	jsr	_nsd_stop_bgm
	jsr	_nsd_stop_se
	jsr	_nsd_snd_init

	rts
.endproc
