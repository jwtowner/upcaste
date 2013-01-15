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
;; int fegetexceptflag(fexcept_t* flagp, int excepts);
;;
;; Get current exception status flags for both the x87 FPU and the
;; SSE control/status register.
;;

.CODE
ALIGN 4
PUBLIC _fegetexceptflag
_fegetexceptflag PROC

    mov         ecx, DWORD PTR [esp+4]
    test        ecx, ecx
    jz          _invalid_flagp
    fnstsw      ax
    mov         edx, DWORD PTR [esp+8]
    stmxcsr     DWORD PTR [esp-4]
    and         edx, FE_ALL_EXCEPT
    or          eax, DWORD PTR [esp-4]
    and         edx, eax
    mov         WORD PTR [ecx], dx
    xor         eax, eax
    ret

_invalid_flagp:
    mov         eax, -1
    ret

_fegetexceptflag ENDP

END
