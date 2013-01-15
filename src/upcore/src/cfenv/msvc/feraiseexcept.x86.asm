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
INCLUDE fenv.inc

;;
;; int feraiseexcept(int excepts);
;;
;; Raise each of the specified exception flags one at a time in sequence,
;; placing the address for the next instruction after the triggering one
;; into EAX, allowing exception/signal handlers to skip over the offending
;; instruction (this is mostly for debugging purposes).
;;

.CODE
ALIGN 4
PUBLIC _feraiseexcept
_feraiseexcept   PROC

    ; raise an invalid operation using 0.0 / 0.0
    mov         ecx, DWORD PTR [esp + 4]
    test        ecx, FE_INVALID
    jz          _raise_denormal
    fnstenv     [esp - SIZEOF fenv]
    or          [esp - SIZEOF fenv].fenv.status_word, FE_INVALID
    fldenv      [esp - SIZEOF fenv]
    lea         eax, _raise_denormal
    fwait

    ; raise a denormal exception
_raise_denormal:
    test        ecx, FE_DENORMAL
    jz          _raise_divbyzero
    fnstenv     [esp - SIZEOF fenv]
    or          [esp - SIZEOF fenv].fenv.status_word, FE_DENORMAL
    fldenv      [esp - SIZEOF fenv]
    lea         eax, _raise_divbyzero
    fwait

    ; raise a division by zero using 1.0 / 0.0
_raise_divbyzero:
    test        ecx, FE_DIVBYZERO
    jz          _raise_overflow
    fnstenv     [esp - SIZEOF fenv]
    or          [esp - SIZEOF fenv].fenv.status_word, FE_DIVBYZERO
    fldenv      [esp - SIZEOF fenv]
    lea         eax, _raise_overflow
    fwait

    ; raise an overflow exception
_raise_overflow:
    test        ecx, FE_OVERFLOW
    jz          _raise_underflow
    fnstenv     [esp - SIZEOF fenv]
    or          [esp - SIZEOF fenv].fenv.status_word, FE_OVERFLOW
    fldenv      [esp - SIZEOF fenv]
    lea         eax, _raise_underflow
    fwait
    
    ; raise an underflow exception
_raise_underflow:
    test        ecx, FE_UNDERFLOW
    jz          _raise_inexact
    fnstenv     [esp - SIZEOF fenv]
    or          [esp - SIZEOF fenv].fenv.status_word, FE_UNDERFLOW
    fldenv      [esp - SIZEOF fenv]
    lea         eax, _raise_inexact
    fwait

    ; raise an inexact exception
_raise_inexact:
    test        ecx, FE_INEXACT
    jz          _done
    fnstenv     [esp - SIZEOF fenv]
    or          [esp - SIZEOF fenv].fenv.status_word, FE_INEXACT
    fldenv      [esp - SIZEOF fenv]
    lea         eax, _done
    fwait

_done:
    xor         eax, eax
    ret

_feraiseexcept   ENDP

END
