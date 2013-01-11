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
;; int fesetexceptflag(fexcept_t const* flagp, int excepts);
;;
;; Set the exception status flags for both the x87 FPU and the
;; SSE control/status register.
;;
;; Input:
;;          rcx = flagp
;;          edx = excepts
;; Output:
;;          rax = 0 on success, -1 on error
;;

.CODE
ALIGN 8
PUBLIC fesetexceptflag
fesetexceptflag PROC
    
    ; test flagp argument for validity
    test        rcx, rcx
    jz          _invalid_flagp

    ; mask out status flags to set
    mov         ax, WORD PTR [ecx]
    and         edx, FE_ALL_EXCEPT
    and         eax, edx
    not         edx

    ; set exception flags in x87 FPU
    fnstenv     [rsp - SIZEOF fenv]
    and         [rsp - SIZEOF fenv].fenv.status_word, dx
    or          [rsp - SIZEOF fenv].fenv.status_word, ax
    fldenv      [rsp - SIZEOF fenv]

    ; set exception flags in SSE control/status register
    stmxcsr     [rsp - SIZEOF fenv].fenv.mxcsr
    and         [rsp - SIZEOF fenv].fenv.mxcsr, edx
    or          [rsp - SIZEOF fenv].fenv.mxcsr, eax
    ldmxcsr     [rsp - SIZEOF fenv].fenv.mxcsr

    ; done, return zero
    xor         rax, rax
    ret

    ; return error code
_invalid_flagp:
    mov         rax, -1
    ret

fesetexceptflag ENDP

END
