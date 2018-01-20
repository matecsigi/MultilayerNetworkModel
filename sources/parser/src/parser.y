/**
   Generate parser with the followign commands:
   >>flex -o scanner.cpp scanner.l
   >>bison -o parser.cpp parser.y
 */

%skeleton "lalr1.cc" /* -*- C++ -*- */
%require "3.0"
%defines
%define parser_class_name { Parser }

%define api.token.constructor
%define api.value.type variant
%define parse.assert
%define api.namespace { EquationParser }
%code requires
{
    #include <iostream>
    #include <string>
    #include <vector>
    #include <stdint.h>
    #include "CalculationNode.hh"

    namespace EquationParser {
        class Scanner;
        class Interpreter;
    }
}

%code top
{
    #include <iostream>
    #include "scanner.h"
    #include "parser.hpp"
    #include "interpreter.h"
    #include "location.hh"
    
    // yylex() arguments are defined in parser.y
    static EquationParser::Parser::symbol_type yylex(EquationParser::Scanner &scanner, EquationParser::Interpreter &driver) {
        return scanner.get_next_token();
    }
    using namespace std;        
    using namespace EquationParser;
}

%lex-param { EquationParser::Scanner &scanner }
%lex-param { EquationParser::Interpreter &driver }
%parse-param { EquationParser::Scanner &scanner }
%parse-param { EquationParser::Interpreter &driver }
%locations
%define parse.trace
%define parse.error verbose

%define api.token.prefix {TOKEN_}

%token END 0 "end of file"
%token <std::string> STRING  "string";
%token LEFTPAR "leftpar";
%token RIGHTPAR "rightpar";
%token SEMICOLON "semicolon";
%token COMMA "comma";
%token <int> INTEGER "integer"
%token <double> DOUBLE "double"
%token PLUS "plus"
%token MINUS "minus"
%token MULTIPLY "multiply"
%token DIVIDE "divide"
%token POWER "power"

%type <class CalculationNode*> constant variable;
%type <class CalculationNode*> atomexpr powexpr unaryexpr mulexpr addexpr expr;

//uncommenting any of these gives a segmentation fault
//%destructor { delete $$; } constant variable;
//%destructor { delete $$; } atomexpr powexpr unaryexpr mulexpr addexpr expr;

%start start

%%

variable : STRING INTEGER
           {
	     if($1 == "ID")
	     {
	       $$ = new CNId($2);
	     }
	   }

constant : INTEGER
           {
	     $$ = new CNConstant($1);
	   }
         | DOUBLE
           {
	     $$ = new CNConstant($1);
	   }
         | MINUS INTEGER
	   {
	     $$ = new CNConstant(-$2);
	   }
         | MINUS DOUBLE
	   {
	     $$ = new CNConstant(-$2);
	   }

atomexpr : constant MULTIPLY variable
           {
	     $$ = new CNMultiply($1, $3);	     
           } 
         | constant
           {
	     $$ = $1;
	   }
         | variable
           {
	     $$ = $1;
	   }
         | LEFTPAR expr RIGHTPAR
           {
	     $$ = $2;
	   }

powexpr	: atomexpr
          {
	    $$ = $1;
	  }
        | atomexpr POWER powexpr
          {
	    $$ = new CNPower($1, $3);
	  }

unaryexpr : powexpr
            {
	      $$ = $1;
	    }
//          | PLUS powexpr
//            {
//	      $$ = $2;
//	    }
//          | MINUS powexpr
//            {
//	      $$ = new CNNegate($2);
//	    }

mulexpr : unaryexpr
          {
	    $$ = $1;
	  }
        | mulexpr MULTIPLY unaryexpr
          {
	    $$ = new CNMultiply($1, $3);
	  }
        | mulexpr DIVIDE unaryexpr
          {
	    $$ = new CNDivide($1, $3);
	  }

addexpr : mulexpr
          {
	    $$ = $1;
	  }
        | addexpr PLUS mulexpr
          {
	    $$ = new CNAdd($1, $3);
	  }
        | addexpr MINUS mulexpr
          {
	    $$ = new CNSubtract($1, $3);
	  }

expr	: addexpr
          {
	    $$ = $1;
	  }

start	: /* empty */
        | start SEMICOLON
        | start END
        | expr SEMICOLON
          {
	    driver.mCalcBase = $1;
	  }

        | expr END
          {
	    driver.mCalcBase = $1;
       
	  }
    
        | expr
          {
	    driver.mCalcBase = $1;
	  }
    
%%

void EquationParser::Parser::error(const location &loc , const std::string &message) {
        
  std::cout << "Error: " << message << std::endl << "Error location: " << driver.location() << std::endl;
}
