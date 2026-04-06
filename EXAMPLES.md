# dev-tools Examples

This file contains examples of how to use the utilities in this repository. It is not meant to be comprehensive, but rather to illustrate common patterns and conventions.

## Using the SEH helpers

```nasm
%include "seh.inc"

PDATA_SECTION
    PDATA MyFunction
    ; emits:
    ;  __MyFunction_pdata:
    ;      dd MyFunction - $$
    ;      dd MyFunction_End - $$
    ;      dd __MyFunction_xdata - $$

    PDATA MyFunction2
    ; emits:
    ;  __MyFunction2_pdata:
    ;      dd MyFunction2 - $$
    ;      dd MyFunction2_End - $$
    ;      dd __MyFunction2_xdata - $$

XDATA_SECTION
    XDATA MyFunction, 1, 0, 0
    ; emits:
    ;  __MyFunction_xdata:
    ;      db (1 << (0 << 3)
    ;      db 0
    ;      db 0
    ;      db (0 | ((0 / 16) << 4))

    XDATA MyFunction2, 1, 0, 4, 1
    ; emits:
    ;  __MyFunction2_xdata:
    ;      db (1 << (0 << 3)
    ;      db 4
    ;      db 1
    ;      db (0 | ((0 / 16) << 4))
    UWOP_ALLOC 4, 40
    ; emits:
    ;  db 4
    ;  db (2 | (((40 - 8) / 8) << 4))
```

## Using the visibility helpers

```nasm
%include "visibility.inc"

PUBLIC MyPublicFunction
  ; ... function body ...
END
; emits:
;  global MyPublicFunction
;  extern MyPublicFunction
;  align 16
;  MyPublicFunction:
;  ... function body ...
;  MyPublicFunction_End:

INTERNAL MyInternalFunction
  ; ... function body ...
END MyCustomLabel
; emits:
; global MyInternalFunction
; align 16
; MyInternalFunction:
; ... function body ...
; MyCustomLabel:

PRIVATE MyPrivateFunction
  ; ... function body ...
END
; emits:
;  align 16
;  MyPrivateFunction:
;  ... function body ...
;  MyPrivateFunction_End:
```

## Using the import includes

```nasm
%include "freetype.inc"

; Now you can use the imported functions and types from FreeType, such as:
;  call ft_Init_FreeType
;  call ft_Done_FreeType

%include "glew32.inc"
; Now you can use the imported functions and types from GLEW, such as:
;  call glewInit
```