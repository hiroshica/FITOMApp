namespace ROM {
UINT8 nonecompati[] = {
	{ DEVICE_NONE, },
};

UINT8 ssgcompati[] = {
	DEVICE_SSG, DEVICE_PSG, DEVICE_SSGL, DEVICE_SSGLP, DEVICE_SSGS, DEVICE_EPSG, DEVICE_NONE,
};
UINT8 opncompati[] = {
	DEVICE_OPN, DEVICE_OPN2, DEVICE_OPNA, DEVICE_OPN3L, DEVICE_OPNB, DEVICE_OPNC, DEVICE_OPN2C, DEVICE_OPN2L, DEVICE_2610B, DEVICE_F286, DEVICE_OPN3_N3, DEVICE_NONE,
};
UINT8 opmcompati[] = {
	DEVICE_OPM, DEVICE_OPP, DEVICE_OPZ, DEVICE_OPZ2, DEVICE_NONE,
};
UINT8 oplcompati[] = {
	DEVICE_OPL, DEVICE_OPL2, DEVICE_OPL3_2, DEVICE_Y8950, DEVICE_OPL3, DEVICE_NONE,
};
UINT8 opllcompati[] = {
	DEVICE_OPLL, DEVICE_OPLL2, DEVICE_OPLLP, DEVICE_OPLLX, DEVICE_NONE,
};
UINT8 opl3compati[] = {
	DEVICE_OPL3, DEVICE_OPN3_L3, DEVICE_NONE,
};
UINT8 opkcompati[] = {
	DEVICE_OPK, DEVICE_OPK2, DEVICE_NONE,
};
UINT8 opzcompati[] = {
	DEVICE_OPZ, DEVICE_OPZ2, DEVICE_NONE,
};

UINT8* compatilist[] = {
	nonecompati,	//00:DEVICE_NONE
	ssgcompati,		//01:DEVICE_SSG
	opncompati,		//02:DEVICE_OPN
	opncompati,		//03:DEVICE_OPN2
	opncompati,		//04:DEVICE_OPNA
	opmcompati,		//05:DEVICE_OPM
	opllcompati,	//06:DEVICE_OPLL
	oplcompati,		//07:DEVICE_OPL
	oplcompati,		//08:DEVICE_OPL2
	opl3compati,	//09:DEVICE_OPL3
	opncompati,		//10:DEVICE_OPN3L
	opncompati,		//11:DEVICE_OPNB
	nonecompati,	//12:DEVICE_SAA
	nonecompati,	//13:DEVICE_DSG
	0,
	ssgcompati,		//15:DEVICE_PSG
	nonecompati,	//16:DEVICE_DCSG
	nonecompati,	//17:DEVICE_SCC
	ssgcompati,		//18:DEVICE_SSGS
	nonecompati,	//19:DEVICE_APSG
	ssgcompati,		//20:DEVICE_SSGL
	ssgcompati,		//21:DEVICE_SSGLP
	0,
	0,
	oplcompati,		//24:DEVICE_Y8950
	oplcompati,		//25:DEVICE_OPL3_2
	opmcompati,		//26:DEVICE_OPP
	opzcompati,		//27:DEVICE_OPZ
	opzcompati,		//28:DEVICE_OPZ2
	opllcompati,	//29:DEVICE_OPLLP
	opllcompati,	//30:DEVICE_OPLL2
	opncompati,		//31:DEVICE_OPNC
	opncompati,		//32:DEVICE_OPN2C
	opncompati,		//33:DEVICE_OPN2L
	opncompati,		//34:DEVICE_2610B
	opncompati,		//35:DEVICE_F286
	nonecompati,	//36:DEVICE_OPN3
	opncompati,		//37:DEVICE_OPN3_N3
	opl3compati,	//38:DEVICE_OPN3_L3
	opllcompati,	//39:DEVICE_OPLLX
	opkcompati,		//40:DEVICE_OPK
	opkcompati,		//41:DEVICE_OPK2
	nonecompati,	//42:DEVICE_OPQ
	nonecompati,	//43:DEVICE_RYP4
	nonecompati,	//44:DEVICE_RYP6
	nonecompati,	//45:DEVICE_FMS
};

struct DEVIDMAP {
	const DWORD devid;
	const DWORD voicegroup;
	const DWORD voicetype;
	const DWORD regsize;
	const SC_CHIP_TYPE sctype;
	const TCHAR* chipname;
	const TCHAR* chipcode;
};
DEVIDMAP devmap[] = {
	{ DEVICE_OPN, VOICE_GROUP_OPNA, VOICE_TYPE_OPN, 0x100, SC_CHIP_TYPE::SC_TYPE_YM2203, _T("OPN"), _T("YM2203"), },
	{ DEVICE_OPNA, VOICE_GROUP_OPNA, VOICE_TYPE_OPNA, 0x200, SC_CHIP_TYPE::SC_TYPE_YM2608, _T("OPNA"), _T("YM2608"), },
	{ DEVICE_OPNB, VOICE_GROUP_OPNA, VOICE_TYPE_OPNA, 0x200, SC_CHIP_TYPE::SC_TYPE_YM2610, _T("OPNB"), _T("YM2610"), },
	{ DEVICE_OPNC, VOICE_GROUP_OPNA, VOICE_TYPE_OPN, 0x100, SC_CHIP_TYPE::SC_TYPE_YM2203, _T("OPNC"), _T("YMF264"), },
	{ DEVICE_2610B, VOICE_GROUP_OPNA, VOICE_TYPE_OPNA, 0x200, SC_CHIP_TYPE::SC_TYPE_YM2610B, _T("OPNBB"), _T("YM2610B"), },
	{ DEVICE_F286, VOICE_GROUP_OPNA, VOICE_TYPE_OPNA, 0x200, SC_CHIP_TYPE::SC_TYPE_YMF286, _T("OPNBK"), _T("YMF286K"), },
	{ DEVICE_OPN2, VOICE_GROUP_OPNA, VOICE_TYPE_OPNA, 0x200, SC_CHIP_TYPE::SC_TYPE_YM2612, _T("OPN2"), _T("YM2612"), },
	{ DEVICE_OPN2C, VOICE_GROUP_OPNA, VOICE_TYPE_OPNA, 0x200, SC_CHIP_TYPE::SC_TYPE_YM2612, _T("OPN2C"), _T("YM3438"), },
	{ DEVICE_OPN2L, VOICE_GROUP_OPNA, VOICE_TYPE_OPNA, 0x200, SC_CHIP_TYPE::SC_TYPE_YMF276, _T("OPN2L"), _T("YMF276"), },
	{ DEVICE_OPN3L, VOICE_GROUP_OPNA, VOICE_TYPE_OPNA, 0x200, SC_CHIP_TYPE::SC_TYPE_YMF288, _T("OPN3L"), _T("YMF288"), },
	{ DEVICE_OPN3_L3, VOICE_GROUP_OPNA, VOICE_TYPE_OPL3, 0x200, SC_CHIP_TYPE::SC_TYPE_YMF262, _T("OPN3_L"), _T("YMF294(OPL)"), },
	{ DEVICE_OPN3_N3, VOICE_GROUP_OPNA, VOICE_TYPE_OPNA, 0x200, SC_CHIP_TYPE::SC_TYPE_YMF288, _T("OPN3_N"), _T("YMF294(OPN)"), },
	{ DEVICE_OPL, VOICE_GROUP_OPL2, VOICE_TYPE_OPL, 0x100, SC_CHIP_TYPE::SC_TYPE_YM3526, _T("OPL"), _T("YM3526"), },
	{ DEVICE_OPL2, VOICE_GROUP_OPL2, VOICE_TYPE_OPL2, 0x100, SC_CHIP_TYPE::SC_TYPE_YM3812, _T("OPL2"), _T("YM3812"), },
	{ DEVICE_OPL3, VOICE_GROUP_OPL3, VOICE_TYPE_OPL3, 0x200, SC_CHIP_TYPE::SC_TYPE_YMF262, _T("OPL3"), _T("YMF262"), },
	{ DEVICE_OPL3_2, VOICE_GROUP_OPL2, VOICE_TYPE_OPL2, 0x200, SC_CHIP_TYPE::SC_TYPE_YMF262, _T("OPL3_2"), _T("YMF262(2op)"), },
	{ DEVICE_Y8950, VOICE_GROUP_OPL2, VOICE_TYPE_OPL, 0x100, SC_CHIP_TYPE::SC_TYPE_Y8950, _T("Y8950"), _T("YM3801"), },
	{ DEVICE_OPLL, VOICE_GROUP_OPLL, VOICE_TYPE_OPLL, 0x40, SC_CHIP_TYPE::SC_TYPE_YM2413, _T("OPLL"), _T("YM2413"), },
	{ DEVICE_OPLL2, VOICE_GROUP_OPLL, VOICE_TYPE_OPLL, 0x40, SC_CHIP_TYPE::SC_TYPE_YM2420, _T("OPLL2"), _T("YM2420"), },
	{ DEVICE_OPLLX, VOICE_GROUP_OPLL, VOICE_TYPE_OPLL, 0x40, SC_CHIP_TYPE::SC_TYPE_YM2423, _T("OPLLX"), _T("YM2423"), },
	{ DEVICE_OPLLP, VOICE_GROUP_OPLL, VOICE_TYPE_OPLL, 0x40, SC_CHIP_TYPE::SC_TYPE_YMF281, _T("OPLLP"), _T("YMF281"), },
	{ DEVICE_OPM, VOICE_GROUP_OPM, VOICE_TYPE_OPM, 0x100, SC_CHIP_TYPE::SC_TYPE_YM2151, _T("OPM"), _T("YM2151"), },
	{ DEVICE_OPP, VOICE_GROUP_OPM, VOICE_TYPE_OPM, 0x100, SC_CHIP_TYPE::SC_TYPE_YM2164, _T("OPP"), _T("YM2164"), },
	{ DEVICE_OPZ, VOICE_GROUP_OPM, VOICE_TYPE_OPZ, 0x100, SC_CHIP_TYPE::SC_TYPE_YM2414, _T("OPZ"), _T("YM2414"), },
	{ DEVICE_OPZ2, VOICE_GROUP_OPM, VOICE_TYPE_OPZ, 0x100, SC_CHIP_TYPE::SC_TYPE_UNKNOWN, _T("OPZ2"), _T("YM2424"), },
	{ DEVICE_OPK, VOICE_GROUP_OPL2, VOICE_TYPE_OPK, 0x88, SC_CHIP_TYPE::SC_TYPE_UNKNOWN, _T("OPK"), _T("YM7116"), },
	{ DEVICE_OPK2, VOICE_GROUP_OPL2, VOICE_TYPE_OPK, 0x88, SC_CHIP_TYPE::SC_TYPE_UNKNOWN, _T("OPK2"), _T("YM7129"), },
	{ DEVICE_PSG, VOICE_GROUP_PSG, VOICE_TYPE_SSG, 0x10, SC_CHIP_TYPE::SC_TYPE_AY8910, _T("PSG"), _T("AY-3-8910"), },
	{ DEVICE_SSG, VOICE_GROUP_PSG, VOICE_TYPE_SSG, 0x10, SC_CHIP_TYPE::SC_TYPE_YM2149, _T("SSG"), _T("YM2149"), },
	{ DEVICE_DCSG, VOICE_GROUP_PSG, VOICE_TYPE_DCSG, 0x10, SC_CHIP_TYPE::SC_TYPE_SN76489, _T("DCSG"), _T("SN76489"), },
	{ DEVICE_EPSG, VOICE_GROUP_PSG, VOICE_TYPE_EPSG, 0x20, SC_CHIP_TYPE::SC_TYPE_AY8930, _T("EPSG"), _T("AY8930"), },
	{ DEVICE_DSG, VOICE_GROUP_PSG, VOICE_TYPE_DSG, 0x100, SC_CHIP_TYPE::SC_TYPE_YM2163, _T("DSG"), _T("YM2163"), },
	{ DEVICE_SCC, VOICE_GROUP_PSG, VOICE_TYPE_SCC, 0x100, SC_CHIP_TYPE::SC_TYPE_SCC, _T("SCC"), _T("SCC"), },
	{ DEVICE_SCCP, VOICE_GROUP_PSG, VOICE_TYPE_SCC, 0x100, SC_CHIP_TYPE::SC_TYPE_SCCS, _T("SCC+"), _T("SCC+"), },
	{ DEVICE_SAA, VOICE_GROUP_PSG, VOICE_TYPE_SAA, 0x100, SC_CHIP_TYPE::SC_TYPE_UNKNOWN, _T("SAA"), _T("SAA1099"), },
	{ DEVICE_ADPCM, VOICE_GROUP_PCM, VOICE_TYPE_PCM, 0x100, SC_CHIP_TYPE::SC_TYPE_NONE, _T("ADPCM"), _T("ADPCM"), },
	{ DEVICE_ADPCMA, VOICE_GROUP_PCM, VOICE_TYPE_PCM, 0x100, SC_CHIP_TYPE::SC_TYPE_NONE, _T("ADPCM_A"), _T("ADPCM_A"), },
	{ DEVICE_ADPCMB, VOICE_GROUP_PCM, VOICE_TYPE_PCM, 0x100, SC_CHIP_TYPE::SC_TYPE_NONE, _T("ADPCM_B"), _T("ADPCM_B"), },
	{ DEVICE_PCMD8, VOICE_GROUP_PCM, VOICE_TYPE_PCM, 0x100, SC_CHIP_TYPE::SC_TYPE_NONE, _T("PCMD8"), _T("YMZ280"), },
	{ DEVICE_OPL4, VOICE_GROUP_OPL4, VOICE_TYPE_OPL4, 0x100, SC_CHIP_TYPE::SC_TYPE_UNKNOWN, _T("OPL4"), _T("YMF278"), },
	{ DEVICE_OPL4ML, VOICE_GROUP_OPL4, VOICE_TYPE_OPL4, 0x100, SC_CHIP_TYPE::SC_TYPE_UNKNOWN, _T("OPL4ML"), _T("YMF704"), },
	{ DEVICE_OPL4ML2, VOICE_GROUP_OPL4, VOICE_TYPE_OPL4, 0x100, SC_CHIP_TYPE::SC_TYPE_UNKNOWN, _T("OPL4ML2"), _T("YMF721"), },
	{ DEVICE_OPL4ML2, VOICE_GROUP_OPL4, VOICE_TYPE_OPL4, 0x100, SC_CHIP_TYPE::SC_TYPE_UNKNOWN, _T("OPL4ML2"), _T("YMF721"), },
	{ DEVICE_NBV, VOICE_GROUP_NONE, 0, 0, SC_CHIP_TYPE::SC_TYPE_NBV4, _T("NBV4"), _T("NBV4"), },
	{ DEVICE_AYB, VOICE_GROUP_NONE, 0, 0, SC_CHIP_TYPE::SC_TYPE_AYB02, _T("AYB02"), _T("AYB02"), },
	{ DEVICE_RHYTHM, VOICE_GROUP_RHYTHM, 0, 0, SC_CHIP_TYPE::SC_TYPE_NONE, _T("RHYTHM"), _T("RHYTHM"), },
	{ DEVICE_NONE, 0, 0, 0, SC_CHIP_TYPE::SC_TYPE_NONE, 0, 0, },
};

};
