#pragma once

class Vector;

class INetChannel
{

	using WowFn = int(__thiscall*)(void*, uintptr_t*);
	using TransmitFn = bool(__thiscall*)(void*, bool);
public:
	char pad_0000[20]; //0x0000
	bool m_bProcessingMessages; //0x0014
	bool m_bShouldDelete; //0x0015
	char pad_0016[2]; //0x0016
	int m_nOutSequenceNr;    //0x0018 last send outgoing sequence number
	int m_nInSequenceNr;     //0x001C last received incoming sequnec number
	int m_nOutSequenceNrAck; //0x0020 last received acknowledge outgoing sequnce number
	int m_nOutReliableState; //0x0024 state of outgoing reliable data (0/1) flip flop used for loss detection
	int m_nInReliableState;  //0x0028 state of incoming reliable data
	int m_nChokedPackets;    //0x002C number of choked packets
	char pad_0030[1044]; //0x0030
	
	int SendDatagram() // idk if this is right
	{
		return g_Utils.GetVFunc<WowFn>(this, 49)(this, nullptr);
	}
	bool Transmit(bool reliable) // idk if this is right
	{
		return g_Utils.GetVFunc<TransmitFn>(this, 53)(this, reliable);
	}

}; //Size: 0x0444

class CEventInfo
{
public:

	unsigned short classID; //0x0000 0 implies not in use
	char pad_0002[4]; //0x0002 
	float fire_delay; //0x0004 If non-zero, the delay time when the event should be fired ( fixed up on the client )
	char pad_0008[4]; //0x0008
	ClientClass* pClientClass; //0x000C
	void* pData; //0x0010 Raw event data
	char pad_0014[48]; //0x0014
}; //Size: 0x0044

class CClockDriftMgr
{
public:
	float m_ClockOffsets[17];   //0x0000
	int m_iCurClockOffset; //0x0044
	int m_nServerTick;     //0x0048
	int m_nClientTick;     //0x004C
}; //Size: 0x0050


class IClientState {
public:
	void ForceFullUpdate()
	{
		*(int*)((uintptr_t)this + 0x174) = -1;
	}

	char pad_0000[156];             //0x0000
	INetChannel* m_NetChannel;          //0x009C
	int m_nChallengeNr;        //0x00A0
	char pad_00A4[100];             //0x00A4
	int m_nSignonState;        //0x0108
	char pad_010C[8];               //0x010C
	float m_flNextCmdTime;          //0x0114
	int m_nServerCount;        //0x0118
	int m_nCurrentSequence;    //0x011C
	char pad_0120[8];               //0x0120
	CClockDriftMgr m_ClockDriftMgr; //0x0128
	int m_nDeltaTick;          //0x0178
	bool m_bPaused;                 //0x017C
	char pad_017D[3];               //0x017D
	int m_nViewEntity;         //0x0180
	int m_nPlayerSlot;         //0x0184
	char m_szLevelName[260];        //0x0188
	char m_szLevelNameShort[40];    //0x028C
	char m_szGroupName[40];         //0x02B4
	char pad_02DC[52];              //0x02DC
	int m_nMaxClients;         //0x0310
	char pad_0314[18820];           //0x0314
	float m_flLastServerTickTime;   //0x4C98
	bool insimulation;              //0x4C9C
	char pad_4C9D[3];               //0x4C9D
	int oldtickcount;          //0x4CA0
	float m_tickRemainder;          //0x4CA4
	float m_frameTime;              //0x4CA8
	int lastoutgoingcommand;   //0x4CAC
	int chokedcommands;        //0x4CB0
	int last_command_ack;      //0x4CB4
	int command_ack;           //0x4CB8
	int m_nSoundSequence;      //0x4CBC
	char pad_4CC0[80];              //0x4CC0
	Vector viewangles;              //0x4D10
};