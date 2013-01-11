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
;; int fesetenv(fenv_t const* envp);
;;
;; Sets the floating-point environment to the one supplied by the
;; caller, handling special environments FE_DFL_ENV and FE_NOMASK_ENV.
;; This function preserves the x87 FPU precision mode for all given
;; environments. The default and no-mask environments also preserve
;; the SSE flush-to-zero and denormals-as-zeros flags, but a user
;; environment is allowed to override them. This should ensure
;; maximum compatability with the MSVC CRT and user-code.
;;
;; Input:
;;          rcx = envp
;; Output:
;;          rax = 0
;;

.CODE
ALIGN 8
PUBLIC fesetenv
fesetenv PROC

    ; check input arguments
    test        rcx, rcx
    jz          _invalid_envp

    ; fetch current floating point environment
    fnstenv     [rsp - SIZEOF fenv]
    stmxcsr     [rsp - SIZEOF fenv].fenv.mxcsr

    ; check for special environments
    xor         rax, rax
    cmp         rcx, FE_NOMASK_ENV
    jb          _user_environment

    ; clear out bits we don't want to set
    mov         [rsp - SIZEOF fenv].fenv.ip_offset, eax
    mov         [rsp - SIZEOF fenv].fenv.ip_selector, ax
    mov         [rsp - SIZEOF fenv].fenv.opcode, ax
    mov         [rsp - SIZEOF fenv].fenv.data_offset, eax
    mov         [rsp - SIZEOF fenv].fenv.data_selector, ax

    ; apply default environment if specified by caller
    je          _nomask_environment
    mov         cx, [rsp - SIZEOF fenv].fenv.control_word
    mov         edx, [rsp - SIZEOF fenv].fenv.mxcsr
    or          cx, FE_CTRL_MASK_INIT                                       ; exceptions are disabled
    and         edx, MXSCR_NOROUND_UNMASK                                   ; round-to-nearest & exception status cleared
    and         cx, FE_CTRL_NOROUND                                         ; round-to-nearest
    or          edx, MXSCR_ALL_EXCEPT                                       ; exceptions are disabled
    mov         [rsp - SIZEOF fenv].fenv.control_word, cx
    and         [rsp - SIZEOF fenv].fenv.status_word, FE_STAT_UNMASK        ; exception status cleared
    mov         [rsp - SIZEOF fenv].fenv.mxcsr, edx        
    jmp         _done

    ; apply no-mask environment if specified by caller
_nomask_environment:
    and         [rsp - SIZEOF fenv].fenv.control_word, FE_CTRL_NOMASK_INIT  ; exceptions enabled & round-to-nearest
    and         [rsp - SIZEOF fenv].fenv.status_word, FE_STAT_UNMASK        ; exception status cleared
    and         [rsp - SIZEOF fenv].fenv.mxcsr, MXSCR_NOMASK_INIT           ; round-to-nearest, exceptions enabled, exception status cleared
    jmp         _done

    ; set user supplied environment
_user_environment:
    mov         r8w, [rcx].fenv.control_word
    mov         r9w, [rcx].fenv.status_word
    mov         r10w, [rsp - SIZEOF fenv].fenv.control_word
    mov         r11w, [rsp - SIZEOF fenv].fenv.status_word
    and         r8w, FE_CTRL_USER_INIT                                      ; envp->control_word & (FE_ALL_EXCEPT | FE_TOWARDZERO)
    and         r9w, FE_ALL_EXCEPT                                          ; envp->status_word & FE_ALL_EXCEPT
    and         r10w, FE_CTRL_NOMASK_INIT                                   ; control_word &= ~(FE_ALL_EXCEPT | FE_TOWARDZERO)
    and         r11w, FE_STAT_UNMASK                                        ; status_word &= ~FE_ALL_EXCEPT
    or          r10w, r8w
    or          r11w, r9w
    mov         [rsp - SIZEOF fenv].fenv.control_word, r10w
    mov         [rsp - SIZEOF fenv].fenv.status_word, r11w
    mov         r9d, [rcx].fenv.ip_offset
    mov         r8w, [rcx].fenv.ip_selector
    mov         dx, [rcx].fenv.opcode
    mov         r10d, [rcx].fenv.data_offset
    mov         r11w, [rcx].fenv.data_selector
    mov         [rsp - SIZEOF fenv].fenv.ip_offset, r9d
    mov         [rsp - SIZEOF fenv].fenv.ip_selector, r8w
    mov         [rsp - SIZEOF fenv].fenv.opcode, dx
    mov         r9d, [rcx].fenv.mxcsr
    mov         [rsp - SIZEOF fenv].fenv.data_offset, r10d
    mov         [rsp - SIZEOF fenv].fenv.data_selector, r11w
    mov         [rsp - SIZEOF fenv].fenv.mxcsr, r9d

    ; finish by setting the x87 FPU environment and SSE control-status register
_done:
    fldenv      [rsp - SIZEOF fenv]
    ldmxcsr     [rsp - SIZEOF fenv].fenv.mxcsr
    ret

    ; return error code
_invalid_envp:
    mov         rax, -1
    ret

fesetenv ENDP

END
