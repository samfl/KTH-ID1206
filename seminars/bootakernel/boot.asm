bits 16

start:
    mov ax, 0x07C0  ; Where we are.
    add ax, 0x02    ; Add 0x02 (when shifted 512)
    mov ss, ax      ; Set the stack segment
    mov sp, 0x1000  ; Set the stack pointer

    mov ax, 0x07C0  ; Set the data segment
    mov ds, ax      ; More about this later.. 

    mov si, msg     ; Pointer to the message in SI
    mov ah, 0x0E    ; Print char BIOS procedure

.next: 
    lodsb           ; next byte to AL, increment SI
    cmp al, 0       ; If the byte is zero
    je .done        ; Jump to done
    int 0x10        ; Invoke the BIOS system call
    jmp .next       ; Loop

.done: 
    jmp $           ; Loop forever

msg: db 'Hello', 0  ; The string we want to print


times 510-($-$$) db 0 ; Fill up tp 512 bytes
dw 0xAA55             ; Master boot record signature

