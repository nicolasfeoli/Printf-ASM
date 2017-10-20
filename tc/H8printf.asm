;Printf ASM
;Nicolas Feoli Chacon
;Lenguajes de Programacion
;Grupo 2
;Entrega: 19/10/17

dirStr equ 2

strOri equ 4

segundoParametro equ 6

PUBLIC _H8Printf
PUBLIC _ValorEspecial

datos segment

    acerca db "Hecho por Nicolas Feoli. Carne 2016081332", 0Dh, 0Ah, '$'
 
    stringParametro db "Prueba, %s Esta es una prueba %s",0

    stringAgregado db "hola",0

    _ValorEspecial dw 0

    error  db "Error, la opcion que ingreso es incorrecta, intente utilizar -A", 0Dh, 0Ah
           db "para desplegar la ayuda",0Dh, 0Ah, '$'
   
datos ends

                  
pila segment stack 'stack' 

    dw 512 dup (?)

pila ends


codigo segment

    assume  cs:codigo, ds:datos, ss:pila
     

imprimeAcercaDe proc
  ;Imprime el mensaje inicial de informacion 
        mov ah, 09h ; subrutina de la interrupcion 21h para imprimir string
        lea dx, acerca
        int 21h
        ret
 imprimeAcercaDe endp

msjError proc
  ;Imprime el mensaje de error del programa
         mov ah, 09h ; subrutina de la interrupcion 21h para imprimir string
         lea dx, error
         int 21h
         
         ;call cerrarPrograma    
 msjError endp

imprimirNumeros proc
    ; recibe el numero en ax
    ; usa los registros bx, dx, cx
         push ax
         push bx ;guarda el estado de bx
         push dx ;guarda el estado de dx
         push cx ;guarda el estado de cx
         mov bx, 10 
         
         xor cx, cx ;para usarlo para contar digitos

   cicNum:
         xor dx, dx
         div bx ;el residuo queda en dx. Como es division por 10 siempre va a estar en dl
         push dx ;guarda el digito en la pila
         inc cx ; incrementa el contador
         cmp ax, 0 ; si ax=0 sale
         jz salCicNum
         jmp cicNum
         
   salCicNum:
         pop dx ;saca el ultimo digito en meter a la pila
                ;para darle vuelta al numero
         add dl, 30h ;pasar dl a ascii
         

         ;mov ah, 2 ;rutina de la 21h para imprimir caracter
         ;int 21h
         dec cx
         jnz salCicNum
         
         pop cx ;recupera el estado de cx
         pop dx ;recupera el estado de dx
         pop bx ;recupera el estado de bx
         pop ax
         ret
 
 imprimirNumeros endp

 _H8Printf proc far

    push bp
    mov bp, sp
    sub sp, 1 ;reserva 2 bytes para memoria local
              ; Entonces en el bp+6 -> offset str original
              ;                bp+10 -> puede ser un entero o el offset del string
    mov byte ptr [bp-1], 5 ;meter en la variable local un 5
    mov si,[bp+6] ;primer parametro
    ciclo: 
        mov dl,byte ptr [si]
        cmp dl,0
        jnz seguir
        jmp salir

        seguir:
        cmp dl, '%'
        jz colocarInfo
        cmp dl, '\'
        jz colocarBck
        mov ah, 2 ;rutina de la 21h para imprimir caracter
        int 21h     
        inc si 
        jmp ciclo

        colocarInfo:
            inc si
            mov dl,byte ptr [si]
            cmp dl, 's'
            jz agregarString
            cmp dl, 'd'
            jz agregarEntero
            cmp dl, 'i'
            jz agregarEntero
            call msjError
            jmp salir
        colocarBck:
            inc si 
            mov dl, byte ptr [si]
            cmp dl, 'n'
            jnz ciclo
            mov dl, 0Ah
            mov ah, 2 ;rutina de la 21h para imprimir caracter
            int 21h
            mov dl, 0Dh
            mov ah, 2 ;rutina de la 21h para imprimir caracter
            int 21h
            jmp ciclo

            agregarString:
                push si
                push bx
                xor bx, bx
                mov bl, byte ptr [bp-1]
                shl bx, 1
                mov si, bx
                mov ax, word ptr [bp+si]
                inc byte ptr [bp-1]
                inc byte ptr [bp-1]
                mov si,ax
                pop bx
                cicloAgregarString:
                    mov dl, byte ptr [si]
                    cmp dl, 0
                    jz terminoAgregarString
                    ;mov byte ptr [di], dl
                    mov ah, 2 ;rutina de la 21h para imprimir caracter
                    int 21h
                    
                    inc si
                    jmp cicloAgregarString
                terminoAgregarString:
                    pop si
                    inc si 
                    jmp ciclo

            agregarEntero:
                xor cx,cx
                push bx
                push si
                xor bx, bx
                mov bl, byte ptr [bp-1]
                shl bx, 1
                mov si, bx
                mov ax, word ptr [bp+si]
                inc byte ptr [bp-1]
                pop si
                pop bx
                ;mov si, word ptr [bp+10]
                ;mov ax, si
                mov bx, 10
            cicloAgregarEntero:
                xor dx, dx
                div bx    ; el residuo queda en dx. Siempre va a estar en dl.
                push dx   ; guarda el digito en la pila
                inc cx    ; incrementa el contador
                cmp ax, 0 ; si ax=0 sale
                jz terminoAgregarEntero
                jmp cicloAgregarEntero
                         
                terminoAgregarEntero:
                    pop dx ;saca el ultimo digito en meter a la pila
                           ;para darle vuelta al numero
                    add dl, 30h ;pasar dl a ascii

                    ;mov byte ptr [di],dl
                    mov ah, 2 ;rutina de la 21h para imprimir caracter
                    int 21h
                    dec cx
                    jnz terminoAgregarEntero
         
        inc si 
        jmp ciclo
    salir:

    mov sp, bp
    pop bp
    ret far
 _H8Printf endp


 inicio: mov ax, ds ;inicializacion del programa
         mov es, ax
        
         mov ax, datos
         mov ds, ax

         mov ax, pila
         mov ss, ax ;fin de inicializacion
         
         
         call imprimeAcercaDe

         lea dx, stringAgregado
         ;mov dx, 52
         push dx
         lea dx, stringParametro
         push dx
         call _H8Printf

         mov ax, 4C00h
         int 21h

codigo ends

end inicio