;;
;; Upcaste Performance Libraries
;; Copyright (C) 2012-2013 Jesse W. Towner
;;
;; Permission is hereby granted, free of charge, to any person obtaining
;; a copy of this software and associated documentation files (the
;; "Software"), to deal in the Software without restriction, including
;; without limitation the rights to use, copy, modify, merge, publish,
;; distribute, sublicense, and/or sell copies of the Software, and to
;; permit persons to whom the Software is furnished to do so, subject to
;; the following conditions:
;;
;; The above copyright notice and this permission notice shall be
;; included in all copies or substantial portions of the Software.
;;
;; THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
;; EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
;; MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
;; IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
;; CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
;; TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
;; SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
;;

;;
;; uint64_t memhash(void const* p, size_t n, uint64_t seed);
;;
;; Based off of the fast byteswap hash function by Steven Fuerst.
;; http://locklessinc.com/articles/fast_hash/ 
;;

.CODE

ALIGN 16
factor128 OWORD 0d6c573e9c613993d5a379ab38dc5a46bh

ALIGN 16
PUBLIC memhash
memhash PROC

    movd        xmm0, rdx
    movd        xmm1, r8
    movdqa      xmm2, XMMWORD PTR [factor128]
    cmp         rdx, 32
    jb          SHORT _test_remaining
    mov         rax, rdx
    shr         rax, 5
    test        cl, 15
    jne         _unaligned_loop

    ;
    ; Aligned main-loop, process 32-byte aligned chunks at a time
    ; using 16-byte signed multiplication and unpack to mix.
    ;

ALIGN 8
_aligned_loop:
    paddq       xmm0, XMMWORD PTR [rcx]
    paddq       xmm1, XMMWORD PTR [rcx + 16]
    pmullw      xmm0, xmm2
    pmullw      xmm1, xmm2
    add         rcx, 32
    movdqa      xmm3, xmm0
    punpckhbw   xmm0, xmm1
    punpcklbw   xmm1, xmm3
    dec         rax
    jne         SHORT _aligned_loop
    pxor        xmm0, xmm1

    ;
    ; Check for remaining chunk, if not extract 128 bits state and
    ; proceed to final mix.
    ;

_test_remaining:
    test        dl, 31
    jne         SHORT _remaining
    movhlps     xmm1, xmm0
    movd        rax, xmm0
    movd        r10, xmm1

    ;
    ; Use 3x 128bit multiply and xorshift for final mix.
    ;

_final_mix:
    mov         r9, 02bb6863566c4e761h
    mov         r8, 01591aefa5e7e5a17h
    imul        r10, r9
    mul         r8
    add         r10, rdx
    xor         rax, r10
    imul        r10, r9
    mul         r8
    add         r10, rdx
    xor         rax, r10
    imul        r10, r9
    mul         r8
    add         r10, rdx
    xor         rax, r10
    ret

    ;
    ; Process remaining chunk in piece-wise manner, up to 31-bytes in size.
    ;

_remaining:
    test        dl, 16
    je          SHORT _remaining_qword
    movdqu      xmm3, XMMWORD PTR [rcx]
    pxor        xmm0, xmm3
    add         rcx, 16
    pmullw      xmm0, xmm2
_remaining_qword:
    xor         r11, r11
    movhlps     xmm1, xmm0
    movd        rax, xmm0
    movd        r10, xmm1
    test        dl, 8
    je          SHORT _remaining_dword
    xor         rax, QWORD PTR [rcx]
    add         rcx, 8
_remaining_dword:
    test        dl, 4
    je          SHORT _remaining_word
    mov         r11d, DWORD PTR [rcx]
    add         rcx, 4
_remaining_word:
    test        dl, 2
    je          SHORT _remaining_byte
    movzx       r8, WORD PTR [rcx]
    shl         r11, 16
    add         rcx, 2
    add         r11, r8
_remaining_byte:
    test        dl, 1
    je          SHORT _remaining_done
    movzx       r8, BYTE PTR [rcx]
    shl         r11, 8
    add         r11, r8
_remaining_done:
    xor         r10, r11
    jmp         _final_mix

    ;
    ; Unaligned main-loop, process 32-byte unaligned chunks at a time
    ; using 16-byte signed multiplication and unpack to mix.
    ;

ALIGN 16
_unaligned_loop:
    movdqu      xmm3, XMMWORD PTR [rcx]
    movdqu      xmm4, XMMWORD PTR [rcx + 16]
    paddq       xmm0, xmm3
    paddq       xmm1, xmm4
    pmullw      xmm0, xmm2
    pmullw      xmm1, xmm2
    add         rcx, 32
    movdqa      xmm3, xmm0
    punpckhbw   xmm0, xmm1
    punpcklbw   xmm1, xmm3
    dec         rax
    jne         SHORT _unaligned_loop
    pxor        xmm0, xmm1
    jmp         _test_remaining

memhash ENDP

END
