%{

	#include <stdio.h>

	enum yytokentype {
	WRD = 1,
	SPC,
	PLS1,
	PLS2
	};

	#define TRUE 1
	#define FALSE 0

	#define UNDEF '-'

	char buffer[1024];
	int len = 0;

	char sspace = UNDEF;
	char espace = UNDEF;

	long value = 0;

	void printValue()
	{
		if(sspace != UNDEF)
			printf("%c", sspace);
		printf("%ld", value);
		value = 0;
		if(espace != UNDEF)
			printf("%c", espace);

		espace = UNDEF;
		sspace = UNDEF;
	}
%}

WS [\t\n ]
NUM [0-9]+


%s ADD1
%s ADD2

%%

<INITIAL,ADD2>{WS}{NUM}{WS}+"+"{WS}+{NUM}{WS} {
	// printf("Integer add: >>%s<<\n", yytext);
	BEGIN(ADD1);

	long vala, valb;
	sscanf(yytext, "%ld + %ld", &vala, &valb);
	// printf("--%ld-- + -- %ld --\n", vala, valb);
	value = vala + valb;

	sspace = yytext[0];
	espace = yytext[yyleng - 1];

	// printf(">>>%c<<<", espace);
	return PLS1;
}

<INITIAL>^{NUM}{WS}+"+"{WS}+{NUM}{WS} {
	// printf("Start Integer add2: >>%s<<\n", yytext);
	BEGIN(ADD1);

	long vala, valb;
	sscanf(yytext, "%ld + %ld", &vala, &valb);
	// printf("--%ld-- + -- %ld --\n", vala, valb);
	value = vala + valb;

	sspace = UNDEF;
	espace = yytext[yyleng - 1];
	return PLS1;
}

<ADD1>{WS}*"+"{WS}+{NUM}{WS} {
	// printf("Integer add: >>%s<<\n", yytext);
	BEGIN(ADD1);

	long vala;
	sscanf(yytext, "%*[ \n\t+] %ld ", &vala);
	// printf(" + -- %ld --\n", vala);
	value += vala;

	espace = yytext[yyleng - 1];

	return PLS2;
}

<ADD1>. {
	// printf("Val printing: >>%s<<\n", yytext);
	printValue();
	printf("%s", yytext);	
	BEGIN(ADD2);
}

<ADD1>{WS} {
	// printf("Val printing2: >>%s<<\n", yytext);
	printValue();
	printf("%s", yytext);
	BEGIN(ADD2);
	return SPC;
}

<ADD1><<EOF>> {
	// printf("EOF printing\n");
	printValue();
	return 0;
}

{WS} {
	// printf("Whitespace\n");
	// printf(">>%s<<\n", yytext);
	printf("%s", yytext);
	return SPC;
}

<<EOF>> {
	// printf("EOF");	
	return 0;
}

. {
	// printf("Word\n");
	// printf(">>%s<<\n", yytext);	
	printf("%s", yytext);
	return WRD;
}

%%

int main(int argc, char **argv)
{
	yyin = stdin;	
	int curtok = 0;

	while(curtok = yylex() > 0)
	{
		// printf("curtok: %d\n", curtok);
	}
}
