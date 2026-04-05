%ifndef VISIBILITY_INC
%define VISIBILITY_INC

%define CODE_SECTION section .text code align=16

; %1: function name
%macro PUBLIC 1
    %ifdef __CUR_FUNC
        %error "PUBLIC: a function is already being defined, please close the previous function with END before starting a new one"
    %endif
    
    %xdefine __CUR_FUNC %1
    global %1
    export %1
    align 16
    %1:
%endmacro

; %1: function name
%macro INTERNAL 1
    %ifdef __CUR_FUNC
        %error "INTERNAL: a function is already being defined, please close the previous function with END before starting a new one"
    %endif

    %xdefine __CUR_FUNC %1
    global %1
    align 16
    %1:
%endmacro

; %1: function name
%macro PRIVATE 1
    %ifdef __CUR_FUNC
        %error "PRIVATE: a function is already being defined, please close the previous function with END before starting a new one"
    %endif

    %xdefine __CUR_FUNC %1
    align 16
    %1:
%endmacro

; %1: end label name (optional, defaults to %{__CUR_FUNC}_End)
%macro END 0-1
    %ifndef __CUR_FUNC
        %error "END: no function is currently being defined, please start a function with PUBLIC, INTERNAL, or PRIVATE before using END"
    %endif

    %if %0 = 1
        %define end_label %1
    %else
        %define end_label %{__CUR_FUNC}_End
    %endif

    %undef __CUR_FUNC
    end_label:
%endmacro

%endif ; !VISIBILITY_INC