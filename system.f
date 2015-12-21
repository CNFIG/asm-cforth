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
"I can say : "hello world".
Then I say : "bye world"" ;
: d	5 do -- dup while ." "looping" loop ;
