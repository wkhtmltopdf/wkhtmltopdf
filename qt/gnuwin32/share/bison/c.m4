                                                            -*- Autoconf -*-

# C M4 Macros for Bison.
# Copyright (C) 2002, 2004, 2005, 2006, 2007, 2008 Free Software
# Foundation, Inc.

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

## ---------------- ##
## Identification.  ##
## ---------------- ##

# b4_comment(TEXT)
# ----------------
m4_define([b4_comment], [/* m4_bpatsubst([$1], [
], [
   ])  */])

# b4_identification
# -----------------
# Depends on individual skeletons to define b4_pure_flag, b4_push_flag, or
# b4_pull_flag if they use the values of the %define variables api.pure or
# api.push_pull.
m4_define([b4_identification],
[[/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "]b4_version["

/* Skeleton name.  */
#define YYSKELETON_NAME ]b4_skeleton[]m4_ifdef([b4_pure_flag], [[

/* Pure parsers.  */
#define YYPURE ]b4_pure_flag])[]m4_ifdef([b4_push_flag], [[

/* Push parsers.  */
#define YYPUSH ]b4_push_flag])[]m4_ifdef([b4_pull_flag], [[

/* Pull parsers.  */
#define YYPULL ]b4_pull_flag])[

/* Using locations.  */
#define YYLSP_NEEDED ]b4_locations_flag[
]])


## ---------------- ##
## Default values.  ##
## ---------------- ##

# If the %union is not named, its name is YYSTYPE.
m4_define_default([b4_union_name], [YYSTYPE])

# If the %name-prefix is not given, it is yy.
m4_define_default([b4_prefix], [yy])

## ------------------------ ##
## Pure/impure interfaces.  ##
## ------------------------ ##

# b4_user_args
# ------------
m4_define([b4_user_args],
[m4_ifset([b4_parse_param], [, b4_c_args(b4_parse_param)])])


# b4_parse_param
# --------------
# If defined, b4_parse_param arrives double quoted, but below we prefer
# it to be single quoted.
m4_define([b4_parse_param],
b4_parse_param)


# b4_parse_param_for(DECL, FORMAL, BODY)
# ---------------------------------------
# Iterate over the user parameters, binding the declaration to DECL,
# the formal name to FORMAL, and evaluating the BODY.
m4_define([b4_parse_param_for],
[m4_foreach([$1_$2], m4_defn([b4_parse_param]),
[m4_pushdef([$1], m4_unquote(m4_car($1_$2)))dnl
m4_pushdef([$2], m4_shift($1_$2))dnl
$3[]dnl
m4_popdef([$2])dnl
m4_popdef([$1])dnl
])])

# b4_parse_param_use
# ------------------
# `YYUSE' all the parse-params.
m4_define([b4_parse_param_use],
[b4_parse_param_for([Decl], [Formal], [  YYUSE (Formal);
])dnl
])


## ------------ ##
## Data Types.  ##
## ------------ ##

# b4_int_type(MIN, MAX)
# ---------------------
# Return the smallest int type able to handle numbers ranging from
# MIN to MAX (included).
m4_define([b4_int_type],
[m4_if(b4_ints_in($@,      [0],   [255]), [1], [unsigned char],
       b4_ints_in($@,   [-128],   [127]), [1], [signed char],

       b4_ints_in($@,      [0], [65535]), [1], [unsigned short int],
       b4_ints_in($@, [-32768], [32767]), [1], [short int],

       m4_eval([0 <= $1]),                [1], [unsigned int],

					       [int])])


# b4_int_type_for(NAME)
# ---------------------
# Return the smallest int type able to handle numbers ranging from
# `NAME_min' to `NAME_max' (included).
m4_define([b4_int_type_for],
[b4_int_type($1_min, $1_max)])



## ---------##
## Values.  ##
## ---------##

# b4_null
---------
# Return a null pointer constant.  NULL infringes on the user name
# space in C, so use 0 rather than NULL.
m4_define([b4_null], [0])




## ------------------------- ##
## Assigning token numbers.  ##
## ------------------------- ##

# b4_token_define(TOKEN-NAME, TOKEN-NUMBER)
# -----------------------------------------
# Output the definition of this token as #define.
m4_define([b4_token_define],
[#define $1 $2
])


# b4_token_defines(LIST-OF-PAIRS-TOKEN-NAME-TOKEN-NUMBER)
# -------------------------------------------------------
# Output the definition of the tokens (if there are) as #defines.
m4_define([b4_token_defines],
[m4_if([$#$1], [1], [],
[/* Tokens.  */
m4_map([b4_token_define], [$@])])
])


# b4_token_enum(TOKEN-NAME, TOKEN-NUMBER)
# ---------------------------------------
# Output the definition of this token as an enum.
m4_define([b4_token_enum],
[$1 = $2])


# b4_token_enums(LIST-OF-PAIRS-TOKEN-NAME-TOKEN-NUMBER)
# -----------------------------------------------------
# Output the definition of the tokens (if there are) as enums.
m4_define([b4_token_enums],
[m4_if([$#$1], [1], [],
[/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
m4_map_sep([     b4_token_enum], [,
],
	   [$@])
   };
#endif
])])


# b4_token_enums_defines(LIST-OF-PAIRS-TOKEN-NAME-TOKEN-NUMBER)
# -------------------------------------------------------------
# Output the definition of the tokens (if there are any) as enums and, if POSIX
# Yacc is enabled, as #defines.
m4_define([b4_token_enums_defines],
[b4_token_enums($@)b4_yacc_if([b4_token_defines($@)], [])
])



## --------------------------------------------- ##
## Defining C functions in both K&R and ANSI-C.  ##
## --------------------------------------------- ##


# b4_modern_c
# -----------
# A predicate useful in #if to determine whether C is ancient or modern.
#
# If __STDC__ is defined, the compiler is modern.  IBM xlc 7.0 when run
# as 'cc' doesn't define __STDC__ (or __STDC_VERSION__) for pedantic
# reasons, but it defines __C99__FUNC__ so check that as well.
# Microsoft C normally doesn't define these macros, but it defines _MSC_VER.
# Consider a C++ compiler to be modern if it defines __cplusplus.
#
m4_define([b4_c_modern],
  [[(defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)]])

# b4_c_function_def(NAME, RETURN-VALUE, [DECL1, NAME1], ...)
# ----------------------------------------------------------
# Declare the function NAME.
m4_define([b4_c_function_def],
[#if b4_c_modern
b4_c_ansi_function_def($@)
#else
$2
$1 (b4_c_knr_formal_names(m4_shift2($@)))
b4_c_knr_formal_decls(m4_shift2($@))
#endif[]dnl
])


# b4_c_ansi_function_def(NAME, RETURN-VALUE, [DECL1, NAME1], ...)
# ---------------------------------------------------------------
# Declare the function NAME in ANSI.
m4_define([b4_c_ansi_function_def],
[$2
$1 (b4_c_ansi_formals(m4_shift2($@)))[]dnl
])


# b4_c_ansi_formals([DECL1, NAME1], ...)
# --------------------------------------
# Output the arguments ANSI-C definition.
m4_define([b4_c_ansi_formals],
[m4_if([$#], [0], [void],
       [$#$1], [1], [void],
	       [m4_map_sep([b4_c_ansi_formal], [, ], [$@])])])

m4_define([b4_c_ansi_formal],
[$1])


# b4_c_knr_formal_names([DECL1, NAME1], ...)
# ------------------------------------------
# Output the argument names.
m4_define([b4_c_knr_formal_names],
[m4_map_sep([b4_c_knr_formal_name], [, ], [$@])])

m4_define([b4_c_knr_formal_name],
[$2])


# b4_c_knr_formal_decls([DECL1, NAME1], ...)
# ------------------------------------------
# Output the K&R argument declarations.
m4_define([b4_c_knr_formal_decls],
[m4_map_sep([b4_c_knr_formal_decl],
	    [
],
	    [$@])])

m4_define([b4_c_knr_formal_decl],
[    $1;])



## ------------------------------------------------------------ ##
## Declaring (prototyping) C functions in both K&R and ANSI-C.  ##
## ------------------------------------------------------------ ##


# b4_c_function_decl(NAME, RETURN-VALUE, [DECL1, NAME1], ...)
# -----------------------------------------------------------
# Declare the function NAME.
m4_define([b4_c_function_decl],
[#if defined __STDC__ || defined __cplusplus
b4_c_ansi_function_decl($@)
#else
$2 $1 ();
#endif[]dnl
])


# b4_c_ansi_function_decl(NAME, RETURN-VALUE, [DECL1, NAME1], ...)
# ----------------------------------------------------------------
# Declare the function NAME.
m4_define([b4_c_ansi_function_decl],
[$2 $1 (b4_c_ansi_formals(m4_shift2($@)));[]dnl
])




## --------------------- ##
## Calling C functions.  ##
## --------------------- ##


# b4_c_function_call(NAME, RETURN-VALUE, [DECL1, NAME1], ...)
# -----------------------------------------------------------
# Call the function NAME with arguments NAME1, NAME2 etc.
m4_define([b4_c_function_call],
[$1 (b4_c_args(m4_shift2($@)))[]dnl
])


# b4_c_args([DECL1, NAME1], ...)
# ------------------------------
# Output the arguments NAME1, NAME2...
m4_define([b4_c_args],
[m4_map_sep([b4_c_arg], [, ], [$@])])

m4_define([b4_c_arg],
[$2])


## ----------- ##
## Synclines.  ##
## ----------- ##

# b4_sync_start(LINE, FILE)
# -----------------------
m4_define([b4_sync_start], [[#]line $1 $2])


## -------------- ##
## User actions.  ##
## -------------- ##

# b4_case(LABEL, STATEMENTS)
# --------------------------
m4_define([b4_case],
[  case $1:
$2
    break;])

# b4_symbol_actions(FILENAME, LINENO,
#                   SYMBOL-TAG, SYMBOL-NUM,
#                   SYMBOL-ACTION, SYMBOL-TYPENAME)
# -------------------------------------------------
m4_define([b4_symbol_actions],
[m4_pushdef([b4_dollar_dollar],
   [m4_ifval([$6], [(yyvaluep->$6)], [(*yyvaluep)])])dnl
m4_pushdef([b4_at_dollar], [(*yylocationp)])dnl
      case $4: /* $3 */
b4_syncline([$2], [$1])
	$5;
b4_syncline([@oline@], [@ofile@])
	break;
m4_popdef([b4_at_dollar])dnl
m4_popdef([b4_dollar_dollar])dnl
])


# b4_yydestruct_generate(FUNCTION-DECLARATOR)
# -------------------------------------------
# Generate the "yydestruct" function, which declaration is issued using
# FUNCTION-DECLARATOR, which may be "b4_c_ansi_function_def" for ISO C
# or "b4_c_function_def" for K&R.
m4_define_default([b4_yydestruct_generate],
[[/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
]$1([yydestruct],
    [static void],
    [[const char *yymsg],    [yymsg]],
    [[int yytype],           [yytype]],
    [[YYSTYPE *yyvaluep],    [yyvaluep]][]dnl
b4_locations_if(            [, [[YYLTYPE *yylocationp], [yylocationp]]])[]dnl
m4_ifset([b4_parse_param], [, b4_parse_param]))[
{
  YYUSE (yyvaluep);
]b4_locations_if([  YYUSE (yylocationp);
])dnl
b4_parse_param_use[]dnl
[
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {
]m4_map([b4_symbol_actions], m4_defn([b4_symbol_destructors]))[
      default:
	break;
    }
}]dnl
])


# b4_yy_symbol_print_generate(FUNCTION-DECLARATOR)
# ------------------------------------------------
# Generate the "yy_symbol_print" function, which declaration is issued using
# FUNCTION-DECLARATOR, which may be "b4_c_ansi_function_def" for ISO C
# or "b4_c_function_def" for K&R.
m4_define_default([b4_yy_symbol_print_generate],
[[
/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
]$1([yy_symbol_value_print],
    [static void],
	       [[FILE *yyoutput],                       [yyoutput]],
	       [[int yytype],                           [yytype]],
	       [[YYSTYPE const * const yyvaluep],       [yyvaluep]][]dnl
b4_locations_if([, [[YYLTYPE const * const yylocationp], [yylocationp]]])[]dnl
m4_ifset([b4_parse_param], [, b4_parse_param]))[
{
  if (!yyvaluep)
    return;
]b4_locations_if([  YYUSE (yylocationp);
])dnl
b4_parse_param_use[]dnl
[# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
]m4_map([b4_symbol_actions], m4_defn([b4_symbol_printers]))dnl
[      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

]$1([yy_symbol_print],
    [static void],
	       [[FILE *yyoutput],                       [yyoutput]],
	       [[int yytype],                           [yytype]],
	       [[YYSTYPE const * const yyvaluep],       [yyvaluep]][]dnl
b4_locations_if([, [[YYLTYPE const * const yylocationp], [yylocationp]]])[]dnl
m4_ifset([b4_parse_param], [, b4_parse_param]))[
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

]b4_locations_if([  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
])dnl
[  yy_symbol_value_print (yyoutput, yytype, yyvaluep]dnl
b4_locations_if([, yylocationp])[]b4_user_args[);
  YYFPRINTF (yyoutput, ")");
}]dnl
])
