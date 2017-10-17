;Printf ASM
;Nicolas Feoli Chacon
;Lenguajes de Programacion
;Grupo 2
;Entrega: 18/10/17

;Manual de Usuario:
    ;Para 



;Sistema ABCDE:
    ;Recibe entrada de la linea de comando:   A

    

datos segment

    acerca db "Hecho por Nicolas Feoli. Carne 2016081332", 0Dh, 0Ah, '$'

    buffer db 1000 dup(?),'$' ;En este buffer se va a poner el string final que se va a imprimir.    
 
    stringParametro db "Prueba, %s JAJA era broma %s",0

    stringAgregado db "hola",0

    dirStr dw 0,0

    error  db "Error, la opcion que ingreso es incorrecta, intente utilizar -A", 0Dh, 0Ah
           db "para desplegar la ayuda",0Dh, 0Ah, '$'

    ;contador bytes = 1049
   
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
         
         mov ah, 2 ;rutina de la 21h para imprimir caracter
         int 21h
         dec cx
         jnz salCicNum
         
         pop cx ;recupera el estado de cx
         pop dx ;recupera el estado de dx
         pop bx ;recupera el estado de bx
         pop ax
         ret
 
 imprimirNumeros endp


 _H8printf proc

    push bp
    mov bp, sp

    ;sacar el primer parametro -> direccion del string.
    lea ax, stringParametro ; se cambiara por mov ax, [bp+4]
    mov word ptr dirStr,ds
    mov word ptr dirStr[2], ax

    xor di,di
    xor si,si
    ciclo: 
        mov dl,byte ptr stringParametro[si]
        cmp dl,0
        jz salir

        cmp dl, '%'
        jz colocarInfo
        ;mov dl, byte ptr [stringParametro+si]
        mov byte ptr buffer[di], dl
        inc di 
        inc si 
        jmp ciclo

        colocarInfo:
            inc si
            mov dl,byte ptr stringParametro[si]
            cmp dl, 's'
            jz agregarString
            cmp dl, 'd'
            jz agregarEntero
            call msjError
            jmp salir

            agregarString:
                push si
                xor si, si
                cicloAgregarString:
                    mov dl, byte ptr stringAgregado[si]
                    cmp dl, 0
                    jz terminoAgregarString
                    mov byte ptr buffer[di], dl
                    inc di
                    inc si
                    jmp cicloAgregarString
                terminoAgregarString:
                    pop si
                    inc si 
                    jmp ciclo

            agregarEntero:

        inc di 
        inc si 
        jmp ciclo
    salir:
    ;inc di
    mov byte ptr buffer[di], '$'
    lea dx, buffer
    mov ah, 09h
    int 21h

    pop bp
    ret
 _H8printf endp


 inicio: mov ax, ds ;inicializacion del programa
         mov es, ax
        
         mov ax, datos
         mov ds, ax

         mov ax, pila
         mov ss, ax ;fin de inicializacion
         
         
         call imprimeAcercaDe

         call _H8printf

         mov ax, 4C00h
         int 21h

codigo ends

end inicio