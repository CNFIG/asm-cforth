struct _word
{
	char*name;
	char checkCode;
	cell**addr;
	struct _word * next;
};
typedef struct _word word;

//
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
	//compute jump adrress
	*(int*)(charP+1)=(int)word_call_addr - wordNeck_len - (int)(w->addr);

//	memcpy(w->addr, wordNeck, wordNeck_len);
	memcpy((charP+wordNeck_len), addr, n);

	
	w->next=colonDictHead;
	colonDictHead=w;
}

void markAddr(){ *(++XP)=(cell)tmpLp++;}

void _if()	{TMPLP_NEXT(zbranchh);	markAddr();}
void _endif()	{*(cell*)*XP=(cell)tmpLp-(cell)*XP;	XP--;}
void _else()
{
	TMPLP_NEXT(branchh);
	tmpLp++; _endif();
	tmpLp--; markAddr();
}

void _switch()	{TMPLP_NEXT(doo); *(++RP)=0; markAddr();}
void _case()	{TMPLP_NEXT(casee); markAddr();}
void _break()	{TMPLP_NEXT(breakk); if(*RP==0){_endif();} }
void _ends()
{
	TMPLP_NEXT((*RP==4?dropr44:droprr)); 
	tmpLp--; _endif(); tmpLp++;
	RP--;
}

void __do()	{TMPLP_NEXT(doo); *(++RP)=1; markAddr();}
void __loop()	{TMPLP_NEXT(loopp); _ends();}
void __for()	{TMPLP_NEXT(forr); *(++RP)=4; markAddr();}
void __next()	{TMPLP_NEXT(nextt); _ends();}
