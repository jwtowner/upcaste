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
;; int fedisableexcept(int excepts);
;;
;; Disable the exception mask for both the x87 FPU and the
;; SSE control/status register, returing the inclusive-or of
;; the control masks for both.
;;

.CODE
ALIGN 4
PUBLIC _fedisableexcept
_fedisableexcept PROC

    push        ebx
    push        edi
    mov         ecx, DWORD PTR [esp+12]
    and         ecx, FE_ALL_EXCEPT
    stmxcsr     DWORD PTR [esp-8]
    mov         edi, ecx
    fstcw       WORD PTR [esp-2]
    mov         edx, DWORD PTR [esp-8]
    mov         bx, WORD PTR [esp-2]
    mov         eax, edx
    shl         edi, MXSCR_EXCEPT_SHIFT
    shr         eax, MXSCR_EXCEPT_SHIFT
    or          ax, bx
    or          edx, edi
    or          bx, cx
    mov         DWORD PTR [esp-8], edx
    mov         WORD PTR [esp-2], bx
    not         eax
    ldmxcsr     DWORD PTR [esp-8]
    and         eax, FE_ALL_EXCEPT
    fldcw       WORD PTR [esp-2]
    pop         edi
    pop         ebx
    ret

_fedisableexcept ENDP

END
