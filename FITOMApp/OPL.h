#ifndef __OPL_H__
#define __OPL_H__

#include "FITOMdefine.h"
#include "MIDI.h"
#include "MultiDev.h"

class COPL : public CSoundDevice {
	friend class COPL3;
protected:
	virtual void Init();
	virtual void UpdateVolExp(UINT8 ch);
	virtual void UpdateVoice(UINT8 ch);
	virtual void UpdateFreq(UINT8 ch, const FNUM* fnum);
	virtual void UpdatePanpot(UINT8 ch);
	virtual void UpdateSustain(UINT8 ch);
	virtual void UpdateTL(UINT8 ch, UINT8 op, UINT8 lev);
	virtual void UpdateKey(UINT8 ch, UINT8 keyon);
	static UINT8 map[];
public:
	COPL(CPort* pt, int fsamp, UINT8 devtype=DEVICE_OPL);
	virtual void TimerCallBack(UINT32 tick);
};

class C3801 : public COPL {
protected:
public:
	C3801(CPort* pt, int fsamp);
	virtual void Init() {};
};

class COPL2 : public COPL {
public:
	COPL2(CPort* pt, int fsamp);
};

class COPL3 : public CSoundDevice {
protected:
	virtual void UpdateVolExp(UINT8 ch);
	virtual void UpdateVoice(UINT8 ch);
	virtual void UpdateFreq(UINT8 ch, const FNUM* fnum=0);
	virtual void UpdatePanpot(UINT8 ch);
	virtual void UpdateSustain(UINT8 ch);
	virtual void UpdateKey(UINT8 ch, UINT8 keyon);
	virtual void UpdateTL(UINT8 ch, UINT8 op, UINT8 lev);
	virtual void UpdateFnumber(UINT8 ch, int update=1);
	FNUM PseudoDT1[6];
	FNUM PseudoDT2[6];
	static UINT8 carmsk[];
	static UINT8 opmap[];
public:
	COPL3(CPort* pt, int fsamp);
	virtual void Init();
};

class COPL3_2 : public CSpanDevice {
protected:
public:
	COPL3_2(CPort* pt1, CPort* pt2, UINT8 mode, int fsamp);
	virtual void Init() {};
};

#endif
