#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define	DEBUGER  CLOSE
#ifndef	DEBUGER 
#define	DEBUG(str)	printf("[DEBUG]%s\n",str);
#define	DEBUG2(str1,str2)	printf("[DEBUG]%s %s\n",str1,str2);
#else
#define	DEBUG(str)	
#define	DEBUG2(str1,str2)	
#endif 


#ifdef _WIN64
#define	cell	long long	//for 64bits
#else
#define	cell	long		//for 32bits
#endif 

cell CELL=sizeof(cell); 

#define	SYSPATH	".\\system.f"

#define	NEXT		tmpReg=(cell)*(IP++);goto *(cell*)tmpReg;//
#define	_PUSH		*(++DP)=TOS;
#define	_POP		TOS=*(DP--);
#define	PUSH(X)		_PUSH; TOS=X;
#define	POP(X)		X=TOS; _POP;


#define	STACK_LEN	100
cell DS[STACK_LEN], RS[STACK_LEN], XS[STACK_LEN];//(data | return | X)stack
cell *_showSTACK;
char *new_name=NULL;
cell**pushh;



#include "adict.h"

#include "str.h"



int search_word(char *w)
{
	word * dict=dictHead;
	while (strcmp(dict->name,w))
	{
		dict=dict->next;
		if(dict==NULL)
		{
			if (!is_num(w))	 return 0;
			//转换成数字
			*tmpLp=(cell*)pushh;	tmpLp++;
			*tmpLp=(cell*)atoi(w);	tmpLp++;
			return 1;			
		}
	}DEBUG2("success find:",w)
	
	*tmpLp=(cell*)(dict->addr);	tmpLp++;
	return 1;
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
	}
	char *w;
	while (*s!=0)
	{
		w=s;
		s=split_word(s);

		if(!search_word(w) )
		{
			printf("[%s]?\n",w);
			return 0;
		}
		s=ignore_blankchar(s);
	}
	*tmpLp=_showSTACK; 

	if (new_name!=NULL)
		colon(new_name,tmpList);
	tmpLp=tmpList;//临时区复原
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
	register cell tmpReg=0;	
	register cell TOS;	
	register cell** IP;
	register cell*DP;//stack pointer
	cell*RP;//stack pointer
	cell*XP;//stack pointer

	_showSTACK=&&showSTACK;
	dictHead=NULL;
	word_call_addr=(&&call);
//	wordNeck=(&&_wordNeck);




pushh=code("push",&&push);
	code("bye",&&bye);
	code("words",&&words);
	
	code(">r",&&tor);
	code("r>",&&rto);
	code("r@",&&rat);
	code("dropr",&&dropr);

	code(">x",&&tox);
	code("x>",&&xto);
	code("x@",&&xat);
	code("dropx",&&dropx);

	code("+",&&add);
	code("-",&&sub);
	code("*",&&mul);
	code("/",&&divv);




	code("ret",&&ret);
	code(";",&&ret);


	code("swaps",&&swaps);
	code("swap",&&swap);
	code("drops",&&drops);
	code("drop",&&drop);
	code("over",&&over);
	code("dup",&&dup);




	FILE*fp;
	char *loadInf="succeed";
loadsys:	
	fp=fopen(SYSPATH,"r");
	if (fp==NULL)
		loadInf="FAIL";
	else
	{
		while (fgets(cmdstr,CMDSTR_LEN,fp))
		{
			printf("%s\n",cmdstr);
			checkcmd(cmdstr);
			if (!compile(cmdstr))
			{
				loadInf="FAIL";
				break;
			}
		}
	}
	fclose(fp);
	printf("-------------------------");
	printf(loadInf);	 printf(" to load system");
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
	cell*i;
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
	cell** j=tmpList;
	for (;j<=tmpLp+5 ;j++ )
		printf("%d ",*j);
	printf("\n");
//*/
/*
	printf("RS> ");
	cell *k=RS;
	for (;k<=RP ;k++ )
		printf("%d ",*k);
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
			goto cmd_line;

		DEBUG("entering: explain")
		IP=tmpList;
		NEXT
	}






//DATA STACK OPERATE 
push:	PUSH((cell)*IP++)	NEXT
dup:	DEBUG("entering: dup")
	_PUSH			NEXT
over:	PUSH(*(DP-1))	NEXT
drop:	_POP			NEXT
drops:	DP--;			NEXT
swap:	tmpReg=*DP; *DP=TOS; TOS=tmpReg;		NEXT
swaps:	tmpReg=*DP; *DP=*(DP-1); *(DP-1)=tmpReg;	NEXT
//RETURN STACK
tor:		POP(*(++RP))		NEXT
rto:		PUSH(*RP--)		NEXT
rat:		PUSH(*RP)		NEXT
dropr:	RP--;			NEXT
//X STACK
tox:		POP(*(++XP))		NEXT
xto:		PUSH(*XP--)		NEXT
xat:		PUSH(*XP)		NEXT
dropx:	XP--;			NEXT
//+*- /
add:		TOS+=(*DP); DP--;	 	NEXT
mul:		TOS*=(*DP); DP--;		NEXT
sub:		TOS=(*DP)-TOS; DP--;	NEXT
divv:		TOS=(*DP)/TOS; DP--;	NEXT




		word* wtmp;
words:	wtmp=dictHead;
		do{
			printf("%s ",wtmp->name);
		} while(wtmp=wtmp->next);
		printf("\n");
		NEXT
		




//_wordNeck:	goto *(int*)word_call_addr;
ret:		IP=(cell**)*RP--;	DEBUG("entering: ret")	NEXT

call:		*(++RP)=(cell)IP;
		IP=(cell**)( tmpReg+ wordNeck_len);
		DEBUG("entering: call")
		NEXT

bye:		return 0;
}
