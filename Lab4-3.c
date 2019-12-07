#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#define MAXOP 100 //操作数或运算符大小
#define NUMBER '0'//表示找到数的信号
#define MAXVAL 100//栈VAL的最大深度
#define BUFSIZE 100

int sp = 0;
double val[MAXVAL];//值栈
char buf[BUFSIZ];
int bufp = 0 ;

int gettop ( char s[] );//取出栈顶元素
void push (double f);//入栈操作
double pop (void);//出栈操作
int getch(void);
void ungetch( int c);

int main (){
	int type;
	double op2;
	char s[MAXOP];

	while ( ( type = gettop( s )) != EOF ){//检查字符是不是停止符
		switch (type)//检查字符
		{
			case NUMBER:
				push( atof (s) ); //如果返回的是‘0’，将读取的字符转换成浮点数，并压入栈中
				break;
			
			case '+':
				push ( pop () + pop ());
				break;

			case '*':
				push (pop() * pop() );
				break;

			case '-':
				op2 = pop();
				push ( pop() - op2);
				break ;
			
			case '/':
				op2 = pop();
				if( op2 != 0)
					push (pop() / op2 );
				else printf("error : zero divisor\n");
				break;
			
			case '%':
				op2 = pop();
				if( op2 != 0)
					push (fmod (pop(),op2));
				else printf("error : zero divisor\n");
				break;
			
			case '\n':
				printf("result = %2.8g\n",pop());
				break;
			default:
				printf("unknown commond %s\n",s);
				break;
		}
	}
	return 0;
}

void push (double f){//把f下推到值栈中
	if ( f < MAXVAL)
		val[sp++] = f;// double val[MAXVAL] 为全局变量；
	else printf("error : stack full,can not push %g\n",f);
}

double pop (void){//弹出并返回栈顶的值
	if ( sp > 0)
		return val[--sp];
	else printf("error : stack empty!\n");
}

int gettop (char s[] ){//取下运算符或运算分量
	int i,c;

	while ( (s[0] = c = getch()) == ' ' || c == '\t' ) //如果读入的是空白符或者制表符，跳过字符
	 ;
	s[1] = '\0';//将数组作废
	if ( !isdigit(c) && c != '.' && c != '-') //如果读入的字符 不是数字或者小数点，返回字符
		return c;
	i = 0;
	if ( c == '-' ){
		if ( isdigit(s[++i] = c = getch() ) || c == '.')
			s[i] = c;
		else {
			if ( c != EOF )
				ungetch(c);
		return '-';
		}
	}	
	if ( isdigit(s[i]) ) 
		while ( isdigit(s[++i] = c = getch()) )
		;//如果读入的是数字，并且当下一个字符也是数字的时候 return '0';
	if ( c == '.' )
		while ( isdigit(s[++i] = c = getch()) )
		;//如果读取的是小数点，并且下一个字符是数字是 return '0';
	s[i] = '\0';//将数组标\0
	if ( c != EOF ) //如果字符不是文件结束符，调用unget(c)函数，将字符存到缓冲区
		ungetch( c );
	return NUMBER;
}

int getch(void){
	return (bufp > 0) ? buf[--bufp] : getchar() ;//如果缓冲数组中无值，即bufp = 0 时，getchar()读入一个字符，否则返回缓冲数组中存储的值。
}

void ungetch( int c){
	if ( bufp >= BUFSIZ)
		printf("ungetch : too many characters!\n");
	else buf[bufp++] = c ;
}