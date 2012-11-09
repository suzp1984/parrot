%{
#include "main.hpp"
#include "frame.tab.h"
extern "C" {
int yywrap(void);
int yylex(void);
}
%}
%x _INCLUDE_
%x _INCLUDE_FILE_
%%
"#"[\t]*"include" {
	BEGIN _INCLUDE_;
	yylval.clear();
	return INCLUDE;
}
<_INCLUDE_>[\"|<] {
	BEGIN _INCLUDE_FILE_;
	return *yytext;
}
<_INCLUDE_FILE_>[^\">]* {
	yylval.headerfile += yytext;
	return HEADERFILE;
}
<_INCLUDE_FILE_>[\"|>] {
	BEGIN INITIAL;
	return *yytext;
}
[\t\n] ;
%%
int yywrap(void)
{
        return 1;
}
