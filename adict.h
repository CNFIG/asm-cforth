struct _word
{
	struct _word * next;
	char*name;
	char checkCode;
	cell**addr;
};
typedef struct _word word;


//缓存区
#define	CMDSTR_LEN	1024
char cmdstr[CMDSTR_LEN];
cell*tmpList[CMDSTR_LEN/4];
cell** tmpLp=tmpList;


word *immeDictHead=NULL, *codeDictHead=NULL, *colonDictHead=NULL;

char computeCheckCode(char*s)
{
	char code=0;
	while (*s)
		code^=*s++;
	return *(--s)+code;
}

word * creatWord(char*s, cell** addr)
{
	word *w=(word*)malloc(sizeof(word));
	w->checkCode=computeCheckCode(s);
	w->name=s;
	w->addr=addr;
	return w;
}

void immediate(char*s, cell** addr)
{
	word *w=creatWord(s,addr);
	w->next=immeDictHead;
	immeDictHead=w;
}

cell** code(char*s, cell** addr)
{
	word *w=creatWord(s,addr);
	w->next=codeDictHead;
	codeDictHead=w;
	return w->addr;
}


#define wordNeck_len	5
char*word_call_addr;
//char*wordNeck;
void colon(char*s, cell** addr)
{
	word *w=(word*)malloc(sizeof(word));
	w->checkCode=computeCheckCode(s);

	w->name=(char*)malloc(strlen(s)+1);
	strcpy(w->name,s);

	cell n=(cell)tmpLp-(cell)tmpList;
	w->addr=(cell**)malloc(wordNeck_len+n);

	//jmp to label call
	char*charP=(char*)w->addr; 
	*charP=0xE9;
	//计算跳转偏移地址
	*(int*)(charP+1)=(int)word_call_addr - wordNeck_len - (int)(w->addr);

//	memcpy(w->addr, wordNeck, wordNeck_len);
	memcpy((charP+wordNeck_len), addr, n);

	
	w->next=colonDictHead;
	colonDictHead=w;
}
