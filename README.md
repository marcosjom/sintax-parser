This repository contains linked files from [sys-nbframework-src](https://github.com/marcosjom/sys-nbframework-src); it exists to expose and document a specific feature of the bigger framework.

# sintax-parser

Universal sintax parser from sintax specifications; tokenize, validate and refactor source code.

# Origin

Created by [Marcos Ortega](https://mortegam.com/) to facilitate the validation and refactoring of code; and to build tools that automatize the writting of new source code, like creating glue layers from one language to another (like from C to C++).

# Features

- Feed sintax rules.
- It builds C enumerations and parsing rules.
- Use the resulting C code to parse.
- Link your callbacks functions

# Example of use: a C99 sintax parser

Get the sintax specifications: https://www.open-std.org/jtc1/sc22/WG14/www/docs/n1256.pdf

Create a text file or string with the sintax rules separated by empty lines.

Note that:
- literal values are expressed between square-brackets.
- optional elements ends with "(opt)"
- and "(callback)" tells the parser to call your program to validate that element.

```
initializer-list:
designation(opt) initializer
initializer-list [,] designation(opt) initializer

type-name:
specifier-qualifier-list abstract-declarator(opt)

argument-expression-list:
assignment-expression
argument-expression-list [,] assignment-expression

primary-expression:
identifier
constant
string-literal

...

c-char:
(callback)
//any member of the source character set except
//the single-quote ', backslash \, or new-line character
escape-sequence

...

assignment-operator:
[=]
[*=]
[/=]
[%=]
[+=]
[-=]
[<<=]
[>>=]
[&=]
[^=]
[|=]

expression:
assignment-expression
expression [,] assignment-expression

constant-expression:
conditional-expression

```

Feed the rules to the parser:

```
STNBSintaxParser parser;
NBSintaxParser_init(&parser);
NBSintaxParser_rulesFeedStart(&parser);
if(!NBSintaxParser_rulesFeed(&parser, rulesStr)){
    //error
} else if(!NBSintaxParser_rulesFeedEnd(&parser)){
    //error
} else {
    //rules loaded
    //...do next steps here...
    //...
}
NBSintaxParser_release(&parser);
```

Optionally, you can generate a C-header to build code able to parse these rules in an optimized way:

```
NBSintaxParser_rulesConcatAsRulesInC(obj, "my_parser_of_C99TC3", dst);
```

Once the rules are loaded you can configure your callbacks:

```
STNBSintaxParserCallbacks callbacks = {};
callbacks.userParam = myData;
callbacks.validateElem = myValidateElemFunc;
callbacks.consumeResults = myConsumeResultsFunc;
callbacks.unconsumedChar = myUnconsumedCharFunc;
callbacks.errorFound = myErrorFoundFunc;
NBSintaxParser_setCallbacks(&parser, &callbacks);
```

Define the root rules you are parsing, and start feeding the code to be parsed:

```
//
//Define the root rules I will be feeding
//
const char* rootElems[] = { "constant-expression" };
STNBSintaxParserConfig config = {};
config.rootElems    = rootElems;
config.rootElemsSz  = (sizeof(rootElems) / sizeof(rootElems[0]));
config.resultsMode  = ENSintaxParserResultsMode_LongestsPerRootChildElem;
NBSintaxParser_feedStart(&parser, &config);

//
//Feed data and process the callbacks
//
NBSintaxParser_feed(&parser, str);
NBSintaxParser_feedByte(&parser, c);
NBSintaxParser_feedBytes(&parser, data, dataSz);
...
```

As a specific example, these roots will be parsing the C99 pre-processor rules:

```
//
//Define the root rules I will be feeding
//
const char* rootElems[] = { "if-group", "elif-group", "else-group", "endif-line", "control-line" };
STNBSintaxParserConfig config; = {};
config.rootElems    = rootElems;
config.rootElemsSz  = (sizeof(rootElems) / sizeof(rootElems[0]));
config.resultsMode  = ENSintaxParserResultsMode_LongestsPerRootChildElem;
NBSintaxParser_feedStart(&parser, &config);
...
```

... which could be the first step to build a C99 compiler.

# Implementations

This parser is in use for C and C++ code validation and refactoring.

It is intended to be used to automatize the generation of glue layers for [sys-nbframework-src](https://github.com/marcosjom/sys-nbframework-src) and to allow the use of this framework from other programming languages. Also to add support for scripting languages someday.

# Contact

Visit [mortegam.com](https://mortegam.com/) for more information and visual examples of projects built with this libray.

May you be surrounded by passionate and curious people. :-) 
