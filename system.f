:x	if 22 22 else 33 33 endif ;
:true	1 ;
:false	0 ;
:u<	swap u> ;
:<	swap > ;
:default sameAs r@ ;
:s
switch 
	1 case 11 break 
	2 case 22 break
	3 case 33 break 
	default case 55 break
ends ;
