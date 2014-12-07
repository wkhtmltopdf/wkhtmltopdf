divert(-1)#                                                  -*- Autoconf -*-
# This file is part of Autoconf.
# Base M4 layer.
# Requires GNU M4.
#
# Copyright (C) 1999, 2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007,
# 2008 Free Software Foundation, Inc.
#
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

# As a special exception, the Free Software Foundation gives unlimited
# permission to copy, distribute and modify the configure scripts that
# are the output of Autoconf.  You need not follow the terms of the GNU
# General Public License when using or distributing such scripts, even
# though portions of the text of Autoconf appear in them.  The GNU
# General Public License (GPL) does govern all other use of the material
# that constitutes the Autoconf program.
#
# Certain portions of the Autoconf source text are designed to be copied
# (in certain cases, depending on the input) into the output of
# Autoconf.  We call these the "data" portions.  The rest of the Autoconf
# source text consists of comments plus executable code that decides which
# of the data portions to output in any given case.  We call these
# comments and executable code the "non-data" portions.  Autoconf never
# copies any of the non-data portions into its output.
#
# This special exception to the GPL applies to versions of Autoconf
# released by the Free Software Foundation.  When you make and
# distribute a modified version of Autoconf, you may extend this special
# exception to the GPL to apply to your modified version as well, *unless*
# your modified version has the potential to copy into its output some
# of the text that was the non-data portion of the version that you started
# with.  (In other words, unless your change moves or copies text from
# the non-data portions to the data portions.)  If your modification has
# such potential, you must delete any notice of this special exception
# to the GPL from your modified version.
#
# Written by Akim Demaille.
#

# Set the quotes, whatever the current quoting system.
changequote()
changequote([, ])

# Some old m4's don't support m4exit.  But they provide
# equivalent functionality by core dumping because of the
# long macros we define.
ifdef([__gnu__], ,
[errprint(M4sugar requires GNU M4. Install it before installing M4sugar or
set the M4 environment variable to its absolute file name.)
m4exit(2)])


## ------------------------------- ##
## 1. Simulate --prefix-builtins.  ##
## ------------------------------- ##

# m4_define
# m4_defn
# m4_undefine
define([m4_define],   defn([define]))
define([m4_defn],     defn([defn]))
define([m4_undefine], defn([undefine]))

m4_undefine([define])
m4_undefine([defn])
m4_undefine([undefine])


# m4_copy(SRC, DST)
# -----------------
# Define DST as the definition of SRC.
# What's the difference between:
# 1. m4_copy([from], [to])
# 2. m4_define([to], [from($@)])
# Well, obviously 1 is more expensive in space.  Maybe 2 is more expensive
# in time, but because of the space cost of 1, it's not that obvious.
# Nevertheless, one huge difference is the handling of `$0'.  If `from'
# uses `$0', then with 1, `to''s `$0' is `to', while it is `from' in 2.
# The user would certainly prefer to see `to'.
m4_define([m4_copy],
[m4_define([$2], m4_defn([$1]))])


# m4_rename(SRC, DST)
# -------------------
# Rename the macro SRC to DST.
m4_define([m4_rename],
[m4_copy([$1], [$2])m4_undefine([$1])])


# m4_rename_m4(MACRO-NAME)
# ------------------------
# Rename MACRO-NAME to m4_MACRO-NAME.
m4_define([m4_rename_m4],
[m4_rename([$1], [m4_$1])])


# m4_copy_unm4(m4_MACRO-NAME)
# ---------------------------
# Copy m4_MACRO-NAME to MACRO-NAME.
m4_define([m4_copy_unm4],
[m4_copy([$1], m4_bpatsubst([$1], [^m4_\(.*\)], [[\1]]))])


# Some m4 internals have names colliding with tokens we might use.
# Rename them a` la `m4 --prefix-builtins'.  Conditionals first, since
# some subsequent renames are conditional.
m4_rename_m4([ifdef])
m4_rename([ifelse], [m4_if])

m4_rename_m4([builtin])
m4_rename_m4([changecom])
m4_rename_m4([changequote])
m4_ifdef([changeword],dnl conditionally available in 1.4.x
[m4_undefine([changeword])])
m4_rename_m4([debugfile])
m4_rename_m4([debugmode])
m4_rename_m4([decr])
m4_undefine([divert])
m4_rename_m4([divnum])
m4_rename_m4([dumpdef])
m4_rename_m4([errprint])
m4_rename_m4([esyscmd])
m4_rename_m4([eval])
m4_rename_m4([format])
m4_undefine([include])
m4_rename_m4([incr])
m4_rename_m4([index])
m4_rename_m4([indir])
m4_rename_m4([len])
m4_rename([m4exit], [m4_exit])
m4_undefine([m4wrap])
m4_ifdef([mkstemp],dnl added in M4 1.4.8
[m4_rename_m4([mkstemp])
m4_copy([m4_mkstemp], [m4_maketemp])
m4_undefine([maketemp])],
[m4_rename_m4([maketemp])
m4_copy([m4_maketemp], [m4_mkstemp])])
m4_rename([patsubst], [m4_bpatsubst])
m4_rename_m4([popdef])
m4_rename_m4([pushdef])
m4_rename([regexp], [m4_bregexp])
m4_rename_m4([shift])
m4_undefine([sinclude])
m4_rename_m4([substr])
m4_ifdef([symbols],dnl present only in alpha-quality 1.4o
[m4_rename_m4([symbols])])
m4_rename_m4([syscmd])
m4_rename_m4([sysval])
m4_rename_m4([traceoff])
m4_rename_m4([traceon])
m4_rename_m4([translit])
m4_undefine([undivert])


## ------------------- ##
## 2. Error messages.  ##
## ------------------- ##


# m4_location
# -----------
m4_define([m4_location],
[__file__:__line__])


# m4_errprintn(MSG)
# -----------------
# Same as `errprint', but with the missing end of line.
m4_define([m4_errprintn],
[m4_errprint([$1
])])


# m4_warning(MSG)
# ---------------
# Warn the user.
m4_define([m4_warning],
[m4_errprintn(m4_location[: warning: $1])])


# m4_fatal(MSG, [EXIT-STATUS])
# ----------------------------
# Fatal the user.                                                      :)
m4_define([m4_fatal],
[m4_errprintn(m4_location[: error: $1])dnl
m4_expansion_stack_dump()dnl
m4_exit(m4_if([$2],, 1, [$2]))])


# m4_assert(EXPRESSION, [EXIT-STATUS = 1])
# ----------------------------------------
# This macro ensures that EXPRESSION evaluates to true, and exits if
# EXPRESSION evaluates to false.
m4_define([m4_assert],
[m4_if(m4_eval([$1]), 0,
       [m4_fatal([assert failed: $1], [$2])])])



## ------------- ##
## 3. Warnings.  ##
## ------------- ##


# _m4_warn(CATEGORY, MESSAGE, STACK-TRACE)
# ----------------------------------------
# Report a MESSAGE to the user if the CATEGORY of warnings is enabled.
# This is for traces only.
# The STACK-TRACE is a \n-separated list of "LOCATION: MESSAGE".
#
# Within m4, the macro is a no-op.  This macro really matters
# when autom4te post-processes the trace output.
m4_define([_m4_warn], [])


# m4_warn(CATEGORY, MESSAGE)
# --------------------------
# Report a MESSAGE to the user if the CATEGORY of warnings is enabled.
m4_define([m4_warn],
[_m4_warn([$1], [$2],
m4_ifdef([m4_expansion_stack],
	 [_m4_defn([m4_expansion_stack])
m4_location[: the top level]]))dnl
])



## ------------------- ##
## 4. File inclusion.  ##
## ------------------- ##


# We also want to neutralize include (and sinclude for symmetry),
# but we want to extend them slightly: warn when a file is included
# several times.  This is, in general, a dangerous operation, because
# too many people forget to quote the first argument of m4_define.
#
# For instance in the following case:
#   m4_define(foo, [bar])
# then a second reading will turn into
#   m4_define(bar, [bar])
# which is certainly not what was meant.

# m4_include_unique(FILE)
# -----------------------
# Declare that the FILE was loading; and warn if it has already
# been included.
m4_define([m4_include_unique],
[m4_ifdef([m4_include($1)],
	  [m4_warn([syntax], [file `$1' included several times])])dnl
m4_define([m4_include($1)])])


# m4_include(FILE)
# ----------------
# Like the builtin include, but warns against multiple inclusions.
m4_define([m4_include],
[m4_include_unique([$1])dnl
m4_builtin([include], [$1])])


# m4_sinclude(FILE)
# -----------------
# Like the builtin sinclude, but warns against multiple inclusions.
m4_define([m4_sinclude],
[m4_include_unique([$1])dnl
m4_builtin([sinclude], [$1])])



## ------------------------------------ ##
## 5. Additional branching constructs.  ##
## ------------------------------------ ##

# Both `m4_ifval' and `m4_ifset' tests against the empty string.  The
# difference is that `m4_ifset' is specialized on macros.
#
# In case of arguments of macros, eg. $1, it makes little difference.
# In the case of a macro `FOO', you don't want to check `m4_ifval(FOO,
# TRUE)', because if `FOO' expands with commas, there is a shifting of
# the arguments.  So you want to run `m4_ifval([FOO])', but then you just
# compare the *string* `FOO' against `', which, of course fails.
#
# So you want the variation `m4_ifset' that expects a macro name as $1.
# If this macro is both defined and defined to a non empty value, then
# it runs TRUE, etc.


# m4_ifval(COND, [IF-TRUE], [IF-FALSE])
# -------------------------------------
# If COND is not the empty string, expand IF-TRUE, otherwise IF-FALSE.
# Comparable to m4_ifdef.
m4_define([m4_ifval],
[m4_if([$1], [], [$3], [$2])])


# m4_n(TEXT)
# ----------
# If TEXT is not empty, return TEXT and a new line, otherwise nothing.
m4_define([m4_n],
[m4_if([$1],
       [], [],
	   [$1
])])


# m4_ifvaln(COND, [IF-TRUE], [IF-FALSE])
# --------------------------------------
# Same as `m4_ifval', but add an extra newline to IF-TRUE or IF-FALSE
# unless that argument is empty.
m4_define([m4_ifvaln],
[m4_if([$1],
       [],   [m4_n([$3])],
	     [m4_n([$2])])])


# m4_ifset(MACRO, [IF-TRUE], [IF-FALSE])
# --------------------------------------
# If MACRO has no definition, or of its definition is the empty string,
# expand IF-FALSE, otherwise IF-TRUE.
m4_define([m4_ifset],
[m4_ifdef([$1],
	  [m4_ifval(_m4_defn([$1]), [$2], [$3])],
	  [$3])])


# m4_ifndef(NAME, [IF-NOT-DEFINED], [IF-DEFINED])
# -----------------------------------------------
m4_define([m4_ifndef],
[m4_ifdef([$1], [$3], [$2])])


# m4_case(SWITCH, VAL1, IF-VAL1, VAL2, IF-VAL2, ..., DEFAULT)
# -----------------------------------------------------------
# m4 equivalent of
# switch (SWITCH)
# {
#   case VAL1:
#     IF-VAL1;
#     break;
#   case VAL2:
#     IF-VAL2;
#     break;
#   ...
#   default:
#     DEFAULT;
#     break;
# }.
# All the values are optional, and the macro is robust to active
# symbols properly quoted.
#
# Please keep foreach.m4 in sync with any adjustments made here.
m4_define([m4_case],
[m4_if([$#], 0, [],
       [$#], 1, [],
       [$#], 2, [$2],
       [$1], [$2], [$3],
       [$0([$1], m4_shift3($@))])])


# m4_bmatch(SWITCH, RE1, VAL1, RE2, VAL2, ..., DEFAULT)
# -----------------------------------------------------
# m4 equivalent of
#
# if (SWITCH =~ RE1)
#   VAL1;
# elif (SWITCH =~ RE2)
#   VAL2;
# elif ...
#   ...
# else
#   DEFAULT
#
# All the values are optional, and the macro is robust to active symbols
# properly quoted.
#
# Please keep foreach.m4 in sync with any adjustments made here.
m4_define([m4_bmatch],
[m4_if([$#], 0, [m4_fatal([$0: too few arguments: $#])],
       [$#], 1, [m4_fatal([$0: too few arguments: $#: $1])],
       [$#], 2, [$2],
       [m4_if(m4_bregexp([$1], [$2]), -1, [$0([$1], m4_shift3($@))],
	      [$3])])])


# m4_car(LIST)
# m4_cdr(LIST)
# ------------
# Manipulate m4 lists.
m4_define([m4_car], [[$1]])
m4_define([m4_cdr],
[m4_if([$#], 0, [m4_fatal([$0: cannot be called without arguments])],
       [$#], 1, [],
       [m4_dquote(m4_shift($@))])])

# _m4_cdr(LIST)
# -------------
# Like m4_cdr, except include a leading comma unless only one element
# remains.  Why?  Because comparing a large list against [] is more
# expensive in expansion time than comparing the number of arguments; so
# _m4_cdr can be used to reduce the number of arguments when it is time
# to end recursion.
m4_define([_m4_cdr],
[m4_if([$#], 1, [],
       [, m4_dquote(m4_shift($@))])])



# m4_cond(TEST1, VAL1, IF-VAL1, TEST2, VAL2, IF-VAL2, ..., [DEFAULT])
# -------------------------------------------------------------------
# Similar to m4_if, except that each TEST is expanded when encountered.
# If the expansion of TESTn matches the string VALn, the result is IF-VALn.
# The result is DEFAULT if no tests passed.  This macro allows
# short-circuiting of expensive tests, where it pays to arrange quick
# filter tests to run first.
#
# For an example, consider a previous implementation of _AS_QUOTE_IFELSE:
#
#    m4_if(m4_index([$1], [\]), [-1], [$2],
#          m4_eval(m4_index([$1], [\\]) >= 0), [1], [$2],
#          m4_eval(m4_index([$1], [\$]) >= 0), [1], [$2],
#          m4_eval(m4_index([$1], [\`]) >= 0), [1], [$3],
#          m4_eval(m4_index([$1], [\"]) >= 0), [1], [$3],
#          [$2])
#
# Here, m4_index is computed 5 times, and m4_eval 4, even if $1 contains
# no backslash.  It is more efficient to do:
#
#    m4_cond([m4_index([$1], [\])], [-1], [$2],
#            [m4_eval(m4_index([$1], [\\]) >= 0)], [1], [$2],
#            [m4_eval(m4_index([$1], [\$]) >= 0)], [1], [$2],
#            [m4_eval(m4_index([$1], [\`]) >= 0)], [1], [$3],
#            [m4_eval(m4_index([$1], [\"]) >= 0)], [1], [$3],
#            [$2])
#
# In the common case of $1 with no backslash, only one m4_index expansion
# occurs, and m4_eval is avoided altogether.
#
# Please keep foreach.m4 in sync with any adjustments made here.
m4_define([m4_cond],
[m4_if([$#], [0], [m4_fatal([$0: cannot be called without arguments])],
       [$#], [1], [$1],
       m4_eval([$# % 3]), [2], [m4_fatal([$0: missing an argument])],
       [_$0($@)])])

m4_define([_m4_cond],
[m4_if(($1), [($2)], [$3],
       [$#], [3], [],
       [$#], [4], [$4],
       [$0(m4_shift3($@))])])


## ---------------------------------------- ##
## 6. Enhanced version of some primitives.  ##
## ---------------------------------------- ##

# m4_bpatsubsts(STRING, RE1, SUBST1, RE2, SUBST2, ...)
# ----------------------------------------------------
# m4 equivalent of
#
#   $_ = STRING;
#   s/RE1/SUBST1/g;
#   s/RE2/SUBST2/g;
#   ...
#
# All the values are optional, and the macro is robust to active symbols
# properly quoted.
#
# I would have liked to name this macro `m4_bpatsubst', unfortunately,
# due to quotation problems, I need to double quote $1 below, therefore
# the anchors are broken :(  I can't let users be trapped by that.
#
# Recall that m4_shift3 always results in an argument.  Hence, we need
# to distinguish between a final deletion vs. ending recursion.
#
# Please keep foreach.m4 in sync with any adjustments made here.
m4_define([m4_bpatsubsts],
[m4_if([$#], 0, [m4_fatal([$0: too few arguments: $#])],
       [$#], 1, [m4_fatal([$0: too few arguments: $#: $1])],
       [$#], 2, [m4_unquote(m4_builtin([patsubst], [[$1]], [$2]))],
       [$#], 3, [m4_unquote(m4_builtin([patsubst], [[$1]], [$2], [$3]))],
       [_$0($@m4_if(m4_eval($# & 1), 0, [,]))])])
m4_define([_m4_bpatsubsts],
[m4_if([$#], 2, [$1],
       [$0(m4_builtin([patsubst], [[$1]], [$2], [$3]),
	   m4_shift3($@))])])


# m4_define_default(MACRO, VALUE)
# -------------------------------
# If MACRO is undefined, set it to VALUE.
m4_define([m4_define_default],
[m4_ifndef([$1], [m4_define($@)])])


# m4_default(EXP1, EXP2)
# ----------------------
# Returns EXP1 if non empty, otherwise EXP2.
#
# This macro is called on hot paths, so inline the contents of m4_ifval,
# for one less round of expansion.
m4_define([m4_default],
[m4_if([$1], [], [$2], [$1])])


# m4_defn(NAME)
# -------------
# Like the original, except guarantee a warning when using something which is
# undefined (unlike M4 1.4.x).  This replacement is not a full-featured
# replacement: if any of the defined macros contain unbalanced quoting, but
# when pasted together result in a well-quoted string, then only native m4
# support is able to get it correct.  But that's where quadrigraphs come in
# handy, if you really need unbalanced quotes inside your macros.
#
# This macro is called frequently, so minimize the amount of additional
# expansions by skipping m4_ifndef.  Better yet, if __m4_version__ exists,
# (added in M4 1.6), then let m4 do the job for us (see m4_init).
#
# _m4_defn is for internal use only - it bypasses the wrapper, so it
# must only be used on one argument at a time, and only on macros
# known to be defined.  Make sure this still works if the user renames
# m4_defn but not _m4_defn.
m4_copy([m4_defn], [_m4_defn])
m4_define([m4_defn],
[m4_if([$#], [0], [[$0]],
       [$#], [1], [m4_ifdef([$1], [_m4_defn([$1])],
			    [m4_fatal([$0: undefined macro: $1])])],
       [m4_foreach([_m4_macro], [$@], [$0(_m4_defn([_m4_macro]))])])])


# _m4_dumpdefs_up(NAME)
# ---------------------
m4_define([_m4_dumpdefs_up],
[m4_ifdef([$1],
	  [m4_pushdef([_m4_dumpdefs], _m4_defn([$1]))dnl
m4_dumpdef([$1])dnl
_m4_popdef([$1])dnl
_m4_dumpdefs_up([$1])])])


# _m4_dumpdefs_down(NAME)
# -----------------------
m4_define([_m4_dumpdefs_down],
[m4_ifdef([_m4_dumpdefs],
	  [m4_pushdef([$1], _m4_defn([_m4_dumpdefs]))dnl
_m4_popdef([_m4_dumpdefs])dnl
_m4_dumpdefs_down([$1])])])


# m4_dumpdefs(NAME)
# -----------------
# Similar to `m4_dumpdef(NAME)', but if NAME was m4_pushdef'ed, display its
# value stack (most recent displayed first).
m4_define([m4_dumpdefs],
[_m4_dumpdefs_up([$1])dnl
_m4_dumpdefs_down([$1])])


# m4_popdef(NAME)
# ---------------
# Like the original, except guarantee a warning when using something which is
# undefined (unlike M4 1.4.x).
#
# This macro is called frequently, so minimize the amount of additional
# expansions by skipping m4_ifndef.  Better yet, if __m4_version__ exists,
# (added in M4 1.6), then let m4 do the job for us (see m4_init).
#
# _m4_popdef is for internal use only - it bypasses the wrapper, so it
# must only be used on macros known to be defined.  Make sure this
# still works if the user renames m4_popdef but not _m4_popdef.
m4_copy([m4_popdef], [_m4_popdef])
m4_define([m4_popdef],
[m4_if([$#], [0], [[$0]],
       [$#], [1], [m4_ifdef([$1], [_m4_popdef([$1])],
			    [m4_fatal([$0: undefined macro: $1])])],
       [m4_foreach([_m4_macro], [$@], [$0(_m4_defn([_m4_macro]))])])])


# m4_shiftn(N, ...)
# -----------------
# Returns ... shifted N times.  Useful for recursive "varargs" constructs.
#
# Autoconf does not use this macro, because it is inherently slower than
# calling the common cases of m4_shift2 or m4_shift3 directly.  But it
# might as well be fast for other clients, such as Libtool.  One way to
# do this is to expand $@ only once in _m4_shiftn (otherwise, for long
# lists, the expansion of m4_if takes twice as much memory as what the
# list itself occupies, only to throw away the unused branch).  The end
# result is strictly equivalent to
#   m4_if([$1], 1, [m4_shift(,m4_shift(m4_shift($@)))],
#         [_m4_shiftn(m4_decr([$1]), m4_shift(m4_shift($@)))])
# but with the final `m4_shift(m4_shift($@)))' shared between the two
# paths.  The first leg uses a no-op m4_shift(,$@) to balance out the ().
#
# Please keep foreach.m4 in sync with any adjustments made here.
m4_define([m4_shiftn],
[m4_assert(0 < $1 && $1 < $#)_$0($@)])

m4_define([_m4_shiftn],
[m4_if([$1], 1, [m4_shift(],
       [$0(m4_decr([$1])]), m4_shift(m4_shift($@)))])

# m4_shift2(...)
# m4_shift3(...)
# -----------------
# Returns ... shifted twice, and three times.  Faster than m4_shiftn.
m4_define([m4_shift2], [m4_shift(m4_shift($@))])
m4_define([m4_shift3], [m4_shift(m4_shift(m4_shift($@)))])

# _m4_shift2(...)
# _m4_shift3(...)
# ---------------
# Like m4_shift2 or m4_shift3, except include a leading comma unless shifting
# consumes all arguments.  Why?  Because in recursion, it is nice to
# distinguish between 1 element left and 0 elements left, based on how many
# arguments this shift expands to.
m4_define([_m4_shift2],
[m4_if([$#], [2], [],
       [, m4_shift(m4_shift($@))])])
m4_define([_m4_shift3],
[m4_if([$#], [3], [],
       [, m4_shift(m4_shift(m4_shift($@)))])])


# m4_undefine(NAME)
# -----------------
# Like the original, except guarantee a warning when using something which is
# undefined (unlike M4 1.4.x).
#
# This macro is called frequently, so minimize the amount of additional
# expansions by skipping m4_ifndef.  Better yet, if __m4_version__ exists,
# (added in M4 1.6), then let m4 do the job for us (see m4_init).
#
# _m4_undefine is for internal use only - it bypasses the wrapper, so
# it must only be used on macros known to be defined.  Make sure this
# still works if the user renames m4_undefine but not _m4_undefine.
m4_copy([m4_undefine], [_m4_undefine])
m4_define([m4_undefine],
[m4_if([$#], [0], [[$0]],
       [$#], [1], [m4_ifdef([$1], [_m4_undefine([$1])],
			    [m4_fatal([$0: undefined macro: $1])])],
       [m4_foreach([_m4_macro], [$@], [$0(_m4_defn([_m4_macro]))])])])

# _m4_wrap(PRE, POST)
# -------------------
# Helper macro for m4_wrap and m4_wrap_lifo.  Allows nested calls to
# m4_wrap within wrapped text.  Use _m4_defn and _m4_popdef for speed.
m4_define([_m4_wrap],
[m4_ifdef([$0_text],
	  [m4_define([$0_text], [$1]_m4_defn([$0_text])[$2])],
	  [m4_builtin([m4wrap], [m4_unquote(
  _m4_defn([$0_text])_m4_popdef([$0_text]))])m4_define([$0_text], [$1$2])])])

# m4_wrap(TEXT)
# -------------
# Append TEXT to the list of hooks to be executed at the end of input.
# Whereas the order of the original may be LIFO in the underlying m4,
# this version is always FIFO.
m4_define([m4_wrap],
[_m4_wrap([], [$1[]])])

# m4_wrap_lifo(TEXT)
# ------------------
# Prepend TEXT to the list of hooks to be executed at the end of input.
# Whereas the order of m4_wrap may be FIFO in the underlying m4, this
# version is always LIFO.
m4_define([m4_wrap_lifo],
[_m4_wrap([$1[]])])

## ------------------------- ##
## 7. Quoting manipulation.  ##
## ------------------------- ##


# m4_apply(MACRO, LIST)
# ---------------------
# Invoke MACRO, with arguments provided from the quoted list of
# comma-separated quoted arguments.  If LIST is empty, invoke MACRO
# without arguments.  The expansion will not be concatenated with
# subsequent text.
m4_define([m4_apply],
[m4_if([$2], [], [$1], [$1($2)])[]])

# _m4_apply(MACRO, LIST)
# ----------------------
# Like m4_apply, except do nothing if LIST is empty.
m4_define([_m4_apply],
[m4_if([$2], [], [], [$1($2)[]])])


# m4_count(ARGS)
# --------------
# Return a count of how many ARGS are present.
m4_define([m4_count], [$#])


# m4_do(STRING, ...)
# ------------------
# This macro invokes all its arguments (in sequence, of course).  It is
# useful for making your macros more structured and readable by dropping
# unnecessary dnl's and have the macros indented properly.  No concatenation
# occurs after a STRING; use m4_unquote(m4_join(,STRING)) for that.
#
# Please keep foreach.m4 in sync with any adjustments made here.
m4_define([m4_do],
[m4_if([$#], 0, [],
       [$#], 1, [$1[]],
       [$1[]$0(m4_shift($@))])])


# m4_dquote(ARGS)
# ---------------
# Return ARGS as a quoted list of quoted arguments.
m4_define([m4_dquote],  [[$@]])


# m4_dquote_elt(ARGS)
# -------------------
# Return ARGS as an unquoted list of double-quoted arguments.
#
# Please keep foreach.m4 in sync with any adjustments made here.
m4_define([m4_dquote_elt],
[m4_if([$#], [0], [],
       [$#], [1], [[[$1]]],
       [[[$1]],$0(m4_shift($@))])])


# m4_echo(ARGS)
# -------------
# Return the ARGS, with the same level of quoting.  Whitespace after
# unquoted commas are consumed.
m4_define([m4_echo], [$@])


# m4_expand(ARG)
# --------------
# Return the expansion of ARG as a single string.  Unlike m4_quote($1), this
# correctly preserves whitespace following single-quoted commas that appeared
# within ARG.
#
#   m4_define([active], [ACT, IVE])
#   m4_define([active2], [[ACT, IVE]])
#   m4_quote(active, active2)
#   => ACT,IVE,ACT, IVE
#   m4_expand([active, active2])
#   => ACT, IVE, ACT, IVE
#
# Unfortunately, due to limitations in m4, ARG must expand to something
# with balanced quotes (use quadrigraphs to get around this).  The input
# is not likely to have unbalanced -=<{(/)}>=- quotes, and it is possible
# to have unbalanced (), provided it was specified with proper [] quotes.
#
# Exploit that extra () will group unquoted commas and the following
# whitespace, then convert () to [].  m4_bpatsubst can't handle newlines
# inside $1, and m4_substr strips quoting.  So we (ab)use m4_changequote.
m4_define([m4_expand], [_$0(-=<{($1)}>=-)])
m4_define([_m4_expand],
[m4_changequote([-=<{(], [)}>=-])$1m4_changequote([, ])])


# m4_ignore(ARGS)
# ---------------
# Expands to nothing.  Useful for conditionally ignoring an arbitrary
# number of arguments (see _m4_list_cmp for an example).
m4_define([m4_ignore])


# m4_make_list(ARGS)
# ------------------
# Similar to m4_dquote, this creates a quoted list of quoted ARGS.  This
# version is less efficient than m4_dquote, but separates each argument
# with a comma and newline, rather than just comma, for readability.
# When developing an m4sugar algorithm, you could temporarily use
#   m4_pushdef([m4_dquote],m4_defn([m4_make_list]))
# around your code to make debugging easier.
m4_define([m4_make_list], [m4_join([,
], m4_dquote_elt($@))])


# m4_noquote(STRING)
# ------------------
# Return the result of ignoring all quotes in STRING and invoking the
# macros it contains.  Amongst other things, this is useful for enabling
# macro invocations inside strings with [] blocks (for instance regexps
# and help-strings).  On the other hand, since all quotes are disabled,
# any macro expanded during this time that relies on nested [] quoting
# will likely crash and burn.  This macro is seldom useful; consider
# m4_unquote or m4_expand instead.
m4_define([m4_noquote],
[m4_changequote([-=<{(],[)}>=-])$1-=<{()}>=-m4_changequote([,])])


# m4_quote(ARGS)
# --------------
# Return ARGS as a single argument.  Any whitespace after unquoted commas
# is stripped.  There is always output, even when there were no arguments.
#
# It is important to realize the difference between `m4_quote(exp)' and
# `[exp]': in the first case you obtain the quoted *result* of the
# expansion of EXP, while in the latter you just obtain the string
# `exp'.
m4_define([m4_quote],  [[$*]])


# _m4_quote(ARGS)
# ---------------
# Like m4_quote, except that when there are no arguments, there is no
# output.  For conditional scenarios (such as passing _m4_quote as the
# macro name in m4_mapall), this feature can be used to distinguish between
# one argument of the empty string vs. no arguments.  However, in the
# normal case with arguments present, this is less efficient than m4_quote.
m4_define([_m4_quote],
[m4_if([$#], [0], [], [[$*]])])


# m4_reverse(ARGS)
# ----------------
# Output ARGS in reverse order.
#
# Please keep foreach.m4 in sync with any adjustments made here.
m4_define([m4_reverse],
[m4_if([$#], [0], [], [$#], [1], [[$1]],
       [$0(m4_shift($@)), [$1]])])


# m4_unquote(ARGS)
# ----------------
# Remove one layer of quotes from each ARG, performing one level of
# expansion.  For one argument, m4_unquote([arg]) is more efficient than
# m4_do([arg]), but for multiple arguments, the difference is that
# m4_unquote separates arguments with commas while m4_do concatenates.
# Follow this macro with [] if concatenation with subsequent text is
# undesired.
m4_define([m4_unquote], [$*])


## -------------------------- ##
## 8. Implementing m4 loops.  ##
## -------------------------- ##


# m4_for(VARIABLE, FIRST, LAST, [STEP = +/-1], EXPRESSION)
# --------------------------------------------------------
# Expand EXPRESSION defining VARIABLE to FROM, FROM + 1, ..., TO with
# increments of STEP.  Both limits are included, and bounds are
# checked for consistency.  The algorithm is robust to indirect
# VARIABLE names.  Changing VARIABLE inside EXPRESSION will not impact
# the number of iterations.
#
# Uses _m4_defn for speed, and avoid dnl in the macro body.
m4_define([m4_for],
[m4_pushdef([$1], m4_eval([$2]))]dnl
[m4_cond([m4_eval(([$3]) > ([$2]))], 1,
	   [m4_pushdef([_m4_step], m4_eval(m4_default([$4],
	      1)))m4_assert(_m4_step > 0)_$0([$1], _m4_defn([$1]),
  m4_eval((([$3]) - ([$2])) / _m4_step * _m4_step + ([$2])),
  _m4_step, [$5])],
	 [m4_eval(([$3]) < ([$2]))], 1,
	   [m4_pushdef([_m4_step], m4_eval(m4_default([$4],
	      -1)))m4_assert(_m4_step < 0)_$0([$1], _m4_defn([$1]),
  m4_eval((([$2]) - ([$3])) / -(_m4_step) * _m4_step + ([$2])),
  _m4_step, [$5])],
	 [m4_pushdef([_m4_step])$5])[]]dnl
[m4_popdef([_m4_step], [$1])])


# _m4_for(VARIABLE, COUNT, LAST, STEP, EXPRESSION)
# ------------------------------------------------
# Core of the loop, no consistency checks, all arguments are plain
# numbers.  Define VARIABLE to COUNT, expand EXPRESSION, then alter
# COUNT by STEP and iterate if COUNT is not LAST.
m4_define([_m4_for],
[m4_define([$1], [$2])$5[]m4_if([$2], [$3], [],
      [$0([$1], m4_eval([$2 + $4]), [$3], [$4], [$5])])])


# Implementing `foreach' loops in m4 is much more tricky than it may
# seem.  For example, the old M4 1.4.4 manual had an incorrect example,
# which looked like this (when translated to m4sugar):
#
# | # foreach(VAR, (LIST), STMT)
# | m4_define([foreach],
# |   [m4_pushdef([$1])_foreach([$1], [$2], [$3])m4_popdef([$1])])
# | m4_define([_arg1], [$1])
# | m4_define([_foreach],
# |   [m4_if([$2], [()], ,
# |     [m4_define([$1], _arg1$2)$3[]_foreach([$1], (m4_shift$2), [$3])])])
#
# But then if you run
#
# | m4_define(a, 1)
# | m4_define(b, 2)
# | m4_define(c, 3)
# | foreach([f], [([a], [(b], [c)])], [echo f
# | ])
#
# it gives
#
#  => echo 1
#  => echo (2,3)
#
# which is not what is expected.
#
# Of course the problem is that many quotes are missing.  So you add
# plenty of quotes at random places, until you reach the expected
# result.  Alternatively, if you are a quoting wizard, you directly
# reach the following implementation (but if you really did, then
# apply to the maintenance of m4sugar!).
#
# | # foreach(VAR, (LIST), STMT)
# | m4_define([foreach], [m4_pushdef([$1])_foreach($@)m4_popdef([$1])])
# | m4_define([_arg1], [[$1]])
# | m4_define([_foreach],
# |  [m4_if($2, [()], ,
# |    [m4_define([$1], [_arg1$2])$3[]_foreach([$1], [(m4_shift$2)], [$3])])])
#
# which this time answers
#
#  => echo a
#  => echo (b
#  => echo c)
#
# Bingo!
#
# Well, not quite.
#
# With a better look, you realize that the parens are more a pain than
# a help: since anyway you need to quote properly the list, you end up
# with always using an outermost pair of parens and an outermost pair
# of quotes.  Rejecting the parens both eases the implementation, and
# simplifies the use:
#
# | # foreach(VAR, (LIST), STMT)
# | m4_define([foreach], [m4_pushdef([$1])_foreach($@)m4_popdef([$1])])
# | m4_define([_arg1], [$1])
# | m4_define([_foreach],
# |  [m4_if($2, [], ,
# |    [m4_define([$1], [_arg1($2)])$3[]_foreach([$1], [m4_shift($2)], [$3])])])
#
#
# Now, just replace the `$2' with `m4_quote($2)' in the outer `m4_if'
# to improve robustness, and you come up with a nice implementation
# that doesn't require extra parentheses in the user's LIST.
#
# But wait -  now the algorithm is quadratic, because every recursion of
# the algorithm keeps the entire LIST and merely adds another m4_shift to
# the quoted text.  If the user has a lot of elements in LIST, you can
# bring the system to its knees with the memory m4 then requires, or trip
# the m4 --nesting-limit recursion factor.  The only way to avoid
# quadratic growth is ensure m4_shift is expanded prior to the recursion.
# Hence the design below.
#
# The M4 manual now includes a chapter devoted to this issue, with
# the lessons learned from m4sugar.  And still, this design is only
# optimal for M4 1.6; see foreach.m4 for yet more comments on why
# M4 1.4.x uses yet another implementation.


# m4_foreach(VARIABLE, LIST, EXPRESSION)
# --------------------------------------
#
# Expand EXPRESSION assigning each value of the LIST to VARIABLE.
# LIST should have the form `item_1, item_2, ..., item_n', i.e. the
# whole list must *quoted*.  Quote members too if you don't want them
# to be expanded.
#
# This macro is robust to active symbols:
#      | m4_define(active, [ACT, IVE])
#      | m4_foreach(Var, [active, active], [-Var-])
#     => -ACT--IVE--ACT--IVE-
#
#      | m4_foreach(Var, [[active], [active]], [-Var-])
#     => -ACT, IVE--ACT, IVE-
#
#      | m4_foreach(Var, [[[active]], [[active]]], [-Var-])
#     => -active--active-
#
# This macro is called frequently, so avoid extra expansions such as
# m4_ifval and dnl.  Also, since $2 might be quite large, try to use it
# as little as possible in _m4_foreach; each extra use requires that much
# more memory for expansion.  So, rather than directly compare $2 against
# [] and use m4_car/m4_cdr for recursion, we instead unbox the list (which
# requires swapping the argument order in the helper), insert an ignored
# third argument, and use m4_shift3 to detect when recursion is complete.
#
# Please keep foreach.m4 in sync with any adjustments made here.
m4_define([m4_foreach],
[m4_if([$2], [], [],
       [m4_pushdef([$1])_$0([$1], [$3], [], $2)m4_popdef([$1])])])

m4_define([_m4_foreach],
[m4_if([$#], [3], [],
       [m4_define([$1], [$4])$2[]$0([$1], [$2], m4_shift3($@))])])


# m4_foreach_w(VARIABLE, LIST, EXPRESSION)
# ----------------------------------------
#
# Like m4_foreach, but the list is whitespace separated.
#
# This macro is robust to active symbols:
#    m4_foreach_w([Var], [ active
#    b	act\
#    ive  ], [-Var-])end
#    => -active--b--active-end
#
m4_define([m4_foreach_w],
[m4_foreach([$1], m4_split(m4_normalize([$2]), [ ]), [$3])])


# m4_map(MACRO, LIST)
# m4_mapall(MACRO, LIST)
# ----------------------
# Invoke MACRO($1), MACRO($2) etc. where $1, $2... are the elements of
# LIST.  $1, $2... must in turn be lists, appropriate for m4_apply.
# If LIST contains an empty sublist, m4_map skips the expansion of
# MACRO, while m4_mapall expands MACRO with no arguments.
#
# Since LIST may be quite large, we want to minimize how often it
# appears in the expansion.  Rather than use m4_car/m4_cdr iteration,
# we unbox the list, ignore the second argument, and use m4_shift2 to
# detect the end of recursion.  The mismatch in () is intentional; see
# _m4_map.  For m4_map, an empty list behaves like an empty sublist
# and gets ignored; for m4_mapall, we must special-case the empty
# list.
#
# Please keep foreach.m4 in sync with any adjustments made here.
m4_define([m4_map],
[_m4_map([_m4_apply([$1]], [], $2)])

m4_define([m4_mapall],
[m4_if([$2], [], [],
       [_m4_map([m4_apply([$1]], [], $2)])])


# m4_map_sep(MACRO, SEPARATOR, LIST)
# m4_mapall_sep(MACRO, SEPARATOR, LIST)
# -------------------------------------
# Invoke MACRO($1), SEPARATOR, MACRO($2), ..., MACRO($N) where $1,
# $2... $N are the elements of LIST, and are in turn lists appropriate
# for m4_apply.  SEPARATOR is expanded, in order to allow the creation
# of a list of arguments by using a single-quoted comma as the
# separator.  For each empty sublist, m4_map_sep skips the expansion
# of MACRO and SEPARATOR, while m4_mapall_sep expands MACRO with no
# arguments.
#
# For m4_mapall_sep, merely expand the first iteration without the
# separator, then include separator as part of subsequent recursion;
# but avoid extra expansion of LIST's side-effects via a helper macro.
# For m4_map_sep, things are trickier - we don't know if the first
# list element is an empty sublist, so we must define a self-modifying
# helper macro and use that as the separator instead.
m4_define([m4_map_sep],
[m4_pushdef([m4_Sep], [m4_define([m4_Sep], _m4_defn([m4_unquote]))])]dnl
[_m4_map([_m4_apply([m4_Sep([$2])[]$1]], [], $3)m4_popdef([m4_Sep])])

m4_define([m4_mapall_sep],
[m4_if([$3], [], [], [_$0([$1], [$2], $3)])])

m4_define([_m4_mapall_sep],
[m4_apply([$1], [$3])_m4_map([m4_apply([$2[]$1]], m4_shift2($@))])

# _m4_map(PREFIX, IGNORED, SUBLIST, ...)
# --------------------------------------
# Common implementation for all four m4_map variants.  The mismatch in
# the number of () is intentional.  PREFIX must supply a form of
# m4_apply, the open `(', and the MACRO to be applied.  Each iteration
# then appends `,', the current SUBLIST and the closing `)', then
# recurses to the next SUBLIST.  IGNORED is an aid to ending recursion
# efficiently.
#
# Please keep foreach.m4 in sync with any adjustments made here.
m4_define([_m4_map],
[m4_if([$#], [2], [],
       [$1, [$3])$0([$1], m4_shift2($@))])])

# m4_transform(EXPRESSION, ARG...)
# --------------------------------
# Expand EXPRESSION([ARG]) for each argument.  More efficient than
# m4_foreach([var], [ARG...], [EXPRESSION(m4_defn([var]))])
#
# Please keep foreach.m4 in sync with any adjustments made here.
m4_define([m4_transform],
[m4_if([$#], [0], [m4_fatal([$0: too few arguments: $#])],
       [$#], [1], [],
       [$#], [2], [$1([$2])[]],
       [$1([$2])[]$0([$1], m4_shift2($@))])])


# m4_transform_pair(EXPRESSION, [END-EXPR = EXPRESSION], ARG...)
# --------------------------------------------------------------
# Perform a pairwise grouping of consecutive ARGs, by expanding
# EXPRESSION([ARG1], [ARG2]).  If there are an odd number of ARGs, the
# final argument is expanded with END-EXPR([ARGn]).
#
# For example:
#   m4_define([show], [($*)m4_newline])dnl
#   m4_transform_pair([show], [], [a], [b], [c], [d], [e])dnl
#   => (a,b)
#   => (c,d)
#   => (e)
#
# Please keep foreach.m4 in sync with any adjustments made here.
m4_define([m4_transform_pair],
[m4_if([$#], [0], [m4_fatal([$0: too few arguments: $#])],
       [$#], [1], [m4_fatal([$0: too few arguments: $#: $1])],
       [$#], [2], [],
       [$#], [3], [m4_default([$2], [$1])([$3])[]],
       [$#], [4], [$1([$3], [$4])[]],
       [$1([$3], [$4])[]$0([$1], [$2], m4_shift(m4_shift3($@)))])])


## --------------------------- ##
## 9. More diversion support.  ##
## --------------------------- ##


# _m4_divert(DIVERSION-NAME or NUMBER)
# ------------------------------------
# If DIVERSION-NAME is the name of a diversion, return its number,
# otherwise if it is a NUMBER return it.
m4_define([_m4_divert],
[m4_ifdef([_m4_divert($1)],
	  [m4_indir([_m4_divert($1)])],
	  [$1])])

# KILL is only used to suppress output.
m4_define([_m4_divert(KILL)],           -1)

# The empty diversion name is a synonym for 0.
m4_define([_m4_divert()],                0)


# _m4_divert_n_stack
# ------------------
# Print m4_divert_stack with newline prepended, if it's nonempty.
m4_define([_m4_divert_n_stack],
[m4_ifdef([m4_divert_stack], [
_m4_defn([m4_divert_stack])])])


# m4_divert(DIVERSION-NAME)
# -------------------------
# Change the diversion stream to DIVERSION-NAME.
m4_define([m4_divert],
[m4_define([m4_divert_stack], m4_location[: $0: $1]_m4_divert_n_stack)]dnl
[m4_builtin([divert], _m4_divert([$1]))])


# m4_divert_push(DIVERSION-NAME)
# ------------------------------
# Change the diversion stream to DIVERSION-NAME, while stacking old values.
m4_define([m4_divert_push],
[m4_pushdef([m4_divert_stack], m4_location[: $0: $1]_m4_divert_n_stack)]dnl
[m4_pushdef([_m4_divert_diversion], [$1])]dnl
[m4_builtin([divert], _m4_divert([$1]))])


# m4_divert_pop([DIVERSION-NAME])
# -------------------------------
# Change the diversion stream to its previous value, unstacking it.
# If specified, verify we left DIVERSION-NAME.
# When we pop the last value from the stack, we divert to -1.
m4_define([m4_divert_pop],
[m4_ifndef([_m4_divert_diversion],
	   [m4_fatal([too many m4_divert_pop])])]dnl
[m4_if([$1], [], [],
       [$1], _m4_defn([_m4_divert_diversion]), [],
       [m4_fatal([$0($1): diversion mismatch: ]_m4_divert_n_stack)])]dnl
[_m4_popdef([m4_divert_stack], [_m4_divert_diversion])]dnl
[m4_builtin([divert],
	    m4_ifdef([_m4_divert_diversion],
		     [_m4_divert(_m4_defn([_m4_divert_diversion]))],
		     -1))])


# m4_divert_text(DIVERSION-NAME, CONTENT)
# ---------------------------------------
# Output CONTENT into DIVERSION-NAME (which may be a number actually).
# An end of line is appended for free to CONTENT.
m4_define([m4_divert_text],
[m4_divert_push([$1])$2
m4_divert_pop([$1])])


# m4_divert_once(DIVERSION-NAME, CONTENT)
# ---------------------------------------
# Output CONTENT into DIVERSION-NAME once, if not already there.
# An end of line is appended for free to CONTENT.
m4_define([m4_divert_once],
[m4_expand_once([m4_divert_text([$1], [$2])])])


# m4_undivert(DIVERSION-NAME)
# ---------------------------
# Undivert DIVERSION-NAME.  Unlike the M4 version, this only takes a single
# diversion identifier, and should not be used to undivert files.
m4_define([m4_undivert],
[m4_builtin([undivert], _m4_divert([$1]))])


## --------------------------------------------- ##
## 10. Defining macros with bells and whistles.  ##
## --------------------------------------------- ##

# `m4_defun' is basically `m4_define' but it equips the macro with the
# needed machinery for `m4_require'.  A macro must be m4_defun'd if
# either it is m4_require'd, or it m4_require's.
#
# Two things deserve attention and are detailed below:
#  1. Implementation of m4_require
#  2. Keeping track of the expansion stack
#
# 1. Implementation of m4_require
# ===============================
#
# Of course m4_defun AC_PROVIDE's the macro, so that a macro which has
# been expanded is not expanded again when m4_require'd, but the
# difficult part is the proper expansion of macros when they are
# m4_require'd.
#
# The implementation is based on two ideas, (i) using diversions to
# prepare the expansion of the macro and its dependencies (by Franc,ois
# Pinard), and (ii) expand the most recently m4_require'd macros _after_
# the previous macros (by Axel Thimm).
#
#
# The first idea: why use diversions?
# -----------------------------------
#
# When a macro requires another, the other macro is expanded in new
# diversion, GROW.  When the outer macro is fully expanded, we first
# undivert the most nested diversions (GROW - 1...), and finally
# undivert GROW.  To understand why we need several diversions,
# consider the following example:
#
# | m4_defun([TEST1], [Test...REQUIRE([TEST2])1])
# | m4_defun([TEST2], [Test...REQUIRE([TEST3])2])
# | m4_defun([TEST3], [Test...3])
#
# Because m4_require is not required to be first in the outer macros, we
# must keep the expansions of the various levels of m4_require separated.
# Right before executing the epilogue of TEST1, we have:
#
#	   GROW - 2: Test...3
#	   GROW - 1: Test...2
#	   GROW:     Test...1
#	   BODY:
#
# Finally the epilogue of TEST1 undiverts GROW - 2, GROW - 1, and
# GROW into the regular flow, BODY.
#
#	   GROW - 2:
#	   GROW - 1:
#	   GROW:
#	   BODY:        Test...3; Test...2; Test...1
#
# (The semicolons are here for clarification, but of course are not
# emitted.)  This is what Autoconf 2.0 (I think) to 2.13 (I'm sure)
# implement.
#
#
# The second idea: first required first out
# -----------------------------------------
#
# The natural implementation of the idea above is buggy and produces
# very surprising results in some situations.  Let's consider the
# following example to explain the bug:
#
# | m4_defun([TEST1],  [REQUIRE([TEST2a])REQUIRE([TEST2b])])
# | m4_defun([TEST2a], [])
# | m4_defun([TEST2b], [REQUIRE([TEST3])])
# | m4_defun([TEST3],  [REQUIRE([TEST2a])])
# |
# | AC_INIT
# | TEST1
#
# The dependencies between the macros are:
#
#		 3 --- 2b
#		/        \              is m4_require'd by
#	       /          \       left -------------------- right
#	    2a ------------ 1
#
# If you strictly apply the rules given in the previous section you get:
#
#	   GROW - 2: TEST3
#	   GROW - 1: TEST2a; TEST2b
#	   GROW:     TEST1
#	   BODY:
#
# (TEST2a, although required by TEST3 is not expanded in GROW - 3
# because is has already been expanded before in GROW - 1, so it has
# been AC_PROVIDE'd, so it is not expanded again) so when you undivert
# the stack of diversions, you get:
#
#	   GROW - 2:
#	   GROW - 1:
#	   GROW:
#	   BODY:        TEST3; TEST2a; TEST2b; TEST1
#
# i.e., TEST2a is expanded after TEST3 although the latter required the
# former.
#
# Starting from 2.50, we use an implementation provided by Axel Thimm.
# The idea is simple: the order in which macros are emitted must be the
# same as the one in which macros are expanded.  (The bug above can
# indeed be described as: a macro has been AC_PROVIDE'd before its
# dependent, but it is emitted after: the lack of correlation between
# emission and expansion order is guilty).
#
# How to do that?  You keep the stack of diversions to elaborate the
# macros, but each time a macro is fully expanded, emit it immediately.
#
# In the example above, when TEST2a is expanded, but it's epilogue is
# not run yet, you have:
#
#	   GROW - 2:
#	   GROW - 1: TEST2a
#	   GROW:     Elaboration of TEST1
#	   BODY:
#
# The epilogue of TEST2a emits it immediately:
#
#	   GROW - 2:
#	   GROW - 1:
#	   GROW:     Elaboration of TEST1
#	   BODY:     TEST2a
#
# TEST2b then requires TEST3, so right before the epilogue of TEST3, you
# have:
#
#	   GROW - 2: TEST3
#	   GROW - 1: Elaboration of TEST2b
#	   GROW:     Elaboration of TEST1
#	   BODY:      TEST2a
#
# The epilogue of TEST3 emits it:
#
#	   GROW - 2:
#	   GROW - 1: Elaboration of TEST2b
#	   GROW:     Elaboration of TEST1
#	   BODY:     TEST2a; TEST3
#
# TEST2b is now completely expanded, and emitted:
#
#	   GROW - 2:
#	   GROW - 1:
#	   GROW:     Elaboration of TEST1
#	   BODY:     TEST2a; TEST3; TEST2b
#
# and finally, TEST1 is finished and emitted:
#
#	   GROW - 2:
#	   GROW - 1:
#	   GROW:
#	   BODY:     TEST2a; TEST3; TEST2b: TEST1
#
# The idea is simple, but the implementation is a bit evolved.  If you
# are like me, you will want to see the actual functioning of this
# implementation to be convinced.  The next section gives the full
# details.
#
#
# The Axel Thimm implementation at work
# -------------------------------------
#
# We consider the macros above, and this configure.ac:
#
#	    AC_INIT
#	    TEST1
#
# You should keep the definitions of _m4_defun_pro, _m4_defun_epi, and
# m4_require at hand to follow the steps.
#
# This implements tries not to assume that the current diversion is
# BODY, so as soon as a macro (m4_defun'd) is expanded, we first
# record the current diversion under the name _m4_divert_dump (denoted
# DUMP below for short).  This introduces an important difference with
# the previous versions of Autoconf: you cannot use m4_require if you
# are not inside an m4_defun'd macro, and especially, you cannot
# m4_require directly from the top level.
#
# We have not tried to simulate the old behavior (better yet, we
# diagnose it), because it is too dangerous: a macro m4_require'd from
# the top level is expanded before the body of `configure', i.e., before
# any other test was run.  I let you imagine the result of requiring
# AC_STDC_HEADERS for instance, before AC_PROG_CC was actually run....
#
# After AC_INIT was run, the current diversion is BODY.
# * AC_INIT was run
#   DUMP:                undefined
#   diversion stack:     BODY |-
#
# * TEST1 is expanded
# The prologue of TEST1 sets _m4_divert_dump, which is the diversion
# where the current elaboration will be dumped, to the current
# diversion.  It also m4_divert_push to GROW, where the full
# expansion of TEST1 and its dependencies will be elaborated.
#   DUMP:        BODY
#   BODY:        empty
#   diversions:  GROW, BODY |-
#
# * TEST1 requires TEST2a
# _m4_require_call m4_divert_pushes another temporary diversion,
# GROW - 1, and expands TEST2a in there.
#   DUMP:        BODY
#   BODY:        empty
#   GROW - 1:    TEST2a
#   diversions:  GROW - 1, GROW, BODY |-
# Than the content of the temporary diversion is moved to DUMP and the
# temporary diversion is popped.
#   DUMP:        BODY
#   BODY:        TEST2a
#   diversions:  GROW, BODY |-
#
# * TEST1 requires TEST2b
# Again, _m4_require_call pushes GROW - 1 and heads to expand TEST2b.
#   DUMP:        BODY
#   BODY:        TEST2a
#   diversions:  GROW - 1, GROW, BODY |-
#
# * TEST2b requires TEST3
# _m4_require_call pushes GROW - 2 and expands TEST3 here.
# (TEST3 requires TEST2a, but TEST2a has already been m4_provide'd, so
# nothing happens.)
#   DUMP:        BODY
#   BODY:        TEST2a
#   GROW - 2:    TEST3
#   diversions:  GROW - 2, GROW - 1, GROW, BODY |-
# Than the diversion is appended to DUMP, and popped.
#   DUMP:        BODY
#   BODY:        TEST2a; TEST3
#   diversions:  GROW - 1, GROW, BODY |-
#
# * TEST1 requires TEST2b (contd.)
# The content of TEST2b is expanded...
#   DUMP:        BODY
#   BODY:        TEST2a; TEST3
#   GROW - 1:    TEST2b,
#   diversions:  GROW - 1, GROW, BODY |-
# ... and moved to DUMP.
#   DUMP:        BODY
#   BODY:        TEST2a; TEST3; TEST2b
#   diversions:  GROW, BODY |-
#
# * TEST1 is expanded: epilogue
# TEST1's own content is in GROW...
#   DUMP:        BODY
#   BODY:        TEST2a; TEST3; TEST2b
#   GROW:        TEST1
#   diversions:  BODY |-
# ... and it's epilogue moves it to DUMP and then undefines DUMP.
#   DUMP:       undefined
#   BODY:       TEST2a; TEST3; TEST2b; TEST1
#   diversions: BODY |-
#
#
# 2. Keeping track of the expansion stack
# =======================================
#
# When M4 expansion goes wrong it is often extremely hard to find the
# path amongst macros that drove to the failure.  What is needed is
# the stack of macro `calls'. One could imagine that GNU M4 would
# maintain a stack of macro expansions, unfortunately it doesn't, so
# we do it by hand.  This is of course extremely costly, but the help
# this stack provides is worth it.  Nevertheless to limit the
# performance penalty this is implemented only for m4_defun'd macros,
# not for define'd macros.
#
# The scheme is simplistic: each time we enter an m4_defun'd macros,
# we prepend its name in m4_expansion_stack, and when we exit the
# macro, we remove it (thanks to pushdef/popdef).
#
# In addition, we want to detect circular m4_require dependencies.
# Each time we expand a macro FOO we define _m4_expanding(FOO); and
# m4_require(BAR) simply checks whether _m4_expanding(BAR) is defined.


# m4_expansion_stack_push(TEXT)
# -----------------------------
m4_define([m4_expansion_stack_push],
[m4_pushdef([m4_expansion_stack],
	    [$1]m4_ifdef([m4_expansion_stack], [
_m4_defn([m4_expansion_stack])]))])


# m4_expansion_stack_pop
# ----------------------
m4_define([m4_expansion_stack_pop],
[m4_popdef([m4_expansion_stack])])


# m4_expansion_stack_dump
# -----------------------
# Dump the expansion stack.
m4_define([m4_expansion_stack_dump],
[m4_ifdef([m4_expansion_stack],
	  [m4_errprintn(_m4_defn([m4_expansion_stack]))])dnl
m4_errprintn(m4_location[: the top level])])


# _m4_divert(GROW)
# ----------------
# This diversion is used by the m4_defun/m4_require machinery.  It is
# important to keep room before GROW because for each nested
# AC_REQUIRE we use an additional diversion (i.e., two m4_require's
# will use GROW - 2.  More than 3 levels has never seemed to be
# needed.)
#
# ...
# - GROW - 2
#   m4_require'd code, 2 level deep
# - GROW - 1
#   m4_require'd code, 1 level deep
# - GROW
#   m4_defun'd macros are elaborated here.

m4_define([_m4_divert(GROW)],       10000)


# _m4_defun_pro(MACRO-NAME)
# -------------------------
# The prologue for Autoconf macros.
#
# This is called frequently, so minimize the number of macro invocations
# by avoiding dnl and m4_defn overhead.
m4_define([_m4_defun_pro],
m4_do([[m4_ifdef([m4_expansion_stack], [], [_m4_defun_pro_outer[]])]],
      [[m4_expansion_stack_push(_m4_defn(
	  [m4_location($1)])[: $1 is expanded from...])]],
      [[m4_pushdef([_m4_expanding($1)])]]))

m4_define([_m4_defun_pro_outer],
[m4_copy([_m4_divert_diversion], [_m4_divert_dump])m4_divert_push([GROW])])

# _m4_defun_epi(MACRO-NAME)
# -------------------------
# The Epilogue for Autoconf macros.  MACRO-NAME only helps tracing
# the PRO/EPI pairs.
#
# This is called frequently, so minimize the number of macro invocations
# by avoiding dnl and m4_popdef overhead.
m4_define([_m4_defun_epi],
m4_do([[_m4_popdef([_m4_expanding($1)])]],
      [[m4_expansion_stack_pop()]],
      [[m4_ifdef([m4_expansion_stack], [], [_m4_defun_epi_outer[]])]],
      [[m4_provide([$1])]]))

m4_define([_m4_defun_epi_outer],
[_m4_undefine([_m4_divert_dump])m4_divert_pop([GROW])m4_undivert([GROW])])


# m4_defun(NAME, EXPANSION)
# -------------------------
# Define a macro which automatically provides itself.  Add machinery
# so the macro automatically switches expansion to the diversion
# stack if it is not already using it.  In this case, once finished,
# it will bring back all the code accumulated in the diversion stack.
# This, combined with m4_require, achieves the topological ordering of
# macros.  We don't use this macro to define some frequently called
# macros that are not involved in ordering constraints, to save m4
# processing.
m4_define([m4_defun],
[m4_define([m4_location($1)], m4_location)dnl
m4_define([$1],
	  [_m4_defun_pro([$1])$2[]_m4_defun_epi([$1])])])


# m4_defun_once(NAME, EXPANSION)
# ------------------------------
# As m4_defun, but issues the EXPANSION only once, and warns if used
# several times.
m4_define([m4_defun_once],
[m4_define([m4_location($1)], m4_location)dnl
m4_define([$1],
	  [m4_provide_if([$1],
			 [m4_warn([syntax], [$1 invoked multiple times])],
			 [_m4_defun_pro([$1])$2[]_m4_defun_epi([$1])])])])


# m4_pattern_forbid(ERE, [WHY])
# -----------------------------
# Declare that no token matching the forbidden extended regular
# expression ERE should be seen in the output unless...
m4_define([m4_pattern_forbid], [])


# m4_pattern_allow(ERE)
# ---------------------
# ... that token also matches the allowed extended regular expression ERE.
# Both used via traces.
m4_define([m4_pattern_allow], [])


## --------------------------------- ##
## 11. Dependencies between macros.  ##
## --------------------------------- ##


# m4_before(THIS-MACRO-NAME, CALLED-MACRO-NAME)
# ---------------------------------------------
# Issue a warning if CALLED-MACRO-NAME was called before THIS-MACRO-NAME.
m4_define([m4_before],
[m4_provide_if([$2],
	       [m4_warn([syntax], [$2 was called before $1])])])


# m4_require(NAME-TO-CHECK, [BODY-TO-EXPAND = NAME-TO-CHECK])
# -----------------------------------------------------------
# If NAME-TO-CHECK has never been expanded (actually, if it is not
# m4_provide'd), expand BODY-TO-EXPAND *before* the current macro
# expansion.  Once expanded, emit it in _m4_divert_dump.  Keep track
# of the m4_require chain in m4_expansion_stack.
#
# The normal cases are:
#
# - NAME-TO-CHECK == BODY-TO-EXPAND
#   Which you can use for regular macros with or without arguments, e.g.,
#     m4_require([AC_PROG_CC], [AC_PROG_CC])
#     m4_require([AC_CHECK_HEADERS(limits.h)], [AC_CHECK_HEADERS(limits.h)])
#   which is just the same as
#     m4_require([AC_PROG_CC])
#     m4_require([AC_CHECK_HEADERS(limits.h)])
#
# - BODY-TO-EXPAND == m4_indir([NAME-TO-CHECK])
#   In the case of macros with irregular names.  For instance:
#     m4_require([AC_LANG_COMPILER(C)], [indir([AC_LANG_COMPILER(C)])])
#   which means `if the macro named `AC_LANG_COMPILER(C)' (the parens are
#   part of the name, it is not an argument) has not been run, then
#   call it.'
#   Had you used
#     m4_require([AC_LANG_COMPILER(C)], [AC_LANG_COMPILER(C)])
#   then m4_require would have tried to expand `AC_LANG_COMPILER(C)', i.e.,
#   call the macro `AC_LANG_COMPILER' with `C' as argument.
#
#   You could argue that `AC_LANG_COMPILER', when it receives an argument
#   such as `C' should dispatch the call to `AC_LANG_COMPILER(C)'.  But this
#   `extension' prevents `AC_LANG_COMPILER' from having actual arguments that
#   it passes to `AC_LANG_COMPILER(C)'.
#
# This is called frequently, so minimize the number of macro invocations
# by avoiding dnl and other overhead on the common path.
m4_define([m4_require],
m4_do([[m4_ifdef([_m4_expanding($1)],
		 [m4_fatal([$0: circular dependency of $1])])]],
      [[m4_ifdef([_m4_divert_dump], [],
		 [m4_fatal([$0($1): cannot be used outside of an ]dnl
m4_bmatch([$0], [^AC_], [[AC_DEFUN]], [[m4_defun]])['d macro])])]],
      [[m4_provide_if([$1],
		      [],
		      [_m4_require_call([$1], [$2])])]]))


# _m4_require_call(NAME-TO-CHECK, [BODY-TO-EXPAND = NAME-TO-CHECK])
# -----------------------------------------------------------------
# If m4_require decides to expand the body, it calls this macro.
#
# This is called frequently, so minimize the number of macro invocations
# by avoiding dnl and other overhead on the common path.
m4_define([_m4_require_call],
m4_do([[m4_define([_m4_divert_grow], m4_decr(_m4_divert_grow))]],
      [[m4_divert_push(_m4_divert_grow)]],
      [[m4_default([$2], [$1])
m4_provide_if([$1],
	      [],
	      [m4_warn([syntax],
		       [$1 is m4_require'd but not m4_defun'd])])]],
      [[m4_divert(_m4_defn([_m4_divert_dump]))]],
      [[m4_undivert(_m4_divert_grow)]],
      [[m4_divert_pop(_m4_divert_grow)]],
      [[m4_define([_m4_divert_grow], m4_incr(_m4_divert_grow))]]))


# _m4_divert_grow
# ---------------
# The counter for _m4_require_call.
m4_define([_m4_divert_grow], _m4_divert([GROW]))


# m4_expand_once(TEXT, [WITNESS = TEXT])
# --------------------------------------
# If TEXT has never been expanded, expand it *here*.  Use WITNESS as
# as a memory that TEXT has already been expanded.
m4_define([m4_expand_once],
[m4_provide_if(m4_ifval([$2], [[$2]], [[$1]]),
	       [],
	       [m4_provide(m4_ifval([$2], [[$2]], [[$1]]))[]$1])])


# m4_provide(MACRO-NAME)
# ----------------------
m4_define([m4_provide],
[m4_define([m4_provide($1)])])


# m4_provide_if(MACRO-NAME, IF-PROVIDED, IF-NOT-PROVIDED)
# -------------------------------------------------------
# If MACRO-NAME is provided do IF-PROVIDED, else IF-NOT-PROVIDED.
# The purpose of this macro is to provide the user with a means to
# check macros which are provided without letting her know how the
# information is coded.
m4_define([m4_provide_if],
[m4_ifdef([m4_provide($1)],
	  [$2], [$3])])


## --------------------- ##
## 12. Text processing.  ##
## --------------------- ##


# m4_cr_letters
# m4_cr_LETTERS
# m4_cr_Letters
# -------------
m4_define([m4_cr_letters], [abcdefghijklmnopqrstuvwxyz])
m4_define([m4_cr_LETTERS], [ABCDEFGHIJKLMNOPQRSTUVWXYZ])
m4_define([m4_cr_Letters],
m4_defn([m4_cr_letters])dnl
m4_defn([m4_cr_LETTERS])dnl
)


# m4_cr_digits
# ------------
m4_define([m4_cr_digits], [0123456789])


# m4_cr_alnum
# -----------
m4_define([m4_cr_alnum],
m4_defn([m4_cr_Letters])dnl
m4_defn([m4_cr_digits])dnl
)


# m4_cr_symbols1
# m4_cr_symbols2
# -------------------------------
m4_define([m4_cr_symbols1],
m4_defn([m4_cr_Letters])dnl
_)

m4_define([m4_cr_symbols2],
m4_defn([m4_cr_symbols1])dnl
m4_defn([m4_cr_digits])dnl
)

# m4_cr_all
# ---------
# The character range representing everything, with `-' as the last
# character, since it is special to m4_translit.  Use with care, because
# it contains characters special to M4 (fortunately, both ASCII and EBCDIC
# have [] in order, so m4_defn([m4_cr_all]) remains a valid string).  It
# also contains characters special to terminals, so it should never be
# displayed in an error message.  Also, attempts to map [ and ] to other
# characters via m4_translit must deal with the fact that m4_translit does
# not add quotes to the output.
#
# It is mainly useful in generating inverted character range maps, for use
# in places where m4_translit is faster than an equivalent m4_bpatsubst;
# the regex `[^a-z]' is equivalent to:
#  m4_translit(m4_dquote(m4_defn([m4_cr_all])), [a-z])
m4_define([m4_cr_all],
m4_translit(m4_dquote(m4_format(m4_dquote(m4_for(
  ,1,255,,[[%c]]))m4_for([i],1,255,,[,i]))), [-])-)


# _m4_define_cr_not(CATEGORY)
# ---------------------------
# Define m4_cr_not_CATEGORY as the inverse of m4_cr_CATEGORY.
m4_define([_m4_define_cr_not],
[m4_define([m4_cr_not_$1],
	   m4_translit(m4_dquote(m4_defn([m4_cr_all])),
		       m4_defn([m4_cr_$1])))])


# m4_cr_not_letters
# m4_cr_not_LETTERS
# m4_cr_not_Letters
# m4_cr_not_digits
# m4_cr_not_alnum
# m4_cr_not_symbols1
# m4_cr_not_symbols2
# ------------------
# Inverse character sets
_m4_define_cr_not([letters])
_m4_define_cr_not([LETTERS])
_m4_define_cr_not([Letters])
_m4_define_cr_not([digits])
_m4_define_cr_not([alnum])
_m4_define_cr_not([symbols1])
_m4_define_cr_not([symbols2])


# m4_newline
# ----------
# Expands to a newline.  Exists for formatting reasons.
m4_define([m4_newline], [
])


# m4_re_escape(STRING)
# --------------------
# Escape RE active characters in STRING.
m4_define([m4_re_escape],
[m4_bpatsubst([$1],
	      [[][*+.?\^$]], [\\\&])])


# m4_re_string
# ------------
# Regexp for `[a-zA-Z_0-9]*'
# m4_dquote provides literal [] for the character class.
m4_define([m4_re_string],
m4_dquote(m4_defn([m4_cr_symbols2]))dnl
[*]dnl
)


# m4_re_word
# ----------
# Regexp for `[a-zA-Z_][a-zA-Z_0-9]*'
m4_define([m4_re_word],
m4_dquote(m4_defn([m4_cr_symbols1]))dnl
m4_defn([m4_re_string])dnl
)


# m4_tolower(STRING)
# m4_toupper(STRING)
# ------------------
# These macros convert STRING to lowercase or uppercase.
#
# Rather than expand the m4_defn each time, we inline them up front.
m4_define([m4_tolower],
[m4_translit([$1], ]m4_dquote(m4_defn([m4_cr_LETTERS]))[,
		   ]m4_dquote(m4_defn([m4_cr_letters]))[)])
m4_define([m4_toupper],
[m4_translit([$1], ]m4_dquote(m4_defn([m4_cr_letters]))[,
		   ]m4_dquote(m4_defn([m4_cr_LETTERS]))[)])


# m4_split(STRING, [REGEXP])
# --------------------------
#
# Split STRING into an m4 list of quoted elements.  The elements are
# quoted with [ and ].  Beginning spaces and end spaces *are kept*.
# Use m4_strip to remove them.
#
# REGEXP specifies where to split.  Default is [\t ]+.
#
# If STRING is empty, the result is an empty list.
#
# Pay attention to the m4_changequotes.  When m4 reads the definition of
# m4_split, it still has quotes set to [ and ].  Luckily, these are matched
# in the macro body, so the definition is stored correctly.  Use the same
# alternate quotes as m4_noquote; it must be unlikely to appear in $1.
#
# Also, notice that $1 is quoted twice, since we want the result to
# be quoted.  Then you should understand that the argument of
# patsubst is -=<{(STRING)}>=- (i.e., with additional -=<{( and )}>=-).
#
# This macro is safe on active symbols, i.e.:
#   m4_define(active, ACTIVE)
#   m4_split([active active ])end
#   => [active], [active], []end
#
# Optimize on regex of ` ' (space), since m4_foreach_w already guarantees
# that the list contains single space separators, and a common case is
# splitting a single-element list.  This macro is called frequently,
# so avoid unnecessary dnl inside the definition.
m4_define([m4_split],
[m4_if([$1], [], [],
       [$2], [ ], [m4_if(m4_index([$1], [ ]), [-1], [[[$1]]], [_$0($@)])],
       [$2], [], [_$0([$1], [[	 ]+])],
       [_$0($@)])])

m4_define([_m4_split],
[m4_changequote([-=<{(],[)}>=-])]dnl
[[m4_bpatsubst(-=<{(-=<{($1)}>=-)}>=-, -=<{($2)}>=-,
	       -=<{(], [)}>=-)]m4_changequote([, ])])



# m4_flatten(STRING)
# ------------------
# If STRING contains end of lines, replace them with spaces.  If there
# are backslashed end of lines, remove them.  This macro is safe with
# active symbols.
#    m4_define(active, ACTIVE)
#    m4_flatten([active
#    act\
#    ive])end
#    => active activeend
#
# In m4, m4_bpatsubst is expensive, so first check for a newline.
m4_define([m4_flatten],
[m4_if(m4_index([$1], [
]), [-1], [[$1]],
       [m4_translit(m4_bpatsubst([[[$1]]], [\\
]), [
], [ ])])])


# m4_strip(STRING)
# ----------------
# Expands into STRING with tabs and spaces singled out into a single
# space, and removing leading and trailing spaces.
#
# This macro is robust to active symbols.
#    m4_define(active, ACTIVE)
#    m4_strip([  active <tab> <tab>active ])end
#    => active activeend
#
# First, notice that we guarantee trailing space.  Why?  Because regular
# expressions are greedy, and `.* ?' would always group the space into the
# .* portion.  The algorithm is simpler by avoiding `?' at the end.  The
# algorithm correctly strips everything if STRING is just ` '.
#
# Then notice the second pattern: it is in charge of removing the
# leading/trailing spaces.  Why not just `[^ ]'?  Because they are
# applied to over-quoted strings, i.e. more or less [STRING], due
# to the limitations of m4_bpatsubsts.  So the leading space in STRING
# is the *second* character; equally for the trailing space.
m4_define([m4_strip],
[m4_bpatsubsts([$1 ],
	       [[	 ]+], [ ],
	       [^. ?\(.*\) .$], [[[\1]]])])


# m4_normalize(STRING)
# --------------------
# Apply m4_flatten and m4_strip to STRING.
#
# The argument is quoted, so that the macro is robust to active symbols:
#
#    m4_define(active, ACTIVE)
#    m4_normalize([  act\
#    ive
#    active ])end
#    => active activeend

m4_define([m4_normalize],
[m4_strip(m4_flatten([$1]))])



# m4_join(SEP, ARG1, ARG2...)
# ---------------------------
# Produce ARG1SEPARG2...SEPARGn.  Avoid back-to-back SEP when a given ARG
# is the empty string.  No expansion is performed on SEP or ARGs.
#
# Since the number of arguments to join can be arbitrarily long, we
# want to avoid having more than one $@ in the macro definition;
# otherwise, the expansion would require twice the memory of the already
# long list.  Hence, m4_join merely looks for the first non-empty element,
# and outputs just that element; while _m4_join looks for all non-empty
# elements, and outputs them following a separator.  The final trick to
# note is that we decide between recursing with $0 or _$0 based on the
# nested m4_if ending with `_'.
#
# Please keep foreach.m4 in sync with any adjustments made here.
m4_define([m4_join],
[m4_if([$#], [1], [],
       [$#], [2], [[$2]],
       [m4_if([$2], [], [], [[$2]_])$0([$1], m4_shift2($@))])])
m4_define([_m4_join],
[m4_if([$#$2], [2], [],
       [m4_if([$2], [], [], [[$1$2]])$0([$1], m4_shift2($@))])])

# m4_joinall(SEP, ARG1, ARG2...)
# ------------------------------
# Produce ARG1SEPARG2...SEPARGn.  An empty ARG results in back-to-back SEP.
# No expansion is performed on SEP or ARGs.
#
# Please keep foreach.m4 in sync with any adjustments made here.
m4_define([m4_joinall], [[$2]_$0([$1], m4_shift($@))])
m4_define([_m4_joinall],
[m4_if([$#], [2], [], [[$1$3]$0([$1], m4_shift2($@))])])

# m4_combine([SEPARATOR], PREFIX-LIST, [INFIX], SUFFIX...)
# --------------------------------------------------------
# Produce the pairwise combination of every element in the quoted,
# comma-separated PREFIX-LIST with every element from the SUFFIX arguments.
# Each pair is joined with INFIX, and pairs are separated by SEPARATOR.
# No expansion occurs on SEPARATOR, INFIX, or elements of either list.
#
# For example:
#   m4_combine([, ], [[a], [b], [c]], [-], [1], [2], [3])
#   => a-1, a-2, a-3, b-1, b-2, b-3, c-1, c-2, c-3
#
# In order to have the correct number of SEPARATORs, we use a temporary
# variable that redefines itself after the first use.  We must use defn
# rather than overquoting in case PREFIX or SUFFIX contains $1, but use
# _m4_defn for speed.  Likewise, we compute the m4_shift3 only once,
# rather than in each iteration of the outer m4_foreach.
m4_define([m4_combine],
[m4_if(m4_eval([$# > 3]), [1],
       [m4_pushdef([m4_Separator], [m4_define([m4_Separator],
				    _m4_defn([m4_echo]))])]]dnl
[[m4_foreach([m4_Prefix], [$2],
	     [m4_foreach([m4_Suffix], ]m4_dquote(m4_dquote(m4_shift3($@)))[,
	[m4_Separator([$1])[]_m4_defn([m4_Prefix])[$3]_m4_defn(
						      [m4_Suffix])])])]]dnl
[[_m4_popdef([m4_Separator])])])


# m4_append(MACRO-NAME, STRING, [SEPARATOR])
# ------------------------------------------
# Redefine MACRO-NAME to hold its former content plus `SEPARATOR`'STRING'
# at the end.  It is valid to use this macro with MACRO-NAME undefined,
# in which case no SEPARATOR is added.  Be aware that the criterion is
# `not being defined', and not `not being empty'.
#
# Note that neither STRING nor SEPARATOR are expanded here; rather, when
# you expand MACRO-NAME, they will be expanded at that point in time.
#
# This macro is robust to active symbols.  It can be used to grow
# strings.
#
#    | m4_define(active, ACTIVE)dnl
#    | m4_append([sentence], [This is an])dnl
#    | m4_append([sentence], [ active ])dnl
#    | m4_append([sentence], [symbol.])dnl
#    | sentence
#    | m4_undefine([active])dnl
#    | sentence
#    => This is an ACTIVE symbol.
#    => This is an active symbol.
#
# It can be used to define hooks.
#
#    | m4_define(active, ACTIVE)dnl
#    | m4_append([hooks], [m4_define([act1], [act2])])dnl
#    | m4_append([hooks], [m4_define([act2], [active])])dnl
#    | m4_undefine([active])dnl
#    | act1
#    | hooks
#    | act1
#    => act1
#    =>
#    => active
#
# It can also be used to create lists, although this particular usage was
# broken prior to autoconf 2.62.
#    | m4_append([list], [one], [, ])dnl
#    | m4_append([list], [two], [, ])dnl
#    | m4_append([list], [three], [, ])dnl
#    | list
#    | m4_dquote(list)
#    => one, two, three
#    => [one],[two],[three]
#
# Note that m4_append can benefit from amortized O(n) m4 behavior, if
# the underlying m4 implementation is smart enough to avoid copying existing
# contents when enlarging a macro's definition into any pre-allocated storage
# (m4 1.4.x unfortunately does not implement this optimization).  We do
# not implement m4_prepend, since it is inherently O(n^2) (pre-allocated
# storage only occurs at the end of a macro, so the existing contents must
# always be moved).
#
# Use _m4_defn for speed.
m4_define([m4_append],
[m4_define([$1], m4_ifdef([$1], [_m4_defn([$1])[$3]])[$2])])


# m4_append_uniq(MACRO-NAME, STRING, [SEPARATOR], [IF-UNIQ], [IF-DUP])
# --------------------------------------------------------------------
# Like `m4_append', but append only if not yet present.  Additionally,
# expand IF-UNIQ if STRING was appended, or IF-DUP if STRING was already
# present.  Also, warn if SEPARATOR is not empty and occurs within STRING,
# as the algorithm no longer guarantees uniqueness.
#
# Note that while m4_append can be O(n) (depending on the quality of the
# underlying M4 implementation), m4_append_uniq is inherently O(n^2)
# because each append operation searches the entire string.
m4_define([m4_append_uniq],
[m4_ifval([$3], [m4_if(m4_index([$2], [$3]), [-1], [],
		       [m4_warn([syntax],
				[$0: `$2' contains `$3'])])])_$0($@)])
m4_define([_m4_append_uniq],
[m4_ifdef([$1],
	  [m4_if(m4_index([$3]_m4_defn([$1])[$3], [$3$2$3]), [-1],
		 [m4_append([$1], [$2], [$3])$4], [$5])],
	  [m4_define([$1], [$2])$4])])

# m4_append_uniq_w(MACRO-NAME, STRINGS)
# -------------------------------------
# For each of the words in the whitespace separated list STRINGS, append
# only the unique strings to the definition of MACRO-NAME.
#
# Use _m4_defn for speed.
m4_define([m4_append_uniq_w],
[m4_foreach_w([m4_Word], [$2],
	      [_m4_append_uniq([$1], _m4_defn([m4_Word]), [ ])])])


# m4_text_wrap(STRING, [PREFIX], [FIRST-PREFIX], [WIDTH])
# -------------------------------------------------------
# Expands into STRING wrapped to hold in WIDTH columns (default = 79).
# If PREFIX is given, each line is prefixed with it.  If FIRST-PREFIX is
# specified, then the first line is prefixed with it.  As a special case,
# if the length of FIRST-PREFIX is greater than that of PREFIX, then
# FIRST-PREFIX will be left alone on the first line.
#
# No expansion occurs on the contents STRING, PREFIX, or FIRST-PREFIX,
# although quadrigraphs are correctly recognized.
#
# Typical outputs are:
#
# m4_text_wrap([Short string */], [   ], [/* ], 20)
#  => /* Short string */
#
# m4_text_wrap([Much longer string */], [   ], [/* ], 20)
#  => /* Much longer
#  =>    string */
#
# m4_text_wrap([Short doc.], [          ], [  --short ], 30)
#  =>   --short Short doc.
#
# m4_text_wrap([Short doc.], [          ], [  --too-wide ], 30)
#  =>   --too-wide
#  =>           Short doc.
#
# m4_text_wrap([Super long documentation.], [          ], [  --too-wide ], 30)
#  =>   --too-wide
#  =>      Super long
#  =>      documentation.
#
# FIXME: there is no checking of a longer PREFIX than WIDTH, but do
# we really want to bother with people trying each single corner
# of a software?
#
# This macro does not leave a trailing space behind the last word of a line,
# which complicates it a bit.  The algorithm is otherwise stupid and simple:
# all the words are preceded by m4_Separator which is defined to empty for
# the first word, and then ` ' (single space) for all the others.
#
# The algorithm uses a helper that uses $2 through $4 directly, rather than
# using local variables, to avoid m4_defn overhead, or expansion swallowing
# any $.  It also bypasses m4_popdef overhead with _m4_popdef since no user
# macro expansion occurs in the meantime.  Also, the definition is written
# with m4_do, to avoid time wasted on dnl during expansion (since this is
# already a time-consuming macro).
m4_define([m4_text_wrap],
[_$0([$1], [$2], m4_if([$3], [], [[$2]], [[$3]]),
     m4_if([$4], [], [79], [[$4]]))])
m4_define([_m4_text_wrap],
m4_do(dnl set up local variables, to avoid repeated calculations
[[m4_pushdef([m4_Indent], m4_qlen([$2]))]],
[[m4_pushdef([m4_Cursor], m4_qlen([$3]))]],
[[m4_pushdef([m4_Separator], [m4_define([m4_Separator], [ ])])]],
dnl expand the first prefix, then check its length vs. regular prefix
dnl same length: nothing special
dnl prefix1 longer: output on line by itself, and reset cursor
dnl prefix1 shorter: pad to length of prefix, and reset cursor
[[[$3]m4_cond([m4_Cursor], m4_Indent, [],
	      [m4_eval(m4_Cursor > m4_Indent)], [1], [
[$2]m4_define([m4_Cursor], m4_Indent)],
	      [m4_format([%*s], m4_max([0],
  m4_eval(m4_Indent - m4_Cursor)), [])m4_define([m4_Cursor], m4_Indent)])]],
dnl now, for each word, compute the curser after the word is output, then
dnl check if the cursor would exceed the wrap column
dnl if so, reset cursor, and insert newline and prefix
dnl if not, insert the separator (usually a space)
dnl either way, insert the word
[[m4_foreach_w([m4_Word], [$1],
  [m4_define([m4_Cursor],
	     m4_eval(m4_Cursor + m4_qlen(_m4_defn([m4_Word]))
		     + 1))m4_if(m4_eval(m4_Cursor > ([$4])),
      [1], [m4_define([m4_Cursor],
		      m4_eval(m4_Indent + m4_qlen(_m4_defn([m4_Word])) + 1))
[$2]],
      [m4_Separator[]])_m4_defn([m4_Word])])]],
dnl finally, clean up the local variabls
[[_m4_popdef([m4_Separator], [m4_Cursor], [m4_Indent])]]))


# m4_text_box(MESSAGE, [FRAME-CHARACTER = `-'])
# ---------------------------------------------
# Turn MESSAGE into:
#  ## ------- ##
#  ## MESSAGE ##
#  ## ------- ##
# using FRAME-CHARACTER in the border.
m4_define([m4_text_box],
[m4_pushdef([m4_Border],
	    m4_translit(m4_format([%*s], m4_qlen(m4_expand([$1])), []),
			[ ], m4_if([$2], [], [[-]], [[$2]])))dnl
@%:@@%:@ m4_Border @%:@@%:@
@%:@@%:@ $1 @%:@@%:@
@%:@@%:@ m4_Border @%:@@%:@_m4_popdef([m4_Border])dnl
])


# m4_qlen(STRING)
# ---------------
# Expands to the length of STRING after autom4te converts all quadrigraphs.
#
# Avoid bpatsubsts for the common case of no quadrigraphs.
m4_define([m4_qlen],
[m4_if(m4_index([$1], [@]), [-1], [m4_len([$1])],
       [m4_len(m4_bpatsubst([[$1]],
			    [@\(\(<:\|:>\|S|\|%:\|\{:\|:\}\)\(@\)\|&t@\)],
			    [\3]))])])


# m4_qdelta(STRING)
# -----------------
# Expands to the net change in the length of STRING from autom4te converting the
# quadrigraphs in STRING.  This number is always negative or zero.
m4_define([m4_qdelta],
[m4_eval(m4_qlen([$1]) - m4_len([$1]))])



## ----------------------- ##
## 13. Number processing.  ##
## ----------------------- ##

# m4_cmp(A, B)
# ------------
# Compare two integer expressions.
# A < B -> -1
# A = B ->  0
# A > B ->  1
m4_define([m4_cmp],
[m4_eval((([$1]) > ([$2])) - (([$1]) < ([$2])))])


# m4_list_cmp(A, B)
# -----------------
#
# Compare the two lists of integer expressions A and B.  For instance:
#   m4_list_cmp([1, 0],     [1])    ->  0
#   m4_list_cmp([1, 0],     [1, 0]) ->  0
#   m4_list_cmp([1, 2],     [1, 0]) ->  1
#   m4_list_cmp([1, 2, 3],  [1, 2]) ->  1
#   m4_list_cmp([1, 2, -3], [1, 2]) -> -1
#   m4_list_cmp([1, 0],     [1, 2]) -> -1
#   m4_list_cmp([1],        [1, 2]) -> -1
#   m4_define([xa], [oops])dnl
#   m4_list_cmp([[0xa]],    [5+5])  -> 0
#
# Rather than face the overhead of m4_case, we use a helper function whose
# expansion includes the name of the macro to invoke on the tail, either
# m4_ignore or m4_unquote.  This is particularly useful when comparing
# long lists, since less text is being expanded for deciding when to end
# recursion.  The recursion is between a pair of macros that alternate
# which list is trimmed by one element; this is more efficient than
# calling m4_cdr on both lists from a single macro.  Guarantee exactly
# one expansion of both lists' side effects.
#
# Please keep foreach.m4 in sync with any adjustments made here.
m4_define([m4_list_cmp],
[_$0_raw(m4_dquote($1), m4_dquote($2))])

m4_define([_m4_list_cmp_raw],
[m4_if([$1], [$2], [0], [_m4_list_cmp_1([$1], $2)])])

m4_define([_m4_list_cmp],
[m4_if([$1], [], [0m4_ignore], [$2], [0], [m4_unquote], [$2m4_ignore])])

m4_define([_m4_list_cmp_1],
[_m4_list_cmp_2([$2], [m4_shift2($@)], $1)])

m4_define([_m4_list_cmp_2],
[_m4_list_cmp([$1$3], m4_cmp([$3+0], [$1+0]))(
  [_m4_list_cmp_1(m4_dquote(m4_shift3($@)), $2)])])

# m4_max(EXPR, ...)
# m4_min(EXPR, ...)
# -----------------
# Return the decimal value of the maximum (or minimum) in a series of
# integer expressions.
#
# M4 1.4.x doesn't provide ?:.  Hence this huge m4_eval.  Avoid m4_eval
# if both arguments are identical, but be aware of m4_max(0xa, 10) (hence
# the use of <=, not just <, in the second multiply).
#
# Please keep foreach.m4 in sync with any adjustments made here.
m4_define([m4_max],
[m4_if([$#], [0], [m4_fatal([too few arguments to $0])],
       [$#], [1], [m4_eval([$1])],
       [$#$1], [2$2], [m4_eval([$1])],
       [$#], [2], [_$0($@)],
       [_m4_minmax([_$0], $@)])])

m4_define([_m4_max],
[m4_eval((([$1]) > ([$2])) * ([$1]) + (([$1]) <= ([$2])) * ([$2]))])

m4_define([m4_min],
[m4_if([$#], [0], [m4_fatal([too few arguments to $0])],
       [$#], [1], [m4_eval([$1])],
       [$#$1], [2$2], [m4_eval([$1])],
       [$#], [2], [_$0($@)],
       [_m4_minmax([_$0], $@)])])

m4_define([_m4_min],
[m4_eval((([$1]) < ([$2])) * ([$1]) + (([$1]) >= ([$2])) * ([$2]))])

# _m4_minmax(METHOD, ARG1, ARG2...)
# ---------------------------------
# Common recursion code for m4_max and m4_min.  METHOD must be _m4_max
# or _m4_min, and there must be at least two arguments to combine.
#
# Please keep foreach.m4 in sync with any adjustments made here.
m4_define([_m4_minmax],
[m4_if([$#], [3], [$1([$2], [$3])],
       [$0([$1], $1([$2], [$3]), m4_shift3($@))])])


# m4_sign(A)
# ----------
# The sign of the integer expression A.
m4_define([m4_sign],
[m4_eval((([$1]) > 0) - (([$1]) < 0))])



## ------------------------ ##
## 14. Version processing.  ##
## ------------------------ ##


# m4_version_unletter(VERSION)
# ----------------------------
# Normalize beta version numbers with letters to numeric expressions, which
# can then be handed to m4_eval for the purpose of comparison.
#
#   Nl -> (N+1).-1.(l#)
#
# for example:
#   [2.14a] -> [2.14+1.-1.[0r36:a]] -> 2.15.-1.10
#   [2.14b] -> [2.15+1.-1.[0r36:b]] -> 2.15.-1.11
#   [2.61aa.b] -> [2.61+1.-1.[0r36:aa],+1.-1.[0r36:b]] -> 2.62.-1.370.1.-1.11
#
# This macro expects reasonable version numbers, but can handle double
# letters and does not expand any macros.  Original version strings can
# use both `.' and `-' separators.
#
# Inline constant expansions, to avoid m4_defn overhead.
# _m4_version_unletter is the real workhorse used by m4_version_compare,
# but since [0r36:a] is less readable than 10, we provide a wrapper for
# human use.
m4_define([m4_version_unletter],
[m4_map_sep([m4_eval], [.],
	    m4_dquote(m4_dquote_elt(m4_unquote(_$0([$1])))))])
m4_define([_m4_version_unletter],
[m4_bpatsubst(m4_translit([[[$1]]], [.-], [,,]),]dnl
m4_dquote(m4_dquote(m4_defn([m4_cr_Letters])))[[+],
	      [+1,-1,[0r36:\&]])])


# m4_version_compare(VERSION-1, VERSION-2)
# ----------------------------------------
# Compare the two version numbers and expand into
#  -1 if VERSION-1 < VERSION-2
#   0 if           =
#   1 if           >
#
# Since _m4_version_unletter does not output side effects, we can
# safely bypass the overhead of m4_version_cmp.
m4_define([m4_version_compare],
[_m4_list_cmp_raw(_m4_version_unletter([$1]), _m4_version_unletter([$2]))])


# m4_PACKAGE_NAME
# m4_PACKAGE_TARNAME
# m4_PACKAGE_VERSION
# m4_PACKAGE_STRING
# m4_PACKAGE_BUGREPORT
# --------------------
# If m4sugar/version.m4 is present, then define version strings.  This
# file is optional, provided by Autoconf but absent in Bison.
m4_sinclude([m4sugar/version.m4])


# m4_version_prereq(VERSION, [IF-OK], [IF-NOT = FAIL])
# ----------------------------------------------------
# Check this Autoconf version against VERSION.
m4_define([m4_version_prereq],
m4_ifdef([m4_PACKAGE_VERSION],
[[m4_if(m4_version_compare(]m4_dquote(m4_defn([m4_PACKAGE_VERSION]))[, [$1]),
	[-1],
	[m4_default([$3],
		    [m4_fatal([Autoconf version $1 or higher is required],
			      [63])])],
	[$2])]],
[[m4_fatal([m4sugar/version.m4 not found])]]))


## ------------------ ##
## 15. Set handling.  ##
## ------------------ ##

# Autoconf likes to create arbitrarily large sets; for example, as of
# this writing, the configure.ac for coreutils tracks a set of more
# than 400 AC_SUBST.  How do we track all of these set members,
# without introducing duplicates?  We could use m4_append_uniq, with
# the set NAME residing in the contents of the macro NAME.
# Unfortunately, m4_append_uniq is quadratic for set creation, because
# it costs O(n) to search the string for each of O(n) insertions; not
# to mention that with m4 1.4.x, even using m4_append is slow, costing
# O(n) rather than O(1) per insertion.  Other set operations, not used
# by Autoconf but still possible by manipulation of the definition
# tracked in macro NAME, include O(n) deletion of one element and O(n)
# computation of set size.  Because the set is exposed to the user via
# the definition of a single macro, we cannot cache any data about the
# set without risking the cache being invalidated by the user
# redefining NAME.
#
# Can we do better?  Yes, because m4 gives us an O(1) search function
# for free: ifdef.  Additionally, even m4 1.4.x gives us an O(1)
# insert operation for free: pushdef.  But to use these, we must
# represent the set via a group of macros; to keep the set consistent,
# we must hide the set so that the user can only manipulate it through
# accessor macros.  The contents of the set are maintained through two
# access points; _m4_set([name]) is a pushdef stack of values in the
# set, useful for O(n) traversal of the set contents; while the
# existence of _m4_set([name],value) with no particular value is
# useful for O(1) querying of set membership.  And since the user
# cannot externally manipulate the set, we are free to add additional
# caching macros for other performance improvements.  Deletion can be
# O(1) per element rather than O(n), by reworking the definition of
# _m4_set([name],value) to be 0 or 1 based on current membership, and
# adding _m4_set_cleanup(name) to defer the O(n) cleanup of
# _m4_set([name]) until we have another reason to do an O(n)
# traversal.  The existence of _m4_set_cleanup(name) can then be used
# elsewhere to determine if we must dereference _m4_set([name],value),
# or assume that definition implies set membership.  Finally, size can
# be tracked in an O(1) fashion with _m4_set_size(name).
#
# The quoting in _m4_set([name],value) is chosen so that there is no
# ambiguity with a set whose name contains a comma, and so that we can
# supply the value via _m4_defn([_m4_set([name])]) without needing any
# quote manipulation.

# m4_set_add(SET, VALUE, [IF-UNIQ], [IF-DUP])
# -------------------------------------------
# Add VALUE as an element of SET.  Expand IF-UNIQ on the first
# addition, and IF-DUP if it is already in the set.  Addition of one
# element is O(1), such that overall set creation is O(n).
#
# We do not want to add a duplicate for a previously deleted but
# unpruned element, but it is just as easy to check existence directly
# as it is to query _m4_set_cleanup($1).
m4_define([m4_set_add],
[m4_ifdef([_m4_set([$1],$2)],
	  [m4_if(m4_indir([_m4_set([$1],$2)]), [0],
		 [m4_define([_m4_set([$1],$2)],
			    [1])_m4_set_size([$1], [m4_incr])$3], [$4])],
	  [m4_define([_m4_set([$1],$2)],
		     [1])m4_pushdef([_m4_set([$1])],
				    [$2])_m4_set_size([$1], [m4_incr])$3])])

# m4_set_add_all(SET, VALUE...)
# -----------------------------
# Add each VALUE into SET.  This is O(n) in the number of VALUEs, and
# can be faster than calling m4_set_add for each VALUE.
#
# Implement two recursion helpers; the check variant is slower but
# handles the case where an element has previously been removed but
# not pruned.  The recursion helpers ignore their second argument, so
# that we can use the faster m4_shift2 and 2 arguments, rather than
# _m4_shift2 and one argument, as the signal to end recursion.
#
# Please keep foreach.m4 in sync with any adjustments made here.
m4_define([m4_set_add_all],
[m4_define([_m4_set_size($1)], m4_eval(m4_set_size([$1])
  + m4_len(m4_ifdef([_m4_set_cleanup($1)], [_$0_check], [_$0])([$1], $@))))])

m4_define([_m4_set_add_all],
[m4_if([$#], [2], [],
       [m4_ifdef([_m4_set([$1],$3)], [],
		 [m4_define([_m4_set([$1],$3)], [1])m4_pushdef([_m4_set([$1])],
	   [$3])-])$0([$1], m4_shift2($@))])])

m4_define([_m4_set_add_all_check],
[m4_if([$#], [2], [],
       [m4_set_add([$1], [$3])$0([$1], m4_shift2($@))])])

# m4_set_contains(SET, VALUE, [IF-PRESENT], [IF-ABSENT])
# ------------------------------------------------------
# Expand IF-PRESENT if SET contains VALUE, otherwise expand IF-ABSENT.
# This is always O(1).
m4_define([m4_set_contains],
[m4_ifdef([_m4_set_cleanup($1)],
	  [m4_if(m4_ifdef([_m4_set([$1],$2)],
		    [m4_indir([_m4_set([$1],$2)])], [0]), [1], [$3], [$4])],
	  [m4_ifdef([_m4_set([$1],$2)], [$3], [$4])])])

# m4_set_contents(SET, [SEP])
# ---------------------------
# Expand to a single string containing all the elements in SET,
# separated by SEP, without modifying SET.  No provision is made for
# disambiguating set elements that contain non-empty SEP as a
# sub-string, or for recognizing a set that contains only the empty
# string.  Order of the output is not guaranteed.  If any elements
# have been previously removed from the set, this action will prune
# the unused memory.  This is O(n) in the size of the set before
# pruning.
#
# Use _m4_popdef for speed.  The existence of _m4_set_cleanup($1)
# determines which version of _1 helper we use.
m4_define([m4_set_contents],
[m4_ifdef([_m4_set_cleanup($1)], [_$0_1c], [_$0_1])([$1])_$0_2([$1],
    [_m4_defn([_m4_set_($1)])], [[$2]])])

# _m4_set_contents_1(SET)
# _m4_set_contents_1c(SET)
# _m4_set_contents_2(SET, SEP, PREP)
# ----------------------------------
# Expand to a list of quoted elements currently in the set, separated
# by SEP, and moving PREP in front of SEP on recursion.  To avoid
# nesting limit restrictions, the algorithm must be broken into two
# parts; _1 destructively copies the stack in reverse into
# _m4_set_($1), producing no output; then _2 destructively copies
# _m4_set_($1) back into the stack in reverse.  SEP is expanded while
# _m4_set_($1) contains the current element, so a SEP containing
# _m4_defn([_m4_set_($1)]) can produce output in the order the set was
# created.  Behavior is undefined if SEP tries to recursively list or
# modify SET in any way other than calling m4_set_remove on the
# current element.  Use _1 if all entries in the stack are guaranteed
# to be in the set, and _1c to prune removed entries.  Uses _m4_defn
# and _m4_popdef for speed.
m4_define([_m4_set_contents_1],
[m4_ifdef([_m4_set([$1])], [m4_pushdef([_m4_set_($1)],
    _m4_defn([_m4_set([$1])]))_m4_popdef([_m4_set([$1])])$0([$1])])])

m4_define([_m4_set_contents_1c],
[m4_ifdef([_m4_set([$1])],
	  [m4_set_contains([$1], _m4_defn([_m4_set([$1])]),
		   [m4_pushdef([_m4_set_($1)], _m4_defn([_m4_set([$1])]))],
		   [_m4_popdef([_m4_set([$1],]_m4_defn(
      [_m4_set([$1])])[)])])_m4_popdef([_m4_set([$1])])$0([$1])],
	  [_m4_popdef([_m4_set_cleanup($1)])])])

m4_define([_m4_set_contents_2],
[m4_ifdef([_m4_set_($1)], [m4_pushdef([_m4_set([$1])],
    _m4_defn([_m4_set_($1)]))$2[]_m4_popdef([_m4_set_($1)])$0([$1], [$3$2])])])

# m4_set_delete(SET)
# ------------------
# Delete all elements in SET, and reclaim any memory occupied by the
# set.  This is O(n) in the set size.
#
# Use _m4_defn and _m4_popdef for speed.
m4_define([m4_set_delete],
[m4_ifdef([_m4_set([$1])],
	  [_m4_popdef([_m4_set([$1],]_m4_defn([_m4_set([$1])])[)],
		      [_m4_set([$1])])$0([$1])],
	  [m4_ifdef([_m4_set_cleanup($1)],
		    [_m4_popdef([_m4_set_cleanup($1)])])m4_ifdef(
		    [_m4_set_size($1)],
		    [_m4_popdef([_m4_set_size($1)])])])])

# m4_set_difference(SET1, SET2)
# -----------------------------
# Produce a LIST of quoted elements that occur in SET1 but not SET2.
# Output a comma prior to any elements, to distinguish the empty
# string from no elements.  This can be directly used as a series of
# arguments, such as for m4_join, or wrapped inside quotes for use in
# m4_foreach.  Order of the output is not guaranteed.
#
# Short-circuit the idempotence relation.  Use _m4_defn for speed.
m4_define([m4_set_difference],
[m4_if([$1], [$2], [],
       [m4_set_foreach([$1], [_m4_element],
		       [m4_set_contains([$2], _m4_defn([_m4_element]), [],
					[,_m4_defn([_m4_element])])])])])

# m4_set_dump(SET, [SEP])
# -----------------------
# Expand to a single string containing all the elements in SET,
# separated by SEP, then delete SET.  In general, if you only need to
# list the contents once, this is faster than m4_set_contents.  No
# provision is made for disambiguating set elements that contain
# non-empty SEP as a sub-string.  Order of the output is not
# guaranteed.  This is O(n) in the size of the set before pruning.
#
# Use _m4_popdef for speed.  Use existence of _m4_set_cleanup($1) to
# decide if more expensive recursion is needed.
m4_define([m4_set_dump],
[m4_ifdef([_m4_set_size($1)],
	  [_m4_popdef([_m4_set_size($1)])])m4_ifdef([_m4_set_cleanup($1)],
    [_$0_check], [_$0])([$1], [], [$2])])

# _m4_set_dump(SET, SEP, PREP)
# _m4_set_dump_check(SET, SEP, PREP)
# ----------------------------------
# Print SEP and the current element, then delete the element and
# recurse with empty SEP changed to PREP.  The check variant checks
# whether the element has been previously removed.  Use _m4_defn and
# _m4_popdef for speed.
m4_define([_m4_set_dump],
[m4_ifdef([_m4_set([$1])],
	  [[$2]_m4_defn([_m4_set([$1])])_m4_popdef([_m4_set([$1],]_m4_defn(
		[_m4_set([$1])])[)], [_m4_set([$1])])$0([$1], [$2$3])])])

m4_define([_m4_set_dump_check],
[m4_ifdef([_m4_set([$1])],
	  [m4_set_contains([$1], _m4_defn([_m4_set([$1])]),
			   [[$2]_m4_defn([_m4_set([$1])])])_m4_popdef(
    [_m4_set([$1],]_m4_defn([_m4_set([$1])])[)],
    [_m4_set([$1])])$0([$1], [$2$3])],
	  [_m4_popdef([_m4_set_cleanup($1)])])])

# m4_set_empty(SET, [IF-EMPTY], [IF-ELEMENTS])
# --------------------------------------------
# Expand IF-EMPTY if SET has no elements, otherwise IF-ELEMENTS.
m4_define([m4_set_empty],
[m4_ifdef([_m4_set_size($1)],
	  [m4_if(m4_indir([_m4_set_size($1)]), [0], [$2], [$3])], [$2])])

# m4_set_foreach(SET, VAR, ACTION)
# --------------------------------
# For each element of SET, define VAR to the element and expand
# ACTION.  ACTION should not recursively list SET's contents, add
# elements to SET, nor delete any element from SET except the one
# currently in VAR.  The order that the elements are visited in is not
# guaranteed.  This is faster than the corresponding m4_foreach([VAR],
#   m4_indir([m4_dquote]m4_set_listc([SET])), [ACTION])
m4_define([m4_set_foreach],
[m4_pushdef([$2])m4_ifdef([_m4_set_cleanup($1)],
    [_m4_set_contents_1c], [_m4_set_contents_1])([$1])_m4_set_contents_2([$1],
       [m4_define([$2], _m4_defn([_m4_set_($1)]))$3[]])m4_popdef([$2])])

# m4_set_intersection(SET1, SET2)
# -------------------------------
# Produce a LIST of quoted elements that occur in both SET1 or SET2.
# Output a comma prior to any elements, to distinguish the empty
# string from no elements.  This can be directly used as a series of
# arguments, such as for m4_join, or wrapped inside quotes for use in
# m4_foreach.  Order of the output is not guaranteed.
#
# Iterate over the smaller set, and short-circuit the idempotence
# relation.  Use _m4_defn for speed.
m4_define([m4_set_intersection],
[m4_if([$1], [$2], [m4_set_listc([$1])],
       m4_eval(m4_set_size([$2]) < m4_set_size([$1])), [1], [$0([$2], [$1])],
       [m4_set_foreach([$1], [_m4_element],
		       [m4_set_contains([$2], _m4_defn([_m4_element]),
					[,_m4_defn([_m4_element])])])])])

# m4_set_list(SET)
# m4_set_listc(SET)
# -----------------
# Produce a LIST of quoted elements of SET.  This can be directly used
# as a series of arguments, such as for m4_join or m4_set_add_all, or
# wrapped inside quotes for use in m4_foreach or m4_map.  With
# m4_set_list, there is no way to distinguish an empty set from a set
# containing only the empty string; with m4_set_listc, a leading comma
# is output if there are any elements.
m4_define([m4_set_list],
[m4_ifdef([_m4_set_cleanup($1)], [_m4_set_contents_1c],
	  [_m4_set_contents_1])([$1])_m4_set_contents_2([$1],
	       [_m4_defn([_m4_set_($1)])], [,])])

m4_define([m4_set_listc],
[m4_ifdef([_m4_set_cleanup($1)], [_m4_set_contents_1c],
	  [_m4_set_contents_1])([$1])_m4_set_contents_2([$1],
	       [,_m4_defn([_m4_set_($1)])])])

# m4_set_remove(SET, VALUE, [IF-PRESENT], [IF-ABSENT])
# ----------------------------------------------------
# If VALUE is an element of SET, delete it and expand IF-PRESENT.
# Otherwise expand IF-ABSENT.  Deleting a single value is O(1),
# although it leaves memory occupied until the next O(n) traversal of
# the set which will compact the set.
#
# Optimize if the element being removed is the most recently added,
# since defining _m4_set_cleanup($1) slows down so many other macros.
# In particular, this plays well with m4_set_foreach.
m4_define([m4_set_remove],
[m4_set_contains([$1], [$2], [_m4_set_size([$1],
    [m4_decr])m4_if(_m4_defn([_m4_set([$1])]), [$2],
		    [_m4_popdef([_m4_set([$1],$2)], [_m4_set([$1])])],
		    [m4_define([_m4_set_cleanup($1)])m4_define(
		      [_m4_set([$1],$2)], [0])])$3], [$4])])

# m4_set_size(SET)
# ----------------
# Expand to the number of elements currently in SET.  This operation
# is O(1), and thus more efficient than m4_count(m4_set_list([SET])).
m4_define([m4_set_size],
[m4_ifdef([_m4_set_size($1)], [m4_indir([_m4_set_size($1)])], [0])])

# _m4_set_size(SET, ACTION)
# -------------------------
# ACTION must be either m4_incr or m4_decr, and the size of SET is
# changed accordingly.  If the set is empty, ACTION must not be
# m4_decr.
m4_define([_m4_set_size],
[m4_define([_m4_set_size($1)],
	   m4_ifdef([_m4_set_size($1)], [$2(m4_indir([_m4_set_size($1)]))],
		    [1]))])

# m4_set_union(SET1, SET2)
# ------------------------
# Produce a LIST of double quoted elements that occur in either SET1
# or SET2, without duplicates.  Output a comma prior to any elements,
# to distinguish the empty string from no elements.  This can be
# directly used as a series of arguments, such as for m4_join, or
# wrapped inside quotes for use in m4_foreach.  Order of the output is
# not guaranteed.
#
# We can rely on the fact that m4_set_listc prunes SET1, so we don't
# need to check _m4_set([$1],element) for 0.  Use _m4_defn for speed.
# Short-circuit the idempotence relation.
m4_define([m4_set_union],
[m4_set_listc([$1])m4_if([$1], [$2], [], [m4_set_foreach([$2], [_m4_element],
  [m4_ifdef([_m4_set([$1],]_m4_defn([_m4_element])[)], [],
	    [,_m4_defn([_m4_element])])])])])


## ------------------- ##
## 16. File handling.  ##
## ------------------- ##


# It is a real pity that M4 comes with no macros to bind a diversion
# to a file.  So we have to deal without, which makes us a lot more
# fragile than we should.


# m4_file_append(FILE-NAME, CONTENT)
# ----------------------------------
m4_define([m4_file_append],
[m4_syscmd([cat >>$1 <<_m4eof
$2
_m4eof
])
m4_if(m4_sysval, [0], [],
      [m4_fatal([$0: cannot write: $1])])])



## ------------------------ ##
## 17. Setting M4sugar up.  ##
## ------------------------ ##


# m4_init
# -------
# Initialize the m4sugar language.
m4_define([m4_init],
[# All the M4sugar macros start with `m4_', except `dnl' kept as is
# for sake of simplicity.
m4_pattern_forbid([^_?m4_])
m4_pattern_forbid([^dnl$])

# If __m4_version__ is defined, we assume that we are being run by M4
# 1.6 or newer, and thus that $@ recursion is linear and debugmode(d)
# is available for faster checks of dereferencing undefined macros.
# But if it is missing, we assume we are being run by M4 1.4.x, that
# $@ recursion is quadratic, and that we need foreach-based
# replacement macros.  Use the raw builtin to avoid tripping up
# include tracing.
m4_ifdef([__m4_version__],
[m4_debugmode([+d])
m4_copy([_m4_defn], [m4_defn])
m4_copy([_m4_popdef], [m4_popdef])
m4_copy([_m4_undefine], [m4_undefine])],
[m4_builtin([include], [m4sugar/foreach.m4])])

# _m4_divert_diversion should be defined:
m4_divert_push([KILL])

# Check the divert push/pop perfect balance.
m4_wrap([m4_divert_pop([])
	 m4_ifdef([_m4_divert_diversion],
	   [m4_fatal([$0: unbalanced m4_divert_push:]_m4_divert_n_stack)])[]])
])
