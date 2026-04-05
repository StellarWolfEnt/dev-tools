%ifndef SEH_INC
%define SEH_INC

%define PDATA_SECTION section .pdata rdata align=4
%define XDATA_SECTION section .xdata rdata align=4

; %1: function name
; %2: end label (optional, defaults to %1_End)
; meant to be paired with XDATA macro, which defines the unwind information for the function
; as such the name of the xdata label is derived from the function name, so the function name must be unique within the module
%macro PDATA 1-2

    %ifdef __%1_PDATA_GUARD
        %error "PDATA: function '%1' already has pdata defined"
    %endif

    %define start %1
    %define end %1_End

    %if(%0 >= 2)
        %define end %2
    %endif

    %xdefine __%1_PDATA_GUARD

    align 4
    __%1_pdata:
        dd start - $$
        dd end - $$
        dd __%1_xdata - $$
%endmacro

; %1: function name
; %2: version [usually 1]
; %3: flags [usually UNW_FLAG_NHANDLER (0)]
; %4: size of prolog in bytes (optional, defaults to 0)
; %5: count of unwind codes (required when size of prolog is specified, defaults to 0)
; %6: frame register (optional, defaults to 0)
; %7: frame register offset (in bytes) (required when frame register is specified, defaults to 0)
%macro XDATA 3-7

    %ifdef __%1_XDATA_GUARD
        %error "XDATA: function '%1' already has xdata defined"
    %endif

    %define version %2
    %define flags %3
    %define prolog_size 0
    %define unwind_code_count 0
    %define frame_register 0
    %define frame_register_offset 0

    %if %0 >= 4 && %4 != 0
        %if %0 < 5
            %error "XDATA: if size of prolog is specified, count of unwind codes must also be specified"
        %endif

        %define prolog_size %4
        %define unwind_code_count %5
    %endif

    %if %0 >= 6 && %6 != 0
        %if %0 < 7
            %error "XDATA: if frame register is specified, frame register offset must also be specified"
        %endif

        %if %7 != 0 && (%7 % 16) != 0
            %error "XDATA: frame register offset must be a multiple of 16"
        %endif

        %define frame_register %6
        %define frame_register_offset %7
    %endif

    %if (version != 1)
        %error "XDATA: only version 1 is currently supported"
    %endif

    %if (flags & ~(UNW_FLAG_NHANDLER | UNW_FLAG_EHANDLER | UNW_FLAG_UHANDLER | UNW_FLAG_CHAININFO)) != 0
        %error "XDATA: invalid flags specified"
    %endif

    %if (prolog_size < 0) || (prolog_size > 255)
        %error "XDATA: size of prolog must be between 0 and 255 bytes"
    %endif

    %if (unwind_code_count > 0) && (prolog_size == 0)
        %error "XDATA: if unwind codes are specified, size of prolog must also be specified"
    %endif

    %if (frame_register != 0) && (frame_register_offset == 0)
        %error "XDATA: if frame register is specified, frame register offset must also be specified"
    %endif

    %if (frame_register_offset / 16) > 0x0F
        %error "XDATA: frame register offset must be less than or equal to 240 bytes"
    %endif

    %xdefine __%1_XDATA_GUARD

    align 4
    __%1_xdata:
        db (version | (flags << 3))
        db prolog_size
        db unwind_code_count
        db (frame_register | ((frame_register_offset / 16) << 4))

    %xassign p_count unwind_code_count
    %xassign u_count 0
%endmacro

; %1: unwind code offset (in bytes)
; %2: register number (0-15)
%macro UWOP_PUSH_NONVOL 2
    %if (%2 < 0) || (%2 > 15)
        %error "UWOP_PUSH_NONVOL: register number must be between 0 and 15"
    %endif

    db %1
    db (0 | (%2 << 4))
    %xassign u_count u_count + 1
%endmacro

; %1: unwind code offset (in bytes)
; %2: size of stack allocation (must be 8 + (N * 8) bytes, between 8 and 4GB-8 bytes)
%macro UWOP_ALLOC 2
    %if (%2 < 8) || (%2 > 0xFFFFFFFF - 8)
        %error "UWOP_ALLOC: stack size must be between 8 bytes and 4GB-8 bytes"
    %endif

    %if (%2 % 8) != 0
        %error "UWOP_ALLOC: stack size must be a multiple of 8 bytes"
    %endif

    %if ((%2 - 8) % 16) != 0
        %error "UWOP_ALLOC: stack size must be 8 + (N * 16) bytes"
    %endif

    %if(%2 <= 128)
        db %1
        db (2 | (((%2 - 8) / 8) << 4))
        %xassign u_count u_count + 1
    %elif (%2 <= 0x7FFF8)
        db %1
        db (1 | (0 << 4))
        dw %2 / 8
        %xassign u_count u_count + 2
    %else
        db %1
        db (1 | (1 << 4))
        dd %2
        %xassign u_count u_count + 3
    %endif
%endmacro

; %1: unwind code offset (in bytes)
%macro UWOP_SET_FPREG 1
    db %1
    db (3 | (0 << 4))
    %xassign u_count u_count + 1
%endmacro

; %1: unwind code offset (in bytes)
; %2: register number (0-15)
; %3: stack offset (must be a multiple of 8 bytes)
%macro UWOP_SAVE_NONVOL 3
    %if (%2 < 0) || (%2 > 15)
        %error "UWOP_SAVE_NONVOL: register number must be between 0 and 15"
    %endif

    %if (%3 % 8) != 0
        %error "UWOP_SAVE_NONVOL: stack offset must be a multiple of 8"
    %endif

    %if (%3 <= 0x7FFF8)
         db %1
         db (4 | (%2 << 4))
         dw (%3 / 8)
        %xassign u_count u_count + 2
    %else
        db %1
        db (5 | (%2 << 4))
        dd %3
        %xassign u_count u_count + 3
    %endif
%endmacro

; %1: unwind code offset (in bytes)
; %2: XMM register number (0-15)
; %3: stack offset (must be a multiple of 16 bytes)
%macro UWOP_SAVE_XMM 3
    %if (%2 < 0) || (%2 > 15)
        %error "UWOP_SAVE_XMM: XMM register number must be between 0 and 15"
    %endif

    %if (%3 % 16) != 0
        %error "UWOP_SAVE_XMM: stack offset must be a multiple of 16"
    %endif

    %if (%3 <= 0xFFFF0)
        db %1
        db (8 | (%2 << 4))
        dw (%3 / 16)
        %xassign u_count u_count + 2
    %else
        db %1
        db (9 | (%2 << 4))
        dd %3
        %xassign u_count u_count + 3
    %endif
%endmacro

; %1: unwind code offset (in bytes)
; %2: has_error_code (0 or 1)
%macro UWOP_PUSH_MACHFRAME 2
    %if (%2 != 0) && (%2 != 1)
        %error "UWOP_PUSH_MACHFRAME: has_error_code must be 0 or 1"
    %endif

    db %1
    db (10 | (%2 << 4))
    %xassign u_count u_count + 1
%endmacro

; %1: function name [optional, unused, this just helps with readability]
%macro XDATA_END 0-1
    %if u_count != p_count
        %error "XDATA_END: unwind code count does not match the number of unwind codes emitted"
    %endif
%endmacro

; at this time, only NHANDLER is actually
; supported, this is because I don't yet understand
; how to emit exception handlers or chained unwind info,
; but the flags are defined here for completeness and future use
%define UNW_FLAG_NHANDLER 0x00
%define UNW_FLAG_EHANDLER 0x01
%define UNW_FLAG_UHANDLER 0x02
%define UNW_FLAG_CHAININFO 0x04

%define RAX 0
%define RCX 1
%define RDX 2
%define RBX 3
%define RSP 4
%define RBP 5
%define RSI 6
%define RDI 7
%define R8 8
%define R9 9
%define R10 10
%define R11 11
%define R12 12
%define R13 13
%define R14 14
%define R15 15

%define XMM0 0
%define XMM1 1
%define XMM2 2
%define XMM3 3
%define XMM4 4
%define XMM5 5
%define XMM6 6
%define XMM7 7
%define XMM8 8
%define XMM9 9
%define XMM10 10
%define XMM11 11
%define XMM12 12
%define XMM13 13
%define XMM14 14
%define XMM15 15

%endif ; !SEH_INC