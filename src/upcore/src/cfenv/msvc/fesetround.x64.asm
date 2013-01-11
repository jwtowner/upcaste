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
;; int fesetround(int round);
;;
;; Set the rounding mode for both the x87 FPU and the SSE
;; control/status register.
;;
;; Input:
;;          ecx = rounding-mode flags
;; Output:
;;          rax = 0 if successful, -1 if error occurred
;;

.CODE
ALIGN 8
PUBLIC fesetround
fesetround PROC
    
    ; make sure input flags are valid
    test        ecx, 0FFFFF3FFh
    jnz         _invalid_rounding_mode
    
    ; apply x87 FPU rounding mode
    fnstcw      WORD PTR [rsp-2]
    mov         ax, WORD PTR [rsp-2]
    and         ax, FE_CTRL_NOROUND
    or          ax, cx
    mov         WORD PTR [rsp-2], ax
    fldcw       WORD PTR [rsp-2]

    ; apply SSE rounding mode
    stmxcsr     DWORD PTR [rsp-8]
    shl         ecx, MXSCR_ROUND_SHIFT
    mov         eax, DWORD PTR [rsp-8]
    and         eax, MXSCR_NOROUND
    or          eax, ecx
    mov         DWORD PTR [rsp-8], eax
    ldmxcsr     DWORD PTR [rsp-8]

    ; return success code
    xor         rax, rax
    ret

    ; return error code
_invalid_rounding_mode:
    mov         rax, -1
    ret

fesetround ENDP

END
