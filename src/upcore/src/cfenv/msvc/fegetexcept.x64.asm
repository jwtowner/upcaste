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
;; int fegetexcept(void);
;;
;; Get current exception control mask for both the x87 FPU and the
;; SSE control/status register.
;;
;; Output:
;;          rax = inclusive-or of exception control masks of both
;;                x87 FPU and SSE control/status register.
;;

.CODE
ALIGN 8
PUBLIC fegetexcept
fegetexcept PROC
    
    stmxcsr     DWORD PTR [rsp-8]
    fnstcw      WORD PTR [rsp-2]
    mov         eax, DWORD PTR [rsp-8]
    shr         eax, MXSCR_EXCEPT_SHIFT
    or          ax, WORD PTR [rsp-2]
    not         rax
    and         rax, FE_ALL_EXCEPT
    ret

fegetexcept ENDP

END
