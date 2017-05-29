//inner list file
//create by inner-trans

#ifdef _MD_ENABLE_WATERMARK

//include files
extern unsigned char __md_watermark_dat_watermark1_data[]; //watermark1.dat.c 
extern unsigned char __md_watermark_dat_watermark2_data[]; //watermark2.dat.c 
extern unsigned char __md_watermark_dat_watermark3_data[]; //watermark3.dat.c 
extern unsigned char __md_watermark_dat_watermark4_data[]; //watermark4.dat.c 
extern unsigned char __md_watermark_dat_watermark5_data[]; //watermark5.dat.c 

//declear arrays
static INNER_RES __md_watermark_watermark_inner_res[]={
	{ 0x10066F, (Uint8*)__md_watermark_dat_watermark1_data, 5582, (void*)"dat"}, //watermark1.dat.c
	{ 0x10366F, (Uint8*)__md_watermark_dat_watermark2_data, 5866, (void*)"dat"}, //watermark2.dat.c
	{ 0x10266F, (Uint8*)__md_watermark_dat_watermark3_data, 5759, (void*)"dat"}, //watermark3.dat.c
	{ 0x10566F, (Uint8*)__md_watermark_dat_watermark4_data, 5681, (void*)"dat"}, //watermark4.dat.c
	{ 0x10466F, (Uint8*)__md_watermark_dat_watermark5_data, 4018, (void*)"dat"}, //watermark5.dat.c
};

#endif // _MD_ENABLE_WATERMARK

