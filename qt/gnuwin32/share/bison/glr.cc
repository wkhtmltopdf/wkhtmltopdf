                                                                    -*- C -*-

# C++ GLR skeleton for Bison
# Copyright (C) 2002, 2003, 2004, 2005, 2006, 2007 Free Software Foundation,
# Inc.

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


# This skeleton produces a C++ class that encapsulates a C glr parser.
# This is in order to reduce the maintenance burden.  The glr.c
# skeleton is clean and pure enough so that there are no real
# problems.  The C++ interface is the same as that of lalr1.cc.  In
# fact, glr.c can replace yacc.c without the user noticing any
# difference, and similarly for glr.cc replacing lalr1.cc.
#
# The passing of parse-params
#
#   The additional arguments are stored as members of the parser
#   object, yyparser.  The C routines need to carry yyparser
#   throughout the C parser; that easy: just let yyparser become an
#   additional parse-param.  But because the C++ skeleton needs to
#   know the "real" original parse-param, we save them
#   (b4_parse_param_orig).  Note that b4_parse_param is overquoted
#   (and c.m4 strips one level of quotes).  This is a PITA, and
#   explains why there are so many levels of quotes.
#
# The locations
#
#   We use location.cc just like lalr1.cc, but because glr.c stores
#   the locations in a (C++) union, the position and location classes
#   must not have a constructor.  Therefore, contrary to lalr1.cc, we
#   must not define "b4_location_constructors".  As a consequence the
#   user must initialize the first positions (in particular the
#   filename member).

# We require a pure interface using locations.
m4_define([b4_locations_flag], [1])
m4_define([b4_pure_flag],      [1])

# The header is mandatory.
b4_defines_if([],
              [b4_fatal([b4_skeleton[: using %%defines is mandatory]])])

m4_include(b4_pkgdatadir/[c++.m4])
m4_include(b4_pkgdatadir/[location.cc])

m4_define([b4_parser_class_name],
          [b4_percent_define_get([[parser_class_name]])])

# Save the parse parameters.
m4_define([b4_parse_param_orig], m4_defn([b4_parse_param]))


# b4_yy_symbol_print_generate
# ---------------------------
# Bypass the default implementation to generate the "yy_symbol_print"
# and "yy_symbol_value_print" functions.
m4_define([b4_yy_symbol_print_generate],
[[
/*--------------------.
| Print this symbol.  |
`--------------------*/

]b4_c_ansi_function_def([yy_symbol_print],
    [static void],
    [[FILE *],               []],
    [[int yytype],           [yytype]],
    [[const b4_namespace_ref::b4_parser_class_name::semantic_type *yyvaluep],
			     [yyvaluep]],
    [[const b4_namespace_ref::b4_parser_class_name::location_type *yylocationp],
			     [yylocationp]],
    b4_parse_param)[
{
]b4_parse_param_use[]dnl
[  yyparser.yy_symbol_print_ (yytype, yyvaluep]b4_locations_if([, yylocationp])[);
}
]])


# Declare yyerror.
m4_append([b4_post_prologue],
[b4_syncline([@oline@], [@ofile@])

b4_c_ansi_function_decl([yyerror],
    [static void],
    [[b4_namespace_ref::b4_parser_class_name::location_type *yylocationp], [yylocationp]],
    b4_parse_param,
    [[const char* msg], [msg]])])


# Define yyerror.
m4_append([b4_epilogue],
[b4_syncline([@oline@], [@ofile@])[
/*------------------.
| Report an error.  |
`------------------*/

]b4_c_ansi_function_def([yyerror],
    [static void],
    [[b4_namespace_ref::b4_parser_class_name::location_type *yylocationp], [yylocationp]],
    b4_parse_param,
    [[const char* msg], [msg]])[
{
]b4_parse_param_use[]dnl
[  yyparser.error (*yylocationp, msg);
}


]b4_namespace_open[
]dnl In this section, the parse param are the original parse_params.
m4_pushdef([b4_parse_param], m4_defn([b4_parse_param_orig]))dnl
[  /// Build a parser object.
  ]b4_parser_class_name::b4_parser_class_name[ (]b4_parse_param_decl[)]m4_ifset([b4_parse_param], [
    :])[
#if YYDEBUG
    ]m4_ifset([b4_parse_param], [  ], [ :])[yydebug_ (false),
      yycdebug_ (&std::cerr)]m4_ifset([b4_parse_param], [,])[
#endif]b4_parse_param_cons[
  {
  }

  ]b4_parser_class_name::~b4_parser_class_name[ ()
  {
  }

  int
  ]b4_parser_class_name[::parse ()
  {
    return ::yyparse (*this]b4_user_args[);
  }

#if YYDEBUG
  /*--------------------.
  | Print this symbol.  |
  `--------------------*/

  inline void
  ]b4_parser_class_name[::yy_symbol_value_print_ (int yytype,
			   const semantic_type* yyvaluep, const location_type* yylocationp)
  {
    /* Pacify ``unused variable'' warnings.  */
    YYUSE (yyvaluep);
    YYUSE (yylocationp);
    switch (yytype)
      {
  ]m4_map([b4_symbol_actions], m4_defn([b4_symbol_printers]))dnl
[        default:
	  break;
      }
  }


  void
  ]b4_parser_class_name[::yy_symbol_print_ (int yytype,
			   const semantic_type* yyvaluep, const location_type* yylocationp)
  {
    *yycdebug_ << (yytype < YYNTOKENS ? "token" : "nterm")
	       << ' ' << yytname[yytype] << " ("
	       << *yylocationp << ": ";
    yy_symbol_value_print_ (yytype, yyvaluep, yylocationp);
    *yycdebug_ << ')';
  }

  std::ostream&
  ]b4_parser_class_name[::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  ]b4_parser_class_name[::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  ]b4_parser_class_name[::debug_level_type
  ]b4_parser_class_name[::debug_level () const
  {
    return yydebug_;
  }

  void
  ]b4_parser_class_name[::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }

#endif
]m4_popdef([b4_parse_param])dnl
b4_namespace_close[

]])


# Let glr.c believe that the user arguments include the parser itself.
m4_ifset([b4_parse_param],
[m4_pushdef([b4_parse_param],
	    m4_dquote([[[b4_namespace_ref::b4_parser_class_name& yyparser], [[yyparser]]],]
m4_defn([b4_parse_param])))],
[m4_pushdef([b4_parse_param],
	    [[[[b4_namespace_ref::b4_parser_class_name& yyparser], [[yyparser]]]]])
])
m4_include(b4_pkgdatadir/[glr.c])
m4_popdef([b4_parse_param])

m4_divert_push(0)
@output(b4_spec_defines_file@)
b4_copyright([Skeleton interface for Bison GLR parsers in C++],
  [2002, 2003, 2004, 2005, 2006])[

/* C++ GLR parser skeleton written by Akim Demaille.  */

#ifndef PARSER_HEADER_H
# define PARSER_HEADER_H

]b4_percent_code_get([[requires]])[

#include <string>
#include <iostream>

/* Using locations.  */
#define YYLSP_NEEDED ]b4_locations_flag[

]b4_namespace_open[
  class position;
  class location;
]b4_namespace_close[

#include "location.hh"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG ]b4_debug_flag[
#endif

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (N)								\
	{								\
	  (Current).begin  = YYRHSLOC (Rhs, 1).begin;			\
	  (Current).end    = YYRHSLOC (Rhs, N).end;			\
	}								\
      else								\
	{								\
	  (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;	\
	}								\
    while (/*CONSTCOND*/ 0)
#endif

]b4_namespace_open[
  /// A Bison parser.
  class ]b4_parser_class_name[
  {
  public:
    /// Symbol semantic values.
#ifndef YYSTYPE
]m4_ifdef([b4_stype],
[    union semantic_type
    {
b4_user_stype
    };],
[m4_if(b4_tag_seen_flag, 0,
[[    typedef int semantic_type;]],
[[    typedef YYSTYPE semantic_type;]])])[
#else
    typedef YYSTYPE semantic_type;
#endif
    /// Symbol locations.
    typedef ]b4_percent_define_get([[location_type]])[ location_type;
    /// Tokens.
    struct token
    {
      ]b4_token_enums(b4_tokens)[
    };
    /// Token type.
    typedef token::yytokentype token_type;

    /// Build a parser object.
    ]b4_parser_class_name[ (]b4_parse_param_decl[);
    virtual ~]b4_parser_class_name[ ();

    /// Parse.
    /// \returns  0 iff parsing succeeded.
    virtual int parse ();

    /// The current debugging stream.
    std::ostream& debug_stream () const;
    /// Set the current debugging stream.
    void set_debug_stream (std::ostream &);

    /// Type for debugging levels.
    typedef int debug_level_type;
    /// The current debugging level.
    debug_level_type debug_level () const;
    /// Set the current debugging level.
    void set_debug_level (debug_level_type l);

  private:

  public:
    /// Report a syntax error.
    /// \param loc    where the syntax error is found.
    /// \param msg    a description of the syntax error.
    virtual void error (const location_type& loc, const std::string& msg);
  private:

#if YYDEBUG
  public:
    /// \brief Report a symbol value on the debug stream.
    /// \param yytype       The token type.
    /// \param yyvaluep     Its semantic value.
    /// \param yylocationp  Its location.
    virtual void yy_symbol_value_print_ (int yytype,
					 const semantic_type* yyvaluep,
					 const location_type* yylocationp);
    /// \brief Report a symbol on the debug stream.
    /// \param yytype       The token type.
    /// \param yyvaluep     Its semantic value.
    /// \param yylocationp  Its location.
    virtual void yy_symbol_print_ (int yytype,
				   const semantic_type* yyvaluep,
				   const location_type* yylocationp);
  private:
    /* Debugging.  */
    int yydebug_;
    std::ostream* yycdebug_;
#endif


    /// \brief Reclaim the memory associated to a symbol.
    /// \param yymsg        Why this token is reclaimed.
    /// \param yytype       The symbol type.
    /// \param yyvaluep     Its semantic value.
    /// \param yylocationp  Its location.
    inline void yydestruct_ (const char* yymsg,
			     int yytype,
			     semantic_type* yyvaluep,
			     location_type* yylocationp);

]b4_parse_param_vars[
  };

]dnl Redirections for glr.c.
b4_percent_define_flag_if([[global_tokens_and_yystype]],
[b4_token_defines(b4_tokens)])
[
#ifndef YYSTYPE
# define YYSTYPE ]b4_namespace_ref[::]b4_parser_class_name[::semantic_type
#endif
#ifndef YYLTYPE
# define YYLTYPE ]b4_namespace_ref[::]b4_parser_class_name[::location_type
#endif

]b4_namespace_close[

]b4_percent_code_get([[provides]])[]dnl

[#endif /* ! defined PARSER_HEADER_H */]
m4_divert_pop(0)
