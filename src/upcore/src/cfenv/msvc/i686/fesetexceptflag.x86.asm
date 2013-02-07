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
;; int fesetexceptflag(fexcept_t const* flagp, int excepts);
;;
;; Set the exception status flags for both the x87 FPU and the
;; SSE control/status register.
;;

.CODE
ALIGN 4
PUBLIC _fesetexceptflag
_fesetexceptflag PROC
    
    ; test flagp argument for validity
    mov         ecx, DWORD PTR [esp + 4]
    test        ecx, ecx
    jz          _invalid_flagp

    ; mask out status flags to set
    mov         edx, DWORD PTR [esp + 8]
    mov         ax, WORD PTR [ecx]
    and         edx, FE_ALL_EXCEPT
    and         eax, edx
    not         edx

    ; set exception flags in x87 FPU
    fnstenv     [esp - SIZEOF fenv]
    and         [esp - SIZEOF fenv].fenv.status_word, dx
    or          [esp - SIZEOF fenv].fenv.status_word, ax
    fldenv      [esp - SIZEOF fenv]

    ; set exception flags in SSE control/status register
    stmxcsr     [esp - SIZEOF fenv].fenv.mxcsr
    and         [esp - SIZEOF fenv].fenv.mxcsr, edx
    or          [esp - SIZEOF fenv].fenv.mxcsr, eax
    ldmxcsr     [esp - SIZEOF fenv].fenv.mxcsr

    ; done, return zero
    xor         eax, eax
    ret

    ; return error code
_invalid_flagp:
    mov         eax, -1
    ret

_fesetexceptflag ENDP

END
