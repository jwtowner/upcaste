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
EXTRN fesetenv:PROC
EXTRN feraiseexcept:PROC

;;
;; int feupdateenv(fenv_t const* envp);
;;
;; Saves the current exception status, applies the given environment, and
;; then raises the previously saved exceptions.
;;
;; Input:
;;          rcx = envp
;; Output:
;;          rax = 0 on success, -1 on error
;;

.CODE
ALIGN 8
PUBLIC feupdateenv
feupdateenv PROC

    test        rcx, rcx
    jz          _invalid_envp
    xor         rax, rax
    sub         rsp, 48h
    fnstsw      ax
    stmxcsr     DWORD PTR [rsp]
    or          eax, DWORD PTR [rsp]
    and         eax, FE_ALL_EXCEPT
    mov         QWORD PTR [rsp], rax
    call        fesetenv
    mov         rcx, QWORD PTR [rsp]
    call        feraiseexcept
    add         rsp, 48h
    ret

_invalid_envp:
    mov         rax, -1
    ret

feupdateenv ENDP

END
