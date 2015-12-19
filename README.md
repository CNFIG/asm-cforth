# asm-cforth
一个接近ASM的高效cforth

forth是一种堆栈机,使用后缀表达式,冒号是定义函数的符号,以栈为参数传递和传回返回值的通道, 在栈上运算数据

1 2 +

显示3

: x	if 22 22 else 33 33 endif ;

1 2 > x

显示 33 33

:s

switch 

	1 case 11 break 
	
	2 case 22 break
	
	3 case 33 break 
	
	default case 55 break
	
ends ;

1 s 显示11

2 s 显示22

switch的另一种用法


true switch

  1 2 < case 11 break
  
  1 2 == case 22 break
  
ends



system.f里只能编译,不能解释

