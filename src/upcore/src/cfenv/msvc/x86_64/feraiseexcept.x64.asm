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

INCLUDE fenv.inc

;;
;; int feraiseexcept(int excepts);
;;
;; Raise each of the specified exception flags one at a time in sequence,
;; placing the address for the next instruction after the triggering one
;; into RAX, allowing exception/signal handlers to skip over the offending
;; instruction (this is mostly for debugging purposes).
;;
;; Input:
;;          ecx = excepts
;; Output:
;;          rax = 0
;;

.DATA
ALIGN 4
iee754_binary32_one DWORD 3F800000h

.CODE
ALIGN 8
PUBLIC feraiseexcept
feraiseexcept   PROC

    ; raise an invalid operation using 0.0 / 0.0
    test        ecx, FE_INVALID
    jz          _raise_denormal
    xorps       xmm0, xmm0
    lea         rax, _raise_denormal
    divss       xmm0, xmm0

    ; raise a denormal exception
_raise_denormal:
    test        ecx, FE_DENORMAL
    jz          _raise_divbyzero
    fnstenv     [rsp - SIZEOF fenv]
    or          [rsp - SIZEOF fenv].fenv.status_word, FE_DENORMAL
    fldenv      [rsp - SIZEOF fenv]
    lea         rax, _raise_divbyzero
    fwait

    ; raise a division by zero using 1.0 / 0.0
_raise_divbyzero:
    test        ecx, FE_DIVBYZERO
    jz          _raise_overflow
    movss       xmm1, iee754_binary32_one
    xorps       xmm0, xmm0
    lea         rax, _raise_overflow
    divss       xmm1, xmm0

    ; raise an overflow exception
_raise_overflow:
    test        ecx, FE_OVERFLOW
    jz          _raise_underflow
    fnstenv     [rsp - SIZEOF fenv]
    or          [rsp - SIZEOF fenv].fenv.status_word, FE_OVERFLOW
    fldenv      [rsp - SIZEOF fenv]
    lea         rax, _raise_underflow
    fwait
    
    ; raise an underflow exception
_raise_underflow:
    test        ecx, FE_UNDERFLOW
    jz          _raise_inexact
    fnstenv     [rsp - SIZEOF fenv]
    or          [rsp - SIZEOF fenv].fenv.status_word, FE_UNDERFLOW
    fldenv      [rsp - SIZEOF fenv]
    lea         rax, _raise_inexact
    fwait

    ; raise an inexact exception
_raise_inexact:
    test        ecx, FE_INEXACT
    jz          _done
    fnstenv     [rsp - SIZEOF fenv]
    or          [rsp - SIZEOF fenv].fenv.status_word, FE_INEXACT
    fldenv      [rsp - SIZEOF fenv]
    lea         rax, _done
    fwait

_done:
    xor         rax, rax
    ret

feraiseexcept   ENDP

END
