%{
	#include <iostream>
	#include <cstdlib>
	#include "scanner.h"
	#include "interpreter.h"
	#include "parser.hpp"
	#include "location.hh"

       //typedef EquationParser::Parser::token token;
       //typedef EquationParser::Parser::token_type token_type;

	using namespace std;

	#define yyterminate() EquationParser::Parser::make_END(EquationParser::location());

	// This will track current scanner location.
	// Action is called when length of the token is known.
	#define YY_USER_ACTION m_driver.increaseLocation(yyleng);	
%}

%option nodefault
%option noyywrap
%option c++
%option yyclass="Scanner"
%option prefix="EquationParser_"

%%


\(          {
                return EquationParser::Parser::make_LEFTPAR(EquationParser::location());
            }
            
\)          { 
                //cout << "Scanner: ')'" << endl;
                return EquationParser::Parser::make_RIGHTPAR(EquationParser::location());
            }
            
;           {
                //cout << "Scanner: ';'" << endl;
                return EquationParser::Parser::make_SEMICOLON(EquationParser::location());
            }
            
,           {
                //cout << "Scanner: ','" << endl;
                return EquationParser::Parser::make_COMMA(EquationParser::location());
            }
            
[\n\t ]     {
                //cout << "Scanner: whitechar (ignored)" << endl;
            }

[1-9][0-9]* {
                //cout << "Scanner: decimal number: " << yytext << endl;
                int number = atoi(yytext);
                return EquationParser::Parser::make_INTEGER(number, EquationParser::location());
            }

[0-9]+"."[0-9]* {
                //cout << "Scanner: decimal number: " << yytext << endl;
                double number = atof(yytext);
                return EquationParser::Parser::make_DOUBLE(number, EquationParser::location());
            }

[a-zA-Z]+      { 
                //cout << "Scanner: identifier [" << yytext << "]" << endl;
                return EquationParser::Parser::make_STRING(yytext, EquationParser::location());
            }

\+         {
               //cout<<"Plus character:"<<yytext<<endl;
               return EquationParser::Parser::make_PLUS(EquationParser::location());
           }

\-         {
               //cout<<"Minus character:"<<yytext<<endl;
               return EquationParser::Parser::make_MINUS(EquationParser::location());
           }

\*         {
               //cout<<"Multiply character:"<<yytext<<endl;
               return EquationParser::Parser::make_MULTIPLY(EquationParser::location());
           }

\/         {
               //cout<<"Divide character:"<<yytext<<endl;
               return EquationParser::Parser::make_DIVIDE(EquationParser::location());
           }


\^         {
               //cout<<"Power character:"<<yytext<<endl;
               return EquationParser::Parser::make_POWER(EquationParser::location());
           }

.           { 
                //cout << "Scanner: unknown character [" << yytext << "]" << endl; 
            }
            
<<EOF>>     { return yyterminate(); }


%%
