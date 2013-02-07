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
;; int feclearexcept(int excepts);
;;
;; Use the fnstenv/fldenv and stmxcsr/ldmxcsr instruction pairs to
;; clear the exception status flags for x87 FPU and SSE respectively.
;;
;; Input:
;;          ecx = excepts
;; Output:
;;          eax = 0
;;

.CODE
ALIGN 8
PUBLIC feclearexcept
feclearexcept PROC

    ; mask and invert the excepts argument
    and         ecx, FE_ALL_EXCEPT
    not         ecx

    ; clear exception flags in x87 FPU
    fnstenv     [rsp - SIZEOF fenv]
    and         [rsp - SIZEOF fenv].fenv.status_word, cx
    fldenv      [rsp - SIZEOF fenv]
    
    ; clear exception flags in SSE control/status register
    stmxcsr     [rsp - SIZEOF fenv].fenv.mxcsr
    and         [rsp - SIZEOF fenv].fenv.mxcsr, ecx
    ldmxcsr     [rsp - SIZEOF fenv].fenv.mxcsr

    ; done, return success
    xor         rax, rax
    ret

feclearexcept ENDP

END
