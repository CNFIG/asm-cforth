int is_blankchar(char c)	{ return (c==' ' || c=='	' || c==10 || c==13 ); }

char * ignore_blankchar(char *s)
{
	while (is_blankchar(*s))
		s++;
	return s;
}
char * until_wordend(char *s)
{
	while ( !is_blankchar(*s)  && *s!='\0')
		s++;
	return s;
}

char * split_word(char *s)
{
	s=until_wordend(s);
	if (*s=='\0')
		return s;
	*s='\0';
	s++;
	return s;
}

int is_num(char *s)
{
	while (*s !=0)
	{
		if (!isdigit(*s)) return 0;
		s++;
	}
	return 1;
}
