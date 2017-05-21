#include "STDAFX.H"
#include "ADPCM.h"
#include "MIDI.h"

#define YMDELTA_OFFSET	448
#define YMZ280_OFFSET	356

CAdPcmBase::CAdPcmBase(UINT8 devid, CPort* pt, size_t regsize, int fsamp, int devide, int offset, size_t memsize, UINT8 maxch, UINT8 pardev)
	: CSoundDevice(devid, maxch, fsamp, devide, offset, FnumTableType::DeltaN, pt, regsize)
	, maxmem(memsize), usedmem(0), boundary(0), parentdev(pardev), fcode(fsamp)
{
	::ZeroMemory((void*)&adpcmvoice, sizeof(adpcmvoice));
	ops = 0;
	fmaster = devide ? (fsamp / devide) : 0;
	NoteOffset = -57;	// origin note: O3A
}

void CAdPcmBase::Debug()
{
#ifdef _DEBUG
	fprintf(stderr, _T("%08X:%s\n"), CFITOM::GetInstance()->GetDeviceUniqID(this), CFITOM::GetInstance()->GetDeviceNameFromID(GetDevice()));
	for (int i = 0; i < 128; i++) {
		fprintf(stderr, _T("%i:%s 0x%08X,0x%08X\n"), i, adpcmvoice[i].name, adpcmvoice[i].staddr, adpcmvoice[i].length);
	}
#endif

}

CYmDelta::CYmDelta(UINT8 devid, CPort* pt, size_t regsize, int fsamp, int devide, size_t memsize, UINT8 pardev, const REGMAP& regset)
	: CAdPcmBase(devid, pt, regsize, fsamp, devide, YMDELTA_OFFSET, memsize, 1, pardev), regmap(regset)
{
}

UINT16 CYmDelta::GetDeltaN(int off)
{
	int oct = 0;
	while (off > 1216) {
		off -= 768;
	}
	while (!((YMDELTA_OFFSET - oct * 768) <= off && off <= ((YMDELTA_OFFSET + 768) - oct * 768))) {
		oct++;
	}
	off += 768 * oct;
	UINT ret = Fnum[off - YMDELTA_OFFSET] >> oct;
	return ret;
}

ISoundDevice::FNUM CYmDelta::GetFnumber(UINT8 ch, SINT16 offset)
{
	CHATTR* attr = GetChAttribute(ch);
	int index = attr->GetNoteIndex(MasterTune + (NoteOffset * 64) + offset);
	int oct = (-NoteOffset / 12) - 1;
	FNUM ret;
	ret.block = oct;
	ret.fnum = GetDeltaN(index);
	return ret;
}

void CYmDelta::LoadVoice(int prog, UINT8* data, size_t length)
{
	UINT32 st = 0;
	UINT32 ed = 0;
	if (prog) {
		st = adpcmvoice[prog - 1].staddr + adpcmvoice[prog - 1].length;
	}
	size_t blk = (length * 8) >> 5;
	if ((blk << 5) < length) { blk++; }
	blk <<= 5;
	if ((st >> 18) != ((st + blk) >> 18)) { //break the 256Kbit boundary
		st = ((st >> 18) + 1) << 18;
	}
	ed = st + blk - 1;
	adpcmvoice[prog].staddr = st;
	adpcmvoice[prog].length = blk;
	SetReg(regmap.flag, 0);
	SetReg(regmap.flag, 0x80);
	SetReg(regmap.control1, regmap.ctrl1init);
	SetReg(regmap.control1, 0x60);
	SetReg(regmap.control2, regmap.ctrl2init);
	SetReg(regmap.startLSB, (st >> 5) & 0xff);
	SetReg(regmap.startMSB, (st >> 13) & 0xff);
	SetReg(regmap.endLSB, (ed >> 5) & 0xff);
	SetReg(regmap.endMSB, (ed >> 13) & 0xff);
	(regmap.limitLSB != 0xff) ? SetReg(regmap.limitLSB, (ed >> 5) & 0xff) : void(0);
	(regmap.limitMSB != 0xff) ? SetReg(regmap.limitMSB, (ed >> 13) & 0xff) : void(0);
	for (size_t i = 0; i < (blk >> 3); i++) {
		if (i < length) {
			SetReg(regmap.memory, data[i]);
		}
		else {// padding
			SetReg(regmap.memory, 0x80);
		}
	}
	SetReg(regmap.control1, regmap.ctrl1init);
	SetReg(regmap.control1, 0);
}

void CYmDelta::PlayPCM(UINT8 ch, UINT8 num, UINT8 volume, SINT8 pan, SINT32 offset)
{
	if (num < 128 && adpcmvoice[num].length != 0) {
		pan /= 16;
		UINT8 chena = 0;
		if (pan >= 4) { //R
			chena = 0x40;
		}
		else if (pan <= -4) { //L
			chena = 0x80;
		}
		else { //C
			chena = 0xc0;
		}
		UINT32 st = adpcmvoice[num].staddr;
		UINT32 ed = st + adpcmvoice[num].length - 1;
		UINT32 deltan = GetDeltaN(offset);
		SetReg(regmap.flag, 0x1b);
		SetReg(regmap.flag, 0x80);
		SetReg(regmap.control1, 0x20);
		SetReg(regmap.control2, regmap.ctrl2init | (chena & regmap.panmask));
		SetReg(regmap.startLSB, (st >> 5) & 0xff);
		SetReg(regmap.startMSB, (st >> 13) & 0xff);
		SetReg(regmap.endLSB, (ed >> 5) & 0xff);
		SetReg(regmap.endMSB, (ed >> 13) & 0xff);
		(regmap.limitLSB != 0xff) ? SetReg(regmap.limitLSB, (ed >> 5) & 0xff) : void(0);
		(regmap.limitMSB != 0xff) ? SetReg(regmap.limitMSB, (ed >> 13) & 0xff) : void(0);
		SetReg(regmap.deltanLSB, (deltan)& 0xff);
		SetReg(regmap.deltanMSB, (deltan >> 8) & 0xff);
		SetReg(regmap.volume, (volume << 1) | (volume >> 6));
		SetReg(regmap.control1, 0xa0);
	}
}

void CYmDelta::StopPCM(UINT8 ch, UINT8 num)
{
	SetReg(regmap.control1, GetReg(regmap.control1, 0) & 0x7f);
	SetReg(regmap.control1, 0);
	SetReg(regmap.control1, regmap.ctrl1init);
	SetReg(regmap.control1, 0);
}

void CYmDelta::RhythmOn(UINT8 num, UINT8 vel, SINT8 pan, FMVOICE* rv, FNUM* fnum)
{
	int offset = fnum ? (SINT16)fnum->fnum : 0;
	StopPCM(0, num);
	PlayPCM(0, num, CalcVolExpVel(rhythmvol, 127, vel), pan, offset);
}

void CYmDelta::RhythmOff(UINT8 num)
{
	StopPCM(0, num);
}


void CYmDelta::UpdateKey(UINT8 ch, UINT8 keyon)
{
	if (ch < chs) {
		StopPCM(ch, 0);
		if (keyon) {
			SetReg(regmap.flag, 0x1b);
			SetReg(regmap.flag, 0x80);
			SetReg(regmap.control1, 0xa0);
		}
	}
}

void CYmDelta::UpdateVolExp(UINT8 ch)
{
	UINT8 volume = CalcLinearLevel(GetChAttribute(ch)->GetEffectiveLevel(), 0);
	volume = (volume) << 1;
	SetReg(regmap.volume, volume);
}

void CYmDelta::UpdateFreq(UINT8 ch, const FNUM* fnum)
{
	CHATTR* attr = GetChAttribute(ch);
	fnum = fnum ? fnum : attr->GetLastFnumber();
	SetReg(regmap.deltanLSB, (fnum->fnum)& 0xff);
	SetReg(regmap.deltanMSB, (fnum->fnum >> 8) & 0xff);
}

void CYmDelta::UpdatePanpot(UINT8 ch)
{
	int pan = (GetChAttribute(ch)->panpot) / 8;
	UINT8 chena = 0;
	if (pan >= 4) { //R
		chena = 0x40;
	}
	else if (pan <= -4) { //L
		chena = 0x80;
	}
	else { //C
		chena = 0xc0;
	}
	SetReg(regmap.control2, regmap.ctrl2init | (chena & regmap.panmask));
}

void CYmDelta::UpdateVoice(UINT8 ch)
{
	FMVOICE* voice = GetChAttribute(ch)->GetVoice();
	int num = voice->AL;
	UINT32 st = adpcmvoice[num].staddr;
	UINT32 ed = st + adpcmvoice[num].length - 1;
	SetReg(regmap.startLSB, (st >> 5) & 0xff);
	SetReg(regmap.startMSB, (st >> 13) & 0xff);
	SetReg(regmap.endLSB, (ed >> 5) & 0xff);
	SetReg(regmap.endMSB, (ed >> 13) & 0xff);
	(regmap.limitLSB != 0xff) ? SetReg(regmap.limitLSB, (ed >> 5) & 0xff) : void(0);
	(regmap.limitMSB != 0xff) ? SetReg(regmap.limitMSB, (ed >> 13) & 0xff) : void(0);
}

//CAdPcm3801 YM3801 aka Y8950
CAdPcm3801::CAdPcm3801(CPort* pt, int fsamp, size_t memsize)
	: CYmDelta(DEVICE_ADPCM, pt, 0x20, fsamp, 72, memsize, DEVICE_Y8950, 
{ 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0xff, 0xff, 0x0f, 0x10, 0x11, 0x12, 0x04, 0x01, 0x00, 0x00 })
{
	boundary = 32;
	SetReg(0x06, 0);
	SetReg(0x07, 0);
}

//CAdPcm2608 YM2608 aka OPNA
CAdPcm2608::CAdPcm2608(CPort* pt, int fsamp, size_t memsize)
	: CYmDelta(DEVICE_ADPCM, pt, 0x20, fsamp, 144, memsize, DEVICE_OPNA,
{ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x0c, 0x0d, 0x08, 0x09, 0x0a, 0x0b, 0x10, 0x01, 0x00, 0xc0 })
{
	boundary = 32;
	SetReg(0x00, 0);
	SetReg(0x01, 2);
}

//CAdPcm2610B YM2610 aka OPNB ADPCM_B
CAdPcm2610B::CAdPcm2610B(CPort* pt, int fsamp, size_t memsize, UINT8 pardev)
	: CYmDelta(DEVICE_ADPCMB, pt, 0x20, fsamp, 144, memsize, pardev,
{ 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0xff, 0xff, 0xff, 0x19, 0x1a, 0x1b, 0x1c, 0x01, 0x00, 0xc0 })
{
	boundary = 256;
}

void CAdPcm2610B::LoadVoice(int prog, UINT8* data, size_t length)
{
	UINT32 st = 0;
	UINT32 ed = 0;
	if (prog) {
		st = adpcmvoice[prog - 1].staddr + adpcmvoice[prog - 1].length;
	}
	size_t blk = (length) >> 8;
	if ((blk << 8) < length) { blk++; }
	blk <<= 8;
	ed = st + blk;
	adpcmvoice[prog].staddr = st;
	adpcmvoice[prog].length = blk;

	for (size_t i = 0; i < blk; i++) {
		UINT32 addr = st + i;
		port->writeRaw(0x200, (addr) & 0xff);
		port->writeRaw(0x201, (addr >> 8) & 0xff);
		port->writeRaw(0x202, (addr >> 16) & 0xff);
		port->writeRaw(0x203, 0);
		port->writeRaw(0x204, (i < length) ? (data[i] & 0xff) : 0x80);
	}
}

void CAdPcm2610B::UpdateVoice(UINT8 ch)
{
	FMVOICE* voice = GetChAttribute(ch)->GetVoice();
	int num = voice->AL;
	UINT32 st = adpcmvoice[num].staddr;
	UINT32 ed = st + adpcmvoice[num].length;
	SetReg(regmap.startLSB, (st >> 8) & 0xff);
	SetReg(regmap.startMSB, (st >> 16) & 0xff);
	SetReg(regmap.endLSB, (ed >> 8) & 0xff);
	SetReg(regmap.endMSB, (ed >> 16) & 0xff);
}

//CAdPcm2610A YM2610 aka OPNB ADPCM_A
CAdPcm2610A::CAdPcm2610A(CPort* pt, int fsamp, size_t memsize, UINT8 pardev)
	: CAdPcmBase(DEVICE_ADPCMA, pt, 0x30, fsamp, 0, YMDELTA_OFFSET, memsize, 6, pardev)
{
	boundary = 0x100000;
	rhythmcap = 6;
}

UINT8 CAdPcm2610A::QueryCh(CMidiCh* parent, FMVOICE* voice, int mode)
{
	return CSoundDevice::QueryCh(parent, voice, mode);
}

void CAdPcm2610A::LoadVoice(int prog, UINT8* data, size_t length)
{
	UINT32 st = 0;
	UINT32 ed = 0;
	if (prog) {
		st = adpcmvoice[prog - 1].staddr + adpcmvoice[prog - 1].length;
	}
	size_t blk = (length) >> 8;
	if ((blk << 8) < length) { blk++; }
	blk <<= 8;
	if ((st >> 20) < ((st + blk) >> 20)) {	// break 1MB boundary
		st = (st + 0x100000) & 0xfff00000;
	}
	ed = st + blk;
	adpcmvoice[prog].staddr = st;
	adpcmvoice[prog].length = blk;

	for (size_t i = 0; i < blk; i++) {
		UINT32 addr = st + i;
		port->writeRaw(0x200, (addr) & 0xff);
		port->writeRaw(0x201, (addr >> 8) & 0xff);
		port->writeRaw(0x202, (addr >> 16) & 0xff);
		port->writeRaw(0x203, 1);
		port->writeRaw(0x204, (i < length) ? (data[i] & 0xff) : 0);
	}
}

void CAdPcm2610A::RhythmOn(UINT8 num, UINT8 vel, SINT8 pan, FMVOICE* rv, FNUM* fnum)
{
	if (num < rhythmcap) {
		SetPanpot(num, pan);
		SetVoice(num, rv);
		NoteOn(num, vel);
	}
}

void CAdPcm2610A::RhythmOff(UINT8 num)
{
	if (num < rhythmcap) {
		NoteOff(num);
	}
}


void CAdPcm2610A::UpdateKey(UINT8 ch, UINT8 keyon)
{
	SetReg(0x00, (GetReg(0x00, 0) & ~(1 << ch)) | (keyon ? (1 << ch) : 0), 1);
}

void CAdPcm2610A::UpdateVolExp(UINT8 ch)
{
	CHATTR* attr = GetChAttribute(ch);
	UINT8 evol = 31 - Linear2dB(CalcLinearLevel(CalcEffectiveLevel(attr->velocity, attr->express), 0), RANGE24DB, STEP075DB, 5);
	SetReg(0x08 + ch, (GetReg(0x8 + ch, 0) & 0xe0) | evol, 1);
}

void CAdPcm2610A::UpdateRhythmVol()
{
	UINT8 evol = 63 - Linear2dB(CalcLinearLevel(rhythmvol, 0), RANGE48DB, STEP075DB, 6);
	if (GetReg(0x01, 0) != evol) {
		SetReg(0x01, evol);
	}
}

void CAdPcm2610A::UpdatePanpot(UINT8 ch)
{
	int pan = (GetChAttribute(ch)->panpot) / 8;
	UINT8 chena = 0;
	if (pan >= 4) { //R
		chena = 0x40;
	}
	else if (pan <= -4) { //L
		chena = 0x80;
	}
	else { //C
		chena = 0xc0;
	}
	SetReg(0x08 + ch, (GetReg(0x8 + ch, 0) & 0x3f) | chena, 1);
}

void CAdPcm2610A::UpdateVoice(UINT8 ch)
{
	FMVOICE* voice = GetChAttribute(ch)->GetVoice();
	int num = voice->AL;
	UINT32 st = adpcmvoice[num].staddr;
	UINT32 ed = st + adpcmvoice[num].length;
	SetReg(0x10 + ch, (st >> 8) & 0xff, 1);
	SetReg(0x18 + ch, (st >> 16) & 0xff, 1);
	SetReg(0x20 + ch, (ed >> 8) & 0xff, 1);
	SetReg(0x28 + ch, (ed >> 16) & 0xff, 1);
}

//CAdPcmZ280 YMZ280 aka PCMD8
CAdPcmZ280::CAdPcmZ280(CPort* pt, int fsamp, size_t memsize)
	: CAdPcmBase(DEVICE_ADPCM, pt, 0x100, fsamp, 384, YMZ280_OFFSET, memsize, 8, DEVICE_PCMD8)
{
	SetReg(0xff, 0xc0);	// KON enable/Memory enable
	SetReg(0x81, 0);	// DSP disable
	for (int i = 0; i < 8; i++) {
		SetReg(0x80, 0x88 | (i * 17));
	}
}

UINT16 CAdPcmZ280::GetDeltaN(int off)
{
	int oct = 0;
	while (off > 1216) {
		off -= 768;
	}
	while (!((YMZ280_OFFSET - oct * 768) <= off && off <= ((YMZ280_OFFSET + 768) - oct * 768))) {
		oct++;
	}
	off += 768 * oct;
	UINT ret = Fnum[off - YMZ280_OFFSET] >> oct;
	return ret;
}

ISoundDevice::FNUM CAdPcmZ280::GetFnumber(UINT8 ch, SINT16 offset)
{
	CHATTR* attr = GetChAttribute(ch);
	int index = attr->GetNoteIndex(MasterTune + (NoteOffset * 64) + offset);
	FNUM ret;
	ret.block = 0;
	ret.fnum = GetDeltaN(index);
	return ret;
}

void CAdPcmZ280::LoadVoice(int prog, UINT8* data, size_t length)
{
	UINT32 st = 0;
	UINT32 ed = 0;
	if (prog) {
		st = adpcmvoice[prog - 1].staddr + adpcmvoice[prog - 1].length;
	}
	size_t blk = length >> 2;
	if ((blk << 2) < length) { blk++; }
	blk <<= 2;
	ed = st + blk - 1;
	adpcmvoice[prog].staddr = st;
	adpcmvoice[prog].length = blk;
	for (size_t i = 0; i < blk; i++) {
		UINT32 addr = st + i;
		SetReg(0x84, addr >> 16);
		SetReg(0x85, (addr >> 8) & 0xff);
		SetReg(0x86, addr & 0xff);
		SetReg(0x87, (i < length) ? data[i] : 0x0);
	}
}

void CAdPcmZ280::UpdateKey(UINT8 ch, UINT8 keyon)
{
	UINT8 tmp = GetReg(0x01 + ch * 4, 0) & 1;
	SetReg(0x01 + ch * 4, (keyon ? 0xa0 : 0x20) | tmp);
}

void CAdPcmZ280::UpdateVolExp(UINT8 ch)
{
	UINT8 volume = GetChAttribute(ch)->GetEffectiveLevel();
	SetReg(0x02 + ch * 4, (volume << 1) | (volume >> 6));
}

void CAdPcmZ280::UpdateFreq(UINT8 ch, const FNUM* fnum)
{
	CHATTR* attr = GetChAttribute(ch);
	fnum = fnum ? fnum : attr->GetLastFnumber();
	SetReg(0x00 + ch * 4, (fnum->fnum >> 8) & 0xff);
	UINT8 tmp = (GetReg(0x01 + ch * 4, 0) & 0xfe);
	SetReg(0x01 + ch * 4, tmp | ((fnum->fnum >> 16) & 1));
}

void CAdPcmZ280::UpdatePanpot(UINT8 ch)
{
	int pan = (GetChAttribute(ch)->panpot) / 8;
	SetReg(0x3 + ch * 4, pan & 0xf);
}

void CAdPcmZ280::UpdateVoice(UINT8 ch)
{
	FMVOICE* voice = GetChAttribute(ch)->GetVoice();
	int num = voice->AL;
	UINT32 st = adpcmvoice[num].staddr;
	UINT32 ed = st + adpcmvoice[num].length - 1;
	SetReg(0x20 + ch * 4, (st >> 16) & 0xff);
	SetReg(0x40 + ch * 4, (st >> 8) & 0xff);
	SetReg(0x60 + ch * 4, st & 0xff);
	SetReg(0x23 + ch * 4, (ed >> 16) & 0xff);
	SetReg(0x43 + ch * 4, (ed >> 8) & 0xff);
	SetReg(0x63 + ch * 4, ed & 0xff);

	SetReg(0x21 + ch * 4, 0);
	SetReg(0x41 + ch * 4, 0);
	SetReg(0x61 + ch * 4, 0);
	SetReg(0x22 + ch * 4, 0);
	SetReg(0x42 + ch * 4, 0);
	SetReg(0x62 + ch * 4, 0);
}

