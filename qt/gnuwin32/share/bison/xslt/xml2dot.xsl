<?xml version="1.0" encoding="UTF-8"?>

<!--
    xml2dot.xsl - transform Bison XML Report into DOT.

    Copyright (C) 2007, 2008 Free Software Foundation, Inc.

    This file is part of Bison, the GNU Compiler Compiler.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    Written by Wojciech Polak <polak@gnu.org>.
  -->

<xsl:stylesheet version="1.0"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:bison="http://www.gnu.org/software/bison/">

<xsl:import href="bison.xsl"/>
<xsl:output method="text" encoding="UTF-8" indent="no"/>

<xsl:template match="/">
  <xsl:apply-templates select="bison-xml-report"/>
</xsl:template>

<xsl:template match="bison-xml-report">
  <xsl:apply-templates select="automaton"/>
</xsl:template>

<xsl:template match="automaton">
  <xsl:text>digraph Automaton {&#10;</xsl:text>
  <xsl:apply-templates select="state"/>
  <xsl:text>}&#10;</xsl:text>
</xsl:template>

<xsl:template match="automaton/state">
  <xsl:call-template name="output-node">
    <xsl:with-param name="number" select="@number"/>
    <xsl:with-param name="label">
      <xsl:value-of select="@number"/>
      <xsl:apply-templates select="itemset/item"/>
    </xsl:with-param>
  </xsl:call-template>
  <xsl:apply-templates select="actions/transitions"/>
</xsl:template>

<xsl:template match="actions/transitions">
  <xsl:apply-templates select="transition"/>
</xsl:template>

<xsl:template match="item">
  <xsl:apply-templates select="key('bison:ruleByNumber', @rule-number)">
    <xsl:with-param name="point" select="@point"/>
  </xsl:apply-templates>
  <xsl:apply-templates select="lookaheads"/>
</xsl:template>

<xsl:template match="rule">
  <xsl:param name="point"/>
  <xsl:text>&#10;</xsl:text>
  <xsl:value-of select="lhs"/>
  <xsl:text> -&gt;</xsl:text>
  <xsl:if test="$point = 0">
    <xsl:text> .</xsl:text>
  </xsl:if>
  <xsl:for-each select="rhs/symbol|rhs/empty">
    <xsl:apply-templates select="."/>
    <xsl:if test="$point = position()">
      <xsl:text> .</xsl:text>
    </xsl:if>
  </xsl:for-each>
</xsl:template>

<xsl:template match="symbol">
  <xsl:text> </xsl:text>
  <xsl:value-of select="."/>
</xsl:template>

<xsl:template match="empty"/>

<xsl:template match="lookaheads">
  <xsl:text>[</xsl:text>
  <xsl:apply-templates select="symbol"/>
  <xsl:text>]</xsl:text>
</xsl:template>

<xsl:template match="lookaheads/symbol">
  <xsl:value-of select="."/>
  <xsl:if test="position() != last()">
    <xsl:text>, </xsl:text>
  </xsl:if>
</xsl:template>

<xsl:template match="transition">
  <xsl:call-template name="output-edge">
    <xsl:with-param name="src" select="../../../@number"/>
    <xsl:with-param name="dst" select="@state"/>
    <xsl:with-param name="style">
      <xsl:choose>
	<xsl:when test="@symbol = 'error'">
	  <xsl:text>dotted</xsl:text>
	</xsl:when>
	<xsl:when test="@type = 'shift'">
	  <xsl:text>solid</xsl:text>
	</xsl:when>
	<xsl:otherwise>
	  <xsl:text>dashed</xsl:text>
	</xsl:otherwise>
      </xsl:choose>
    </xsl:with-param>
    <xsl:with-param name="label">
      <xsl:if test="not(@symbol = 'error')">
	<xsl:value-of select="@symbol"/>
      </xsl:if>
    </xsl:with-param>
  </xsl:call-template>
</xsl:template>

<xsl:template name="output-node">
  <xsl:param name="number"/>
  <xsl:param name="label"/>
  <xsl:text>  </xsl:text>
  <xsl:value-of select="$number"/>
  <xsl:text> [label="</xsl:text>
  <xsl:call-template name="escape">
    <xsl:with-param name="subject" select="$label"/>
  </xsl:call-template>
  <xsl:text>"]&#10;</xsl:text>
</xsl:template>

<xsl:template name="output-edge">
  <xsl:param name="src"/>
  <xsl:param name="dst"/>
  <xsl:param name="style"/>
  <xsl:param name="label"/>
  <xsl:text>  </xsl:text>
  <xsl:value-of select="$src"/>
  <xsl:text> -> </xsl:text>
  <xsl:value-of select="$dst"/>
  <xsl:text> [style=</xsl:text>
  <xsl:value-of select="$style"/>
  <xsl:if test="$label and $label != ''">
    <xsl:text> label="</xsl:text>
    <xsl:call-template name="escape">
      <xsl:with-param name="subject" select="$label"/>
    </xsl:call-template>
    <xsl:text>"</xsl:text>
  </xsl:if>
  <xsl:text>]&#10;</xsl:text>
</xsl:template>

<xsl:template name="escape">
  <xsl:param name="subject"/> <!-- required -->
  <xsl:call-template name="string-replace">
    <xsl:with-param name="subject">
      <xsl:call-template name="string-replace">
        <xsl:with-param name="subject">
          <xsl:call-template name="string-replace">
            <xsl:with-param name="subject" select="$subject"/>
            <xsl:with-param name="search" select="'\'"/>
            <xsl:with-param name="replace" select="'\\'"/>
          </xsl:call-template>
        </xsl:with-param>
        <xsl:with-param name="search" select="'&quot;'"/>
        <xsl:with-param name="replace" select="'\&quot;'"/>
      </xsl:call-template>
    </xsl:with-param>
    <xsl:with-param name="search" select="'&#10;'"/>
    <xsl:with-param name="replace" select="'\n'"/>
  </xsl:call-template>
</xsl:template>

<xsl:template name="string-replace">
  <xsl:param name="subject"/>
  <xsl:param name="search"/>
  <xsl:param name="replace"/>
  <xsl:choose>
    <xsl:when test="contains($subject, $search)">
      <xsl:variable name="before" select="substring-before($subject, $search)"/>
      <xsl:variable name="after" select="substring-after($subject, $search)"/>
      <xsl:value-of select="$before"/>
      <xsl:value-of select="$replace"/>
      <xsl:call-template name="string-replace">
        <xsl:with-param name="subject" select="$after"/>
        <xsl:with-param name="search" select="$search"/>
        <xsl:with-param name="replace" select="$replace"/>
      </xsl:call-template>
    </xsl:when> 
    <xsl:otherwise>
      <xsl:value-of select="$subject"/>  
    </xsl:otherwise>
  </xsl:choose>            
</xsl:template>

</xsl:stylesheet>
