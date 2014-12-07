#                                                  -*- Autoconf -*-
# This file is part of Autoconf.
# foreach-based replacements for recursive functions.
# Speeds up GNU M4 1.4.x by avoiding quadratic $@ recursion, but penalizes
# GNU M4 1.6 by requiring more memory and macro expansions.
#
# Copyright (C) 2008 Free Software Foundation, Inc.
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
# Written by Eric Blake.
#

# In M4 1.4.x, every byte of $@ is rescanned.  This means that an
# algorithm on n arguments that recurses with one less argument each
# iteration will scan n * (n + 1) / 2 arguments, for O(n^2) time.  In
# M4 1.6, this was fixed so that $@ is only scanned once, then
# back-references are made to information stored about the scan.
# Thus, n iterations need only scan n arguments, for O(n) time.
# Additionally, in M4 1.4.x, recursive algorithms did not clean up
# memory very well, requiring O(n^2) memory rather than O(n) for n
# iterations.
#
# This file is designed to overcome the quadratic nature of $@
# recursion by writing a variant of m4_foreach that uses m4_for rather
# than $@ recursion to operate on the list.  This involves more macro
# expansions, but avoids the need to rescan a quadratic number of
# arguments, making these replacements very attractive for M4 1.4.x.
# On the other hand, in any version of M4, expanding additional macros
# costs additional time; therefore, in M4 1.6, where $@ recursion uses
# fewer macros, these replacements actually pessimize performance.
# Additionally, the use of $10 to mean the tenth argument violates
# POSIX; although all versions of m4 1.4.x support this meaning, a
# future m4 version may switch to take it as the first argument
# concatenated with a literal 0, so the implementations in this file
# are not future-proof.  Thus, this file is conditionally included as
# part of m4_init(), only when it is detected that M4 probably has
# quadratic behavior (ie. it lacks the macro __m4_version__).
#
# Please keep this file in sync with m4sugar.m4.

# m4_foreach(VARIABLE, LIST, EXPRESSION)
# --------------------------------------
# Expand EXPRESSION assigning each value of the LIST to VARIABLE.
# LIST should have the form `item_1, item_2, ..., item_n', i.e. the
# whole list must *quoted*.  Quote members too if you don't want them
# to be expanded.
#
# This version minimizes the number of times that $@ is evaluated by
# using m4_for to generate a boilerplate into VARIABLE then passing $@
# to that temporary macro.  Thus, the recursion is done in m4_for
# without reparsing any user input, and is not quadratic.  For an idea
# of how this works, note that m4_foreach(i,[1,2],[i]) defines i to be
#   m4_define([$1],[$3])$2[]m4_define([$1],[$4])$2[]m4_popdef([i])
# then calls i([i],[i],[1],[2]).
m4_define([m4_foreach],
[m4_if([$2], [], [], [_$0([$1], [$3], $2)])])

m4_define([_m4_foreach],
[m4_define([$1], m4_pushdef([$1])_m4_for([$1], [3], [$#], [1],
    [$0_([1], [2], _m4_defn([$1]))])[m4_popdef([$1])])m4_indir([$1], $@)])

m4_define([_m4_foreach_],
[[m4_define([$$1], [$$3])$$2[]]])

# m4_case(SWITCH, VAL1, IF-VAL1, VAL2, IF-VAL2, ..., DEFAULT)
# -----------------------------------------------------------
# Find the first VAL that SWITCH matches, and expand the corresponding
# IF-VAL.  If there are no matches, expand DEFAULT.
#
# Use m4_for to create a temporary macro in terms of a boilerplate
# m4_if with final cleanup.  If $# is even, we have DEFAULT; if it is
# odd, then rounding the last $# up in the temporary macro is
# harmless.  For example, both m4_case(1,2,3,4,5) and
# m4_case(1,2,3,4,5,6) result in the intermediate _m4_case being
#   m4_if([$1],[$2],[$3],[$1],[$4],[$5],_m4_popdef([_m4_case])[$6])
m4_define([m4_case],
[m4_if(m4_eval([$# <= 2]), [1], [$2],
[m4_pushdef([_$0], [m4_if(]m4_for([_m4_count], [2], m4_decr([$#]), [2],
     [_$0_([1], _m4_count, m4_incr(_m4_count))])[_m4_popdef(
	 [_$0])]m4_dquote($m4_eval([($# + 1) & ~1]))[)])_$0($@)])])

m4_define([_m4_case_],
[[[$$1],[$$2],[$$3],]])

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
# We build the temporary macro _m4_b:
#   m4_define([_m4_b], _m4_defn([_m4_bmatch]))_m4_b([$1], [$2], [$3])...
#   _m4_b([$1], [$m-1], [$m])_m4_b([], [], [$m+1]_m4_popdef([_m4_b]))
# then invoke m4_unquote(_m4_b($@)), for concatenation with later text.
m4_define([m4_bmatch],
[m4_if([$#], 0, [m4_fatal([$0: too few arguments: $#])],
       [$#], 1, [m4_fatal([$0: too few arguments: $#: $1])],
       [$#], 2, [$2],
       [m4_define([_m4_b], m4_pushdef([_m4_b])[m4_define([_m4_b],
  _m4_defn([_$0]))]_m4_for([_m4_b], [3], m4_eval([($# + 1) / 2 * 2 - 1]),
  [2], [_$0_([1], m4_decr(_m4_b), _m4_b)])[_m4_b([], [],]m4_dquote(
  [$]m4_incr(_m4_b))[_m4_popdef([_m4_b]))])m4_unquote(_m4_b($@))])])

m4_define([_m4_bmatch],
[m4_if(m4_bregexp([$1], [$2]), [-1], [], [[$3]m4_define([$0])])])

m4_define([_m4_bmatch_],
[[_m4_b([$$1], [$$2], [$$3])]])


# m4_cond(TEST1, VAL1, IF-VAL1, TEST2, VAL2, IF-VAL2, ..., [DEFAULT])
# -------------------------------------------------------------------
# Similar to m4_if, except that each TEST is expanded when encountered.
# If the expansion of TESTn matches the string VALn, the result is IF-VALn.
# The result is DEFAULT if no tests passed.  This macro allows
# short-circuiting of expensive tests, where it pays to arrange quick
# filter tests to run first.
#
# m4_cond already guarantees either 3*n or 3*n + 1 arguments, 1 <= n.
# We only have to speed up _m4_cond, by building the temporary _m4_c:
#   m4_define([_m4_c], _m4_defn([m4_unquote]))_m4_c([m4_if(($1), [($2)],
#   [[$3]m4_define([_m4_c])])])_m4_c([m4_if(($4), [($5)],
#   [[$6]m4_define([_m4_c])])])..._m4_c([m4_if(($m-2), [($m-1)],
#   [[$m]m4_define([_m4_c])])])_m4_c([[$m+1]]_m4_popdef([_m4_c]))
# We invoke m4_unquote(_m4_c($@)), for concatenation with later text.
m4_define([_m4_cond],
[m4_define([_m4_c], m4_pushdef([_m4_c])[m4_define([_m4_c],
  _m4_defn([m4_unquote]))]_m4_for([_m4_c], [2], m4_eval([$# / 3 * 3 - 1]), [3],
  [$0_(m4_decr(_m4_c), _m4_c, m4_incr(_m4_c))])[_m4_c(]m4_dquote(m4_dquote(
  [$]m4_eval([$# / 3 * 3 + 1])))[_m4_popdef([_m4_c]))])m4_unquote(_m4_c($@))])

m4_define([_m4_cond_],
[[_m4_c([m4_if(($$1), [($$2)], [[$$3]m4_define([_m4_c])])])]])

# m4_bpatsubsts(STRING, RE1, SUBST1, RE2, SUBST2, ...)
# ----------------------------------------------------
# m4 equivalent of
#
#   $_ = STRING;
#   s/RE1/SUBST1/g;
#   s/RE2/SUBST2/g;
#   ...
#
# m4_bpatsubsts already validated an odd number of arguments; we only
# need to speed up _m4_bpatsubsts.  To avoid nesting, we build the
# temporary _m4_p:
#   m4_define([_m4_p], [$1])m4_define([_m4_p],
#   m4_bpatsubst(m4_dquote(_m4_defn([_m4_p])), [$2], [$3]))m4_define([_m4_p],
#   m4_bpatsubst(m4_dquote(_m4_defn([_m4_p])), [$4], [$5]))m4_define([_m4_p],...
#   m4_bpatsubst(m4_dquote(_m4_defn([_m4_p])), [$m-1], [$m]))m4_unquote(
#   _m4_defn([_m4_p])_m4_popdef([_m4_p]))
m4_define([_m4_bpatsubsts],
[m4_define([_m4_p], m4_pushdef([_m4_p])[m4_define([_m4_p],
  ]m4_dquote([$]1)[)]_m4_for([_m4_p], [3], [$#], [2], [$0_(m4_decr(_m4_p),
  _m4_p)])[m4_unquote(_m4_defn([_m4_p])_m4_popdef([_m4_p]))])_m4_p($@)])

m4_define([_m4_bpatsubsts_],
[[m4_define([_m4_p],
m4_bpatsubst(m4_dquote(_m4_defn([_m4_p])), [$$1], [$$2]))]])

# m4_shiftn(N, ...)
# -----------------
# Returns ... shifted N times.  Useful for recursive "varargs" constructs.
#
# m4_shiftn already validated arguments; we only need to speed up
# _m4_shiftn.  If N is 3, then we build the temporary _m4_s, defined as
#   ,[$5],[$6],...,[$m]_m4_popdef([_m4_s])
# before calling m4_shift(_m4_s($@)).
m4_define([_m4_shiftn],
[m4_if(m4_incr([$1]), [$#], [], [m4_define([_m4_s],
	   m4_pushdef([_m4_s])_m4_for([_m4_s], m4_eval([$1 + 2]), [$#], [1],
  [[,]m4_dquote([$]_m4_s)])[_m4_popdef([_m4_s])])m4_shift(_m4_s($@))])])

# m4_do(STRING, ...)
# ------------------
# This macro invokes all its arguments (in sequence, of course).  It is
# useful for making your macros more structured and readable by dropping
# unnecessary dnl's and have the macros indented properly.
#
# Here, we use the temporary macro _m4_do, defined as
#   $1[]$2[]...[]$n[]_m4_popdef([_m4_do])
m4_define([m4_do],
[m4_if([$#], [0], [],
       [m4_define([_$0], m4_pushdef([_$0])_m4_for([_$0], [1], [$#], [1],
		  [$_$0[[]]])[_m4_popdef([_$0])])_$0($@)])])

# m4_dquote_elt(ARGS)
# -------------------
# Return ARGS as an unquoted list of double-quoted arguments.
#
# m4_foreach to the rescue.  It's easier to shift off the leading comma.
m4_define([m4_dquote_elt],
[m4_shift(m4_foreach([_m4_elt], [$@], [,m4_dquote(_m4_defn([_m4_elt]))]))])

# m4_reverse(ARGS)
# ----------------
# Output ARGS in reverse order.
#
# Invoke _m4_r($@) with the temporary _m4_r built as
#   [$m], [$m-1], ..., [$2], [$1]_m4_popdef([_m4_r])
m4_define([m4_reverse],
[m4_if([$#], [0], [], [$#], [1], [[$1]],
[m4_define([_m4_r], m4_dquote([$$#])m4_pushdef([_m4_r])_m4_for([_m4_r],
      m4_decr([$#]), [1], [-1],
    [[, ]m4_dquote([$]_m4_r)])[_m4_popdef([_m4_r])])_m4_r($@)])])


# m4_map(MACRO, LIST)
# -------------------
# Invoke MACRO($1), MACRO($2) etc. where $1, $2... are the elements
# of LIST.  $1, $2... must in turn be lists, appropriate for m4_apply.
#
# m4_map/m4_map_sep only execute once; the speedup comes in fixing
# _m4_map.  The mismatch in () is intentional, since $1 supplies the
# opening `(' (but it sure looks odd!).  Build the temporary _m4_m:
#   $1, [$3])$1, [$4])...$1, [$m])_m4_popdef([_m4_m])
m4_define([_m4_map],
[m4_if([$#], [2], [],
       [m4_define([_m4_m], m4_pushdef([_m4_m])_m4_for([_m4_m], [3], [$#], [1],
   [$0_([1], _m4_m)])[_m4_popdef([_m4_m])])_m4_m($@)])])

m4_define([_m4_map_],
[[$$1, [$$2])]])

# m4_transform(EXPRESSION, ARG...)
# --------------------------------
# Expand EXPRESSION([ARG]) for each argument.  More efficient than
# m4_foreach([var], [ARG...], [EXPRESSION(m4_defn([var]))])
#
# Invoke the temporary macro _m4_transform, defined as:
#   $1([$2])[]$1([$3])[]...$1([$m])[]_m4_popdef([_m4_transform])
m4_define([m4_transform],
[m4_if([$#], [0], [m4_fatal([$0: too few arguments: $#])],
       [$#], [1], [],
       [m4_define([_$0], m4_pushdef([_$0])_m4_for([_$0], [2], [$#], [1],
   [_$0_([1], _$0)])[_m4_popdef([_$0])])_$0($@)])])

m4_define([_m4_transform_],
[[$$1([$$2])[]]])

# m4_transform_pair(EXPRESSION, [END-EXPR = EXPRESSION], ARG...)
# --------------------------------------------------------------
# Perform a pairwise grouping of consecutive ARGs, by expanding
# EXPRESSION([ARG1], [ARG2]).  If there are an odd number of ARGs, the
# final argument is expanded with END-EXPR([ARGn]).
#
# Build the temporary macro _m4_transform_pair, with the $2([$m+1])
# only output if $# is odd:
#   $1([$3], [$4])[]$1([$5], [$6])[]...$1([$m-1],
#   [$m])[]m4_default([$2], [$1])([$m+1])[]_m4_popdef([_m4_transform_pair])
m4_define([m4_transform_pair],
[m4_if([$#], [0], [m4_fatal([$0: too few arguments: $#])],
       [$#], [1], [m4_fatal([$0: too few arguments: $#: $1])],
       [$#], [2], [],
       [$#], [3], [m4_default([$2], [$1])([$3])[]],
       [m4_define([_$0], m4_pushdef([_$0])_m4_for([_$0], [3],
   m4_eval([$# / 2 * 2 - 1]), [2], [_$0_([1], _$0, m4_incr(_$0))])_$0_end(
   [1], [2], [$#])[_m4_popdef([_$0])])_$0($@)])])

m4_define([_m4_transform_pair_],
[[$$1([$$2], [$$3])[]]])

m4_define([_m4_transform_pair_end],
[m4_if(m4_eval([$3 & 1]), [1], [[m4_default([$$2], [$$1])([$$3])[]]])])

# m4_join(SEP, ARG1, ARG2...)
# ---------------------------
# Produce ARG1SEPARG2...SEPARGn.  Avoid back-to-back SEP when a given ARG
# is the empty string.  No expansion is performed on SEP or ARGs.
#
# Use a self-modifying separator, since we don't know how many
# arguments might be skipped before a separator is first printed, but
# be careful if the separator contains $.  m4_foreach to the rescue.
m4_define([m4_join],
[m4_pushdef([_m4_sep], [m4_define([_m4_sep], _m4_defn([m4_echo]))])]dnl
[m4_foreach([_m4_arg], [m4_shift($@)],
	    [m4_ifset([_m4_arg], [_m4_sep([$1])_m4_defn([_m4_arg])])])]dnl
[_m4_popdef([_m4_sep])])

# m4_joinall(SEP, ARG1, ARG2...)
# ------------------------------
# Produce ARG1SEPARG2...SEPARGn.  An empty ARG results in back-to-back SEP.
# No expansion is performed on SEP or ARGs.
#
# A bit easier than m4_join.  m4_foreach to the rescue.
m4_define([m4_joinall],
[[$2]m4_if(m4_eval([$# <= 2]), [1], [],
	   [m4_foreach([_m4_arg], [m4_shift2($@)],
		       [[$1]_m4_defn([_m4_arg])])])])

# m4_list_cmp(A, B)
# -----------------
# Compare the two lists of integer expressions A and B.
#
# m4_list_cmp takes care of any side effects; we only override
# _m4_list_cmp_raw, where we can safely expand lists multiple times.
# First, insert padding so that both lists are the same length; the
# trailing +0 is necessary to handle a missing list.  Next, create a
# temporary macro to perform pairwise comparisons until an inequality
# is found.  For example, m4_list_cmp([1], [1,2]) creates _m4_cmp as
#   m4_if(m4_eval([($1) != ($3)]), [1], [m4_cmp([$1], [$3])],
#         m4_eval([($2) != ($4)]), [1], [m4_cmp([$2], [$4])],
#         [0]_m4_popdef([_m4_cmp], [_m4_size]))
# then calls _m4_cmp([1+0], [0], [1], [2+0])
m4_define([_m4_list_cmp_raw],
[m4_if([$1], [$2], 0, [m4_pushdef(
     [_m4_size])_m4_list_cmp($1+0_m4_list_pad(m4_count($1), m4_count($2)),
			     $2+0_m4_list_pad(m4_count($2), m4_count($1)))])])

m4_define([_m4_list_pad],
[m4_if(m4_eval($1 < $2), [1],
       [_m4_for([_m4_size], m4_incr([$1]), [$2], [1], [,0])])])

m4_define([_m4_list_cmp],
[m4_define([_m4_size], m4_eval([$# >> 1]))]dnl
[m4_define([_m4_cmp], m4_pushdef([_m4_cmp])[m4_if(]_m4_for([_m4_cmp],
   [1], _m4_size, [1], [$0_(_m4_cmp, m4_eval(_m4_cmp + _m4_size))])[
      [0]_m4_popdef([_m4_cmp], [_m4_size]))])_m4_cmp($@)])

m4_define([_m4_list_cmp_],
[[m4_eval([($$1) != ($$2)]), [1], [m4_cmp([$$1], [$$2])],
]])

# m4_max(EXPR, ...)
# m4_min(EXPR, ...)
# -----------------
# Return the decimal value of the maximum (or minimum) in a series of
# integer expressions.
#
# m4_foreach to the rescue; we only need to replace _m4_minmax.  Here,
# we need a temporary macro to track the best answer so far, so that
# the foreach expression is tractable.
m4_define([_m4_minmax],
[m4_pushdef([_m4_best], m4_eval([$2]))m4_foreach([_m4_arg], [m4_shift2($@)],
    [m4_define([_m4_best], $1(_m4_best, _m4_defn([_m4_arg])))])]dnl
[_m4_best[]_m4_popdef([_m4_best])])

# m4_set_add_all(SET, VALUE...)
# -----------------------------
# Add each VALUE into SET.  This is O(n) in the number of VALUEs, and
# can be faster than calling m4_set_add for each VALUE.
#
# m4_foreach to the rescue.  If no deletions have occurred, then avoid
# the speed penalty of m4_set_add.
m4_define([m4_set_add_all],
[m4_if([$#], [0], [], [$#], [1], [],
       [m4_define([_m4_set_size($1)], m4_eval(m4_set_size([$1])
	  + m4_len(m4_foreach([_m4_arg], [m4_shift($@)],
    m4_ifdef([_m4_set_cleanup($1)],
	     [[m4_set_add([$1], _m4_defn([_m4_arg]))]],
	     [[m4_ifdef([_m4_set([$1],]_m4_defn([_m4_arg])[)], [],
			[m4_define([_m4_set([$1],]_m4_defn([_m4_arg])[)],
				   [1])m4_pushdef([_m4_set([$1])],
	_m4_defn([_m4_arg]))-])]])))))])])
