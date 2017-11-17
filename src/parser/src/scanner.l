%{
	#include <iostream>
	#include <cstdlib>
	#include "scanner.h"
	#include "interpreter.h"
	#include "parser.hpp"
	#include "location.hh"

       //typedef EzAquarii::Parser::token token;
       //typedef EzAquarii::Parser::token_type token_type;

	using namespace std;

	#define yyterminate() EzAquarii::Parser::make_END(EzAquarii::location());

	// This will track current scanner location.
	// Action is called when length of the token is known.
	#define YY_USER_ACTION m_driver.increaseLocation(yyleng);	
%}

%option nodefault
%option noyywrap
%option c++
%option yyclass="Scanner"
%option prefix="EzAquarii_"

%%


\(          {
                return EzAquarii::Parser::make_LEFTPAR(EzAquarii::location());
            }
            
\)          { 
                //cout << "Scanner: ')'" << endl;
                return EzAquarii::Parser::make_RIGHTPAR(EzAquarii::location());
            }
            
;           {
                //cout << "Scanner: ';'" << endl;
                return EzAquarii::Parser::make_SEMICOLON(EzAquarii::location());
            }
            
,           {
                //cout << "Scanner: ','" << endl;
                return EzAquarii::Parser::make_COMMA(EzAquarii::location());
            }
            
[\n\t ]     {
                //cout << "Scanner: whitechar (ignored)" << endl;
            }

[1-9][0-9]* {
                //cout << "Scanner: decimal number: " << yytext << endl;
                int number = atoi(yytext);
                return EzAquarii::Parser::make_INTEGER(number, EzAquarii::location());
            }

[0-9]+"."[0-9]* {
                //cout << "Scanner: decimal number: " << yytext << endl;
                double number = atof(yytext);
                return EzAquarii::Parser::make_DOUBLE(number, EzAquarii::location());
            }

[a-zA-Z]+      { 
                //cout << "Scanner: identifier [" << yytext << "]" << endl;
                return EzAquarii::Parser::make_STRING(yytext, EzAquarii::location());
            }

\+         {
               //cout<<"Plus character:"<<yytext<<endl;
               return EzAquarii::Parser::make_PLUS(EzAquarii::location());
           }

\-         {
               //cout<<"Minus character:"<<yytext<<endl;
               return EzAquarii::Parser::make_MINUS(EzAquarii::location());
           }

\*         {
               //cout<<"Multiply character:"<<yytext<<endl;
               return EzAquarii::Parser::make_MULTIPLY(EzAquarii::location());
           }

\/         {
               //cout<<"Divide character:"<<yytext<<endl;
               return EzAquarii::Parser::make_DIVIDE(EzAquarii::location());
           }


\^         {
               //cout<<"Power character:"<<yytext<<endl;
               return EzAquarii::Parser::make_POWER(EzAquarii::location());
           }

.           { 
                //cout << "Scanner: unknown character [" << yytext << "]" << endl; 
            }
            
<<EOF>>     { return yyterminate(); }


%%
