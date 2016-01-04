#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define	DEBUGER  CLOSE
#ifndef	DEBUGER 
#define	DEBUG(str)		printf("[DEBUG]ENTERING:%s\n",str);
#define	DEBUG2(str1,str2)	printf("[DEBUG]%s %s\n",str1,str2);
#else
#define	DEBUG(str)	
#define	DEBUG2(str1,str2)	
#endif 

#define	P(N)	printf(N);

typedef void(*fnp)();		//funcion pointer
#define	CELL	int		//only for 32bits

CELL cell=sizeof(CELL); 

#define	SYSPATH	".\\system.f"

#define	NEXT		TMPR=(CELL)*(IP++);goto *(CELL*)TMPR;
#define	_PUSH		*(++DP)=TOS;
#define	_POP		TOS=*(DP--);
#define	PUSH(N)		_PUSH; TOS=N;
#define	POP(N)		N=TOS; _POP;
#define	TMPLP_NEXT(N)	*(tmpLp++)=(CELL*)N;

char EOS=' ';//end of string

#define	STACK_LEN	256
CELL DS[STACK_LEN], RS[STACK_LEN], XS[STACK_LEN];//(data | return | X)stack
	CELL*RP;//stack pointer
	CELL*XP;//stack pointer	

CELL *_showSTACK;
char *new_name=NULL;


CELL**pushh;
CELL**zbranchh;
CELL**branchh;
CELL**torr;
CELL**casee;
CELL**droprr;
CELL**dropr44;
CELL**doo;
CELL**breakk;
CELL**loopp;
CELL**forr;
CELL**nextt;


#include "adict.h"

#include "str.h"

#define	dictNum	3
word * dict[dictNum];
void dictIndexInit()
{
	dict[0]=immeDictHead;
	dict[1]=codeDictHead;
	dict[2]=colonDictHead;
}

int search_word(char *w)
{
	char check_code;
	check_code=computeCheckCode(w);
	dictIndexInit();
	int d=0;
	for (; d<dictNum; d++)
	{
		if (dict[d]==NULL) break;
		do
		{
			if (check_code==dict[d]->checkCode && !strcmp(dict[d]->name,w))
			{
				DEBUG2("success find:",w)
				if (d==0)
					((fnp)(dict[d]->addr))();
				else
					TMPLP_NEXT(dict[d]->addr);
				return 1;
			}
		}while(dict[d]=dict[d]->next);
	}
				
	return 0;

}



int compile(char *s)
{
	s=ignore_blankchar( s);
	
	new_name=NULL;
	if (*s==':')
	{
		s++;
		new_name=ignore_blankchar(s);
		s=new_name;
		s=split_word(s);
		s=ignore_blankchar(s);
	}
	char *w, *charp;
	int len;
	while (*s!=0)
	{
		w=s;
		if (*s=='/')
		{
			if (*(s+1)=='/')
			{
				do{
					s++;
				}while (!(*s==10 || *s==13));
			}
			else if(*(s+1)=='*')
			{
				do{
					s++;
				}while(!(*s=='/' && *(s-1)=='*'));
				s++;
			}
			
			s=ignore_blankchar(s);
			w=s;
		}
		else if(*w=='"')
		{
			++w;
			while(1)
			{
				++s;
				if(*s=='"' && *(s+1)==' ')
				{
					 *(++s)='\0';
					 ++s;
					 break;
				}
			}
			len=strlen(w);
			charp=(char*)malloc(len+1);
			strcpy(charp,w);
			TMPLP_NEXT((CELL**)charp);
			*(charp+len-1)='\0';
			continue;
		}
		s=split_word(s);

		if(!search_word(w) )
		{
			if (is_num(w))
			{//change to number
				TMPLP_NEXT(pushh);
				TMPLP_NEXT(atoi(w));
			}
			else
			{
				printf("[%s]?\n",w);
				return 0;
			}
		}
		s=ignore_blankchar(s);
	}
	*tmpLp=_showSTACK; 

	if (new_name!=NULL)
		colon(new_name,tmpList);
	tmpLp=tmpList;
	return 1;
}


void checkcmd(char*s)
{
	strcat(cmdstr," ;");
	s=ignore_blankchar(s);
	char c=*s;
	while ( *s!=';')
		s++;
	if (c==':') 
		s++;
	*s=0;
}


int main() 
{
pushh	=&&push;
zbranchh=&&zbranch;
branchh	=&&branch;
torr	=&&tor;
casee	=&&_casee;
droprr	=&&dropr;
dropr44	=&&dropr4;
doo	=&&_do;
breakk	=&&_break;
loopp	=&&_loop;
forr	=&&_for;
nextt	=&&_next;

	register CELL TMPR=0;	
	register CELL TOS;	
	register CELL** IP;
	register CELL*DP;//stack pointer

	_showSTACK=&&showSTACK;
	word_call_addr=(&&call);
//	wordNeck=(&&_wordNeck);


	code("push",&&push);
	code("bye",&&bye);
	code("words",&&words);
	code("timeStart",&&timeStart);
	code("timeEnd",&&timeEnd);
	code("malloc",&&_malloc);


//	code("fib",&&fib);
	
	code(".f",&&printfloat);
	code(".\"",&&printstr);
	code(".",&&printnum);


	code("sameAs",&&sameAs);
	code("exec",&&exec);

	code("i",&&_i);
	code("continue",&&_continue);
	code("while",&&_while);
	code("_loop",&&_loop);

	code("branch",&&branch);
	code("0branch",&&zbranch);

	code("c!",&&cwrite);
	code("c@",&&cread);
	code("!",&&write);
	code("@",&&read);

	code("u>",&&uabove);
	code(">",&&above);
	code("<",&&below);
	code("==",&&equ);
	code("!=",&&nequ);

	
	code(">r",&&tor);
	code("r>",&&rto);
	code("r@",&&rat);
	code("dropr",&&dropr);
	code("4dropr",&&dropr4);

	code(">x",&&tox);
	code("x>",&&xto);
	code("x@",&&xat);
	code("xdrop",&&xdrop);
	code(">>x",&&varx);
	code("x4",&&x4);
	code("x3",&&x3);
	code("x2",&&x2);
	code("x1",&&xat);

	code("(",&&parenl);
	code(")",&&parenr);
	code("=",&&assign);
	code("+",&&add);
	code("-",&&sub);
	code("*",&&mul);
	code("/",&&divv);
	code("++s",&&adds1);
	code("--s",&&subs1);
	code("++",&&add1);
	code("--",&&sub1);


	code("ret",&&ret);
	code(";",&&ret);

	code("swaps",&&swaps);
	code("swap",&&swap);
	code("drops",&&drops);
	code("drop",&&drop);
	code("over",&&over);
	code("dups",&&dups);
	code("dup",&&dup);



	//immeDict
	immediate("if",(CELL**)_if);
	immediate("endif",(CELL**)_endif);
	immediate("else",(CELL**)_else);
	immediate("switch",(CELL**)_switch);
	immediate("case",(CELL**)_case);
	immediate("break",(CELL**)_break);
	immediate("ends",(CELL**)_ends);
	immediate("do",(CELL**)__do);
	immediate("loop",(CELL**)__loop);
	immediate("for",(CELL**)__for);
	immediate("next",(CELL**)__next);


	DP=DS-1;
	RP=RS-1;
	XP=XS-1;
	tmpLp=tmpList;

	FILE*fp;
	char ch=0;
	char*chp;
	char *loadInf="succeed";
loadsys:
	fp=fopen(SYSPATH,"r");
	if (fp==NULL)
		loadInf="FAIL";
	else
	{
		while(1)
		{
			while(ch!=':' && ch!=EOF)
			{
				ch=fgetc(fp);
		//		putchar(ch);
			}
			
			if(ch==EOF) break;

			chp=cmdstr;
			while (1)
			{
				*chp=ch;
				ch=fgetc(fp);
			//	putchar(ch);
				if (*chp==';' && (is_blankchar(ch) || ch==EOF) )
				{
					*(chp+1)='\0';
					if (compile(cmdstr))
						break;
					
					loadInf="FAIL";
					goto fileclose;					
				}
				chp++;
			}
			//if(ch==EOF) break;
		}
	}
fileclose:
	fclose(fp);
	printf("\n-------------------------");
	printf(loadInf); printf(" to load system");
	printf("-------------------------\n");
	printf("asm-cforth version 0.1------made by ear\nplease input 'words' to see the dictionary\n");
init:
	DP=DS-1;
	RP=RS-1;
	XP=XS-1;
	tmpLp=tmpList;
	DEBUG("INIT()")
//	DS[0]=123;DS[1]=456;DP=DS+2;

showSTACK:
	printf("DS> ");
	CELL*i;
	i=DS+1;
	if (DP>=DS)
	{
		while (i<=DP)
			printf("%d ",*i++);
		printf("%d ",TOS);
	}
	printf("\n");
/*
	printf("tmplist> ");
	CELL** j=tmpList;
	while (*j != (&&showSTACK) )
		printf("%d ",*j++);
	printf("\n");
//*/
/*
	printf("RS> ");
	CELL *k=RS;
	for (;k<=RP ;k++ )
		printf("%d ",*k);
	printf("\n");
//*/
/*
	printf("XS> ");
	CELL *L=XS;
	for (;L<=XP ;L++ )
		printf("%d ",*L);
	printf("\n");
//*/

cmd_line:
	printf(">>>");gets(cmdstr);
	checkcmd(cmdstr);
	if (!compile(cmdstr))
		goto init;
	else 
	{
		if (new_name !=NULL)
		//	goto cmd_line;
			goto showSTACK;

		DEBUG("entering: explain")
		IP=tmpList;
		NEXT
	}






//DATA STACK OPERATE 
push:	DEBUG("push")
	PUSH((CELL)*IP++)	NEXT
dup:	DEBUG("dup")
	_PUSH			NEXT
over:	PUSH(*(DP-1))		NEXT
drop:	_POP			NEXT
drops:	DP--;			NEXT
dups:	TMPR=*DP;*(++DP)=TMPR;			NEXT
swap:	TMPR=*DP; *DP=TOS; TOS=TMPR;		NEXT
swaps:	TMPR=*DP; *DP=*(DP-1); *(DP-1)=TMPR;	NEXT
//RETURN STACK
tor:	POP(*(++RP))		NEXT
rto:	PUSH(*RP--)		NEXT
rat:	PUSH(*RP)		NEXT
dropr:	RP--;			NEXT
dropr4: RP-=4;			NEXT
//X STACK
tox:	POP(*(++XP))		NEXT
xto:	PUSH(*XP--)		NEXT
xat:	PUSH(*XP)		NEXT
//+*-/=
add1:	DEBUG("++") TOS++;	NEXT
sub1:	DEBUG("--") TOS--;	NEXT
adds1:	DEBUG("++s")(*DP)++;	NEXT
subs1:	DEBUG("--s")(*DP)--;	NEXT
add:	TOS+=(*DP--);	NEXT
mul:	TOS*=(*DP--);	NEXT
sub:	TOS=(*DP--)-TOS;	NEXT
divv:	if (!TOS){printf("error: 0 / \n");goto init;}
	TOS=(*DP--)/TOS;	NEXT

assign:	*(CELL*)((CELL)*IP+9)=TOS; _POP; IP++; NEXT

write:	DEBUG("!")
	*(CELL*)TOS=*DP--; _POP; NEXT
read:	DEBUG("@")
	TOS=*(CELL*)TOS; NEXT

cwrite:	DEBUG("c!")
	*(char*)TOS=(char)*DP--; _POP; NEXT
cread:	DEBUG("c@")
	TOS=(CELL)*(char*)TOS; NEXT

branch:
	TMPR=(CELL)(*(IP));
	IP=(CELL**)( (CELL)IP+TMPR );
//	IP=(CELL**)( (CELL)IP+(CELL)(*(IP)) );
	NEXT

zbranch:	TMPR=TOS;
			_POP
			if(TMPR)
zbranch1:		IP++;
			else
zbranch2:		goto branch;
			NEXT

_casee:	DEBUG("case")
	if(*DP==TOS)
		{DP--; _POP; goto zbranch1;}
	else
		{_POP; goto branch;}

_break:	DEBUG("break")
	IP=(CELL**)*RP; IP--; goto branch;

_continue: DEBUG("continue")
	IP=(CELL**)*RP; IP--;
	IP=(CELL**)( (CELL)IP+(CELL)(*(IP))-cell );
	NEXT

_do:	DEBUG("do")
	*(++RP)=(CELL)(++IP); NEXT

_loop:	DEBUG("loop")
	IP=(CELL**)*RP; NEXT

_while:	DEBUG("while")
	TMPR=TOS; _POP 
	if(TMPR==0) goto _break;
	NEXT

_for:	DEBUG("for")//*
	if( *(DP-1)==*DP ) 
	{
		DP-=2; _POP; RP+=4; goto branch;
		//IP=(CELL**)( (CELL)IP+(CELL)(*(IP))+cell ); NEXT
	}//*/
	*(++RP)=TOS; _POP;
	*(++RP)=TOS; _POP;
	*(++RP)=TOS; _POP;
	goto _do;

_next:	DEBUG("next") //goto showSTACK;
	*(RP-1)+=*(RP-3);
	if(*(RP-1)!=*(RP-2))
		goto _loop;
	NEXT

_i:	_PUSH; TOS=*(RP-1); NEXT

_malloc:
	TOS=(CELL)malloc(TOS);
	if (TOS) NEXT;

	printf("malloc error\n");
	goto init;

parenl:	
	*(++RP)=(CELL)DP;
	NEXT
parenr:
	_PUSH;
	TOS=((CELL)DP-1-*(RP--))/cell;
	NEXT
varx:
//	TMPR=TOS;
	while (TOS--)
	{
		*(++XP)=*(DP--);
	}
//	*(++XP)=TMPR;
	_POP; NEXT
xdrop:
	XP-=TOS; _POP; NEXT
x2:
	PUSH(*(XP-1)); NEXT
x3:
	PUSH(*(XP-2)); NEXT
x4:
	PUSH(*(XP-3)); NEXT


//cmp sign
nequ:	TOS-=*DP--;	NEXT
equ:	TOS-=*DP--; TOS= !TOS;	NEXT
above:	TOS=((*DP--)>TOS);	NEXT
below:	TOS=((*DP--)<TOS);	NEXT
uabove:	TOS=((unsigned CELL)*(DP--) > (unsigned CELL)TOS);
	NEXT


exec:	TMPR=TOS; _POP; goto *(CELL*)TMPR;
sameAs:	TMPR=(CELL)*IP; IP=(CELL**)*RP--; goto *(CELL*)TMPR;

printstr:DEBUG("printstr")
	printf("%s",(char*)*IP++);
	NEXT
printnum:DEBUG("printnum")
	printf("%d ",TOS); _POP;
	NEXT

	float*np;
printfloat:DEBUG("printfloat")
	_PUSH
	np=(float*)DP;
	printf("%f ",*np);
	--DP;
	_POP
	NEXT

words:	dictIndexInit();
	int d;
	for (d=0;d<dictNum ; d++)
	{
		if (dict[d]==NULL) break;
		do{
			printf("[%d]%s ",dict[d]->checkCode,dict[d]->name);
		} while(dict[d]=dict[d]->next);
		printf("\n");
	}
	NEXT
		
 
timeStart:
	*(++XP)=clock();
	NEXT;
timeEnd:
	_PUSH;
	*(float*)(++DP)=((float)(clock()-*(XP--))/CLK_TCK) ;
	_POP;
	NEXT;

//fib:TOS=Fib_Common(TOS);NEXT



//_wordNeck:	goto *(int*)word_call_addr;
ret:	DEBUG("entering: ret")	
	IP=(CELL**)*RP--;	NEXT

call:	DEBUG("entering: call")
	*(++RP)=(CELL)IP;
	IP=(CELL**)( TMPR+ wordNeck_len);
	NEXT

bye:	return 0;
}
