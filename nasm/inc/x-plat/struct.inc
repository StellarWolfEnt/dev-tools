%ifndef STRUCT_INC
%define STRUCT_INC

; Begin a struct
; %1 = struct name
%macro STRUCT 1
    %ifdef __CUR_STRUCT
        %error "STRUCT: nested structs are not supported, please close the previous struct with ENDSTRUCT before starting a new one"
    %endif

    %ifdef __CUR_OFFSET
        %error "STRUCT: __CUR_OFFSET is already defined, this likely means a previous struct was not closed properly with ENDSTRUCT"
    %endif

    %ifdef %1%
        %error "STRUCT: struct name '%1' is already defined, please choose a different name"
    %endif


    %xdefine %1% 0
    %xdefine __CUR_STRUCT %1
    %xassign __CUR_OFFSET 0
%endmacro

; Define a field: FIELD name, size[, alignment]
; %1 = field name
; %2 = size in bytes
; %3 = field alignment (optional, defaults to 0)
%macro FIELD 2-3
    %ifndef __CUR_STRUCT
        %error "FIELD: no struct is currently being defined, please start a struct with STRUCT before defining fields"
    %endif

    %ifndef __CUR_OFFSET
        %error "FIELD: __CUR_OFFSET is not defined, this likely means STRUCT was not used to start the struct definition"
    %endif

    %ifdef %{__CUR_STRUCT}_%1%
        %error "FIELD: field name '%1' is already defined in struct '%{__CUR_STRUCT}'"
    %endif
    
    %if %0 = 3 && %3 != 0
        %if (%3 & (%3 - 1)) != 0
            %error "FIELD: field alignment must be a power of 2"
        %endif

        %if (__CUR_OFFSET % %3) != 0
            %xassign __CUR_OFFSET __CUR_OFFSET + (%3 - (__CUR_OFFSET % %3))
        %endif
    %endif

    %xdefine %{__CUR_STRUCT}_%1 __CUR_OFFSET
    %xassign __CUR_OFFSET __CUR_OFFSET + %2
%endmacro

; End struct (pads to 16 bytes)
%macro ENDSTRUCT 0
    %ifndef __CUR_STRUCT
        %error "ENDSTRUCT: no struct is currently being defined, please start a struct with STRUCT before using ENDSTRUCT"
    %endif

    %ifndef __CUR_OFFSET
        %error "ENDSTRUCT: __CUR_OFFSET is not defined, this likely means STRUCT was not used to start the struct definition"
    %endif

    %if (__CUR_OFFSET % 16) != 0
        %xassign __CUR_OFFSET __CUR_OFFSET + (16 - (__CUR_OFFSET % 16))
    %endif

    %xdefine %{__CUR_STRUCT}_SIZE __CUR_OFFSET

    %undef __CUR_STRUCT
    %undef __CUR_OFFSET
%endmacro

%endif ; STRUCT_INC