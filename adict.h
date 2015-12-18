struct _word
{
	struct _word * next;
	char*name;
	cell**addr;
};
typedef struct _word word;


//缓存区
#define	CMDSTR_LEN	1024
char cmdstr[CMDSTR_LEN];
cell*tmpList[CMDSTR_LEN/4];
cell** tmpLp=tmpList;


word*codeDictHead=NULL, *colonDictHead=NULL;

cell** code(char*s, cell** addr)
{
	word *w=(word*)malloc(sizeof(word));
	w->next=codeDictHead;
	codeDictHead=w;
	w->addr=addr;
	w->name=s;
	return w->addr;
}


#define wordNeck_len	5
char*word_call_addr;
//char*wordNeck;
void colon(char*s, cell** addr)
{
	word *w=(word*)malloc(sizeof(word));
	w->next=colonDictHead;
	colonDictHead=w;

	w->name=(char*)malloc(strlen(s)+1);
	strcpy(w->name,s);

	cell n=(cell)tmpLp-(cell)tmpList;
	w->addr=(cell**)malloc(wordNeck_len+n);

	//jmp to label call
	char*charP=(char*)w->addr; 
	*charP=0xE9;
	//计算跳转偏移地址
	*(cell*)(charP+1)=(cell)word_call_addr  -  wordNeck_len  -  (cell)(w->addr);

//	memcpy(w->addr, wordNeck, wordNeck_len);
	memcpy((charP+wordNeck_len), addr, n);
}
