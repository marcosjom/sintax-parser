#ifndef NB_SINTAX_PARSER_C_DEFS_H
#define NB_SINTAX_PARSER_C_DEFS_H

#include "nb/NBFrameworkDefs.h"

#ifdef __cplusplus
extern "C" {
#endif

//C99_TC3: http://www.open-std.org/jtc1/sc22/WG14/www/docs/n1256.pdf

//
//Automaically generated using:
//
//NBSintaxParser_rulesFeedStart(obj);
//NBSintaxParser_rulesFeed(obj, syntaxDefs);
//NBSintaxParser_rulesFeedEnd(obj);
//NBSintaxParser_rulesConcatAsRulesInC(obj, "SintaxParserC99TC3", dst);
//

typedef enum ENSintaxParserC99TC3Rule_ {
	ENSintaxParserC99TC3Rule_token = 0,
	ENSintaxParserC99TC3Rule_keyword,
	ENSintaxParserC99TC3Rule_identifier,
	ENSintaxParserC99TC3Rule_constant,
	ENSintaxParserC99TC3Rule_string_literal,
	ENSintaxParserC99TC3Rule_punctuator,
	ENSintaxParserC99TC3Rule_preprocessing_token,
	ENSintaxParserC99TC3Rule_pp_number,
	ENSintaxParserC99TC3Rule_character_constant,
	ENSintaxParserC99TC3Rule_identifier_nondigit,
	ENSintaxParserC99TC3Rule_digit,
	ENSintaxParserC99TC3Rule_nondigit,
	ENSintaxParserC99TC3Rule_universal_character_name,
	ENSintaxParserC99TC3Rule_hex_quad,
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
	ENSintaxParserC99TC3Rule_unsigned_suffix,
	ENSintaxParserC99TC3Rule_long_suffix,
	ENSintaxParserC99TC3Rule_long_long_suffix,
	ENSintaxParserC99TC3Rule_decimal_floating_constant,
	ENSintaxParserC99TC3Rule_hexadecimal_floating_constant,
	ENSintaxParserC99TC3Rule_fractional_constant,
	ENSintaxParserC99TC3Rule_exponent_part,
	ENSintaxParserC99TC3Rule_floating_suffix,
	ENSintaxParserC99TC3Rule_digit_sequence,
	ENSintaxParserC99TC3Rule_hexadecimal_fractional_constant,
	ENSintaxParserC99TC3Rule_binary_exponent_part,
	ENSintaxParserC99TC3Rule_hexadecimal_digit_sequence,
	ENSintaxParserC99TC3Rule_sign,
	ENSintaxParserC99TC3Rule_c_char_sequence,
	ENSintaxParserC99TC3Rule_c_char,
	ENSintaxParserC99TC3Rule_escape_sequence,
	ENSintaxParserC99TC3Rule_simple_escape_sequence,
	ENSintaxParserC99TC3Rule_octal_escape_sequence,
	ENSintaxParserC99TC3Rule_hexadecimal_escape_sequence,
	ENSintaxParserC99TC3Rule_s_char_sequence,
	ENSintaxParserC99TC3Rule_s_char,
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
	ENSintaxParserC99TC3Rule_elif_group,
	ENSintaxParserC99TC3Rule_pp_tokens,
	ENSintaxParserC99TC3Rule_replacement_list,
	ENSintaxParserC99TC3Rule_lparen,
	//Count
	ENSintaxParserC99TC3Rule_Count
} ENSintaxParserC99TC3Rule;

typedef enum ENSintaxParserC99TC3RulesElem_ {
	ENSintaxParserC99TC3RulesElem_0 = 0, // token
	ENSintaxParserC99TC3RulesElem_1, // keyword
	ENSintaxParserC99TC3RulesElem_2, // identifier
	ENSintaxParserC99TC3RulesElem_3, // constant
	ENSintaxParserC99TC3RulesElem_4, // string-literal
	ENSintaxParserC99TC3RulesElem_5, // punctuator
	ENSintaxParserC99TC3RulesElem_6, // preprocessing-token
	ENSintaxParserC99TC3RulesElem_7, // pp-number
	ENSintaxParserC99TC3RulesElem_8, // character-constant
	ENSintaxParserC99TC3RulesElem_9, // [auto]
	ENSintaxParserC99TC3RulesElem_10, // [break]
	ENSintaxParserC99TC3RulesElem_11, // [case]
	ENSintaxParserC99TC3RulesElem_12, // [char]
	ENSintaxParserC99TC3RulesElem_13, // [const]
	ENSintaxParserC99TC3RulesElem_14, // [continue]
	ENSintaxParserC99TC3RulesElem_15, // [default]
	ENSintaxParserC99TC3RulesElem_16, // [do]
	ENSintaxParserC99TC3RulesElem_17, // [double]
	ENSintaxParserC99TC3RulesElem_18, // [else]
	ENSintaxParserC99TC3RulesElem_19, // [enum]
	ENSintaxParserC99TC3RulesElem_20, // [extern]
	ENSintaxParserC99TC3RulesElem_21, // [float]
	ENSintaxParserC99TC3RulesElem_22, // [for]
	ENSintaxParserC99TC3RulesElem_23, // [goto]
	ENSintaxParserC99TC3RulesElem_24, // [if]
	ENSintaxParserC99TC3RulesElem_25, // [inline]
	ENSintaxParserC99TC3RulesElem_26, // [int]
	ENSintaxParserC99TC3RulesElem_27, // [long]
	ENSintaxParserC99TC3RulesElem_28, // [register]
	ENSintaxParserC99TC3RulesElem_29, // [restrict]
	ENSintaxParserC99TC3RulesElem_30, // [return]
	ENSintaxParserC99TC3RulesElem_31, // [short]
	ENSintaxParserC99TC3RulesElem_32, // [signed]
	ENSintaxParserC99TC3RulesElem_33, // [sizeof]
	ENSintaxParserC99TC3RulesElem_34, // [static]
	ENSintaxParserC99TC3RulesElem_35, // [struct]
	ENSintaxParserC99TC3RulesElem_36, // [switch]
	ENSintaxParserC99TC3RulesElem_37, // [typedef]
	ENSintaxParserC99TC3RulesElem_38, // [union]
	ENSintaxParserC99TC3RulesElem_39, // [unsigned]
	ENSintaxParserC99TC3RulesElem_40, // [void]
	ENSintaxParserC99TC3RulesElem_41, // [volatile]
	ENSintaxParserC99TC3RulesElem_42, // [while]
	ENSintaxParserC99TC3RulesElem_43, // [_Bool]
	ENSintaxParserC99TC3RulesElem_44, // [_Complex]
	ENSintaxParserC99TC3RulesElem_45, // [_Imaginary]
	ENSintaxParserC99TC3RulesElem_46, // identifier-nondigit
	ENSintaxParserC99TC3RulesElem_47, // digit
	ENSintaxParserC99TC3RulesElem_48, // nondigit
	ENSintaxParserC99TC3RulesElem_49, // universal-character-name
	ENSintaxParserC99TC3RulesElem_50, // [_]
	ENSintaxParserC99TC3RulesElem_51, // [a]
	ENSintaxParserC99TC3RulesElem_52, // [b]
	ENSintaxParserC99TC3RulesElem_53, // [c]
	ENSintaxParserC99TC3RulesElem_54, // [d]
	ENSintaxParserC99TC3RulesElem_55, // [e]
	ENSintaxParserC99TC3RulesElem_56, // [f]
	ENSintaxParserC99TC3RulesElem_57, // [g]
	ENSintaxParserC99TC3RulesElem_58, // [h]
	ENSintaxParserC99TC3RulesElem_59, // [i]
	ENSintaxParserC99TC3RulesElem_60, // [j]
	ENSintaxParserC99TC3RulesElem_61, // [k]
	ENSintaxParserC99TC3RulesElem_62, // [l]
	ENSintaxParserC99TC3RulesElem_63, // [m]
	ENSintaxParserC99TC3RulesElem_64, // [n]
	ENSintaxParserC99TC3RulesElem_65, // [o]
	ENSintaxParserC99TC3RulesElem_66, // [p]
	ENSintaxParserC99TC3RulesElem_67, // [q]
	ENSintaxParserC99TC3RulesElem_68, // [r]
	ENSintaxParserC99TC3RulesElem_69, // [s]
	ENSintaxParserC99TC3RulesElem_70, // [t]
	ENSintaxParserC99TC3RulesElem_71, // [u]
	ENSintaxParserC99TC3RulesElem_72, // [v]
	ENSintaxParserC99TC3RulesElem_73, // [w]
	ENSintaxParserC99TC3RulesElem_74, // [x]
	ENSintaxParserC99TC3RulesElem_75, // [y]
	ENSintaxParserC99TC3RulesElem_76, // [z]
	ENSintaxParserC99TC3RulesElem_77, // [A]
	ENSintaxParserC99TC3RulesElem_78, // [B]
	ENSintaxParserC99TC3RulesElem_79, // [C]
	ENSintaxParserC99TC3RulesElem_80, // [D]
	ENSintaxParserC99TC3RulesElem_81, // [E]
	ENSintaxParserC99TC3RulesElem_82, // [F]
	ENSintaxParserC99TC3RulesElem_83, // [G]
	ENSintaxParserC99TC3RulesElem_84, // [H]
	ENSintaxParserC99TC3RulesElem_85, // [I]
	ENSintaxParserC99TC3RulesElem_86, // [J]
	ENSintaxParserC99TC3RulesElem_87, // [K]
	ENSintaxParserC99TC3RulesElem_88, // [L]
	ENSintaxParserC99TC3RulesElem_89, // [M]
	ENSintaxParserC99TC3RulesElem_90, // [N]
	ENSintaxParserC99TC3RulesElem_91, // [O]
	ENSintaxParserC99TC3RulesElem_92, // [P]
	ENSintaxParserC99TC3RulesElem_93, // [Q]
	ENSintaxParserC99TC3RulesElem_94, // [R]
	ENSintaxParserC99TC3RulesElem_95, // [S]
	ENSintaxParserC99TC3RulesElem_96, // [T]
	ENSintaxParserC99TC3RulesElem_97, // [U]
	ENSintaxParserC99TC3RulesElem_98, // [V]
	ENSintaxParserC99TC3RulesElem_99, // [W]
	ENSintaxParserC99TC3RulesElem_100, // [X]
	ENSintaxParserC99TC3RulesElem_101, // [Y]
	ENSintaxParserC99TC3RulesElem_102, // [Z]
	ENSintaxParserC99TC3RulesElem_103, // [0]
	ENSintaxParserC99TC3RulesElem_104, // [1]
	ENSintaxParserC99TC3RulesElem_105, // [2]
	ENSintaxParserC99TC3RulesElem_106, // [3]
	ENSintaxParserC99TC3RulesElem_107, // [4]
	ENSintaxParserC99TC3RulesElem_108, // [5]
	ENSintaxParserC99TC3RulesElem_109, // [6]
	ENSintaxParserC99TC3RulesElem_110, // [7]
	ENSintaxParserC99TC3RulesElem_111, // [8]
	ENSintaxParserC99TC3RulesElem_112, // [9]
	ENSintaxParserC99TC3RulesElem_113, // [\u]
	ENSintaxParserC99TC3RulesElem_114, // hex-quad
	ENSintaxParserC99TC3RulesElem_115, // [\U]
	ENSintaxParserC99TC3RulesElem_116, // hexadecimal-digit
	ENSintaxParserC99TC3RulesElem_117, // integer-constant
	ENSintaxParserC99TC3RulesElem_118, // floating-constant
	ENSintaxParserC99TC3RulesElem_119, // enumeration-constant
	ENSintaxParserC99TC3RulesElem_120, // decimal-constant
	ENSintaxParserC99TC3RulesElem_121, // integer-suffix
	ENSintaxParserC99TC3RulesElem_122, // octal-constant
	ENSintaxParserC99TC3RulesElem_123, // hexadecimal-constant
	ENSintaxParserC99TC3RulesElem_124, // nonzero-digit
	ENSintaxParserC99TC3RulesElem_125, // octal-digit
	ENSintaxParserC99TC3RulesElem_126, // hexadecimal-prefix
	ENSintaxParserC99TC3RulesElem_127, // [0x]
	ENSintaxParserC99TC3RulesElem_128, // [0X]
	ENSintaxParserC99TC3RulesElem_129, // unsigned-suffix
	ENSintaxParserC99TC3RulesElem_130, // long-suffix
	ENSintaxParserC99TC3RulesElem_131, // long-long-suffix
	ENSintaxParserC99TC3RulesElem_132, // [ll]
	ENSintaxParserC99TC3RulesElem_133, // [LL]
	ENSintaxParserC99TC3RulesElem_134, // decimal-floating-constant
	ENSintaxParserC99TC3RulesElem_135, // hexadecimal-floating-constant
	ENSintaxParserC99TC3RulesElem_136, // fractional-constant
	ENSintaxParserC99TC3RulesElem_137, // exponent-part
	ENSintaxParserC99TC3RulesElem_138, // floating-suffix
	ENSintaxParserC99TC3RulesElem_139, // digit-sequence
	ENSintaxParserC99TC3RulesElem_140, // hexadecimal-fractional-constant
	ENSintaxParserC99TC3RulesElem_141, // binary-exponent-part
	ENSintaxParserC99TC3RulesElem_142, // hexadecimal-digit-sequence
	ENSintaxParserC99TC3RulesElem_143, // [.]
	ENSintaxParserC99TC3RulesElem_144, // sign
	ENSintaxParserC99TC3RulesElem_145, // [+]
	ENSintaxParserC99TC3RulesElem_146, // [-]
	ENSintaxParserC99TC3RulesElem_147, // (callback)
	ENSintaxParserC99TC3RulesElem_148, // [']
	ENSintaxParserC99TC3RulesElem_149, // c-char-sequence
	ENSintaxParserC99TC3RulesElem_150, // [L']
	ENSintaxParserC99TC3RulesElem_151, // c-char
	ENSintaxParserC99TC3RulesElem_152, // escape-sequence
	ENSintaxParserC99TC3RulesElem_153, // simple-escape-sequence
	ENSintaxParserC99TC3RulesElem_154, // octal-escape-sequence
	ENSintaxParserC99TC3RulesElem_155, // hexadecimal-escape-sequence
	ENSintaxParserC99TC3RulesElem_156, // [\']
	ENSintaxParserC99TC3RulesElem_157, // [\"]
	ENSintaxParserC99TC3RulesElem_158, // [\?]
	ENSintaxParserC99TC3RulesElem_159, // [\\]
	ENSintaxParserC99TC3RulesElem_160, // [\a]
	ENSintaxParserC99TC3RulesElem_161, // [\b]
	ENSintaxParserC99TC3RulesElem_162, // [\f]
	ENSintaxParserC99TC3RulesElem_163, // [\n]
	ENSintaxParserC99TC3RulesElem_164, // [\r]
	ENSintaxParserC99TC3RulesElem_165, // [\t]
	ENSintaxParserC99TC3RulesElem_166, // [\v]
	ENSintaxParserC99TC3RulesElem_167, // [\]
	ENSintaxParserC99TC3RulesElem_168, // [\x]
	ENSintaxParserC99TC3RulesElem_169, // ["]
	ENSintaxParserC99TC3RulesElem_170, // s-char-sequence
	ENSintaxParserC99TC3RulesElem_171, // [L"]
	ENSintaxParserC99TC3RulesElem_172, // s-char
	ENSintaxParserC99TC3RulesElem_173, // [[]
	ENSintaxParserC99TC3RulesElem_174, // []]
	ENSintaxParserC99TC3RulesElem_175, // [(]
	ENSintaxParserC99TC3RulesElem_176, // [)]
	ENSintaxParserC99TC3RulesElem_177, // [{]
	ENSintaxParserC99TC3RulesElem_178, // [}]
	ENSintaxParserC99TC3RulesElem_179, // [->]
	ENSintaxParserC99TC3RulesElem_180, // [++]
	ENSintaxParserC99TC3RulesElem_181, // [--]
	ENSintaxParserC99TC3RulesElem_182, // [&]
	ENSintaxParserC99TC3RulesElem_183, // [*]
	ENSintaxParserC99TC3RulesElem_184, // [~]
	ENSintaxParserC99TC3RulesElem_185, // [!]
	ENSintaxParserC99TC3RulesElem_186, // [/]
	ENSintaxParserC99TC3RulesElem_187, // [%]
	ENSintaxParserC99TC3RulesElem_188, // [<<]
	ENSintaxParserC99TC3RulesElem_189, // [>>]
	ENSintaxParserC99TC3RulesElem_190, // [<]
	ENSintaxParserC99TC3RulesElem_191, // [>]
	ENSintaxParserC99TC3RulesElem_192, // [<=]
	ENSintaxParserC99TC3RulesElem_193, // [>=]
	ENSintaxParserC99TC3RulesElem_194, // [==]
	ENSintaxParserC99TC3RulesElem_195, // [!=]
	ENSintaxParserC99TC3RulesElem_196, // [^]
	ENSintaxParserC99TC3RulesElem_197, // [|]
	ENSintaxParserC99TC3RulesElem_198, // [&&]
	ENSintaxParserC99TC3RulesElem_199, // [||]
	ENSintaxParserC99TC3RulesElem_200, // [?]
	ENSintaxParserC99TC3RulesElem_201, // [:]
	ENSintaxParserC99TC3RulesElem_202, // [;]
	ENSintaxParserC99TC3RulesElem_203, // [...]
	ENSintaxParserC99TC3RulesElem_204, // [=]
	ENSintaxParserC99TC3RulesElem_205, // [*=]
	ENSintaxParserC99TC3RulesElem_206, // [/=]
	ENSintaxParserC99TC3RulesElem_207, // [%=]
	ENSintaxParserC99TC3RulesElem_208, // [+=]
	ENSintaxParserC99TC3RulesElem_209, // [-=]
	ENSintaxParserC99TC3RulesElem_210, // [<<=]
	ENSintaxParserC99TC3RulesElem_211, // [>>=]
	ENSintaxParserC99TC3RulesElem_212, // [&=]
	ENSintaxParserC99TC3RulesElem_213, // [^=]
	ENSintaxParserC99TC3RulesElem_214, // [|=]
	ENSintaxParserC99TC3RulesElem_215, // [,]
	ENSintaxParserC99TC3RulesElem_216, // [#]
	ENSintaxParserC99TC3RulesElem_217, // [##]
	ENSintaxParserC99TC3RulesElem_218, // [<:]
	ENSintaxParserC99TC3RulesElem_219, // [:>]
	ENSintaxParserC99TC3RulesElem_220, // [<%]
	ENSintaxParserC99TC3RulesElem_221, // [%>]
	ENSintaxParserC99TC3RulesElem_222, // [%:]
	ENSintaxParserC99TC3RulesElem_223, // [%:%:]
	ENSintaxParserC99TC3RulesElem_224, // header-name
	ENSintaxParserC99TC3RulesElem_225, // h-char-sequence
	ENSintaxParserC99TC3RulesElem_226, // q-char-sequence
	ENSintaxParserC99TC3RulesElem_227, // h-char
	ENSintaxParserC99TC3RulesElem_228, // q-char
	ENSintaxParserC99TC3RulesElem_229, // primary-expression
	ENSintaxParserC99TC3RulesElem_230, // expression
	ENSintaxParserC99TC3RulesElem_231, // postfix-expression
	ENSintaxParserC99TC3RulesElem_232, // argument-expression-list
	ENSintaxParserC99TC3RulesElem_233, // type-name
	ENSintaxParserC99TC3RulesElem_234, // initializer-list
	ENSintaxParserC99TC3RulesElem_235, // assignment-expression
	ENSintaxParserC99TC3RulesElem_236, // unary-expression
	ENSintaxParserC99TC3RulesElem_237, // unary-operator
	ENSintaxParserC99TC3RulesElem_238, // cast-expression
	ENSintaxParserC99TC3RulesElem_239, // multiplicative-expression
	ENSintaxParserC99TC3RulesElem_240, // additive-expression
	ENSintaxParserC99TC3RulesElem_241, // shift-expression
	ENSintaxParserC99TC3RulesElem_242, // relational-expression
	ENSintaxParserC99TC3RulesElem_243, // equality-expression
	ENSintaxParserC99TC3RulesElem_244, // AND-expression
	ENSintaxParserC99TC3RulesElem_245, // exclusive-OR-expression
	ENSintaxParserC99TC3RulesElem_246, // inclusive-OR-expression
	ENSintaxParserC99TC3RulesElem_247, // logical-AND-expression
	ENSintaxParserC99TC3RulesElem_248, // logical-OR-expression
	ENSintaxParserC99TC3RulesElem_249, // conditional-expression
	ENSintaxParserC99TC3RulesElem_250, // assignment-operator
	ENSintaxParserC99TC3RulesElem_251, // constant-expression
	ENSintaxParserC99TC3RulesElem_252, // declaration
	ENSintaxParserC99TC3RulesElem_253, // declaration-specifiers
	ENSintaxParserC99TC3RulesElem_254, // init-declarator-list
	ENSintaxParserC99TC3RulesElem_255, // storage-class-specifier
	ENSintaxParserC99TC3RulesElem_256, // type-specifier
	ENSintaxParserC99TC3RulesElem_257, // type-qualifier
	ENSintaxParserC99TC3RulesElem_258, // function-specifier
	ENSintaxParserC99TC3RulesElem_259, // init-declarator
	ENSintaxParserC99TC3RulesElem_260, // declarator
	ENSintaxParserC99TC3RulesElem_261, // initializer
	ENSintaxParserC99TC3RulesElem_262, // struct-or-union-specifier
	ENSintaxParserC99TC3RulesElem_263, // enum-specifier
	ENSintaxParserC99TC3RulesElem_264, // typedef-name
	ENSintaxParserC99TC3RulesElem_265, // struct-or-union
	ENSintaxParserC99TC3RulesElem_266, // struct-declaration-list
	ENSintaxParserC99TC3RulesElem_267, // struct-declaration
	ENSintaxParserC99TC3RulesElem_268, // specifier-qualifier-list
	ENSintaxParserC99TC3RulesElem_269, // struct-declarator-list
	ENSintaxParserC99TC3RulesElem_270, // struct-declarator
	ENSintaxParserC99TC3RulesElem_271, // enumerator-list
	ENSintaxParserC99TC3RulesElem_272, // enumerator
	ENSintaxParserC99TC3RulesElem_273, // pointer
	ENSintaxParserC99TC3RulesElem_274, // direct-declarator
	ENSintaxParserC99TC3RulesElem_275, // type-qualifier-list
	ENSintaxParserC99TC3RulesElem_276, // parameter-type-list
	ENSintaxParserC99TC3RulesElem_277, // identifier-list
	ENSintaxParserC99TC3RulesElem_278, // parameter-list
	ENSintaxParserC99TC3RulesElem_279, // parameter-declaration
	ENSintaxParserC99TC3RulesElem_280, // abstract-declarator
	ENSintaxParserC99TC3RulesElem_281, // direct-abstract-declarator
	ENSintaxParserC99TC3RulesElem_282, // designation
	ENSintaxParserC99TC3RulesElem_283, // designator-list
	ENSintaxParserC99TC3RulesElem_284, // designator
	ENSintaxParserC99TC3RulesElem_285, // statement
	ENSintaxParserC99TC3RulesElem_286, // labeled-statement
	ENSintaxParserC99TC3RulesElem_287, // compound-statement
	ENSintaxParserC99TC3RulesElem_288, // expression-statement
	ENSintaxParserC99TC3RulesElem_289, // selection-statement
	ENSintaxParserC99TC3RulesElem_290, // iteration-statement
	ENSintaxParserC99TC3RulesElem_291, // jump-statement
	ENSintaxParserC99TC3RulesElem_292, // block-item-list
	ENSintaxParserC99TC3RulesElem_293, // block-item
	ENSintaxParserC99TC3RulesElem_294, // translation-unit
	ENSintaxParserC99TC3RulesElem_295, // external-declaration
	ENSintaxParserC99TC3RulesElem_296, // function-definition
	ENSintaxParserC99TC3RulesElem_297, // declaration-list
	ENSintaxParserC99TC3RulesElem_298, // preprocessing-file
	ENSintaxParserC99TC3RulesElem_299, // group
	ENSintaxParserC99TC3RulesElem_300, // group-part
	ENSintaxParserC99TC3RulesElem_301, // if-section
	ENSintaxParserC99TC3RulesElem_302, // control-line
	ENSintaxParserC99TC3RulesElem_303, // text-line
	ENSintaxParserC99TC3RulesElem_304, // non-directive
	ENSintaxParserC99TC3RulesElem_305, // if-group
	ENSintaxParserC99TC3RulesElem_306, // elif-groups
	ENSintaxParserC99TC3RulesElem_307, // else-group
	ENSintaxParserC99TC3RulesElem_308, // endif-line
	ENSintaxParserC99TC3RulesElem_309, // new-line
	ENSintaxParserC99TC3RulesElem_310, // [ifdef]
	ENSintaxParserC99TC3RulesElem_311, // [ifndef]
	ENSintaxParserC99TC3RulesElem_312, // elif-group
	ENSintaxParserC99TC3RulesElem_313, // [elif]
	ENSintaxParserC99TC3RulesElem_314, // [endif]
	ENSintaxParserC99TC3RulesElem_315, // [include]
	ENSintaxParserC99TC3RulesElem_316, // pp-tokens
	ENSintaxParserC99TC3RulesElem_317, // [define]
	ENSintaxParserC99TC3RulesElem_318, // replacement-list
	ENSintaxParserC99TC3RulesElem_319, // lparen
	ENSintaxParserC99TC3RulesElem_320, // [undef]
	ENSintaxParserC99TC3RulesElem_321, // [line]
	ENSintaxParserC99TC3RulesElem_322, // [error]
	ENSintaxParserC99TC3RulesElem_323, // [pragma]
	//Count
	ENSintaxParserC99TC3RulesElem_Count
} ENSintaxParserC99TC3RulesElem;


//Static data

const char* NBSintaxParserCDefs_getDefaultSintaxRulesStr(void);
const char* NBSintaxParserCDefs_getRuleNameByIdx(const UI32 iRule);
ENSintaxParserC99TC3Rule NBSintaxParserCDefs_getRuleIdxByElemIdx(const UI32 iElem);

#ifdef __cplusplus
} //extern "C"
#endif

#endif
