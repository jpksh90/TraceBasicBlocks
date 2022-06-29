	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 12, 0	sdk_version 12, 3
	.globl	_fib                            ; -- Begin function fib
	.p2align	2
_fib:                                   ; @fib
	.cfi_startproc
; %bb.0:
	sub	sp, sp, #64                     ; =64
	stp	x20, x19, [sp, #32]             ; 16-byte Folded Spill
	stp	x29, x30, [sp, #48]             ; 16-byte Folded Spill
	add	x29, sp, #48                    ; =48
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	mov	x19, x0
	bl	_incrementCounter
	stur	w19, [x29, #-20]
	str	wzr, [sp, #24]
	mov	w8, #1
	str	w8, [sp, #20]
	str	w8, [sp, #12]
LBB0_1:                                 ; =>This Inner Loop Header: Depth=1
	bl	_incrementCounter
	ldr	w8, [sp, #12]
	ldur	w9, [x29, #-20]
	cmp	w8, w9
	b.gt	LBB0_4
; %bb.2:                                ;   in Loop: Header=BB0_1 Depth=1
	bl	_incrementCounter
	ldr	w8, [sp, #24]
	ldr	w9, [sp, #20]
	add	w8, w8, w9
	str	w8, [sp, #16]
	ldr	w8, [sp, #20]
	str	w8, [sp, #24]
	ldr	w8, [sp, #16]
	str	w8, [sp, #20]
; %bb.3:                                ;   in Loop: Header=BB0_1 Depth=1
	bl	_incrementCounter
	ldr	w8, [sp, #12]
	add	w8, w8, #1                      ; =1
	str	w8, [sp, #12]
	b	LBB0_1
LBB0_4:
	bl	_incrementCounter
	ldr	w0, [sp, #16]
	ldp	x29, x30, [sp, #48]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp, #32]             ; 16-byte Folded Reload
	add	sp, sp, #64                     ; =64
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	_main                           ; -- Begin function main
	.p2align	2
_main:                                  ; @main
	.cfi_startproc
; %bb.0:
	sub	sp, sp, #32                     ; =32
	stp	x29, x30, [sp, #16]             ; 16-byte Folded Spill
	add	x29, sp, #16                    ; =16
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	bl	_incrementCounter
	stur	wzr, [x29, #-4]
	add	x8, sp, #8                      ; =8
	str	x8, [sp]
	adrp	x0, l_.str@PAGE
	add	x0, x0, l_.str@PAGEOFF
	bl	_scanf
	ldr	w8, [sp, #8]
	tbz	w8, #31, LBB1_2
; %bb.1:
	bl	_incrementCounter
	adrp	x0, l_.str.1@PAGE
	add	x0, x0, l_.str.1@PAGEOFF
	bl	_printf
	mov	w8, #-1
	stur	w8, [x29, #-4]
	b	LBB1_3
LBB1_2:
	bl	_incrementCounter
	ldr	w0, [sp, #8]
	bl	_fib
                                        ; kill: def $w0 killed $w0 def $x0
	str	x0, [sp]
	adrp	x0, l_.str@PAGE
	add	x0, x0, l_.str@PAGEOFF
	bl	_printf
	stur	wzr, [x29, #-4]
LBB1_3:
	bl	_writeBBInfo
	bl	_incrementCounter
	ldur	w0, [x29, #-4]
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	add	sp, sp, #32                     ; =32
	ret
	.cfi_endproc
                                        ; -- End function
	.section	__TEXT,__cstring,cstring_literals
l_.str:                                 ; @.str
	.asciz	"%d"

l_.str.1:                               ; @.str.1
	.asciz	"n is negative"

.subsections_via_symbols
