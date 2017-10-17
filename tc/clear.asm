; Este es un programa que borra la pantalla.

_datos segment
	public _nivel
	public _nombreNivel

	_nivel dw 0		;el numero de nivel con el que se iterara
	_nombreNivel db "bitacXXX.TXT",0	;donde se almacena el nombre del archivo que se accedera
	_nam db "X.txt",0
	_nombreArchivo db "tablero.txt",0 
	_buffer db ?
	
	public _buffer
	;public _archivo2
	
_datos ends

parametros struc

	
	ba dw ?
	su dw ?
	ra dw ?
	
	
	tablero dw 126 dup(?) ;Matriz de 14x9
						 ; si es -1 es rojo
						 ; si es  1 es blanco
						 ; si es  0 es vacio
	numeroDolmensBlancos dw ?
	numeroDolmensRojos   dw ?
	

parametros ends



.MODEL MEDIUM
_pila segment stack 'stack'

    dw 256 dup (?)

_pila ends


_codigo segment

    assume  cs:_codigo, ds:_datos, ss:_pila

	
	_existeNivel proc far
	public _existeNivel
	LOCAL nombre:byte:13,nivel:word=AUTO_SIZE
	push bp 
	mov bp,sp 
	sub sp,AUTO_SIZE 
		lea bx,[nombre]
		mov byte ptr [bx],'b'
		inc bx 
		mov byte ptr [bx],'i'
		inc bx 
		mov byte ptr [bx],'t'
		inc bx 
		mov byte ptr [bx],'a'
		inc bx 
		mov byte ptr [bx],'c'
		inc bx 
		inc bx 
		inc bx 
		inc bx 
		mov byte ptr [bx],'.'
		inc bx 
		mov byte ptr [bx],'t'
		inc bx 
		mov byte ptr [bx],'x'
		inc bx 
		mov byte ptr [bx],'t'
		inc bx 
		mov byte ptr [bx],0
		mov [nivel],0
		
		_leerNivel_inicio:
		lea si,[nombre]
		mov ax,word ptr [nivel]		;obtener el nombre del archivo
		mov bh,10			;para obtener los digitos 
		div bh
		add ah,30h			;poner el caracter ascci de la letra residuo
		mov byte ptr [si+7],ah
		xor ah,ah
		div bh
		add ah,30h		
		;poner el caracter ascci de la letra residuo
		mov [si+6],ah
		xor ah,ah
		div bh
		add ah,30h			;poner el caracter ascci de la letra residuo
		mov [si+5],ah
		xor ah,ah			;ya se tiene el nombre del archivo
		inc [nivel]			;se incrementa el numero de nivel
		cmp word ptr [nivel],1000
		jnae _leerNivel_ContinuarNombre
		mov word ptr [nivel],0			;se reinicia la cuenta
		
		_leerNivel_ContinuarNombre:;el numero de nivel siguiente no es mil, se continua normalmente
		
		mov ah,3dh
		lea dx,[nombre]
		xor al,al
		int 21h					;abrir el archivo
		
		jnc _leerNivel_inicio		;el archivo no esta, se pasa al siguiente
		dec [nivel] 
		mov bx,word ptr [nivel]		;devuelve el numero de archivo que se tiene que crear
		mov ax,bx
		mov sp,bp 
		pop bp
		ret
	_existeNivel endp
	
	
	; _escribeNivel proc far
		; public _escribeNivel
		; LOCAL contx:word,nombre:byte:13,nivel:word=AUTO_SIZE
		; push bp 
		; mov bp,sp 
		; sub sp,AUTO_SIZE 
		; mov ax,[bp].c 
		; mov sp,bp 
		; pop bp
		; ret 2
	; _escribeNivel endp
	
	_pintaNivel1 proc far 
		public _pintaNivel1
		LOCAL buffer:byte:1,nombre:byte:6,contx:word,handle:word=AUTO_SIZE
		push bp
		mov bp,sp
		sub sp,AUTO_SIZE
		lea bx,[nombre]
		mov byte ptr [bx],'t'
		inc bx  
		mov byte ptr [bx],'.'
		inc bx 
		mov byte ptr [bx],'t'
		inc bx 
		mov byte ptr [bx],'x'
		inc bx 
		mov byte ptr [bx],'t'
		inc bx 
		mov byte ptr [bx],0
		
		;abrir el archivo 
		mov ah,3dh 
		lea dx,[nombre]
		xor al,al 		;lectura de archivo
		int 21h 
		mov word ptr [handle],ax 
		xor di,di 			;indexa por donde se va 
		mov ax,0b800h 
		mov es,ax 
		
		mov word ptr [contx],0		;numero de posiciones a pintar 
		;mov cx,0 
		xor si,si 
		cicloD:
		cmp word ptr [contx],2000
		jae salidaD
			mov ah,3fh 
			mov bx,word ptr [handle]
			mov cx,1 
			lea dx,_buffer
			int 21h 
			mov ah,0fh 
			mov dl,byte ptr _buffer
			mov al,dl
			
			mov es:[di],ax 
			inc di 
			inc di 
			inc si 
			inc word ptr [contx]
			jmp cicloD
			
			
		
		
		salidaD:
		
		mov sp,bp 
		pop bp
		ret 
	_pintaNivel1 endp

	
	_printax proc far
	public _printax
; imprime a la salida estándar un número que supone estar en el AX
; supone que es un número positivo y natural en 16 bits.
; lo imprime en decimal.

    
    push AX
    push BX
    push CX
    push DX

    xor cx, cx
    mov bx, 10
ciclo1PAX: xor dx, dx
    div bx
    push dx
    inc cx
    cmp ax, 0
    jne ciclo1PAX
    mov ah, 02h
ciclo2PAX: pop DX
    add dl, 30h
    int 21h
    loop ciclo2PAX
    pop DX
    pop CX
    pop BX
    pop AX
    ret
_printax endP

_getDI proc far
		public _getDI
		push ax 
		cmp dx,50
		jna continuarDI
		mov dx,24	;reinicia las columnas
		inc bx		;aumenta las filas 
		continuarDI:
		dec bl 		;decrementa la fila en al que esta 
		mov al,80 
		mul bl		;obtiene la posicion
		add ax,dx	;le suma las columnas
		inc bl 
		inc dx 
		inc dx 
		shl ax,1			;*2
		mov di,ax 
		pop ax 
		ret
	_getDI endp 
	

	_pintaNivel proc far
	public _pintaNivel
	
		push bp 
		
		mov bp,sp 
		mov ax,0b800h
		mov es,ax 
		mov si,200
		mov ax,[bp].numeroDolmensRojos
		mov ah,0fh 
		add al,30h
		mov es:[si],ax 
		mov si,362 
		mov ax,[bp].numeroDolmensBlancos
		mov ah,0fh 
		add al,30h
		mov es:[si],ax 
		mov si,250 ;250 es el primero que se metio
		mov ax,[bp].tablero[si]
		call _printax
		mov cx,0
		mov di,664 		;posicion de inicio del tablero 
		mov bx,9 
		mov dx,24		;fila y columna 
		cicloTablero:
		cmp cx,126 
		jae finCicloTablero
			mov ax,[bp].tablero[si]
			cmp al,2 		;si esta no activa
			jne cicCont1
			mov ah,11h 
			mov al,'X'
			jmp finTemp
			cicCont1:
			cmp al,0 
			jne cicCont2
			mov ah,11h 		;pone lo invalido en azul 
			mov al,'0'
			jmp finTemp
			cicCont2:
			cmp al,1 
			jne cicCont3
			mov ah,7fh 		;pone la  posicion en blanco  
			mov al,'B'
			jmp finTemp
			cicCont3:
			mov ah,4ch
			mov al,'R'		;lo pone en rojo 
			finTemp:
			call _getDI		;obtiene el valor para el di, donde se pondra la imagen 
			mov es:[di],ax 
			inc cx 
			dec si 
			dec si 
			jmp cicloTablero 
		finCicloTablero:
		pop bp 
		ret
	_pintaNivel endp
		
	
	_pq proc far
	public _pq
	ARG X:WORD
	push bp
	mov bp,sp
	mov ax,[X]
	mov ah,0fh 
	add al,30h 
	mov dx, 0B800h
	mov es, dx
	mov bx,810
	mov es:[bx],ax
	mov sp,bp
	pop bp
	mov ax,6
	ret
	_pq endp
	
	
	_pq1 proc far
public _pq1
	push ss	
	push ax 
	ret
	_pq1 endp
	
	
	
	
	_rayos proc far
	public _rayos
	mov ax,0b800h
	mov es,ax 
	xor di,di 
	mov es:[di],0b35h
	ret 
	_rayos endp
	
	
 _inicio: 
 mov ax, _datos
         mov ds, ax
         mov ax, _pila
         mov ss, ax	
		
		

         mov ax, 4C00h
         int 21h
     
_codigo ends

end _inicio
