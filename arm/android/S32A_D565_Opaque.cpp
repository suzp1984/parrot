static void S32A_D565_Opaque(uint16_t* SK_RESTRICT dst,
		const SkPMColor* SK_RESTRICT src, int count,
		U8CPU alpha, int /*x*/, int /*y*/) {

	__asm__ volatile (
			"pld	[%[dst]]	\n\t"
			"pld	[%[src]]	\n\t"
			"wldrw	wr4, [%[dst]]	\n\t" //load 32-bit: uint16_t*2
			"wldrd	wr5, [%[src]]	\n\t" //load 64-bit: SKPMColor*2
			: [dst]"+r"(dst), [src]"+r"(src) 	//output
			: [count]"r"(count)	//input
			:	//coboler register
			);

}

static inline U16CPU SkSrcOver32To16(SkPMColor src, uint16_t dst) 
{
	unsigned sr = SkGetPackedR32(src);
	unsigned sg = SkGetPackedG32(src);
	unsigned sb = SkGetPackedB32(src);

	unsigned dr = SkGetPackedR16(dst); 
	unsigned dg = SkGetPackedG16(dst);
	unsigned db = SkGetPackedB16(dst);

	unsigned isa = 255 - SkGetPackedA32(src);
	unsigned mask = 65535;

	__asm__ volatile (
			"tbcsth wr0, %[isa]   \n\t" //broadcast 16bit from rm to wrm
			"tbcsth wr1, %[dr]	\n\t"  	//broadcast 16bit dr to wr1, wr1(dr,dg,db)
			"wsrld wr1, wr1, #48 \n\t"  //dr >> 48
			"wslld wr1, wr1, #32 \n\t"  //dr << 32
			"tbcsth wr2, %[dg] \n\t"    //broadcast dg to wr2
			"wsrld wr2, wr2, #48 \n\t"  //dg >> 48
			"wslld wr2, wr2, #16 \n\t"  //dg << 16
			"wor wr1, wr1, wr2 \n\t"    //wr1=(00,dr,dg,00)
			"tbcsth wr2, %[db] \n\t"	//broadcast db to wr2
			"wsrld wr2, wr2, #48 \n\t"	//dg >> 48
			"wor wr1, wr1, wr2 \n\t"	//wr1=(00,dr,dg,db)
			"wmulul wr3, wr0, wr1	\n\t"  //16-bit vector multi
		//	"wor wr4, wr3, wr3 \n\t" 	//mov wr3 -> wr4
		//	"wslld wr4, wr4, #32 \n\t"  //wr4:(00,dr,dg,db)-->(dg,db,00,00)
		//	"wsrld wr4, wr4, #48 \n\t"  //wr4:(00,00,00,dg)
		//	"wslld wr4, wr4, #16 \n\t"  //wr4:(00,00,dg,00)
			"tbcsth wr0, %[mask] \n\t"  //broadcast 1111*4 to wr0
			"wsrld wr0, wr0, #48 \n\t"  //shift right:(00,00,00,11)
			"wslld wr0, wr0, #16 \n\t"	//shift left:wr0=(00,00,11,00)
			"tbcsth wr1, %[mask] \n\t"  //wr1=(11,11,11,11)
			"wxor wr1, wr1, wr0  \n\t"	//wr1=(11,11,00,11)
			"wand wr3, wr3, wr1	 \n\t"	//wr3=(00,dr,00,db)
			"wandn wr4, wr3, wr1 \n\t"  //wr4=(00,00,dg,db)



			: [dr]"+r"(dr), [dg]"+r"(dg), [db]"+r"(db)	//output
			: [sr]"r"(sr), [sg]"r"(sg), [sb]"r"(sb), [isa]"r"(isa), [mask]"r"(mask) //input
			: "memory"
			);

	return SkPackRGB16(dr, dg, db);
}
