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
	
	// !!!WARNING!!!
	// Location API is used, but the location is not initialized, 'cause I'm lazy. When making
	// a token with make_{something} method you can pass detailed token location. Current location
	// is accessible with m_driver.location() method. All puzzle elements are there - just
	// pass location value in every action code block below. I'm going to waste more time writing
	// this excuse than putting this boilerplate below...
	//
	// Location class can be found in location.hh and posistion.hh files. It's just a bit too much
	// boilerplate for this small example. Bummer.
%}

%option nodefault
%option noyywrap
%option c++
%option yyclass="Scanner"
%option prefix="EzAquarii_"

%%


\(          {
                cout << "Scanner: '('" << endl;
                return EzAquarii::Parser::make_LEFTPAR(EzAquarii::location());
            }
            
\)          { 
                cout << "Scanner: ')'" << endl;
                return EzAquarii::Parser::make_RIGHTPAR(EzAquarii::location());
            }
            
;           {
                cout << "Scanner: ';'" << endl;
                return EzAquarii::Parser::make_SEMICOLON(EzAquarii::location());
            }
            
,           {
                cout << "Scanner: ','" << endl;
                return EzAquarii::Parser::make_COMMA(EzAquarii::location());
            }
            
[\n\t ]     {
                cout << "Scanner: whitechar (ignored)" << endl;
            }

[1-9][0-9]* {
                cout << "Scanner: decimal number: " << yytext << endl;
                int number = atoi(yytext);
                return EzAquarii::Parser::make_INTEGER(number, EzAquarii::location());
            }

[0-9]+"."[0-9]* {
                cout << "Scanner: decimal number: " << yytext << endl;
                double number = atof(yytext);
                return EzAquarii::Parser::make_DOUBLE(number, EzAquarii::location());
            }

[a-zA-Z]+      { 
                cout << "Scanner: identifier [" << yytext << "]" << endl;
                return EzAquarii::Parser::make_STRING(yytext, EzAquarii::location( /* put location data here if you want */ ));
            }

\+         {
               cout<<"Plus character:"<<yytext<<endl;
               return EzAquarii::Parser::make_PLUS(EzAquarii::location());
           }

\-         {
               cout<<"Minus character:"<<yytext<<endl;
               return EzAquarii::Parser::make_MINUS(EzAquarii::location());
           }

\*         {
               cout<<"Miltiply character:"<<yytext<<endl;
               return EzAquarii::Parser::make_MULTIPLY(EzAquarii::location());
           }

\/         {
               cout<<"DIVIDE character:"<<yytext<<endl;
               return EzAquarii::Parser::make_DIVIDE(EzAquarii::location());
           }


\^         {
               cout<<"Power character:"<<yytext<<endl;
               return EzAquarii::Parser::make_POWER(EzAquarii::location());
           }

.           { 
                cout << "Scanner: unknown character [" << yytext << "]" << endl; 
            }
            
<<EOF>>     { return yyterminate(); }


%%
