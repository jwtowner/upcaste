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
;; int feclearexcept(int excepts);
;;
;; Use the fnstenv/fldenv and stmxcsr/ldmxcsr instruction pairs to
;; clear the exception status flags for x87 FPU and SSE respectively.
;;

.CODE
ALIGN 4
PUBLIC _feclearexcept
_feclearexcept PROC

    ; mask and invert the excepts argument
    mov         ecx, DWORD PTR [esp + 4]
    and         ecx, FE_ALL_EXCEPT
    not         ecx

    ; clear exception flags in x87 FPU
    fnstenv     [esp - SIZEOF fenv]
    and         [esp - SIZEOF fenv].fenv.status_word, cx
    fldenv      [esp - SIZEOF fenv]
    
    ; clear exception flags in SSE control/status register
    stmxcsr     [esp - SIZEOF fenv].fenv.mxcsr
    and         [esp - SIZEOF fenv].fenv.mxcsr, ecx
    ldmxcsr     [esp - SIZEOF fenv].fenv.mxcsr

    ; done, return success
    xor         eax, eax
    ret

_feclearexcept ENDP

END
