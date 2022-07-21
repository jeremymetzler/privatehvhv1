#pragma once


#define EVENT_DEBUG_ID_INIT 42
#define EVENT_DEBUG_ID_SHUTDOWN 13

class bf_write;
class bf_read;

class IGameEvent {
public:
	virtual					~IGameEvent() = 0;
	virtual const char* GetName() const = 0;

	virtual bool            IsReliable() const = 0;
	virtual bool            IsLocal() const = 0;
	virtual bool            IsEmpty(const char* name = nullptr) = 0;

	virtual bool            GetBool(const char* name = nullptr, bool def = false) = 0;
	virtual int             GetInt(const char* name = nullptr, int def = 0) = 0;
	virtual uint64_t        GetUint64(const char* name = nullptr, unsigned long def = 0) = 0;
	virtual float           GetFloat(const char* name = nullptr, float def = 0.0f) = 0;
	virtual const char* GetString(const char* name = nullptr, const char* def = "") = 0;
	virtual const wchar_t* GetWstring(const char* name, const wchar_t* def = L"") = 0;

	virtual void            SetBool(const char* name, bool value) = 0;
	virtual void            SetInt(const char* name, int value) = 0;
	virtual void            SetUint64(const char* name, unsigned long value) = 0;
	virtual void            SetFloat(const char* name, float value) = 0;
	virtual void            SetString(const char* name, const char* value) = 0;
	virtual void            SetWstring(const char* name, const wchar_t* value) = 0;
};

class IGameEventListener2 {
public:
	virtual ~IGameEventListener2(void) {}

	virtual void FireGameEvent(IGameEvent* event) = 0;

	virtual int GetEventDebugId()
	{
		return 42;
	}
};

class IGameEventManager2 {
public:
	virtual             ~IGameEventManager2() = 0;
	virtual int         LoadEventsFromFile(const char* filename) = 0;
	virtual void        Reset() = 0;
	virtual bool        AddListener(IGameEventListener2* listener, const char* name, bool serverside) = 0;
	virtual bool        FindListener(IGameEventListener2* listener, const char* name) = 0;
	virtual int         RemoveListener(IGameEventListener2* listener) = 0;
	virtual IGameEvent* CreateEvent(const char* name, bool force, unsigned int unk) = 0;
	virtual bool        FireEvent(IGameEvent* event, bool dont_broadcast = false) = 0;
	virtual bool        FireEventClientSide(IGameEvent* event) = 0;
	virtual IGameEvent* DuplicateEvent(IGameEvent* event) = 0;
	virtual void        FreeEvent(IGameEvent* event) = 0;
	virtual bool        SerializeEvent(IGameEvent* event, bf_write* buf) = 0;
	virtual IGameEvent* UnserializeEvent(bf_read* buf) = 0;
};
