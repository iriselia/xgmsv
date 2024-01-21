
#include "gmsv/object.h"
#include "gmsv/map/readmap.h"
#include "gmsv/buf.h"
#include "gmsv/configfile.h"
#include "crossgate/utils.h"
#include "gmsv/char.h"

#define MAP_MAGIC  "LS2MAP"
#define MAP_ID_MAX 5

static MAP_ImageData* MAP_imagedata;
static unsigned int    MAP_imagedatanum;
static  int     MAP_imgfilt[65535];

static MAP_Map* MAP_map[MAP_ID_MAX];
static unsigned int     MAP_mapnum[MAP_ID_MAX];
static unsigned int     MAP_mapnum_index[MAP_ID_MAX];
MAP_Dungeon* MAP_ActiveDungeon;

static int* MAP_idjumptbl[MAP_ID_MAX];
static int  MAP_idtblsize[MAP_ID_MAX];

enum MAP_DATATYPECATEGORY
{
	MAP_INTENTRY = 0,
	MAP_CHARENTRY = 1,
	MAP_INTFUNC = 2,
	MAP_CHARFUNC = 3,
};
struct MAP_itemconfentry
{
	int     type;
	int     index;
	void* func;
};

static void MAP_makeDefaultImageData(MAP_ImageData* one)
{
	one->data[MAP_WALKABLE] = TRUE;
	one->data[MAP_HAVEHEIGHT] = FALSE;
	one->data[MAP_DEFENCE] = 0;
	one->data[MAP_INTODAMAGE] = 0;
	one->data[MAP_OUTOFDAMAGE] = 0;

	one->data[MAP_SETED_BATTLEMAP] = 0;
	one->data[MAP_BATTLEMAP] = 0;
	one->data[MAP_BATTLEMAP2] = 0;
	one->data[MAP_BATTLEMAP3] = 0;
	one->data[MAP_INTODARKNESS] = 0;
	one->data[MAP_INTOCONFUSION] = 0;

	one->data[MAP_OUTOFPOISON] = 0;
	one->data[MAP_OUTOFPARALYSIS] = 0;
	one->data[MAP_OUTOFSILENCE] = 0;
	one->data[MAP_OUTOFSTONE] = 0;
	one->data[MAP_OUTOFDARKNESS] = 0;
	one->data[MAP_OUTOFCONFUSION] = 0;

}

static int MAP_flgSet(char* string)
{
	int     tmp;
	tmp = atoi(string);
	if ( tmp == 0 )
		return 0;
	return 1;
}

MAP_itemconfentry MAP_conf_entries[MAP_DATAINT_NUM + MAP_DATACHAR_NUM] = {
	{MAP_INTENTRY,  MAP_WALKABLE,       MAP_flgSet},
	{MAP_INTENTRY,  MAP_HAVEHEIGHT,     MAP_flgSet},
	{MAP_INTENTRY,  MAP_DEFENCE,       NULL},
	{MAP_INTENTRY,  MAP_INTODAMAGE,     NULL},
	{MAP_INTENTRY,  MAP_OUTOFDAMAGE,    NULL},

	{MAP_INTENTRY,  MAP_SETED_BATTLEMAP,  NULL},
	{MAP_INTENTRY,  MAP_BATTLEMAP,	    NULL},
	{MAP_INTENTRY,  MAP_BATTLEMAP2,	    NULL},
	{MAP_INTENTRY,  MAP_BATTLEMAP3,	    NULL},
	{MAP_INTENTRY,  MAP_INTODARKNESS,   NULL},
	{MAP_INTENTRY,  MAP_INTOCONFUSION,  NULL},

	{MAP_INTENTRY,  MAP_OUTOFPOISON,    NULL},
	{MAP_INTENTRY,  MAP_OUTOFPARALYSIS, NULL},
	{MAP_INTENTRY,  MAP_OUTOFSILENCE,   NULL},
	{MAP_INTENTRY,  MAP_OUTOFSTONE,     NULL},
	{MAP_INTENTRY,  MAP_OUTOFDARKNESS,  NULL},
	{MAP_INTENTRY,  MAP_OUTOFCONFUSION, NULL},

};


BOOL MAP_initMapArray(int map_id, int num)
{
	MAP_map[map_id] = (MAP_Map*)allocateMemory(sizeof(MAP_Map) * num);
	if ( MAP_map == NULL )
	{
		xg_log("ERRROR %s:%d  Can't Allocate Memory For MapID= %d: size = %d\n", map_id, num);
		return FALSE;
	}

	int counter = 0;
	char* pointer = (char*)MAP_map[map_id];
	if ( num > 0 )
	{
		do
		{
			++counter;
			*pointer = -1;
			pointer += 32;
		} while ( counter != num );
	}

	MAP_mapnum[map_id] = num;
	MAP_mapnum_index[map_id] = 0;

	return TRUE;
}

//*
void MAP_endMapArray(void)
{
	for ( int i = 0; i < MAP_ID_MAX; i++ )
	{
		MAP_Map* maps = MAP_map[i];
		for ( int j = 0; j < MAP_mapnum[i]; j++ )
		{
			MAP_Map* map = &maps[j];

			if ( map->tile )
				freeMemory(map->tile);
			if ( map->obj )
				freeMemory(map->obj);
			if ( map->olink )
				freeMemory(map->olink);
			if ( map->encount )
				freeMemory(map->encount);
			if ( map->hit )
				freeMemory(map->hit);
		}
		MAP_mapnum[i] = 0;
		MAP_mapnum_index[i] = 0;
		freeMemory(MAP_map[i]);
		MAP_map[i] = NULL;

	}
}
//*/


static BOOL MAP_IsMapFile(char* filename)
{
	FILE* f;
	char    buf[16];
	int     ret;

	f = fopen(filename, "r");
	if ( f == NULL ) {
		errorprint;
		return FALSE;
	}

	ret = fread(buf, sizeof(char), 6, f);
	if ( ret != 6 )
		goto FCLOSERETURNFALSE;
	buf[ret] = '\0';

	if ( strcmp(buf, MAP_MAGIC) != 0 )
		goto FCLOSERETURNFALSE;
	else
		goto FCLOSERETURNTRUE;

FCLOSERETURNFALSE:
	fclose(f);
	return FALSE;
FCLOSERETURNTRUE:
	fclose(f);
	return TRUE;
}


//#include <WinSock2.h>

BOOL IsValidImagenumber(int imagenumber)
{
	if ( imagenumber < 0 || imagenumber >= arraysizeof(MAP_imgfilt) )
		return FALSE;

	if ( MAP_imgfilt[imagenumber] == -1 )
		return FALSE;
	return TRUE;
}

int MAP_getActiveDungeon(int cfgid, int seq)
{
	if ( !config.auto_dungeon_num )
	{
		return -1;
	}

	for ( int i = 0; i < config.auto_dungeon_num; i++ )
	{
		if ( MAP_ActiveDungeon[i].cfgid == cfgid && MAP_ActiveDungeon[i].seq == seq )
		{
			return i;
		}
	}

	return -1;
}

static BOOL MAP_readMapOne(int map_id, char* filename)
{
	FILE* f;					/*  ファイル    */
	char    buf[16];            /*  マジックナンバー読み用  */
	short   data[1024];         /*  ショート読み用バッファ  */
	int     ret;                /*  返り値受け取り用        */
	int     i;                  /*  ループ変数  */
	int     mapindex;

	int     id = 0, xsiz = 0, ysiz = 0, tile_count = 0;     /*  マップデータの一時格納場所  */
	int     extended_data_size = 0;
	int     has_extended_data = 0;
	int     unknown1 = 0, unknown2 = 0;
	int     cfgid = 0, seq = 0;
	MAP_Map* map_ptr;

	unsigned short* tile = NULL;
	unsigned short* obj = NULL;
	unsigned short* encount = NULL;
	unsigned short* hit = NULL;
	MAP_Objlink** olink = NULL;
	char    showstring[32];         /*  マップデータの一時格納場所  */
	struct  stat    filestat;       /*  チェック用に使う    */
	BOOL    invaliddata = FALSE;
	if ( MAP_mapnum_index[map_id] >= MAP_mapnum[map_id] ) {
		fprint("There is no space left on MapArray.\n");
		return FALSE;
	}
	mapindex = MAP_mapnum_index[map_id];
	f = fopen(filename, "r");
	if ( f == NULL ) {
		errorprint;
		return FALSE;
	}
	if ( fstat(fileno(f), &filestat) != 0 ) {
		errorprint;
		goto FCLOSERETURNFALSE;
	}

	ret = fread(buf, sizeof(char), 6, f);
	if ( ret != 6 ) {
		errorprint;
		goto FCLOSERETURNFALSE;
	}
	buf[ret] = '\0';

	if ( strcmp(buf, MAP_MAGIC) != 0 )
		goto FCLOSERETURNFALSE;
	ret = fread(data, sizeof(short), 1, f);
	if ( ret != 1 ) {
		errorprint;
		goto FCLOSERETURNFALSE;
	}
	id = ntohs(data[0]);

	ret = fread(showstring, sizeof(char), arraysizeof(showstring), f);
	if ( ret != arraysizeof(showstring) ) {
		errorprint;
		goto FCLOSERETURNFALSE;
	}
	showstring[arraysizeof(showstring) - 1] = '\0';
	ret = fread(data, sizeof(short), 1, f);
	if ( ret != 1 ) {
		errorprint;
		goto FCLOSERETURNFALSE;
	}
	xsiz = ntohs(data[0]);
	ret = fread(data, sizeof(short), 1, f);
	if ( ret != 1 ) {
		errorprint;
		goto FCLOSERETURNFALSE;
	}
	ysiz = ntohs(data[0]);

	if ( !map_id && id == 0xFFFF )
	{
		if ( fread(data, sizeof(int), 1, f) != 1 )
		{
			errorprint;
			goto FCLOSERETURNFALSE;
		}
		extended_data_size = ntohl(*(int*)data);
		
		if ( fread(&data, extended_data_size - 4, 1, f) != 1 )// metadata : 16 - 4 bytes
		{
			errorprint;
			goto FCLOSERETURNFALSE;
		}
		struct extended_data
		{
			int extended_id;
			int unknown1;
			int unknown2;
		};

		extended_data* ext = (extended_data*) data;
		id = ntohl(ext->extended_id);
		unknown1 = ntohl(ext->unknown1);
		unknown2 = ntohl(ext->unknown2);
		has_extended_data = 1;
	}
	else
	{
		has_extended_data = 0;
		unknown1 = 0;
		unknown2 = 0;
	}

	if ( MAP_mapnum_index[map_id] )
	{
		int i = 0;
		map_ptr = MAP_map[map_id];

		for( int i = 0; i < MAP_mapnum_index[map_id]; i++ )
		{
			if ( map_ptr->id == id )
			{
				xg_error("duplicate floor id: %d\n", id);
				goto FCLOSERETURNFALSE;
			}
			map_ptr++;
		}
	}

	tile_count = xsiz * ysiz;

	tile = (unsigned short*)allocateMemory(sizeof(short) * tile_count);
	if ( tile == NULL ) {
		fprint("Can't Allocate Memory for tile name:%s xsiz:%d ysiz:%d\n", filename, xsiz, ysiz);
		goto FREEOBJHP;
	}

	obj = (unsigned short*)allocateMemory(sizeof(short) * tile_count);
	if ( obj == NULL ) {
		fprint("Can't Allocate Memory for object\n");
		goto FREETILE;
	}

	encount = (unsigned short*)allocateMemory(sizeof(short) * tile_count);
	if ( encount == NULL ) {
		fprint("Can't Allocate Memory for encount\n");
		goto FREEOBJ;
	}

	olink = (MAP_Objlink**)allocateMemory(sizeof(MAP_Objlink*) * tile_count);
	if ( olink == NULL ) {
		fprint("Can't Allocate Memory for link\n");
		goto FREEENCOUNT;
	}

	ret = fread(tile, sizeof(short) * tile_count, 1, f);
	if ( ret != 1 ) {
		errorprint;
		goto FREELINK;
	}

	for ( i = 0; i < tile_count; i++ )
	{
		tile[i] = ntohs(tile[i]);
	}

	if ( has_extended_data || tile_count <= 0 )
	{
		invaliddata = 0;
	}
	else
	{
		for ( i = 0; i < tile_count; i++ ) {
			if ( !IsValidImagenumber(tile[i]) ) {
				fprint(
					"Invalid tile imagenumber floor:%d x:%d y:%d number:%d\n",
					id, i % xsiz, (int)(i / xsiz), tile[i]);
				invaliddata = TRUE;
			}
		}
	}

	ret = fread(obj, sizeof(short) * tile_count, 1, f);
	if ( ret != 1 ) {
		errorprint;
		goto FREELINK;
	}

	for ( i = 0; i < tile_count; i++ )
	{
		obj[i] = ntohs(obj[i]);
	}

	for ( i = 0; i < tile_count; i++ ) {
		if ( !IsValidImagenumber(obj[i]) ) {
			fprint(
				"Invalid obj imagenumber floor:%d x:%d y:%d number:%d\n",
				id, i % xsiz, (int)(i / xsiz), obj[i]);
			invaliddata = TRUE;
		}
	}

	if ( invaliddata )
	{
		goto FREELINK;
	}

	if ( map_id == 0 || map_id == 4 )                 // some hardcoded stuff?
	{
		cfgid = 0;
		seq = 0;
	}
	else
	{
		if ( fread(data, 2u, 1u, f) != 1 )     // cfgid : 2 bytes
		{
			errorprint;
			goto FREELINK;
		}
		cfgid = ntohs(data[0]);

		if ( fread(data, 2u, 1u, f) != 1 )     // seq : 2 bytes
		{
			errorprint;
			goto FREELINK;
		}
		seq = ntohs(data[0]);
	}

	if ( map_id == 1 && MAP_getActiveDungeon(cfgid, seq) == -1 )
	{
		xg_warning("Invalid auto dungeon:[%s] id:[%d] cfgid:[%d] seq:[%d]\n", filename, id, cfgid, seq);
		freeMemory(olink);
		freeMemory(obj);
		freeMemory(tile);
		fclose(f);
		return id;
	}

	if ( tile_count > 0 )
	{
		for ( int i = 0; i < tile_count; i++ )
			encount[i] = -1;
	}

	hit = 0;
	if ( has_extended_data )
	{
		hit = (unsigned short*)allocateMemory(sizeof(short) * tile_count);
		if ( !hit )
		{
			xg_error("Can't Allocate Memory for hit\n");
			goto FREELINK;
		}
		if ( fread(hit, sizeof(short) * tile_count, 1u, f) != 1 )
		{
			errorprint;
			goto FREEHIT;
		}
		if ( tile_count > 0 )
		{
			for ( int i = 0; i < tile_count; i++ )
			{
				hit[i] = ntohs(hit[i]);
			}
		}
	}

	if ( ftell(f) != filestat.st_size )
	{
		xg_warning("size of file(%s) is strange. realsize:%d readsize:%d\n", filename, filestat.st_size, ftell(f));
	}

	for ( i = 0; i < xsiz * ysiz; i++ )
	{
		olink[i] = NULL;
	}

	if ( map_id == 1 )                            // back compat?
	{
		xg_log("read dungeon file (ID = % d:Floor = % d, Index = % d)....\n", cfgid, id, mapindex);
		mapindex = id;
	}

	map_ptr = &MAP_map[map_id][mapindex];

	map_ptr->id = id;
	map_ptr->cfgid = cfgid;
	map_ptr->seq = seq;
	map_ptr->xsiz = xsiz;
	map_ptr->ysiz = ysiz;
	strcpysafe(map_ptr->string, sizeof(map_ptr->string), showstring);
	map_ptr->tile = tile;
	map_ptr->obj = obj;
	map_ptr->encount = encount;
	map_ptr->padding1 = -1;
	map_ptr->olink = olink;
	map_ptr->padding2 = -1;
	map_ptr->has_extended_data = has_extended_data;
	map_ptr->hit = hit;
	map_ptr->unknown1 = unknown1;
	map_ptr->unknown2 = unknown2;
	map_ptr->padding3 = -1;

	goto FCLOSERETURNTRUE;

FCLOSERETURNTRUE:
	MAP_mapnum_index[map_id]++;
	fclose(f);
	return TRUE;

FREEHIT:
	freeMemory(hit);
FREELINK:
	freeMemory(olink);
FREEENCOUNT:
	freeMemory(encount);
FREEOBJ:
	freeMemory(obj);
FREETILE:
	freeMemory(tile);
FREEOBJHP:
FCLOSERETURNFALSE:
	fclose(f);
	return FALSE;
}


bool MAP_readMapConfFile(char* filename)
{
	FILE* file;
	char    line[512];
	int     maximagenumber = 0;
	int     i;
	int     linenum = 0;
	int     imgdataindex = 0;

	file = fopen(filename, "r");
	if ( file == NULL ) {
		printf("Can't open %s\n", filename);
		return 0;
	}
	while ( fgets(line, sizeof(line), file) ) {
		char    imgnum[16];
		bool    ret;
		int     imgnumber;

		deleteSequentChar(line, " \t");
		pohcd(line, " \t");
		dchop(line, " \t");

		if ( line[0] == '#' )continue;
		if ( line[0] == '\n' )continue;
		chomp(line);

		ret = getStringFromIndexWithDelim(line, " ", 1, imgnum, sizeof(imgnum));
		if ( ret == FALSE )continue;
		imgnumber = atoi(imgnum);
		maximagenumber = max(maximagenumber, imgnumber);
	}

	MAP_imagedatanum = maximagenumber + 1;
	MAP_imagedata = (MAP_ImageData*)allocateMemory(sizeof(MAP_ImageData) * MAP_imagedatanum);
	if ( MAP_imagedata == NULL ) {
		fprint("Can't Allocate for map data. size=%d\n", MAP_imagedatanum);
		return FALSE;
	}
	for ( i = 0; i < arraysizeof(MAP_imgfilt); i++ )
		MAP_imgfilt[i] = -1;
	if ( fseek(file, 0, SEEK_SET) != 0 ) {
		fprint("Can't fseek to SEEK_SET %s\n", strerror(errno));
		return FALSE;
	}
	while ( fgets(line, sizeof(line), file) ) {
		char    token[64];
		int     imagenumber;
		int     ret;
		int     i;
		MAP_ImageData* one;
		int     offset;
		linenum++;

		deleteSequentChar(line, " \t");
		pohcd(line, " \t");
		chomp(line);
		if ( line[0] == '#' )continue;
		if ( line[0] == '\n' )continue;
		chomp(line);

		ret = getStringFromIndexWithDelim(line, " ", 1, token, sizeof(token));
		if ( ret == FALSE )continue;
		imagenumber = atoi(token);
		MAP_imgfilt[imagenumber] = imgdataindex;
		one = &MAP_imagedata[imgdataindex];
		imgdataindex++;

		MAP_makeDefaultImageData(one);
		offset = 4;
		for ( i = 0; i < MAP_DATAINT_NUM + MAP_DATACHAR_NUM; i++ ) {
			BOOL    ret;
			ret = getStringFromIndexWithDelim(line, " ", i + offset, token, sizeof(token));
			if ( ret == FALSE )continue;
			switch ( MAP_conf_entries[i].type ) {
			case MAP_INTENTRY:
				one->data[MAP_conf_entries[i].index]
					= atoi(token);
				break;
			case MAP_CHARENTRY:
				strcpysafe(one->string[MAP_conf_entries[i].index].string,
					sizeof(one->string[MAP_conf_entries[i].index].
						string),
					token);
				break;
			case MAP_INTFUNC:
			{
				using intfunction_t = int (*)(char*);
				intfunction_t intfunction = (intfunction_t)MAP_conf_entries[i].func;
				one->data[MAP_conf_entries[i].index]
					= intfunction(token);
				break;
			}
			case MAP_CHARFUNC:
			{
				using charfunction_t = char* (*)(char*);
				charfunction_t charfunction = (charfunction_t)MAP_conf_entries[i].func;

				strcpysafe(one->string[MAP_conf_entries[i].index].string,
					sizeof(one->string[MAP_conf_entries[i].index].
						string),
					charfunction(token));
				break;
			}
			default:
				break;
			}
		}
	}

	fclose(file);
	return TRUE;
}


//#define MAX_MAP_FILES 1200 // 地圖目錄最大檔案數
#define MAX_MAP_FILES 2000 // 地圖目錄最大檔案數

BOOL MAP_readMapDir(int map_id, char* dirname)
{
	int     mapfilenum = 0;
	STRING64    filenames[MAX_MAP_FILES];
	int     filenum;
	int     i;


	filenum = rgetFileName(dirname, filenames, arraysizeof(filenames));
	if ( filenum == -1 ) {
		fprint("[map] can't get filenames under %s directory\n", dirname);
		return FALSE;
	}

	xg_log("[map] dir %s total file count: %d\n", dirname, filenum);

	for ( i = 0; i < filenum; i++ )
		if ( MAP_IsMapFile(filenames[i].string) )
			mapfilenum++;

	print("[map] found %d valid maps\n", mapfilenum);

	if ( mapfilenum == 0 ) return FALSE;
	if ( !MAP_initMapArray(map_id, mapfilenum) ) {
		fprint("initMapArray Error\n");
		return FALSE;
	}

	FILE* f = fopen(config.mapdebugfile, "a");

	print("[map] loading maps\n", mapfilenum);
	for ( i = 0; i < filenum; i++ )
	{
		if ( MAP_IsMapFile(filenames[i].string) ) {
			BOOL res = MAP_readMapOne(map_id, filenames[i].string);
			print(".");
			if ( f )
			{
				char str[128];
				sprintf(str, "%d %s\n", res, filenames[i].string);
			}
		}

	}

	if ( f )
	{
		fclose(f);
	}

	xg_log("successfully loaded %d floors for map %d\n", MAP_mapnum_index[map_id], map_id);

	int floor_count = MAP_mapnum_index[map_id];
	if ( MAP_mapnum_index[map_id] == 0 ) {
		MAP_endMapArray();
		return FALSE;
	}

	if ( map_id == 0 )
	{
		MAP_mapnum[0] = MAP_mapnum_index[0];
	}
	else
	{
		//v14 = 1;
		//v15 = 4;
		floor_count = MAP_mapnum[map_id];
	}

	{
		int maxfloorid = 0;
		for ( i = 0; i < MAP_mapnum[map_id]; i++ )
		{
			if ( maxfloorid <= MAP_map[map_id][i].id )
			{
				maxfloorid = MAP_map[map_id][i].id;
			}
		}
		
		
		MAP_idtblsize[map_id] = maxfloorid + 1;
		MAP_idjumptbl[map_id] = (int*)allocateMemory(MAP_idtblsize[map_id] * sizeof(int));
		if ( MAP_idjumptbl[map_id] == 0 )
		{
			return FALSE;
		}
		for ( i = 0; i < MAP_idtblsize[map_id]; i++ )
		{
			MAP_idjumptbl[map_id][i] = -1;
		}
		for ( i = 0; i < MAP_mapnum[map_id]; i++ )
		{
			MAP_idjumptbl[map_id][MAP_map[map_id][i].id] = i;
		}
	}

	return TRUE;

}

int MAP_allocateMap(int map_id, char* dir_name)
{
	STRING64    filenames[MAX_MAP_FILES];
	uint32_t floor_count = 0;

	int file_count = rgetFileName(dir_name, filenames, MAX_MAP_FILES);

	if ( file_count == -1 )
	{
		xg_error("Can't get filenames under %s directory\n", dir_name);
		return 0;
	}
	int auto_dungeon_count = 0;

	for ( int i = 0; i < file_count; i++ )
	{
		if ( MAP_IsMapFile(filenames[i].string) )
		{
			auto_dungeon_count++;
		}
	}

	xg_log("%d auto dungeons found.\n", auto_dungeon_count); // [核心]随机地图数量为: %d\n

	if ( map_id == 1 )
		floor_count = config.auto_dungeon_num;
	else
		floor_count = config.house_num;

	if ( !MAP_initMapArray(map_id, floor_count) )
	{
		xg_error("initMapArray Error\n");
		return 0;
	}

	FILE* map_debug_file = fopen(config.mapdebugfile, "a");
	if ( file_count > 0 )
	{
		for ( int i = 0; i < file_count; i++ )
		{
			if ( MAP_IsMapFile(filenames[i].string) )
			{
				bool success = MAP_readMapOne(map_id, filenames[i].string);
				if ( map_debug_file )
				{
					char s[140];
					sprintf(s, "%d  %s\n", success, filenames[i].string);
					fputs(s, map_debug_file);
				}
			}

		}
	}

	if ( map_debug_file )
	{
		fclose(map_debug_file);
	}

	xg_log("successfully loaded %d maps.\n", MAP_mapnum_index[map_id]); // [核心]成功读取地图数量: %d\n

	if ( map_id == 0 )
	{
		MAP_mapnum[0] = MAP_mapnum_index[0];
	}

	MAP_idjumptbl[map_id] = (int*)allocateMemory(floor_count * sizeof(int));

	if ( !MAP_idjumptbl[map_id] )
	{
		return 0;
	}



	if ( MAP_idtblsize[map_id] > 0 )
	{
		for ( int i = 0; i < MAP_idtblsize[map_id]; i++ )
		{
			MAP_idjumptbl[map_id][i] = -1;
		}
	}

	if ( MAP_mapnum[map_id] )
	{
		for ( int i = 0; i < MAP_mapnum[map_id]; i++ )
		{
			int floor_id = MAP_map[i]->id;
			if ( floor_id != -1 )
			{
				MAP_idjumptbl[map_id][i] = floor_id;
			}
		}
	}

	return 1;
}

// StoneAge/Source/gmsv/init.c
int MAP_initReadMap()
{
	if ( !MAP_readMapConfFile(config.map_tile_file) )
		return 0;
	/*
	if ( !MAP_readDungeonConf() )
		return 0;
	if ( !MAP_initActiveDungeon() )
		return 0;
	if ( !MAP_initKillListDungeon() )
		return 0;
	*/
	FILE*  map_debug_file = fopen(config.mapdebugfile, "w");
	fclose(map_debug_file);
	xg_log("reading map directory.\n"); // [核心]初始化普通地图数据...
	if ( !MAP_readMapDir(0, config.map_dir) )
		return 0;
	xg_log("reading material map directory.\n"); // [核心]初始化地图素材数据...
	if ( !MAP_readMapDir(4, config.material_map_dir) )
		return 0;
	xg_log("reading auto dungeon map directory.\n"); // [核心]初始化随机地图...
	if ( !MAP_allocateMap(1, config.auto_dungeon_map_dir) )
		return 0;
	// MAP_printErrorMessage = 1;
	
	return 1;
}

#define CHECKFLOORID(id)	if( MAP_mapnum<=(id) || (id)<0 )return (-1);

INLINE int MAP_getfloorIndex( int mapid, int floorid )
{
    if ( 0 <= floorid && floorid <= MAP_idtblsize[mapid] )
        return MAP_idjumptbl[mapid][floorid];

    print( "readmap.c:[%d] err mapid[%d] floorid[%d]\n", __LINE__, mapid, floorid );
    return -1;
}

int MAP_getfloorX( int mapid, int floorid )
{
    int index;
    index = MAP_getfloorIndex( mapid, floorid );
    if ( index == -1 )return -1;
    return MAP_map[mapid][index].xsiz;
}

char* MAP_getfloorShowstring( int mapid, int floorid )
{
    int     index;
    index = MAP_getfloorIndex( mapid, floorid );
    if ( index == -1 )return NULL;
    return MAP_map[mapid][index].string;
}

int MAP_getfloorY( int mapid, int floorid )
{
    int index;
    index = MAP_getfloorIndex( mapid, floorid );
    if ( index == -1 )return -1;
    return MAP_map[mapid][index].ysiz;
}

BOOL MAP_checkCoordinates( int mapid, int floorid, int x, int y )
{
    if ( MAP_getfloorIndex( mapid, floorid ) == -1 ||
        x < 0 || x > MAP_getfloorX( mapid, floorid ) ||
        y < 0 || y > MAP_getfloorY( mapid, floorid ) )
        return FALSE;

    return TRUE;
}


#define MAP_GETMAXSIZE  45
static char MAP_dataString[STRINGBUFSIZ * 3];
static int MAP_workdatabuffer[MAP_GETMAXSIZE * MAP_GETMAXSIZE];
#define MAP_DATADELIMITER ","

char* MAP_getdataFromRECT( int mapid, int floor, RECT_SA* seekr, RECT_SA* realr )
{
    int floorindex;
    int stringlength = 0;
    char    tmpbuffer[128];
    int i, j;
    int     floorx;
    int databufferindex = 0;
    RECT_SA    scr;
    char    escapebuffer[128];

    floorindex = MAP_getfloorIndex( mapid, floor );
    if ( floorindex == -1 ) {
        print( "%s:%d:err\n", __FILE__, __LINE__ );
        return NULL;
    }
    if ( seekr->width < 0 || seekr->height < 0 ||
        MAP_GETMAXSIZE < seekr->width || MAP_GETMAXSIZE < seekr->height ) {
        print( "%s:%d:err\n", __FILE__, __LINE__ );
        return NULL;
    }
    snprintf( tmpbuffer, sizeof( tmpbuffer ),
        "%s|",
        makeEscapeString( MAP_map[mapid][floorindex].string,
            escapebuffer, sizeof( escapebuffer ) ) );

    strcpysafe( MAP_dataString, sizeof( MAP_dataString ), tmpbuffer );
    stringlength = strlen( tmpbuffer );

    floorx = MAP_map[mapid][floorindex].xsiz;
    scr.x = 0;
    scr.y = 0;
    scr.width = floorx;
    scr.height = MAP_map[mapid][floorindex].ysiz;

    if ( clipRect( seekr, &scr, realr ) == FALSE ) {
        realr->x = realr->y = realr->width = realr->height = 0;
        return MAP_dataString;
    }
    for ( i = realr->y; i < realr->y + realr->height; i++ ) {
        for ( j = realr->x; j < realr->x + realr->width; j++ ) {
            MAP_workdatabuffer[databufferindex++]
                = MAP_map[mapid][floorindex].tile[i * floorx + j];
        }
    }
    for ( i = 0; i < databufferindex; i++ ) {
        char	buf[64];
        snprintf( tmpbuffer, sizeof( tmpbuffer ), "%s" MAP_DATADELIMITER,
            cnv10to62( MAP_workdatabuffer[i], buf, sizeof( buf ) ) );
        strcpysafe( &MAP_dataString[stringlength],
            sizeof( MAP_dataString ) - stringlength,
            tmpbuffer );
        stringlength += strlen( tmpbuffer );
    }
    if ( stringlength < sizeof( MAP_dataString ) )
        MAP_dataString[stringlength - 1] = NONCHAR_DELIMITER[0];
    databufferindex = 0;
    for ( i = realr->y; i < realr->y + realr->height; i++ ) {
        for ( j = realr->x; j < realr->x + realr->width; j++ ) {
            MAP_workdatabuffer[databufferindex++]
                = MAP_map[mapid][floorindex].obj[i * floorx + j];
        }
    }
    for ( i = 0; i < databufferindex; i++ ) {
        char	buf[64];
        if ( MAP_workdatabuffer[i] == 0 ) buf[0] = '\0';
        else cnv10to62( MAP_workdatabuffer[i], buf, sizeof( buf ) );
        snprintf( tmpbuffer, sizeof( tmpbuffer ), "%s" MAP_DATADELIMITER,
            buf );
        strcpysafe( &MAP_dataString[stringlength],
            sizeof( MAP_dataString ) - stringlength,
            tmpbuffer );
        stringlength += strlen( tmpbuffer );
    }

    if ( stringlength < sizeof( MAP_dataString ) )
        MAP_dataString[stringlength - 1] = NONCHAR_DELIMITER[0];

    databufferindex = 0;
    for ( i = realr->y; i < realr->y + realr->height; i++ ) {
        for ( j = realr->x; j < realr->x + realr->width; j++ ) {
            int tile;
            int obj;
			if ( mapid != 3 || ( MAP_getTileAndObjData( 3u, floor, realr->x, realr->y, &tile, &obj ), tile != 0x735 ) )
			{
				if ( floor >= MAP_idtblsize[mapid] )
				{
                    // error
				}
				else
				{
					if ( floorindex != -1 )
					{
						MAP_Map* map = &MAP_map[mapid][floorindex];
						if ( map->xsiz > realr->x && map->ysiz > realr->y && realr->x >= 0 && realr->y >= 0 )
						{
                            OBJECT	object;
                            BOOL	found = FALSE;
                            for ( object = MAP_getTopObj( mapid, floor, j, i ); object;
                                object = NEXT_OBJECT( object ) ) {
                                int o = GET_OBJINDEX( object );
                                if ( OBJECT_getType( o ) == OBJTYPE_CHARA ) {
                                    int	etype;
                                    int charaindex = OBJECT_getIndex( o );
                                    if ( !CHAR_CHECKINDEX( charaindex ) )continue;
                                    etype = CHAR_getWorkInt( charaindex, CHAR_WORKEVENTTYPE );
                                    if ( etype != CHAR_EVENT_NONE ) {
                                        MAP_workdatabuffer[databufferindex++] = etype;
                                        found = TRUE;
                                        break;
                                    }
                                }
                                else if ( OBJECT_getType( o ) == OBJTYPE_NPCSCHARA ) {
									/*
                                    int	etype = OBJECT_getchartype( o );
                                    if ( etype != CHAR_EVENT_NONE ) {
                                        MAP_workdatabuffer[databufferindex++] = (unsigned short)etype;
                                        found = TRUE;
                                        break;
                                    }
									*/
                                    MAP_workdatabuffer[databufferindex++] = CHAR_EVENT_10;
                                    found = TRUE;
                                    break;
                                }
                            }
                            if ( !found ) {
								// error
                                MAP_workdatabuffer[databufferindex++] = CHAR_EVENT_NONE;
                            }
						}
                        MAP_workdatabuffer[databufferindex++] = '\0';
					}
				}
			}
			MAP_workdatabuffer[databufferindex++] = '\n';
        }
    }

    for ( i = 0; i < databufferindex; i++ ) {
        char	buf[64];
        if ( MAP_workdatabuffer[i] == 0 ) buf[0] = '\0';
        else cnv10to62( MAP_workdatabuffer[i], buf, sizeof( buf ) );

        snprintf( tmpbuffer, sizeof( tmpbuffer ), "%s" MAP_DATADELIMITER, buf );
        strcpysafe( &MAP_dataString[stringlength],
            sizeof( MAP_dataString ) - stringlength, tmpbuffer );
        stringlength += strlen( tmpbuffer );
    }
    dchop( MAP_dataString, MAP_DATADELIMITER );
    return MAP_dataString;
}

BOOL MAP_getTileAndObjData( int mapid, int floorid, int fx, int fy, int* tile, int* obj )
{
    int     floorindex, xsiz;
    floorindex = MAP_getfloorIndex( floorid );
    if ( floorindex == -1 ) {
        print( "%s:%d:err\n", __FILE__, __LINE__ );
        return FALSE;
    }
    xsiz = MAP_map[mapid][floorindex].xsiz;

    if ( fx < 0 || fx >= xsiz ) {
        return FALSE;
    }
    if ( fy < 0 || fy >= MAP_map[mapid][floorindex].ysiz ) {
        return FALSE;
    }

    if ( tile ) *tile = MAP_map[mapid][floorindex].tile[fy * xsiz + fx];
    if ( obj ) *obj = MAP_map[mapid][floorindex].obj[fy * xsiz + fx];

    return TRUE;
}

MAP_Objlink* _MAP_getTopObj( const char* file, int line, int mapid, int floor, int x, int y )
{
    int     floor_index;
    int     xsiz;

    floor_index = MAP_getfloorIndex( floor );
    if ( floor_index == -1 ) {
        //print( "callfrom[%s:%d] %s:%d:err floor(%d,%d,%d)\n", file, line, __FILE__, __LINE__,
        //	floor, x, y );
        return NULL;
    }

    xsiz = MAP_map[mapid][floor_index].xsiz;

#if 1
    if ( x >= xsiz ) return NULL;
    if ( y >= MAP_map[mapid][floor_index].ysiz ) return NULL;
    if ( x < 0 ) return NULL;
    if ( y < 0 ) return NULL;
    return MAP_map[mapid][floor_index].olink[y * xsiz + x];
#else
    if ( 0 <= x && x < xsiz && 0 <= y && y < MAP_map[floor_index].ysiz )
        return MAP_map[floor_index].olink[y * xsiz + x];
    else
        return NULL;
#endif
}