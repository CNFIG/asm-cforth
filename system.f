: bb bye ;
: true	1 ;
: false	0 ;
: u<	swap u> ;
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
: f	0 6 1 for i . next cr ;
: times	0 -1 ;
: ff	6 times for i . next cr ;
: fff	6 times 
	for 
		i 2 == if continue endif 
		i 4 == if break endif 
		i .  
	next cr ;

: fs	
6 times 
for 
	i dup .
	switch 
		dup 3 < if ." "i<3" break 
		dup 3 == if ." "i==3" break
		dup 3 > if ." "i>3" break 
	ends drop
next cr ;

: cell	4 ;
: lambda r> ;
: foo	." "I am foo" lambda ." "I am lambda" ;
: fn	0 >r ;
/*
>>>foo
>>>= fn
>>>fn
*/

: self
r> cell - dup 
r@ cell - @ swap !
>r ;

: recur sameAs self ;
: recurse sameAs self ;

: n1!	// n -- result
dup 1 == if ret endif
dup -- recur * ;

: tailSelf	dropr r> cell - >r ;
: tailRecur	 sameAs tailSelf ;

: n2!	// n 1 -- result
over 0 == if drops ret endif
over * --s  tailRecur ;

: loopn! // n 1 -- result
do
over 0 == if drops break endif
over * --s 
loop ;

: args	1 2 3 4 ;

: xx1		//n n n n -- r
+ + + ;

: xx2	//n n n n -- r
4 >>x
x1 x2 + x3 + x4 + 
4 xdrop ;
//>>>args xx2

: cur
r> dup cell + >r
@ >x 
-- times for x@ exec next
1 xdrop ;

/*
>>>( 1 2 3 4 ) cur +
DS> 10
>>>( 1 2 3 4 ) cur *
DS> 10 24
*/

: fib1	//n--r
dup 3 < if drop 1 ret endif
-- dup -- self swap self + ;

: fib2	//n--r
dup 3 < if drop 1 ret endif
>x 1 1 x> 2 - 
times for over + swap next drop ;

: timer
r> dup cell + >r @
timeStart exec timeEnd ." "time=" .f cr ;

: n // n 0 -- result
do
over 0 > while 
over + swap -- swap 
loop swap drop ;

: f fib1 ;
: test1 40 timer f  ;
: test2 100000000 0 timer n drop ;
