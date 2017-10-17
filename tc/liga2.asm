;Archivo para probar ligas de biblioteca


datos segment

    X db ?

datos ends


pila segment stack 'stack'

    dw 256 dup (?)

pila ends



codigo segment

    assume  cs:codigo, ds:datos, ss:pila
    
 public _pintar
 _pintar proc near
	 
	 mov ax, 0B800h
	 mov es, ax
	 
	 xor di , di
	 mov es:[di], 0B35h
	 ret
 
 _pintar endp
 
 
 inicio: mov ax, datos
         mov ds, ax
         mov ax, pila
         mov ss, ax
		
		
		call _pintar

         mov ax, 4C00h
         int 21h
     
codigo ends



end inicio
