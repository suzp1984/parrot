	.global S32A_D565_Opaque
	.type S32A_D565_Opaque, %function

S32A_D565_Opaque:
	push {fp}
	ldrh r6, [r0]
	ldr  r7, [r1]

	pop {fp}
	bx lr

