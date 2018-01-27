#include "nemu.h"
#include "cpu/reg.h"
#include "memory/memory.h"

#include <stdlib.h>
/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

extern uint32_t look_up_symtab(char *sym, bool *success);

CPU_STATE cpu;

enum {
	NOTYPE = 256, EQ,NEQ,NOT,AND,OR,HInt,DInt,REG,DEREF,NEG,SYM

	/* TODO: Add more token types */

};

static struct rule {
	char *regex;
	int token_type;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */

	{" +",	NOTYPE},				// white space
	{"\\+", '+'},
	{"\\-", '-'},
	{"\\*", '*'},
	{"\\/", '/'},
	{"0[xX][0-9a-fA-F]+", HInt},
	{"[0-9]+", DInt},
	{"\\$e[abcdsi][xip]",REG},
	{"\\(", '('},
	{"\\)", ')'},
	{"==", EQ},
	{"!=", NEQ},
	{"!",NOT},
	{"&&", AND},
	{"\\|\\|", OR},
	{"[a-zA-Z_][a-zA-Z0-9_]*", SYM}
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];

/* Rules are used for more times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
	int i;
	char error_msg[128];
	int ret;

	for(i = 0; i < NR_REGEX; i ++) {
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
		if(ret != 0) {
			regerror(ret, &re[i], error_msg, 128);
			assert(ret != 0);
		}
	}
}

typedef struct token {
	int type;
	char str[32];
} Token;

Token tokens[32];
int nr_token;


int power(int base, int index){
	int result=1;
	for(int i=0;i<index;i++)
		result*=base;
	return result;
}

static bool make_token(char *e) {
	int position = 0;
	int i;
	regmatch_t pmatch;
	
	nr_token = 0;

	while(e[position] != '\0') {
		/* Try all rules one by one. */
		for(i = 0; i < NR_REGEX; i ++) {
			if(regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;

				printf("match regex[%d] at position %d with len %d: %.*s\n", i, position, substr_len, substr_len, substr_start);
				position += substr_len;

				/* TODO: Now a new token is recognized with rules[i]. 
				 * Add codes to perform some actions with this token.
				 */

				int sum=0;
				switch(rules[i].token_type) {
					case HInt:tokens[nr_token].type = rules[i].token_type;
						  for(int j=2;j<substr_len;j++)
							if(substr_start[j]>='a'&&substr_start[j]<='f')
		                                        	sum+=power(16,substr_len-j-1)*(substr_start[j]-'a'+10);
							else if(substr_start[j]>='A'&&substr_start[j]<='F')
								sum+=power(16,substr_len-j-1)*(substr_start[j]-'A'+10);  
							else sum+=power(16,substr_len-j-1)*(substr_start[j]-'0'); 
       						  sprintf(tokens[nr_token].str,"%d",sum);
						  nr_token ++; break;
					case DInt:tokens[nr_token].type = rules[i].token_type;
						  for(int j=0;j<substr_len;j++)
						  	tokens[nr_token].str[j]=substr_start[j];//len must less than 32 bits! 
						  for(int j=substr_len;j<32;j++)
						  	tokens[nr_token].str[j]=0;//clear 0!
						  nr_token ++; break;
					case REG:tokens[nr_token].type = rules[i].token_type;
						  if(substr_start[1]=='e'&&substr_start[2]=='a'&&substr_start[3]=='x')
							sprintf(tokens[nr_token].str,"%08x",cpu.eax);
						  else if(substr_start[1]=='e'&&substr_start[2]=='c'&&substr_start[3]=='x')
							sprintf(tokens[nr_token].str,"%08x",cpu.ecx);
						  else if(substr_start[1]=='e'&&substr_start[2]=='d'&&substr_start[3]=='x')
							sprintf(tokens[nr_token].str,"%08x",cpu.edx);
 						  else if(substr_start[1]=='e'&&substr_start[2]=='b'&&substr_start[3]=='x')
							sprintf(tokens[nr_token].str,"%08x",cpu.ebx);
						  else if(substr_start[1]=='e'&&substr_start[2]=='s'&&substr_start[3]=='p')
							sprintf(tokens[nr_token].str,"%08x",cpu.esp);
						  else if(substr_start[1]=='e'&&substr_start[2]=='b'&&substr_start[3]=='p')
							sprintf(tokens[nr_token].str,"%08x",cpu.ebp);
						  else if(substr_start[1]=='e'&&substr_start[2]=='s'&&substr_start[3]=='i')
							sprintf(tokens[nr_token].str,"%08x",cpu.esi);
						  else if(substr_start[1]=='e'&&substr_start[2]=='d'&&substr_start[3]=='i')
							sprintf(tokens[nr_token].str,"%08x",cpu.edi);
						  else if(substr_start[1]=='e'&&substr_start[2]=='i'&&substr_start[3]=='p')
							sprintf(tokens[nr_token].str,"%08x",cpu.eip);
						  else
							tokens[nr_token].str[0]='0';
						  for(int j=0;j<8;j++)
							if(tokens[nr_token].str[j]>='a'&&tokens[nr_token].str[j]<='f')
		                                        	sum+=power(16,7-j)*(tokens[nr_token].str[j]-'a'+10); 
							else sum+=power(16,7-j)*(tokens[nr_token].str[j]-'0');  
       						  sprintf(tokens[nr_token].str,"%d",sum);
						  nr_token ++; break;
					case SYM: tokens[nr_token].type = rules[i].token_type;	
						  bool _b=1;
						  bool *success=&_b;
						  char *str=(char *)malloc(sizeof(char)*(substr_len));
						  int _i=0;
						  for(;_i<substr_len;_i++)
							str[_i]=substr_start[_i];
						  str[_i]='\0';
						  printf("\n%s\n",str);
						  sum=look_up_symtab(str,success);
						  free(str);
						  if(*success==false) {
							printf("No symbol matched with %s!\n",substr_start);
							return false;
						  }
						  sprintf(tokens[nr_token].str,"%d",sum);
						  nr_token ++; break;
					case NOTYPE: break;
					default: tokens[nr_token].type = rules[i].token_type;
							 nr_token ++;
				}
	
				break;
			}
		}

		if(i == NR_REGEX) {
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}
	}

	return true; 
}
bool check_parentheses(uint32_t p, uint32_t q){
	if(tokens[p].type=='('&&tokens[q].type==')'){
		int sum_left_bracket=0,sum_right_bracket=0;
		for(uint32_t i=p+1;i<=q-1;i++)
			if (tokens[i].type == '(')
				++sum_left_bracket;	
			else if (tokens[i].type == ')')
				++sum_right_bracket;
		if(sum_left_bracket==sum_right_bracket)
			return true;
		else return false;
	}
	else return false;
}

bool exp_is_legal(uint32_t p, uint32_t q){
	uint32_t sum_left_bracket=0,sum_right_bracket=0;
	for (int i = p; i<= q; i++){
		if (tokens[i].type == '(')
			++sum_left_bracket;
		else if (tokens[i].type == ')')
			++sum_right_bracket;
		if (sum_left_bracket<sum_right_bracket){
			return false;
		}
	}
	if (sum_left_bracket != sum_right_bracket)
		return false;
	else return true;
}

int eval(uint32_t p, uint32_t q) {
	if(p > q) {
		/* Bad expression */
		assert(0);
	}
	
	else if(p == q) {
		/* Single token.
		* For now this token should be a number.
		* Return the value of the number.
		*/
		return atoi(tokens[p].str);
	}

	else if(check_parentheses(p, q) == true) {
		/* The expression is surrounded by a matched pair of parentheses.
		* If that is the case, just throw away the parentheses.
		*/
		return eval(p + 1, q - 1);
	}

	else {
		uint32_t op=0;
		//op = the position of dominant operator in the token expression;
		uint32_t ope_loc[32];
		Token ope_tokens[32];
		uint32_t ope_num=0;
		for(int i=p;i<=q;i++)
			if(tokens[i].type=='('){//find the match ')'
				int sum_left_bracket=1,sum_right_bracket=0;
				int j ;
				for (j = i+1; sum_left_bracket>sum_right_bracket ; j++)
					if (tokens[j].type == '(')
						++sum_left_bracket;
					else if (tokens[j].type == ')')
						++sum_right_bracket;
				i=j-1;
			}
			else if(tokens[i].type=='+'||tokens[i].type=='-'||tokens[i].type=='*'||tokens[i].type=='/'
				||tokens[i].type==EQ||tokens[i].type==NEQ||tokens[i].type==AND||tokens[i].type==OR
				||tokens[i].type==DEREF||tokens[i].type==NEG||tokens[i].type==NOT){
				ope_loc[ope_num]=i;
				ope_tokens[ope_num]=tokens[i];
				ope_num++;
			}	
		bool flag=0;//if there is any "||"
		
		for(int j=0;j<ope_num;j++)
			if(ope_tokens[j].type==OR){
				op=ope_loc[j];//op is the last '||'
				flag=1;
			}

		if(flag==0)//continue
			for(int j=0;j<ope_num;j++)
				if(ope_tokens[j].type==AND){
					op=ope_loc[j];//op is the last '&&'
					flag=1;
				}

		if(flag==0)//now, next
			for(int j=0;j<ope_num;j++)
				if(ope_tokens[j].type==EQ||ope_tokens[j].type==NEQ){
					op=ope_loc[j];//op is the last '==' or '!='
					flag=1;
				}
		if(flag==0)//if not
		for(int j=0;j<ope_num;j++)
			if(ope_tokens[j].type=='+'||ope_tokens[j].type=='-'){
				op=ope_loc[j];//op is the last '+' or '-'
				flag=1;
			}
		if(flag==0)//if not again
			for(int j=0;j<ope_num;j++)
				if(ope_tokens[j].type=='*'||ope_tokens[j].type=='/'){
					op=ope_loc[j];//op is the last '*' or '/'
					flag=1;
				}
		
		if(flag==0){//if not again
			for(int j=0;j<ope_num;j++)
				if(ope_tokens[j].type==NEG||ope_tokens[j].type==DEREF||ope_tokens[j].type==NOT){
					op=ope_loc[j];//op is the first '*' or '-' or '!'
					break;
				}
		}
		int val1, val2;	
		if(tokens[op].type!=NEG&&tokens[op].type!=DEREF&&tokens[op].type!=NOT){
			val1 = eval(p, op - 1);
			val2 = eval(op + 1, q);
		}
		else{
			val1 = 0;
			val2 = eval(op + 1, q);
 		}
		switch(tokens[op].type) {
		case '+': return val1 + val2;
		case '-': return val1 -	val2;
		case '*': return val1 * val2;
		case '/': return val1 / val2;
		case EQ:  return val1 ==val2; 
		case NEQ: return val1 !=val2; 
		case AND: return val1 &&val2; 
		case OR:  return val1 ||val2; 
		case NEG: return -val2;
		case NOT: return !val2;
		case DEREF: return paddr_read(val2,4);
		//more
		default: assert(0);
		}
	}
}

uint32_t expr(char *e, bool *success) {
	if(!make_token(e)||!exp_is_legal(0,nr_token-1)) {
		*success = false;
		return 0;
	}
	//printf("\nPlease implement expr at expr.c\n");
	//assert(0);

	else {
		*success = true;
		int i;
		for(i = 0; i < nr_token; i ++) 
			if(tokens[i].type == '-' && (i == 0 || tokens[i - 1].type == '(') ) 
				tokens[i].type = NEG;		
		for(i = 0; i < nr_token; i ++) 
			if(tokens[i].type == '*' && (i == 0 || tokens[i - 1].type == '('
			|| tokens[i - 1].type == '-'|| tokens[i - 1].type == '+'
			|| tokens[i - 1].type == '*'|| tokens[i - 1].type == '/'
			|| tokens[i - 1].type == AND|| tokens[i - 1].type == OR
			|| tokens[i - 1].type == EQ || tokens[i - 1].type == NEQ
			|| tokens[i - 1].type == NEG|| tokens[i - 1].type == DEREF)) 
				tokens[i].type = DEREF;
		return eval(0,nr_token-1);
	}
}



