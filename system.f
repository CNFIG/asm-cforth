: true	1 ;
: false	0 ;
: u<	swap u> ;
: <	swap > ;
: default drop ;
: do	loop ;
: x	if 22 22 else 33 33 endif ;
: s
switch 
	1 case 11 break 
	2 case 22 break
	3 case 33 break 
	default 55 
ends ;
