#include "nb/NBFrameworkPch.h"
#include "nb/core/NBSintaxParserCDefs.h"
//

//C99_TC3: http://www.open-std.org/jtc1/sc22/WG14/www/docs/n1256.pdf

static const char* NBSintaxParserCDefs_C99TC3Str_ =
"//http://www.open-std.org/jtc1/sc22/WG14/www/docs/n1256.pdf\n" \
"\n" \
"token:\n" \
"keyword\n" \
"identifier\n" \
"constant\n" \
"string-literal\n" \
"punctuator\n" \
"\n" \
"preprocessing-token:\n" \
"//header-name //Only allowed after '#include'\n" \
"identifier\n" \
"pp-number\n" \
"character-constant\n" \
"string-literal\n" \
"punctuator\n" \
"//(callback) //Must be done at code (phase3).\n" \
"//each non-white-space character that cannot be one of the above\n" \
"\n" \
"keyword:\n" \
"[auto]\n" \
"[break]\n" \
"[case]\n" \
"[char]\n" \
"[const]\n" \
"[continue]\n" \
"[default]\n" \
"[do]\n" \
"[double]\n" \
"[else]\n" \
"[enum]\n" \
"[extern]\n" \
"[float]\n" \
"[for]\n" \
"[goto]\n" \
"[if]\n" \
"[inline]\n" \
"[int]\n" \
"[long]\n" \
"[register]\n" \
"[restrict]\n" \
"[return]\n" \
"[short]\n" \
"[signed]\n" \
"[sizeof]\n" \
"[static]\n" \
"[struct]\n" \
"[switch]\n" \
"[typedef]\n" \
"[union]\n" \
"[unsigned]\n" \
"[void]\n" \
"[volatile]\n" \
"[while]\n" \
"[_Bool]\n" \
"[_Complex]\n" \
"[_Imaginary]\n" \
"\n" \
"identifier:\n" \
"identifier-nondigit\n" \
"identifier identifier-nondigit\n" \
"identifier digit\n" \
"\n" \
"identifier-nondigit:\n" \
"nondigit\n" \
"universal-character-name\n" \
"//(callback) //use default implementation\n" \
"//other implementation-defined characters\n" \
"\n" \
"nondigit:\n" \
"[_]\n" \
"[a]\n" \
"[b]\n" \
"[c]\n" \
"[d]\n" \
"[e]\n" \
"[f]\n" \
"[g]\n" \
"[h]\n" \
"[i]\n" \
"[j]\n" \
"[k]\n" \
"[l]\n" \
"[m]\n" \
"[n]\n" \
"[o]\n" \
"[p]\n" \
"[q]\n" \
"[r]\n" \
"[s]\n" \
"[t]\n" \
"[u]\n" \
"[v]\n" \
"[w]\n" \
"[x]\n" \
"[y]\n" \
"[z]\n" \
"[A]\n" \
"[B]\n" \
"[C]\n" \
"[D]\n" \
"[E]\n" \
"[F]\n" \
"[G]\n" \
"[H]\n" \
"[I]\n" \
"[J]\n" \
"[K]\n" \
"[L]\n" \
"[M]\n" \
"[N]\n" \
"[O]\n" \
"[P]\n" \
"[Q]\n" \
"[R]\n" \
"[S]\n" \
"[T]\n" \
"[U]\n" \
"[V]\n" \
"[W]\n" \
"[X]\n" \
"[Y]\n" \
"[Z]\n" \
"\n" \
"digit:\n" \
"[0]\n" \
"[1]\n" \
"[2]\n" \
"[3]\n" \
"[4]\n" \
"[5]\n" \
"[6]\n" \
"[7]\n" \
"[8]\n" \
"[9]\n" \
"\n" \
"universal-character-name:\n" \
"[\\u] hex-quad\n" \
"[\\U] hex-quad hex-quad\n" \
"\n" \
"hex-quad:\n" \
"hexadecimal-digit hexadecimal-digit\n" \
"hexadecimal-digit hexadecimal-digit\n" \
"\n" \
"constant:\n" \
"integer-constant\n" \
"floating-constant\n" \
"enumeration-constant\n" \
"character-constant\n" \
"\n" \
"integer-constant:\n" \
"decimal-constant integer-suffix(opt)\n" \
"octal-constant integer-suffix(opt)\n" \
"hexadecimal-constant integer-suffix(opt)\n" \
"\n" \
"decimal-constant:\n" \
"nonzero-digit\n" \
"decimal-constant digit\n" \
"\n" \
"octal-constant:\n" \
"[0]\n" \
"octal-constant octal-digit\n" \
"\n" \
"hexadecimal-constant:\n" \
"hexadecimal-prefix hexadecimal-digit\n" \
"hexadecimal-constant hexadecimal-digit\n" \
"\n" \
"hexadecimal-prefix:\n" \
"[0x]\n" \
"[0X]\n" \
"\n" \
"nonzero-digit:\n" \
"[1]\n" \
"[2]\n" \
"[3]\n" \
"[4]\n" \
"[5]\n" \
"[6]\n" \
"[7]\n" \
"[8]\n" \
"[9]\n" \
"\n" \
"octal-digit:\n" \
"[0]\n" \
"[1]\n" \
"[2]\n" \
"[3]\n" \
"[4]\n" \
"[5]\n" \
"[6]\n" \
"[7]\n" \
"\n" \
"hexadecimal-digit:\n" \
"[0]\n" \
"[1]\n" \
"[2]\n" \
"[3]\n" \
"[4]\n" \
"[5]\n" \
"[6]\n" \
"[7]\n" \
"[8]\n" \
"[9]\n" \
"[a]\n" \
"[b]\n" \
"[c]\n" \
"[d]\n" \
"[e]\n" \
"[f]\n" \
"[A]\n" \
"[B]\n" \
"[C]\n" \
"[D]\n" \
"[E]\n" \
"[F]\n" \
"\n" \
"integer-suffix:\n" \
"unsigned-suffix long-suffix(opt)\n" \
"unsigned-suffix long-long-suffix\n" \
"long-suffix unsigned-suffix(opt)\n" \
"long-long-suffix unsigned-suffix(opt)\n" \
"\n" \
"unsigned-suffix:\n" \
"[u]\n" \
"[U]\n" \
"\n" \
"long-suffix:\n" \
"[l]\n" \
"[L]\n" \
"\n" \
"long-long-suffix:\n" \
"[ll]\n" \
"[LL]\n" \
"\n" \
"floating-constant:\n" \
"decimal-floating-constant\n" \
"hexadecimal-floating-constant\n" \
"\n" \
"decimal-floating-constant:\n" \
"fractional-constant exponent-part(opt) floating-suffix(opt)\n" \
"digit-sequence exponent-part floating-suffix(opt)\n" \
"\n" \
"hexadecimal-floating-constant:\n" \
"hexadecimal-prefix hexadecimal-fractional-constant\n" \
"binary-exponent-part floating-suffix(opt)\n" \
"hexadecimal-prefix hexadecimal-digit-sequence\n" \
"binary-exponent-part floating-suffix(opt)\n" \
"\n" \
"fractional-constant:\n" \
"digit-sequence(opt) [.] digit-sequence\n" \
"digit-sequence [.]\n" \
"\n" \
"exponent-part:\n" \
"[e] sign(opt) digit-sequence\n" \
"[E] sign(opt) digit-sequence\n" \
"\n" \
"sign:\n" \
"[+]\n" \
"[-]\n" \
"\n" \
"digit-sequence:\n" \
"//digit\n" \
"//digit-sequence\n" \
"(callback) digit\n" \
"\n" \
"hexadecimal-fractional-constant:\n" \
"hexadecimal-digit-sequence(opt) [.]\n" \
"hexadecimal-digit-sequence\n" \
"hexadecimal-digit-sequence [.]\n" \
"\n" \
"binary-exponent-part:\n" \
"[p] sign(opt) digit-sequence\n" \
"[P] sign(opt) digit-sequence\n" \
"\n" \
"hexadecimal-digit-sequence:\n" \
"//hexadecimal-digit\n" \
"//hexadecimal-digit-sequence hexadecimal-digit\n" \
"(callback)\n" \
"\n" \
"floating-suffix:\n" \
"[f]\n" \
"[l]\n" \
"[F]\n" \
"[L]\n" \
"\n" \
"enumeration-constant:\n" \
"identifier\n" \
"\n" \
"character-constant:\n" \
"['] c-char-sequence [']\n" \
"[L'] c-char-sequence [']\n" \
"\n" \
"c-char-sequence:\n" \
"c-char\n" \
"c-char-sequence c-char\n" \
"\n" \
"c-char:\n" \
"(callback)\n" \
"//any member of the source character set except\n" \
"//the single-quote ', backslash \\, or new-line character\n" \
"escape-sequence\n" \
"\n" \
"escape-sequence:\n" \
"simple-escape-sequence\n" \
"octal-escape-sequence\n" \
"hexadecimal-escape-sequence\n" \
"universal-character-name\n" \
"\n" \
"simple-escape-sequence:\n" \
"[\\']\n" \
"[\\\"]\n" \
"[\\?]\n" \
"[\\\\]\n" \
"[\\a]\n" \
"[\\b]\n" \
"[\\f]\n" \
"[\\n" \
"]\n" \
"[\\r]\n" \
"[\\t]\n" \
"[\\v]\n" \
"\n" \
"octal-escape-sequence:\n" \
"[\\] octal-digit\n" \
"[\\] octal-digit octal-digit\n" \
"[\\] octal-digit octal-digit octal-digit\n" \
"\n" \
"hexadecimal-escape-sequence:\n" \
"[\\x] hexadecimal-digit\n" \
"hexadecimal-escape-sequence hexadecimal-digit\n" \
"\n" \
"string-literal:\n" \
"[\"] s-char-sequence(opt) [\"]\n" \
"[L\"] s-char-sequence(opt) [\"]\n" \
"\n" \
"s-char-sequence:\n" \
"s-char\n" \
"s-char-sequence s-char\n" \
"\n" \
"s-char:\n" \
"(callback)\n" \
"//any member of the source character set except\n" \
"//the double-quote \", backslash \\, or new-line character\n" \
"escape-sequence\n" \
"\n" \
"punctuator:\n" \
"[[]\n" \
"[]]\n" \
"[(]\n" \
"[)]\n" \
"[{]\n" \
"[}]\n" \
"[.]\n" \
"[->]\n" \
"[++]\n" \
"[--]\n" \
"[&]\n" \
"[*]\n" \
"[+]\n" \
"[-]\n" \
"[~]\n" \
"[!]\n" \
"[/]\n" \
"[%]\n" \
"[<<]\n" \
"[>>]\n" \
"[<]\n" \
"[>]\n" \
"[<=]\n" \
"[>=]\n" \
"[==]\n" \
"[!=]\n" \
"[^]\n" \
"[|]\n" \
"[&&]\n" \
"[||]\n" \
"[?]\n" \
"[:]\n" \
"[;]\n" \
"[...]\n" \
"[=]\n" \
"[*=]\n" \
"[/=]\n" \
"[%=]\n" \
"[+=]\n" \
"[-=]\n" \
"[<<=]\n" \
"[>>=]\n" \
"[&=]\n" \
"[^=]\n" \
"[|=]\n" \
"[,]\n" \
"[#]\n" \
"[##]\n" \
"[<:]\n" \
"[:>]\n" \
"[<%]\n" \
"[%>]\n" \
"[%:]\n" \
"[%:%:]\n" \
"\n" \
"header-name:\n" \
"[<] h-char-sequence [>]\n" \
"[\"] q-char-sequence [\"]\n" \
"\n" \
"h-char-sequence:\n" \
"h-char\n" \
"h-char-sequence h-char\n" \
"\n" \
"h-char:\n" \
"(callback)\n" \
"//any member of the source character set except\n" \
"//the new-line character and >\n" \
"\n" \
"q-char-sequence:\n" \
"q-char\n" \
"q-char-sequence q-char\n" \
"\n" \
"q-char:\n" \
"(callback)\n" \
"//any member of the source character set except\n" \
"//the new-line character and \"\n" \
"\n" \
"pp-number:\n" \
"digit\n" \
"[.] digit\n" \
"pp-number digit\n" \
"pp-number identifier-nondigit\n" \
"pp-number [e] sign\n" \
"pp-number [E] sign\n" \
"pp-number [p] sign\n" \
"pp-number [P] sign\n" \
"pp-number [.]\n" \
"\n" \
"primary-expression:\n" \
"identifier\n" \
"constant\n" \
"string-literal\n" \
"[(] expression [)]\n" \
"\n" \
"postfix-expression:\n" \
"primary-expression\n" \
"postfix-expression [[] expression []]\n" \
"postfix-expression [(] argument-expression-list(opt) [)]\n" \
"postfix-expression [.] identifier\n" \
"postfix-expression [->] identifier\n" \
"postfix-expression [++]\n" \
"postfix-expression [--]\n" \
"[(] type-name [)] [{] initializer-list [}]\n" \
"[(] type-name [)] [{] initializer-list [,] [}]\n" \
"\n" \
"argument-expression-list:\n" \
"assignment-expression\n" \
"argument-expression-list [,] assignment-expression\n" \
"\n" \
"unary-expression:\n" \
"postfix-expression\n" \
"[++] unary-expression\n" \
"[--] unary-expression\n" \
"unary-operator cast-expression\n" \
"[sizeof] unary-expression\n" \
"[sizeof] [(] type-name [)]\n" \
"\n" \
"unary-operator:\n" \
"[&]\n" \
"[*]\n" \
"[+]\n" \
"[-]\n" \
"[~]\n" \
"[!]\n" \
"\n" \
"cast-expression:\n" \
"unary-expression\n" \
"[(] type-name [)] cast-expression\n" \
"\n" \
"multiplicative-expression:\n" \
"cast-expression\n" \
"multiplicative-expression [*] cast-expression\n" \
"multiplicative-expression [/] cast-expression\n" \
"multiplicative-expression [%] cast-expression\n" \
"\n" \
"additive-expression:\n" \
"multiplicative-expression\n" \
"additive-expression [+] multiplicative-expression\n" \
"additive-expression [-] multiplicative-expression\n" \
"\n" \
"shift-expression:\n" \
"additive-expression\n" \
"shift-expression [<<] additive-expression\n" \
"shift-expression [>>] additive-expression\n" \
"\n" \
"relational-expression:\n" \
"shift-expression\n" \
"relational-expression [<] shift-expression\n" \
"relational-expression [>] shift-expression\n" \
"relational-expression [<=] shift-expression\n" \
"relational-expression [>=] shift-expression\n" \
"\n" \
"equality-expression:\n" \
"relational-expression\n" \
"equality-expression [==] relational-expression\n" \
"equality-expression [!=] relational-expression\n" \
"\n" \
"AND-expression:\n" \
"equality-expression\n" \
"AND-expression [&] equality-expression\n" \
"\n" \
"exclusive-OR-expression:\n" \
"AND-expression\n" \
"exclusive-OR-expression [^] AND-expression\n" \
"\n" \
"inclusive-OR-expression:\n" \
"exclusive-OR-expression\n" \
"inclusive-OR-expression [|] exclusive-OR-expression\n" \
"\n" \
"logical-AND-expression:\n" \
"inclusive-OR-expression\n" \
"logical-AND-expression [&&] inclusive-OR-expression\n" \
"\n" \
"logical-OR-expression:\n" \
"logical-AND-expression\n" \
"logical-OR-expression [||] logical-AND-expression\n" \
"\n" \
"conditional-expression:\n" \
"logical-OR-expression\n" \
"logical-OR-expression [?] expression [:] conditional-expression\n" \
"\n" \
"assignment-expression:\n" \
"conditional-expression\n" \
"unary-expression assignment-operator assignment-expression\n" \
"\n" \
"assignment-operator:\n" \
"[=]\n" \
"[*=]\n" \
"[/=]\n" \
"[%=]\n" \
"[+=]\n" \
"[-=]\n" \
"[<<=]\n" \
"[>>=]\n" \
"[&=]\n" \
"[^=]\n" \
"[|=]\n" \
"\n" \
"expression:\n" \
"assignment-expression\n" \
"expression [,] assignment-expression\n" \
"\n" \
"constant-expression:\n" \
"conditional-expression\n" \
"\n" \
"declaration:\n" \
"declaration-specifiers init-declarator-list(opt) [;]\n" \
"\n" \
"declaration-specifiers:\n" \
"storage-class-specifier declaration-specifiers(opt)\n" \
"type-specifier declaration-specifiers(opt)\n" \
"type-qualifier declaration-specifiers(opt)\n" \
"function-specifier declaration-specifiers(opt)\n" \
"\n" \
"init-declarator-list:\n" \
"init-declarator\n" \
"init-declarator-list [,] init-declarator\n" \
"\n" \
"init-declarator:\n" \
"declarator\n" \
"declarator [=] initializer\n" \
"\n" \
"storage-class-specifier:\n" \
"[typedef]\n" \
"[extern]\n" \
"[static]\n" \
"[auto]\n" \
"[register]\n" \
"\n" \
"type-specifier:\n" \
"[void]\n" \
"[char]\n" \
"[short]\n" \
"[int]\n" \
"[long]\n" \
"[float]\n" \
"[double]\n" \
"[signed]\n" \
"[unsigned]\n" \
"[_Bool]\n" \
"[_Complex]\n" \
"struct-or-union-specifier\n" \
"enum-specifier\n" \
"typedef-name\n" \
"\n" \
"struct-or-union-specifier:\n" \
"struct-or-union identifier(opt) [{] struct-declaration-list [}]\n" \
"struct-or-union identifier\n" \
"\n" \
"struct-or-union:\n" \
"[struct]\n" \
"[union]\n" \
"\n" \
"struct-declaration-list:\n" \
"struct-declaration\n" \
"struct-declaration-list struct-declaration\n" \
"\n" \
"struct-declaration:\n" \
"specifier-qualifier-list struct-declarator-list [;]\n" \
"\n" \
"specifier-qualifier-list:\n" \
"type-specifier specifier-qualifier-list(opt)\n" \
"type-qualifier specifier-qualifier-list(opt)\n" \
"\n" \
"struct-declarator-list:\n" \
"struct-declarator\n" \
"struct-declarator-list [,] struct-declarator\n" \
"\n" \
"struct-declarator:\n" \
"declarator\n" \
"declarator(opt) [:] constant-expression\n" \
"\n" \
"enum-specifier:\n" \
"[enum] identifier(opt) [{] enumerator-list [}]\n" \
"[enum] identifier(opt) [{] enumerator-list [,] [}]\n" \
"[enum] identifier\n" \
"\n" \
"enumerator-list:\n" \
"enumerator\n" \
"enumerator-list [,] enumerator\n" \
"\n" \
"enumerator:\n" \
"enumeration-constant\n" \
"enumeration-constant [=] constant-expression\n" \
"\n" \
"type-qualifier:\n" \
"[const]\n" \
"[restrict]\n" \
"[volatile]\n" \
"\n" \
"function-specifier:\n" \
"[inline]\n" \
"\n" \
"declarator:\n" \
"pointer(opt) direct-declarator\n" \
"\n" \
"direct-declarator:\n" \
"identifier\n" \
"[(] declarator [)]\n" \
"direct-declarator [[] type-qualifier-list(opt) assignment-expression(opt) []]\n" \
"direct-declarator [[] [static] type-qualifier-list(opt) assignment-expression []]\n" \
"direct-declarator [[] type-qualifier-list [static] assignment-expression []]\n" \
"direct-declarator [[] type-qualifier-list(opt) [*] []]\n" \
"direct-declarator [(] parameter-type-list [)]\n" \
"direct-declarator [(] identifier-list(opt) [)]\n" \
"\n" \
"pointer:\n" \
"[*] type-qualifier-list(opt)\n" \
"[*] type-qualifier-list(opt) pointer\n" \
"\n" \
"type-qualifier-list:\n" \
"type-qualifier\n" \
"type-qualifier-list type-qualifier\n" \
"\n" \
"parameter-type-list:\n" \
"parameter-list\n" \
"parameter-list [,] [...]\n" \
"\n" \
"parameter-list:\n" \
"parameter-declaration\n" \
"parameter-list [,] parameter-declaration\n" \
"\n" \
"parameter-declaration:\n" \
"declaration-specifiers declarator\n" \
"declaration-specifiers abstract-declarator(opt)\n" \
"\n" \
"identifier-list:\n" \
"identifier\n" \
"identifier-list [,] identifier\n" \
"\n" \
"type-name:\n" \
"specifier-qualifier-list abstract-declarator(opt)\n" \
"\n" \
"abstract-declarator:\n" \
"pointer\n" \
"pointer(opt) direct-abstract-declarator\n" \
"\n" \
"direct-abstract-declarator:\n" \
"[(] abstract-declarator [)]\n" \
"direct-abstract-declarator(opt) [[] type-qualifier-list(opt) assignment-expression(opt) []]\n" \
"direct-abstract-declarator(opt) [[] [static] type-qualifier-list(opt) assignment-expression []]\n" \
"direct-abstract-declarator(opt) [[] type-qualifier-list [static] assignment-expression []]\n" \
"direct-abstract-declarator(opt) [[] [*] []]\n" \
"direct-abstract-declarator(opt) [(] parameter-type-list(opt) [)]\n" \
"\n" \
"typedef-name:\n" \
"identifier\n" \
"\n" \
"initializer:\n" \
"assignment-expression\n" \
"[{] initializer-list [}]\n" \
"[{] initializer-list [,] [}]\n" \
"\n" \
"initializer-list:\n" \
"designation(opt) initializer\n" \
"initializer-list [,] designation(opt) initializer\n" \
"\n" \
"designation:\n" \
"designator-list [=]\n" \
"\n" \
"designator-list:\n" \
"designator\n" \
"designator-list designator\n" \
"\n" \
"designator:\n" \
"[[] constant-expression []]\n" \
"[.] identifier\n" \
"\n" \
"statement:\n" \
"labeled-statement\n" \
"compound-statement\n" \
"expression-statement\n" \
"selection-statement\n" \
"iteration-statement\n" \
"jump-statement\n" \
"\n" \
"labeled-statement:\n" \
"identifier [:] statement\n" \
"[case] constant-expression [:] statement\n" \
"[default] [:] statement\n" \
"\n" \
"compound-statement:\n" \
"[{] block-item-list(opt) [}]\n" \
"\n" \
"block-item-list:\n" \
"block-item\n" \
"block-item-list block-item\n" \
"\n" \
"block-item:\n" \
"declaration\n" \
"statement\n" \
"\n" \
"expression-statement:\n" \
"expression(opt) [;]\n" \
"\n" \
"selection-statement:\n" \
"[if] [(] expression [)] statement\n" \
"[if] [(] expression [)] statement [else] statement\n" \
"[switch] [(] expression [)] statement\n" \
"\n" \
"iteration-statement:\n" \
"[while] [(] expression [)] statement\n" \
"[do] statement [while] [(] expression [)] [;]\n" \
"[for] [(] expression(opt) [;] expression(opt) [;] expression(opt) [)] statement\n" \
"[for] [(] declaration expression(opt) [;] expression(opt) [)] statement\n" \
"\n" \
"jump-statement:\n" \
"[goto] identifier [;]\n" \
"[continue] [;]\n" \
"[break] [;]\n" \
"[return] expression(opt) [;]\n" \
"\n" \
"translation-unit:\n" \
"external-declaration\n" \
"translation-unit external-declaration\n" \
"\n" \
"external-declaration:\n" \
"function-definition\n" \
"declaration\n" \
"\n" \
"function-definition:\n" \
"declaration-specifiers declarator declaration-list(opt) compound-statement\n" \
"\n" \
"declaration-list:\n" \
"declaration\n" \
"declaration-list declaration\n" \
"\n" \
"preprocessing-file:\n" \
"group(opt)\n" \
"\n" \
"group:\n" \
"group-part\n" \
"group group-part\n" \
"\n" \
"group-part:\n" \
"if-section\n" \
"control-line\n" \
"text-line\n" \
"[#] non-directive\n" \
"\n" \
"if-section:\n" \
"if-group elif-groups(opt) else-group(opt) endif-line\n" \
"\n" \
"if-group:\n" \
"[#] [if] constant-expression new-line group(opt)\n" \
"[#] [ifdef] identifier new-line group(opt)\n" \
"[#] [ifndef] identifier new-line group(opt)\n" \
"\n" \
"elif-groups:\n" \
"elif-group\n" \
"elif-groups elif-group\n" \
"\n" \
"elif-group:\n" \
"[#] [elif] constant-expression new-line group(opt)\n" \
"\n" \
"else-group:\n" \
"[#] [else] new-line group(opt)\n" \
"\n" \
"endif-line:\n" \
"[#] [endif] new-line\n" \
"\n" \
"control-line:\n" \
"[#] [include] pp-tokens new-line\n" \
"[#] [define] identifier replacement-list new-line\n" \
"[#] [define] identifier lparen identifier-list(opt) [)] replacement-list new-line\n" \
"[#] [define] identifier lparen [...] [)] replacement-list new-line\n" \
"[#] [define] identifier lparen identifier-list [,] [...] [)] replacement-list new-line\n" \
"[#] [undef] identifier new-line\n" \
"[#] [line] pp-tokens new-line\n" \
"[#] [error] pp-tokens(opt) new-line\n" \
"[#] [pragma] pp-tokens(opt) new-line\n" \
"[#] new-line\n" \
"\n" \
"text-line:\n" \
"pp-tokens(opt) new-line\n" \
"\n" \
"non-directive:\n" \
"pp-tokens new-line\n" \
"\n" \
"lparen:\n" \
"(callback)\n" \
"//a ( character not immediately preceded by white-space\n" \
"\n" \
"replacement-list:\n" \
"pp-tokens(opt)\n" \
"\n" \
"pp-tokens:\n" \
"preprocessing-token\n" \
"pp-tokens preprocessing-token\n" \
"\n" \
"new-line:\n" \
"(callback)\n" \
"//the new-line character\n" \
"\n" \
"\n" \
"";

//
//Automaically generated using:
//
//NBSintaxParser_rulesFeedStart(obj);
//NBSintaxParser_rulesFeed(obj, syntaxDefs);
//NBSintaxParser_rulesFeedEnd(obj);
//NBSintaxParser_rulesConcatAsRulesInC(obj, "SintaxParserC99TC3", dst);
//

const char* NBSintaxParserC99TC3RulesNames_[ENSintaxParserC99TC3Rule_Count] = {
	"token",
	"keyword",
	"identifier",
	"constant",
	"string-literal",
	"punctuator",
	"preprocessing-token",
	"pp-number",
	"character-constant",
	"identifier-nondigit",
	"digit",
	"nondigit",
	"universal-character-name",
	"hex-quad",
	"hexadecimal-digit",
	"integer-constant",
	"floating-constant",
	"enumeration-constant",
	"decimal-constant",
	"integer-suffix",
	"octal-constant",
	"hexadecimal-constant",
	"nonzero-digit",
	"octal-digit",
	"hexadecimal-prefix",
	"unsigned-suffix",
	"long-suffix",
	"long-long-suffix",
	"decimal-floating-constant",
	"hexadecimal-floating-constant",
	"fractional-constant",
	"exponent-part",
	"floating-suffix",
	"digit-sequence",
	"hexadecimal-fractional-constant",
	"binary-exponent-part",
	"hexadecimal-digit-sequence",
	"sign",
	"c-char-sequence",
	"c-char",
	"escape-sequence",
	"simple-escape-sequence",
	"octal-escape-sequence",
	"hexadecimal-escape-sequence",
	"s-char-sequence",
	"s-char",
	"header-name",
	"h-char-sequence",
	"q-char-sequence",
	"h-char",
	"q-char",
	"primary-expression",
	"expression",
	"postfix-expression",
	"argument-expression-list",
	"type-name",
	"initializer-list",
	"assignment-expression",
	"unary-expression",
	"unary-operator",
	"cast-expression",
	"multiplicative-expression",
	"additive-expression",
	"shift-expression",
	"relational-expression",
	"equality-expression",
	"AND-expression",
	"exclusive-OR-expression",
	"inclusive-OR-expression",
	"logical-AND-expression",
	"logical-OR-expression",
	"conditional-expression",
	"assignment-operator",
	"constant-expression",
	"declaration",
	"declaration-specifiers",
	"init-declarator-list",
	"storage-class-specifier",
	"type-specifier",
	"type-qualifier",
	"function-specifier",
	"init-declarator",
	"declarator",
	"initializer",
	"struct-or-union-specifier",
	"enum-specifier",
	"typedef-name",
	"struct-or-union",
	"struct-declaration-list",
	"struct-declaration",
	"specifier-qualifier-list",
	"struct-declarator-list",
	"struct-declarator",
	"enumerator-list",
	"enumerator",
	"pointer",
	"direct-declarator",
	"type-qualifier-list",
	"parameter-type-list",
	"identifier-list",
	"parameter-list",
	"parameter-declaration",
	"abstract-declarator",
	"direct-abstract-declarator",
	"designation",
	"designator-list",
	"designator",
	"statement",
	"labeled-statement",
	"compound-statement",
	"expression-statement",
	"selection-statement",
	"iteration-statement",
	"jump-statement",
	"block-item-list",
	"block-item",
	"translation-unit",
	"external-declaration",
	"function-definition",
	"declaration-list",
	"preprocessing-file",
	"group",
	"group-part",
	"if-section",
	"control-line",
	"text-line",
	"non-directive",
	"if-group",
	"elif-groups",
	"else-group",
	"endif-line",
	"new-line",
	"elif-group",
	"pp-tokens",
	"replacement-list",
	"lparen"
};

const ENSintaxParserC99TC3Rule NBSintaxParserC99TC3RulesElemsRules_[ENSintaxParserC99TC3RulesElem_Count] = {
	ENSintaxParserC99TC3Rule_token,
	ENSintaxParserC99TC3Rule_keyword,
	ENSintaxParserC99TC3Rule_identifier,
	ENSintaxParserC99TC3Rule_constant,
	ENSintaxParserC99TC3Rule_string_literal,
	ENSintaxParserC99TC3Rule_punctuator,
	ENSintaxParserC99TC3Rule_preprocessing_token,
	ENSintaxParserC99TC3Rule_pp_number,
	ENSintaxParserC99TC3Rule_character_constant,
	ENSintaxParserC99TC3Rule_Count, // [auto]
	ENSintaxParserC99TC3Rule_Count, // [break]
	ENSintaxParserC99TC3Rule_Count, // [case]
	ENSintaxParserC99TC3Rule_Count, // [char]
	ENSintaxParserC99TC3Rule_Count, // [const]
	ENSintaxParserC99TC3Rule_Count, // [continue]
	ENSintaxParserC99TC3Rule_Count, // [default]
	ENSintaxParserC99TC3Rule_Count, // [do]
	ENSintaxParserC99TC3Rule_Count, // [double]
	ENSintaxParserC99TC3Rule_Count, // [else]
	ENSintaxParserC99TC3Rule_Count, // [enum]
	ENSintaxParserC99TC3Rule_Count, // [extern]
	ENSintaxParserC99TC3Rule_Count, // [float]
	ENSintaxParserC99TC3Rule_Count, // [for]
	ENSintaxParserC99TC3Rule_Count, // [goto]
	ENSintaxParserC99TC3Rule_Count, // [if]
	ENSintaxParserC99TC3Rule_Count, // [inline]
	ENSintaxParserC99TC3Rule_Count, // [int]
	ENSintaxParserC99TC3Rule_Count, // [long]
	ENSintaxParserC99TC3Rule_Count, // [register]
	ENSintaxParserC99TC3Rule_Count, // [restrict]
	ENSintaxParserC99TC3Rule_Count, // [return]
	ENSintaxParserC99TC3Rule_Count, // [short]
	ENSintaxParserC99TC3Rule_Count, // [signed]
	ENSintaxParserC99TC3Rule_Count, // [sizeof]
	ENSintaxParserC99TC3Rule_Count, // [static]
	ENSintaxParserC99TC3Rule_Count, // [struct]
	ENSintaxParserC99TC3Rule_Count, // [switch]
	ENSintaxParserC99TC3Rule_Count, // [typedef]
	ENSintaxParserC99TC3Rule_Count, // [union]
	ENSintaxParserC99TC3Rule_Count, // [unsigned]
	ENSintaxParserC99TC3Rule_Count, // [void]
	ENSintaxParserC99TC3Rule_Count, // [volatile]
	ENSintaxParserC99TC3Rule_Count, // [while]
	ENSintaxParserC99TC3Rule_Count, // [_Bool]
	ENSintaxParserC99TC3Rule_Count, // [_Complex]
	ENSintaxParserC99TC3Rule_Count, // [_Imaginary]
	ENSintaxParserC99TC3Rule_identifier_nondigit,
	ENSintaxParserC99TC3Rule_digit,
	ENSintaxParserC99TC3Rule_nondigit,
	ENSintaxParserC99TC3Rule_universal_character_name,
	ENSintaxParserC99TC3Rule_Count, // [_]
	ENSintaxParserC99TC3Rule_Count, // [a]
	ENSintaxParserC99TC3Rule_Count, // [b]
	ENSintaxParserC99TC3Rule_Count, // [c]
	ENSintaxParserC99TC3Rule_Count, // [d]
	ENSintaxParserC99TC3Rule_Count, // [e]
	ENSintaxParserC99TC3Rule_Count, // [f]
	ENSintaxParserC99TC3Rule_Count, // [g]
	ENSintaxParserC99TC3Rule_Count, // [h]
	ENSintaxParserC99TC3Rule_Count, // [i]
	ENSintaxParserC99TC3Rule_Count, // [j]
	ENSintaxParserC99TC3Rule_Count, // [k]
	ENSintaxParserC99TC3Rule_Count, // [l]
	ENSintaxParserC99TC3Rule_Count, // [m]
	ENSintaxParserC99TC3Rule_Count, // [n]
	ENSintaxParserC99TC3Rule_Count, // [o]
	ENSintaxParserC99TC3Rule_Count, // [p]
	ENSintaxParserC99TC3Rule_Count, // [q]
	ENSintaxParserC99TC3Rule_Count, // [r]
	ENSintaxParserC99TC3Rule_Count, // [s]
	ENSintaxParserC99TC3Rule_Count, // [t]
	ENSintaxParserC99TC3Rule_Count, // [u]
	ENSintaxParserC99TC3Rule_Count, // [v]
	ENSintaxParserC99TC3Rule_Count, // [w]
	ENSintaxParserC99TC3Rule_Count, // [x]
	ENSintaxParserC99TC3Rule_Count, // [y]
	ENSintaxParserC99TC3Rule_Count, // [z]
	ENSintaxParserC99TC3Rule_Count, // [A]
	ENSintaxParserC99TC3Rule_Count, // [B]
	ENSintaxParserC99TC3Rule_Count, // [C]
	ENSintaxParserC99TC3Rule_Count, // [D]
	ENSintaxParserC99TC3Rule_Count, // [E]
	ENSintaxParserC99TC3Rule_Count, // [F]
	ENSintaxParserC99TC3Rule_Count, // [G]
	ENSintaxParserC99TC3Rule_Count, // [H]
	ENSintaxParserC99TC3Rule_Count, // [I]
	ENSintaxParserC99TC3Rule_Count, // [J]
	ENSintaxParserC99TC3Rule_Count, // [K]
	ENSintaxParserC99TC3Rule_Count, // [L]
	ENSintaxParserC99TC3Rule_Count, // [M]
	ENSintaxParserC99TC3Rule_Count, // [N]
	ENSintaxParserC99TC3Rule_Count, // [O]
	ENSintaxParserC99TC3Rule_Count, // [P]
	ENSintaxParserC99TC3Rule_Count, // [Q]
	ENSintaxParserC99TC3Rule_Count, // [R]
	ENSintaxParserC99TC3Rule_Count, // [S]
	ENSintaxParserC99TC3Rule_Count, // [T]
	ENSintaxParserC99TC3Rule_Count, // [U]
	ENSintaxParserC99TC3Rule_Count, // [V]
	ENSintaxParserC99TC3Rule_Count, // [W]
	ENSintaxParserC99TC3Rule_Count, // [X]
	ENSintaxParserC99TC3Rule_Count, // [Y]
	ENSintaxParserC99TC3Rule_Count, // [Z]
	ENSintaxParserC99TC3Rule_Count, // [0]
	ENSintaxParserC99TC3Rule_Count, // [1]
	ENSintaxParserC99TC3Rule_Count, // [2]
	ENSintaxParserC99TC3Rule_Count, // [3]
	ENSintaxParserC99TC3Rule_Count, // [4]
	ENSintaxParserC99TC3Rule_Count, // [5]
	ENSintaxParserC99TC3Rule_Count, // [6]
	ENSintaxParserC99TC3Rule_Count, // [7]
	ENSintaxParserC99TC3Rule_Count, // [8]
	ENSintaxParserC99TC3Rule_Count, // [9]
	ENSintaxParserC99TC3Rule_Count, // [\u]
	ENSintaxParserC99TC3Rule_hex_quad,
	ENSintaxParserC99TC3Rule_Count, // [\U]
	ENSintaxParserC99TC3Rule_hexadecimal_digit,
	ENSintaxParserC99TC3Rule_integer_constant,
	ENSintaxParserC99TC3Rule_floating_constant,
	ENSintaxParserC99TC3Rule_enumeration_constant,
	ENSintaxParserC99TC3Rule_decimal_constant,
	ENSintaxParserC99TC3Rule_integer_suffix,
	ENSintaxParserC99TC3Rule_octal_constant,
	ENSintaxParserC99TC3Rule_hexadecimal_constant,
	ENSintaxParserC99TC3Rule_nonzero_digit,
	ENSintaxParserC99TC3Rule_octal_digit,
	ENSintaxParserC99TC3Rule_hexadecimal_prefix,
	ENSintaxParserC99TC3Rule_Count, // [0x]
	ENSintaxParserC99TC3Rule_Count, // [0X]
	ENSintaxParserC99TC3Rule_unsigned_suffix,
	ENSintaxParserC99TC3Rule_long_suffix,
	ENSintaxParserC99TC3Rule_long_long_suffix,
	ENSintaxParserC99TC3Rule_Count, // [ll]
	ENSintaxParserC99TC3Rule_Count, // [LL]
	ENSintaxParserC99TC3Rule_decimal_floating_constant,
	ENSintaxParserC99TC3Rule_hexadecimal_floating_constant,
	ENSintaxParserC99TC3Rule_fractional_constant,
	ENSintaxParserC99TC3Rule_exponent_part,
	ENSintaxParserC99TC3Rule_floating_suffix,
	ENSintaxParserC99TC3Rule_digit_sequence,
	ENSintaxParserC99TC3Rule_hexadecimal_fractional_constant,
	ENSintaxParserC99TC3Rule_binary_exponent_part,
	ENSintaxParserC99TC3Rule_hexadecimal_digit_sequence,
	ENSintaxParserC99TC3Rule_Count, // [.]
	ENSintaxParserC99TC3Rule_sign,
	ENSintaxParserC99TC3Rule_Count, // [+]
	ENSintaxParserC99TC3Rule_Count, // [-]
	ENSintaxParserC99TC3Rule_Count, // (callback)
	ENSintaxParserC99TC3Rule_Count, // [']
	ENSintaxParserC99TC3Rule_c_char_sequence,
	ENSintaxParserC99TC3Rule_Count, // [L']
	ENSintaxParserC99TC3Rule_c_char,
	ENSintaxParserC99TC3Rule_escape_sequence,
	ENSintaxParserC99TC3Rule_simple_escape_sequence,
	ENSintaxParserC99TC3Rule_octal_escape_sequence,
	ENSintaxParserC99TC3Rule_hexadecimal_escape_sequence,
	ENSintaxParserC99TC3Rule_Count, // [\']
	ENSintaxParserC99TC3Rule_Count, // [\"]
	ENSintaxParserC99TC3Rule_Count, // [\?]
	ENSintaxParserC99TC3Rule_Count, // [\\]
	ENSintaxParserC99TC3Rule_Count, // [\a]
	ENSintaxParserC99TC3Rule_Count, // [\b]
	ENSintaxParserC99TC3Rule_Count, // [\f]
	ENSintaxParserC99TC3Rule_Count, // [\n]
	ENSintaxParserC99TC3Rule_Count, // [\r]
	ENSintaxParserC99TC3Rule_Count, // [\t]
	ENSintaxParserC99TC3Rule_Count, // [\v]
	ENSintaxParserC99TC3Rule_Count, // [\]
	ENSintaxParserC99TC3Rule_Count, // [\x]
	ENSintaxParserC99TC3Rule_Count, // ["]
	ENSintaxParserC99TC3Rule_s_char_sequence,
	ENSintaxParserC99TC3Rule_Count, // [L"]
	ENSintaxParserC99TC3Rule_s_char,
	ENSintaxParserC99TC3Rule_Count, // [[]
	ENSintaxParserC99TC3Rule_Count, // []]
	ENSintaxParserC99TC3Rule_Count, // [(]
	ENSintaxParserC99TC3Rule_Count, // [)]
	ENSintaxParserC99TC3Rule_Count, // [{]
	ENSintaxParserC99TC3Rule_Count, // [}]
	ENSintaxParserC99TC3Rule_Count, // [->]
	ENSintaxParserC99TC3Rule_Count, // [++]
	ENSintaxParserC99TC3Rule_Count, // [--]
	ENSintaxParserC99TC3Rule_Count, // [&]
	ENSintaxParserC99TC3Rule_Count, // [*]
	ENSintaxParserC99TC3Rule_Count, // [~]
	ENSintaxParserC99TC3Rule_Count, // [!]
	ENSintaxParserC99TC3Rule_Count, // [/]
	ENSintaxParserC99TC3Rule_Count, // [%]
	ENSintaxParserC99TC3Rule_Count, // [<<]
	ENSintaxParserC99TC3Rule_Count, // [>>]
	ENSintaxParserC99TC3Rule_Count, // [<]
	ENSintaxParserC99TC3Rule_Count, // [>]
	ENSintaxParserC99TC3Rule_Count, // [<=]
	ENSintaxParserC99TC3Rule_Count, // [>=]
	ENSintaxParserC99TC3Rule_Count, // [==]
	ENSintaxParserC99TC3Rule_Count, // [!=]
	ENSintaxParserC99TC3Rule_Count, // [^]
	ENSintaxParserC99TC3Rule_Count, // [|]
	ENSintaxParserC99TC3Rule_Count, // [&&]
	ENSintaxParserC99TC3Rule_Count, // [||]
	ENSintaxParserC99TC3Rule_Count, // [?]
	ENSintaxParserC99TC3Rule_Count, // [:]
	ENSintaxParserC99TC3Rule_Count, // [;]
	ENSintaxParserC99TC3Rule_Count, // [...]
	ENSintaxParserC99TC3Rule_Count, // [=]
	ENSintaxParserC99TC3Rule_Count, // [*=]
	ENSintaxParserC99TC3Rule_Count, // [/=]
	ENSintaxParserC99TC3Rule_Count, // [%=]
	ENSintaxParserC99TC3Rule_Count, // [+=]
	ENSintaxParserC99TC3Rule_Count, // [-=]
	ENSintaxParserC99TC3Rule_Count, // [<<=]
	ENSintaxParserC99TC3Rule_Count, // [>>=]
	ENSintaxParserC99TC3Rule_Count, // [&=]
	ENSintaxParserC99TC3Rule_Count, // [^=]
	ENSintaxParserC99TC3Rule_Count, // [|=]
	ENSintaxParserC99TC3Rule_Count, // [,]
	ENSintaxParserC99TC3Rule_Count, // [#]
	ENSintaxParserC99TC3Rule_Count, // [##]
	ENSintaxParserC99TC3Rule_Count, // [<:]
	ENSintaxParserC99TC3Rule_Count, // [:>]
	ENSintaxParserC99TC3Rule_Count, // [<%]
	ENSintaxParserC99TC3Rule_Count, // [%>]
	ENSintaxParserC99TC3Rule_Count, // [%:]
	ENSintaxParserC99TC3Rule_Count, // [%:%:]
	ENSintaxParserC99TC3Rule_header_name,
	ENSintaxParserC99TC3Rule_h_char_sequence,
	ENSintaxParserC99TC3Rule_q_char_sequence,
	ENSintaxParserC99TC3Rule_h_char,
	ENSintaxParserC99TC3Rule_q_char,
	ENSintaxParserC99TC3Rule_primary_expression,
	ENSintaxParserC99TC3Rule_expression,
	ENSintaxParserC99TC3Rule_postfix_expression,
	ENSintaxParserC99TC3Rule_argument_expression_list,
	ENSintaxParserC99TC3Rule_type_name,
	ENSintaxParserC99TC3Rule_initializer_list,
	ENSintaxParserC99TC3Rule_assignment_expression,
	ENSintaxParserC99TC3Rule_unary_expression,
	ENSintaxParserC99TC3Rule_unary_operator,
	ENSintaxParserC99TC3Rule_cast_expression,
	ENSintaxParserC99TC3Rule_multiplicative_expression,
	ENSintaxParserC99TC3Rule_additive_expression,
	ENSintaxParserC99TC3Rule_shift_expression,
	ENSintaxParserC99TC3Rule_relational_expression,
	ENSintaxParserC99TC3Rule_equality_expression,
	ENSintaxParserC99TC3Rule_AND_expression,
	ENSintaxParserC99TC3Rule_exclusive_OR_expression,
	ENSintaxParserC99TC3Rule_inclusive_OR_expression,
	ENSintaxParserC99TC3Rule_logical_AND_expression,
	ENSintaxParserC99TC3Rule_logical_OR_expression,
	ENSintaxParserC99TC3Rule_conditional_expression,
	ENSintaxParserC99TC3Rule_assignment_operator,
	ENSintaxParserC99TC3Rule_constant_expression,
	ENSintaxParserC99TC3Rule_declaration,
	ENSintaxParserC99TC3Rule_declaration_specifiers,
	ENSintaxParserC99TC3Rule_init_declarator_list,
	ENSintaxParserC99TC3Rule_storage_class_specifier,
	ENSintaxParserC99TC3Rule_type_specifier,
	ENSintaxParserC99TC3Rule_type_qualifier,
	ENSintaxParserC99TC3Rule_function_specifier,
	ENSintaxParserC99TC3Rule_init_declarator,
	ENSintaxParserC99TC3Rule_declarator,
	ENSintaxParserC99TC3Rule_initializer,
	ENSintaxParserC99TC3Rule_struct_or_union_specifier,
	ENSintaxParserC99TC3Rule_enum_specifier,
	ENSintaxParserC99TC3Rule_typedef_name,
	ENSintaxParserC99TC3Rule_struct_or_union,
	ENSintaxParserC99TC3Rule_struct_declaration_list,
	ENSintaxParserC99TC3Rule_struct_declaration,
	ENSintaxParserC99TC3Rule_specifier_qualifier_list,
	ENSintaxParserC99TC3Rule_struct_declarator_list,
	ENSintaxParserC99TC3Rule_struct_declarator,
	ENSintaxParserC99TC3Rule_enumerator_list,
	ENSintaxParserC99TC3Rule_enumerator,
	ENSintaxParserC99TC3Rule_pointer,
	ENSintaxParserC99TC3Rule_direct_declarator,
	ENSintaxParserC99TC3Rule_type_qualifier_list,
	ENSintaxParserC99TC3Rule_parameter_type_list,
	ENSintaxParserC99TC3Rule_identifier_list,
	ENSintaxParserC99TC3Rule_parameter_list,
	ENSintaxParserC99TC3Rule_parameter_declaration,
	ENSintaxParserC99TC3Rule_abstract_declarator,
	ENSintaxParserC99TC3Rule_direct_abstract_declarator,
	ENSintaxParserC99TC3Rule_designation,
	ENSintaxParserC99TC3Rule_designator_list,
	ENSintaxParserC99TC3Rule_designator,
	ENSintaxParserC99TC3Rule_statement,
	ENSintaxParserC99TC3Rule_labeled_statement,
	ENSintaxParserC99TC3Rule_compound_statement,
	ENSintaxParserC99TC3Rule_expression_statement,
	ENSintaxParserC99TC3Rule_selection_statement,
	ENSintaxParserC99TC3Rule_iteration_statement,
	ENSintaxParserC99TC3Rule_jump_statement,
	ENSintaxParserC99TC3Rule_block_item_list,
	ENSintaxParserC99TC3Rule_block_item,
	ENSintaxParserC99TC3Rule_translation_unit,
	ENSintaxParserC99TC3Rule_external_declaration,
	ENSintaxParserC99TC3Rule_function_definition,
	ENSintaxParserC99TC3Rule_declaration_list,
	ENSintaxParserC99TC3Rule_preprocessing_file,
	ENSintaxParserC99TC3Rule_group,
	ENSintaxParserC99TC3Rule_group_part,
	ENSintaxParserC99TC3Rule_if_section,
	ENSintaxParserC99TC3Rule_control_line,
	ENSintaxParserC99TC3Rule_text_line,
	ENSintaxParserC99TC3Rule_non_directive,
	ENSintaxParserC99TC3Rule_if_group,
	ENSintaxParserC99TC3Rule_elif_groups,
	ENSintaxParserC99TC3Rule_else_group,
	ENSintaxParserC99TC3Rule_endif_line,
	ENSintaxParserC99TC3Rule_new_line,
	ENSintaxParserC99TC3Rule_Count, // [ifdef]
	ENSintaxParserC99TC3Rule_Count, // [ifndef]
	ENSintaxParserC99TC3Rule_elif_group,
	ENSintaxParserC99TC3Rule_Count, // [elif]
	ENSintaxParserC99TC3Rule_Count, // [endif]
	ENSintaxParserC99TC3Rule_Count, // [include]
	ENSintaxParserC99TC3Rule_pp_tokens,
	ENSintaxParserC99TC3Rule_Count, // [define]
	ENSintaxParserC99TC3Rule_replacement_list,
	ENSintaxParserC99TC3Rule_lparen,
	ENSintaxParserC99TC3Rule_Count, // [undef]
	ENSintaxParserC99TC3Rule_Count, // [line]
	ENSintaxParserC99TC3Rule_Count, // [error]
	ENSintaxParserC99TC3Rule_Count // [pragma]
};


//----------
//- Static methods
//----------

const char* NBSintaxParserCDefs_getDefaultSintaxRulesStr(void){
	return NBSintaxParserCDefs_C99TC3Str_;
}

const char* NBSintaxParserCDefs_getRuleNameByIdx(const UI32 iRule){
	NBASSERT(ENSintaxParserC99TC3Rule_Count == (sizeof(NBSintaxParserC99TC3RulesNames_) / sizeof(NBSintaxParserC99TC3RulesNames_[0])))
	const char* r = NULL;
	if(iRule < ENSintaxParserC99TC3Rule_Count){
		r = NBSintaxParserC99TC3RulesNames_[iRule];
	}
	return r;
}

ENSintaxParserC99TC3Rule NBSintaxParserCDefs_getRuleIdxByElemIdx(const UI32 iElem){
	NBASSERT(ENSintaxParserC99TC3RulesElem_Count == (sizeof(NBSintaxParserC99TC3RulesElemsRules_) / sizeof(NBSintaxParserC99TC3RulesElemsRules_[0])))
	ENSintaxParserC99TC3Rule r = ENSintaxParserC99TC3Rule_Count;
	if(iElem < ENSintaxParserC99TC3RulesElem_Count){
		r = NBSintaxParserC99TC3RulesElemsRules_[iElem];
	}
	return r;
}

