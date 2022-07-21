#pragma once

/* macros */
#define INRANGE(x,a,b)    (x >= a && x <= b) 
#define getBits( x )    (INRANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x,'0','9') ? x - '0' : 0))
#define getByte( x )    (getBits(x[0]) << 4 | getBits(x[1]))

struct datamap_t;

class typedescription_t
{
public:
	int32_t type;
	char* name;
	int offset[2];
	int16_t size;
	int16_t flags;
	char u0[12];
	datamap_t* td;
	char u1[24];
};

struct datamap_t
{
    typedescription_t* data_desc;
    int data_num_fields;
    char const* data_class_name;
    datamap_t* base_map;
    bool chains_validated;
    bool packed_offsets_computed;
    int packed_size;
};

class Utils {
public:
	template <typename t>
	t& GetVFunc(void* class_pointer, size_t index)
	{
		return (*reinterpret_cast<t**>(class_pointer))[index];
	}

	void Init();
	DWORD FindPatternIDA(std::string moduleName, std::string pattern);
	bool WorldToScreen(const Vector &origin, Vector &screen);
    unsigned int DataInMap(datamap_t* map, const char* name);//gladiator cheats

private:
}; extern Utils g_Utils;