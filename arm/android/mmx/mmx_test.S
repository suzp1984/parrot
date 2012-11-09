	.global mmx_add_byte 
    .type   mmx_add_byte, %function

mmx_add_byte:
 	push	{fp}		
 	TMCRR wR0, r0, r0
 	TMCRR wR1, r1, r1
	WADDB wR2, wR0, wR1
	TMRRC r0, r1, wR2
 	pop	{fp}
 	bx	lr

	
