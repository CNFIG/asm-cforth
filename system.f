: true	1 ;
: false	0 ;
: u<	swap u> ;
: <	swap > ;
: default drop ;
: s
switch 
	1 case 11 break 
	2 case 22 break
	3 case 33 break 
	default 55 
ends ;
: x	if  11 ." "YES SIR" else ." "NO SIR" 22 endif 55 ;
: xx	." 
"I can say : "hello world"
Then I say : "bye world"" ;
: d	5 do -- dup while ." "looping" loop ;
: cr	." 
"
" ;
: times	0 swap 1 ;
: ff	6 times for i . next cr ;
: f	 for i . next cr ;
: fff	 6 times 
	for 
		i 2 == if continue endif 
		i 4 == if break endif 
		i .  
	next cr ;

: cell	4 ;
: lambda r> ;
: foo	." "I am foo" lambda ." "I am lambda" ;
: fn	0 >r ;

: recur
r> cell - dup 
r@ cell - @ swap !
>r ;

: n1!	// n -- result
dup 1 == if ret endif
dup -- recur * ;

: tailRecur	dropr r> cell - >r ;

: n2!	// n 1 -- result
over 0 == if drops ret endif
over * --s  tailRecur ;
