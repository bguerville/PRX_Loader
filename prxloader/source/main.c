#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ppu-lv2.h>

//----------------------------------------
//LV1/2 UTILS
//----------------------------------------

#define SYSCALL_NOT_IMPL		0x3C60800160630003ULL
#define SYSCALL_NOT_IMPL2		0x4E800020

#define LV2_BEGIN_SEEK			0x8000000000000000ULL
#define LV2_END_SEEK			0x8000000000600000ULL

void lv1_poke(uint64_t addr, uint64_t val)
{
    lv2syscall2(9, addr, val);
}

uint64_t lv2_peek(uint64_t addr)
{
    lv2syscall1(6, addr);
	return_to_user_prog(uint64_t);
}

void lv2_poke(uint64_t addr, uint64_t val)
{
    lv2syscall2(7, addr, val);
}

uint64_t find_syscall()
{
	uint64_t i = LV2_END_SEEK;

	while(i>LV2_BEGIN_SEEK)
	{
		if(lv2_peek(i) == SYSCALL_NOT_IMPL)
			if(((lv2_peek(i+8) >> 32) & 0xFFFFFFFF) == SYSCALL_NOT_IMPL2)
				return i;
		i-=4;
	}
	return 0;
}

uint64_t reverse_search64(uint64_t val)
{
	uint64_t i = LV2_END_SEEK;

	while(i>LV2_BEGIN_SEEK)
	{
		if(lv2_peek(i) == val) return i;
		i-=4;
	}
	return 0;
}

uint64_t search64(uint64_t val)
{
	uint64_t i;

	for(i=LV2_BEGIN_SEEK;i<LV2_END_SEEK;i+=4)
	{
		if(lv2_peek(i) == val) return i;
	}
	return 0;
}

uint64_t find_syscall_table()
{
	uint64_t sc, opd_sc;
	sc = find_syscall();
	opd_sc = reverse_search64(sc);
	return search64(opd_sc);
}

int get_lv2_version()
{
	uint64_t toc = lv2_peek(0x8000000000003000ULL);
	switch(toc)
	{
		case 0x8000000000330540ULL:
			return 0x355C;
		break;
		case 0x800000000034AC80ULL:
			return 0x355D;
		break;
		case 0x8000000000346390ULL:
			return 0x421C;
		break;
		case 0x8000000000363E80ULL:
			return 0x421D;
		break;
		case 0x8000000000348200ULL:
			return 0x430C;
		break;
		case 0x8000000000365CA0ULL:
			return 0x430D;
		break;
		case 0x8000000000348210ULL:
			return 0x431C;
		break;
		case 0x80000000003487D0ULL:
			return 0x440C;
		break;
		case 0x80000000003487E0ULL:
			return 0x441C;
		break;
		case 0x80000000003665C0ULL:
			return 0x441D;
		break;
		case 0x8000000000366BD0ULL:
			return 0x446D;
		break;
		case 0x8000000000348DF0ULL:
			return 0x446C;
		break;
		case 0x800000000034B160ULL:
			return 0x450C;
		break;
		case 0x800000000036EC40ULL:
			return 0x450D;
		break;
		case 0x800000000034B2E0ULL:
			return 0x453C;
		break;
		case 0x8000000000370620ULL:
			return 0x453D;
		break;
		case 0x800000000034E620ULL:
			return 0x455C;
		break;
		case 0x80000000003738E0ULL:
			return 0x455D;
		break;
		case 0x800000000034F950ULL:
			return 0x460C;
		break;
		case 0x800000000034F960ULL:
			if(lv2_peek(0x800000000031EBA8ULL)==0x323031342F31312FULL) return 0x466C;
			else return 0x465C;
		break;
		case 0x8000000000375510ULL:
			if(lv2_peek(0x800000000031EBA8ULL)==0x323031342F31312FULL) return 0x466D;
			else return 0x465D;
		break;
		default:
			return 0;
		break;
	}
	return 0;
}

uint64_t get_syscall_table()
{
	int version = get_lv2_version();
	switch(version)
	{
		case 0x355C:
			return 0x8000000000346570ULL;
		break;
		case 0x355D:
			return 0x8000000000361578ULL;
		break;
		case 0x421C:
			return 0x800000000035BCA8ULL;
		break;
		case 0x421D:
			return 0x800000000037A1B0ULL;
		break;
		case 0x430C:
			return 0x800000000035DBE0ULL;
		break;
		case 0x430D:
			return 0x800000000037C068ULL;
		break;
		case 0x431C:
			return 0x800000000035DBE0ULL;
		break;
		case 0x440C:
			return 0x800000000035E260ULL;
		break;
		case 0x441C:
			return 0x800000000035E260ULL;
		break;
		case 0x441D:
			return 0x800000000037C9E8ULL;
		break;
		case 0x446C:
			return 0x800000000035E860ULL;
		break;
		case 0x446D:
			return 0x800000000037CFE8ULL;
		break;
		case 0x450C:
			return 0x800000000035F0D0ULL;
		break;
		case 0x450D:
			return 0x8000000000383658ULL;
		break;
		case 0x453C:
			return 0x800000000035F300ULL;
		break;
		case 0x453D:
			return 0x8000000000385108ULL;
		break;
		case 0x455C:
			return 0x8000000000362680ULL;
		break;
		case 0x455D:
			return 0x8000000000388488ULL;
		break;
		case 0x460C:
			return 0x8000000000363A18ULL;
		break;
		case 0x465C:
			return 0x8000000000363A18ULL;
		break;
		case 0x465D:
			return 0x800000000038A120ULL;
		break;
		case 0x466C:
			return 0x8000000000363A18ULL;
		break;
		default:
			return 0;
		break;
	}
	return 0;
}

int install_syscall(int syscall_number, uint64_t *payload, uint32_t payload_size, uint64_t install_offset)
{
	uint64_t syscall_table = get_syscall_table();
	uint64_t payload_opd = install_offset + payload_size + 0x10;
	int i;	
	if(syscall_table)
	{
		for(i=0;i<(payload_size/8);i++) lv2_poke(install_offset+(i*8), payload[i]);
		lv2_poke(payload_opd, install_offset);
		lv2_poke(syscall_table + (8*syscall_number), payload_opd);
		return 1;
	}

	return 0;
}

void write_htab(void)
{
    uint64_t cont = 0;
    uint64_t reg5, reg6;
    uint32_t val;
    while(cont < 0x80) 
	{
        val = (cont << 7);
        reg5 = lv2_peek(0x800000000f000000ULL | ((uint64_t) val));
        reg6 = lv2_peek(0x800000000f000008ULL | ((uint64_t) val));
        reg6 = (reg6  & 0xff0000ULL) | 0x190ULL;
		lv2syscall8(10, 0, (cont << 3ULL), reg5, reg6, 0, 0, 0, 1);
        cont++;
    }
}

//----------------------------------------
//PRX LOADER
//----------------------------------------

#define SYSCALL8_OPCODE_GET_VERSION				0x7000
#define SYSCALL_OPCODE_LOAD_VSH_PLUGIN			0x1EE7
#define SYSCALL_OPCODE_UNLOAD_VSH_PLUGIN		0x364F
#define MAX_VSH_PLUGINS							6
#define PRX_PAYLOAD_PATH						"/dev_hdd0/game/PRXLOADER/USRDIR/payloads/payload_%X.bin"
#define PLUGINS_PATH							"/dev_hdd0/game/PRXLOADER/USRDIR/plugins.txt"

int syscall_1022_load_prx_module(uint32_t slot, char * path, void * arg, uint32_t arg_size) //PRX_LOADER PAYLOAD SYSCALL
{
    lv2syscall5(1022, SYSCALL_OPCODE_LOAD_VSH_PLUGIN, (uint64_t) slot, (uint64_t) path, (uint64_t) arg, (uint64_t) arg_size);
	return_to_user_prog(int);
}

int syscall_8_load_prx_module(uint32_t slot, char * path, void * arg, uint32_t arg_size) //COBRA_MAMBA SYSCALL
{
    lv2syscall5(8, SYSCALL_OPCODE_LOAD_VSH_PLUGIN, (uint64_t) slot, (uint64_t) path, (uint64_t) arg, (uint64_t) arg_size);
	return_to_user_prog(int);
}

int sys8_get_version(u32 *version)
{
    lv2syscall2(8, SYSCALL8_OPCODE_GET_VERSION, (u64)version);
    return_to_user_prog(int);
}

int is_cobra_based(void)
{
    u32 version = 0x99999999;
    if (sys8_get_version(&version) < 0) return 0;
    else if (version != 0x99999999) return 1;
    else return 0;
}

uint8_t * read_file(char *path, uint32_t * file_size, uint16_t round)
{
	uint8_t * buf;
	uint32_t size = 0;
	uint16_t rest;
	FILE * f = fopen(path, "rb");
	if(f)
	{
		uint32_t size = fseek(f, 0, SEEK_END);
		size = ftell(f);
		fseek(f, 0, SEEK_SET);

		if(round)
		{
			rest = size % round;
			if(rest)
				size = size - rest + round;
		}
		buf = malloc(size);
		fread(buf, size, 1, f);
		fclose(f);
		*(file_size) = size;
		return buf;
	}else{
		*(file_size) = 0;
		return NULL;
	}
}

uint32_t load_all_prx(char * config_path, int use_payload)
{
	char line[256];
	int len;
	uint32_t slot = 0;
	FILE * f = fopen(config_path,"r");
	if(!f) return 0;
	while(fgets(line, sizeof line, f) != NULL && slot < 6)
	{
		len = strlen(line);
		if(line[0] != '/' || len == 0) continue;
		if(line[len-1] == '\n') line[len-1] = 0;
		if(line[len-2] == '\r') line[len-2] = 0;
		if(use_payload == 0) syscall_8_load_prx_module(slot, line, 0, 0);
		else if(use_payload == 1) syscall_1022_load_prx_module(slot, line, 0, 0);
		slot++;
	}
	fclose(f);
	return slot;
}

int main()
{
	if(!is_cobra_based())
	{
		int lv2_version = get_lv2_version();
		if(!lv2_version) {{lv2syscall3(392, 0x1004, 0xa, 0x1b6); } return -1; }
		uint64_t * payload;
		uint32_t size;
		char payload_path[256];
		if((lv2_version>>4) == 0x446)//patch lv2 protection (rebug only, doesn't affect others)
		{
			lv1_poke(0x370AA8, 0x0000000000000001ULL);
			lv1_poke(0x370AA8 + 8, 0xE0D251B556C59F05ULL);
			lv1_poke(0x370AA8 + 16, 0xC232FCAD552C80D7ULL);
			lv1_poke(0x370AA8 + 24, 0x65140CD200000000ULL);
		}
		write_htab();
		sprintf(payload_path, PRX_PAYLOAD_PATH, lv2_version);
		payload = (uint64_t *) read_file(payload_path, &size, 8);
		if(payload == NULL) {{free(payload); lv2syscall3(392, 0x1004, 0xa, 0x1b6); } return -1; }
		if (install_syscall(1022, payload, size, 0x80000000007F0000ULL) != 1) {free(payload); {lv2syscall3(392, 0x1004, 0xa, 0x1b6); } return -1; }
		free(payload);
		lv2_poke(0x8000000000003D90ULL, 0x386000014E800020ULL); // /patch permission 4.xx, usually "fixed" by warez payload
		load_all_prx(PLUGINS_PATH, 1);
		{lv2syscall3(392, 0x1004, 0x7, 0x36); }
	}
	else
	{
		load_all_prx(PLUGINS_PATH, 0);
		{lv2syscall3(392, 0x1004, 0x4, 0x6); }
	}
	return 0;
}

