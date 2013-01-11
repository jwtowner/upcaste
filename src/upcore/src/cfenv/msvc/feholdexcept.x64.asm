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
;; int feholdexcept(fenv_t* envp);
;;
;; Saves the current floating-point environment, clears the exception
;; status bits, and sets all exceptions to non-stop.
;;
;; Input:
;;          rcx = envp
;; Output:
;;          rax = 0 on success, -1 on error
;;

.CODE
ALIGN 8
PUBLIC feholdexcept
feholdexcept PROC

    ; check input arguments
    test        rcx, rcx
    jz          _invalid_envp

    ; fetch current environment
    fnstenv     [rsp - SIZEOF fenv]
    stmxcsr     [rsp - SIZEOF fenv].fenv.mxcsr

    ; copy it to caller's envp object
    mov         rax, QWORD PTR[rsp - SIZEOF fenv]
    mov         rdx, QWORD PTR[rsp - SIZEOF fenv + 8]
    mov         r8, QWORD PTR[rsp - SIZEOF fenv + 16]
    mov         r9, QWORD PTR[rsp - SIZEOF fenv + 24]
    mov         QWORD PTR[rcx], rax
    mov         QWORD PTR[rcx + 8], rdx
    mov         QWORD PTR[rcx + 16], r8
    mov         QWORD PTR[rcx + 24], r9

    ; clear all exception status bits and set exception mask to non-stop mode
    mov         eax, [rsp - SIZEOF fenv].fenv.mxcsr
    or          [rsp - SIZEOF fenv].fenv.control_word, FE_ALL_EXCEPT
    or          eax, MXSCR_ALL_EXCEPT
    and         [rsp - SIZEOF fenv].fenv.status_word, FE_STAT_UNMASK
    and         eax, MXSCR_STAT_UNMASK

    ; apply the updated environment and return
    fldenv      [rsp - SIZEOF fenv]
    mov         [rsp - SIZEOF fenv].fenv.mxcsr, eax
    ldmxcsr     [rsp - SIZEOF fenv].fenv.mxcsr
    xor         rax, rax
    ret

_invalid_envp:
    mov         rax, -1
    ret

feholdexcept ENDP

END
