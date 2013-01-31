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

.686P
.XMM
.MODEL FLAT

;;
;; uint64_t memhash(void const* p, size_t n, uint64_t seed);
;;

.CODE

ALIGN 16
factor128   OWORD 0d6c573e9c613993d5a379ab38dc5a46bh
mix1        OWORD 01591aefa5e7e5a171591aefa5e7e5a17h
mix2        OWORD 02bb6863566c4e7612bb6863566c4e761h

ALIGN 16
PUBLIC _memhash
_memhash PROC

    mov         ecx, DWORD PTR [esp + 4]        ; key
    mov         edx, DWORD PTR [esp + 8]        ; n
    movq        xmm2, QWORD PTR [esp + 12]      ; seed
    push        edi
    movd        xmm1, edx
    push        esi
    movdqa      xmm3, XMMWORD PTR [factor128]
    movdqa      xmm6, XMMWORD PTR [mix1]
    movdqa      xmm7, XMMWORD PTR [mix2]
    cmp         edx, 32
    jb          SHORT _test_remaining
    mov         eax, edx
    shr         eax, 5
    test        cl, 15
    jne         _unaligned_loop

    ;
    ; Aligned main-loop, process 32-byte aligned chunks at a time
    ;

ALIGN 8
_aligned_loop:
    paddq       xmm1, XMMWORD PTR [ecx]
    paddq       xmm2, XMMWORD PTR [ecx + 16]
    pmullw      xmm1, xmm3
    pmullw      xmm2, xmm3
    add         ecx, 32
    movdqa      xmm0, xmm1
    punpckhbw   xmm1, xmm2
    punpcklbw   xmm2, xmm0
    dec         eax
    jne         SHORT _aligned_loop
    pxor        xmm1, xmm2

    ;
    ; Check for remaining chunk, if not extract 128 bits state and proceed to final mix
    ;

_test_remaining:
    test        dl, 31
    jne         SHORT _remaining
    movhlps     xmm2, xmm1
    jmp         SHORT _final_mix

    ;
    ; Hash remaining chunk, up to 31-bytes in size
    ;

_remaining:
    test        dl, 16
    je          SHORT _remaining_qword
    movdqu      xmm0, XMMWORD PTR [ecx]
    add         ecx, 16
    pxor        xmm1, xmm0
    pmullw      xmm1, xmm3
_remaining_qword:
    pxor        xmm3, xmm3
    movhlps     xmm2, xmm1
    test        dl, 8
    je          SHORT _remaining_dword
    movq        xmm0, QWORD PTR [ecx]
    add         ecx, 8
    pxor        xmm1, xmm0
_remaining_dword:
    test        dl, 4
    je          SHORT _remaining_word
    movd        xmm3, DWORD PTR [ecx]
    add         ecx, 4
_remaining_word:
    test        dl, 2
    je          SHORT _remaining_byte
    movzx       eax, WORD PTR [ecx]
    psllq       xmm3, 16
    movd        xmm0, eax
    add         ecx, 2
    paddq       xmm3, xmm0
_remaining_byte:
    test        dl, 1
    je          SHORT _remaining_done
    movzx       eax, BYTE PTR [ecx]
    psllq       xmm3, 8
    movd        xmm0, eax
    paddq       xmm3, xmm0
_remaining_done:
    pxor        xmm2, xmm3

    ;
    ; Use 3x 128bit multiply and xorshift for final mix
    ;

_final_mix:

    ;
    ; Iteration #0
    ;

    pshufd      xmm3, xmm6, 010110001b
    punpckldq   xmm1, xmm1
    pmuludq     xmm3, xmm1                          ; [H|G|F|E] = [hash1.high * mix1.high | hash1.low * mix1.high]
    punpckldq   xmm2, xmm2
    pmuludq     xmm1, xmm6                          ; [D|C|B|A] = [hash1.high * mix1.low | hash1.low * mix1.low]
    movd        ecx, xmm3
    pshufd      xmm1, xmm1, 011100001b
    pshufd      xmm3, xmm3, 011100001b
    movd        edx, xmm1
    movd        esi, xmm3
    pshufd      xmm1, xmm1, 011010010b
    pshufd      xmm3, xmm3, 001110010b
    movd        eax, xmm1
    xor         edi, edi
    pshufd      xmm1, xmm1, 000100111b
    add         edx, eax                            ; X = B + C
    movd        eax, xmm1
    adc         eax, 0                              ; Y = D + carry
    adc         edi, 0                              ; Z = carry
    add         edx, ecx                            ; X = X + E
    punpckhdq   xmm1, xmm1                          ; [0|0|0|A]
    movd        xmm0, edx                           ; [0|0|0|X]
    adc         eax, esi                            ; Y = Y + F + carry
    movd        edx, xmm3
    punpckhdq   xmm3, xmm3
    adc         edi, 0                              ; Z = Z + carry
    movd        esi, xmm3
    add         eax, edx                            ; Y = Y + G
    pshufd      xmm3, xmm7, 010101111b
    adc         edi, esi                            ; Z = Z + H + carry
    movd        xmm5, eax                           ; [0|0|0|Y]
    movd        xmm4, edi                           ; [0|0|0|Z]
    punpckldq   xmm1, xmm0                          ; [0|0|X|A] = [0 | lower 64-bits of hash1 * mix1]
    punpckldq   xmm5, xmm4                          ; [0|0|Z|Y] = [0 | upper 64-bits of hash1 * mix1]
    pmuludq     xmm3, xmm2                          ; [h|g|f|e] = [hash2.high * mix2.low | hash2.low * mix2.high]
    pxor        xmm0, xmm0
    pmuludq     xmm2, xmm7                          ; [d|c|b|a] = [hash2.high * mix2.high | hash2.low * mix2.low]
    punpckldq   xmm0, xmm3                          ; [e|0|e|0]
    pxor        xmm4, xmm4
    paddd       xmm2, xmm0                          ; [d+e|c|b+e|a]
    punpckhdq   xmm4, xmm3                          ; [g|0|g|0]
    paddd       xmm2, xmm4                          ; [d+e+g|c|b+e+g|a] = [garbage | lower 64-bits of hash2 * mix2]
    paddq       xmm2, xmm5                          ; hash2 = (hash2 * mix2) + upper_64bits(hash1 * mix1)
    pxor        xmm1, xmm2                          ; hash1 = hash2 ^ lower_64bits(hash1 * mix1)

    ;
    ; Iteration #1
    ;

    pshufd      xmm3, xmm6, 010110001b
    punpckldq   xmm1, xmm1
    pmuludq     xmm3, xmm1                          ; [H|G|F|E] = [hash1.high * mix1.high | hash1.low * mix1.high]
    punpckldq   xmm2, xmm2
    pmuludq     xmm1, xmm6                          ; [D|C|B|A] = [hash1.high * mix1.low | hash1.low * mix1.low]
    movd        ecx, xmm3
    pshufd      xmm1, xmm1, 011100001b
    pshufd      xmm3, xmm3, 011100001b
    movd        edx, xmm1
    movd        esi, xmm3
    pshufd      xmm1, xmm1, 011010010b
    pshufd      xmm3, xmm3, 001110010b
    movd        eax, xmm1
    xor         edi, edi
    pshufd      xmm1, xmm1, 000100111b
    add         edx, eax                            ; X = B + C
    movd        eax, xmm1
    adc         eax, 0                              ; Y = D + carry
    adc         edi, 0                              ; Z = carry
    add         edx, ecx                            ; X = X + E
    punpckhdq   xmm1, xmm1                          ; [0|0|0|A]
    movd        xmm0, edx                           ; [0|0|0|X]
    adc         eax, esi                            ; Y = Y + F + carry
    movd        edx, xmm3
    punpckhdq   xmm3, xmm3
    adc         edi, 0                              ; Z = Z + carry
    movd        esi, xmm3
    add         eax, edx                            ; Y = Y + G
    pshufd      xmm3, xmm7, 010101111b
    adc         edi, esi                            ; Z = Z + H + carry
    movd        xmm5, eax                           ; [0|0|0|Y]
    movd        xmm4, edi                           ; [0|0|0|Z]
    punpckldq   xmm1, xmm0                          ; [0|0|X|A] = [0 | lower 64-bits of hash1 * mix1]
    punpckldq   xmm5, xmm4                          ; [0|0|Z|Y] = [0 | upper 64-bits of hash1 * mix1]
    pmuludq     xmm3, xmm2                          ; [h|g|f|e] = [hash2.high * mix2.low | hash2.low * mix2.high]
    pxor        xmm0, xmm0
    pmuludq     xmm2, xmm7                          ; [d|c|b|a] = [hash2.high * mix2.high | hash2.low * mix2.low]
    punpckldq   xmm0, xmm3                          ; [e|0|e|0]
    pxor        xmm4, xmm4
    paddd       xmm2, xmm0                          ; [d+e|c|b+e|a]
    punpckhdq   xmm4, xmm3                          ; [g|0|g|0]
    paddd       xmm2, xmm4                          ; [d+e+g|c|b+e+g|a] = [garbage | lower 64-bits of hash2 * mix2]
    paddq       xmm2, xmm5                          ; hash2 = (hash2 * mix2) + upper_64bits(hash1 * mix1)
    pxor        xmm1, xmm2                          ; hash1 = hash2 ^ lower_64bits(hash1 * mix1)

    ;
    ; Iteration #2
    ;

    pshufd      xmm3, xmm6, 010110001b
    punpckldq   xmm1, xmm1
    pmuludq     xmm3, xmm1                          ; [H|G|F|E] = [hash1.high * mix1.high | hash1.low * mix1.high]
    punpckldq   xmm2, xmm2
    pmuludq     xmm1, xmm6                          ; [D|C|B|A] = [hash1.high * mix1.low | hash1.low * mix1.low]
    movd        ecx, xmm3
    pshufd      xmm1, xmm1, 011100001b
    pshufd      xmm3, xmm3, 011100001b
    movd        edx, xmm1
    movd        esi, xmm3
    pshufd      xmm1, xmm1, 011010010b
    pshufd      xmm3, xmm3, 001110010b
    movd        eax, xmm1
    xor         edi, edi
    pshufd      xmm1, xmm1, 000100111b
    add         edx, eax                            ; X = B + C
    movd        eax, xmm1
    adc         eax, 0                              ; Y = D + carry
    adc         edi, 0                              ; Z = carry
    add         edx, ecx                            ; X = X + E
    punpckhdq   xmm1, xmm1                          ; [0|0|0|A]
    movd        xmm0, edx                           ; [0|0|0|X]
    adc         eax, esi                            ; Y = Y + F + carry
    movd        edx, xmm3
    punpckhdq   xmm3, xmm3
    adc         edi, 0                              ; Z = Z + carry
    movd        esi, xmm3
    add         eax, edx                            ; Y = Y + G
    pshufd      xmm3, xmm7, 010101111b
    adc         edi, esi                            ; Z = Z + H + carry
    movd        xmm5, eax                           ; [0|0|0|Y]
    movd        xmm4, edi                           ; [0|0|0|Z]
    punpckldq   xmm1, xmm0                          ; [0|0|X|A] = [0 | lower 64-bits of hash1 * mix1]
    punpckldq   xmm5, xmm4                          ; [0|0|Z|Y] = [0 | upper 64-bits of hash1 * mix1]
    pmuludq     xmm3, xmm2                          ; [h|g|f|e] = [hash2.high * mix2.low | hash2.low * mix2.high]
    pxor        xmm0, xmm0
    pmuludq     xmm2, xmm7                          ; [d|c|b|a] = [hash2.high * mix2.high | hash2.low * mix2.low]
    punpckldq   xmm0, xmm3                          ; [e|0|e|0]
    pxor        xmm4, xmm4
    paddd       xmm2, xmm0                          ; [d+e|c|b+e|a]
    punpckhdq   xmm4, xmm3                          ; [g|0|g|0]
    paddd       xmm2, xmm4                          ; [d+e+g|c|b+e+g|a] = [garbage | lower 64-bits of hash2 * mix2]
    paddq       xmm2, xmm5                          ; hash2 = (hash2 * mix2) + upper_64bits(hash1 * mix1)
    pxor        xmm1, xmm2                          ; hash1 = hash2 ^ lower_64bits(hash1 * mix1)

    ;
    ; Place 64-bit result in EDX:EAX and return.
    ;

    pop         esi
    movd        eax, xmm1
    pshufd      xmm1, xmm1, 011100001b
    pop         edi
    movd        edx, xmm1
    ret

    ;
    ; Unaligned main-loop, process 32-byte unaligned chunks at a time
    ;

ALIGN 16
_unaligned_loop:
    movdqu      xmm0, XMMWORD PTR [ecx]
    add         ecx, 32
    paddq       xmm1, xmm0
    movdqu      xmm0, XMMWORD PTR [ecx - 16]
    pmullw      xmm1, xmm3
    paddq       xmm2, xmm0
    pmullw      xmm2, xmm3
    movdqa      xmm0, xmm1
    punpckhbw   xmm1, xmm2
    punpcklbw   xmm2, xmm0
    dec         eax
    jne         SHORT _unaligned_loop
    pxor        xmm1, xmm2
    jmp         _test_remaining

_memhash ENDP

END
