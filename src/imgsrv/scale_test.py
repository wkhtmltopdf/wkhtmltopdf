#!/usr/bin/env python2.7
"""
usage: scale_test.py [-h] [--concurrency CONCURRENCY] [--no-delay] --http IMAGE_SERVER


optional arguments:
  -h, --help            show this help message and exit
  --concurrency CONCURRENCY
                        Number of request threads to run
  --no-delay            Do not wait between requests
  --http IMAGE_SERVER
                        https image server(s) / balancer(s)
"""

import json
import time

# Gevent
import gevent
import gevent.monkey
import gevent.pool
gevent.monkey.patch_all()

import argparse
import datetime
import os
import random
import re
import requests
import simplejson
import simplerison
import sys
import time
import urllib2
from urlparse import urlparse

import argparse
from random import choice

import datetime
import random
import string


HTML0 = """
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">

<html>
<head>
	<title>html fonts</title></title>
</head>

<body>

<center><font face="Impact" size="9"  color="green">Common HTML Font Samples
<br>
<font face="Impact" size="6"  color="green"><a href="javascript:self.print();">>>>Print This Page</font></a>

</center>
	 
<br> 
<table align="center" cellspacing="5" cellpadding="5" border="2">

<tr>
	<th><font face="Arial, Futura" size="6"  color="green">Font Style</font></th>
	<th><font face="Arial, Futura" size="6"  color="green">Font Sample</font></th>
	<th><font face="Arial, Futura" size="6"  color="green">Code (less < >)</font></th>
</tr>
<tr>
    <td><font face="Verdana,Arial,Helvetica" size="4" color="black">Verdana, Arial, Helvetica - size=4</font></td>
    <td><font face="Verdana,Arial,Helvetica" size="4"  color="black">Sample</font></td>
	<td><font face="Times New Roman" size=3" color="black">font face="Verdana,Arial,Helvetica size="4" color="black"</font></td>
</tr>
    <td><font face="Arial" size="4"  color="black">Arial - size=4</font?</td>
    <td><font face="Arial" size="4"  color="green">Sample</font></td>
	<td><font face="Times New Roman" size=3" color="black">font face="Arial" size="4" color="green"</font></td>
</tr>
<tr>
    <td><font face="Impact" size="9">Impact - size=9</font></td>
    <td><font face="Impact" size="9" color="red">Sample</font></td>
	<td><font face="Times New Roman" size="3" color="black">font face="Impact" size="9" color="red"</font></td>
</tr>
<tr>
    <td><font face="Courier" size="4">Courier - size=4</font></td>
    <td><font face="Courier" size="4" color="blue">Sample</font></td>
	<td><font face="Times New Roman" size="3" color="black">font face="Courier" size="4" color="blue"</font></td>
</tr>
<tr>
    <td><font ace="Courier New" size="4">Courier New - size=4</font></td>
    <td><font faceace="Courier New" size="4" color="blue">Sample</font></td>
	<td><font face="Times New Roman" size="3" color="black">font face="Courier New" size="4"  color="blue"</font></td>
</tr>
<tr>
    <td><font face="Book Antigua" size="6">Book Antigua - size=6</font></td>
    <td><font face="Book Antigua" size="6">Sample</font></td>
	<td><font face="Times New Roman" size="3">font face="Book Antigua" size="6"</font></td>
</tr>
<tr>
    <td><font face="Bookman Old Style" size="4">Bookman Old Style - size=3</font></td>
    <td><font face="Bookman Old Style" size="4"  color="orange">Sample</font></td>
	<td><font face="Times New Roman" size="3">font face="Bookman Old Style" font size="3" color="orange"</font></td>
</tr>
<tr>
    <td><font face="Century Gothic" size="6">Century Gothic - size=6</font></td>
    <td><font face="Century Gothic" size="6" color="purple">Sample</font></td>
	<td><font face="Times New Roman" font size="3">font face="Century Gothic" size="6" color="purple"</td>
</tr>
<tr>
    <td><font face="Times New Roman" size="4"  color="black">Times New Roman - size=4</font></td>
    <td><font face="Times New Roman" size="4"  color="black">Sample</font></td>
	<td><font face="Times New Roman" size="3"  color="black">font face="Times New Roman" font size="4"  color="black"</font></td>
</tr>
<tr>
    <td><font face="Times New Roman" size="4" color="black">Times New Roman italicized - size=4</font></td>
    <td><font face="Times New Roman" size="4" color="black"><i>Sample</i></font></td>
	<td><font face="Times New Roman" size="3" color="black">font face="Times New Roman" font size="4" color="black"</font></td>
</tr>
<tr>
    <td><font face="Times New Roman" size="4" color="black">Times New Roman italicized & bold - size=4</font></td>
    <td><font face="Times New Roman" size="4" color="black"><i><b>Sample</b></i></font></td>
	<td><font face="Times New Roman" size="3" color="black">font face="Times New Roman" size="4" color="black"</font></td>
</tr>
<tr>
    <td><font face="Modern" size="5" color="black">Modern - size=5</font></td>
    <td><font face="Modern" size="5" color="black">Sample</font></td>
	<td><font face="Times New Roman" size="3" color="black">font face="Modern" size="5" color="black"</font></td>
</tr>
<tr>
    <td><font face="Modern,Arial" size="5" color="black">Modern,Arial - size=5</font></td>
    <td><font face="Modern,Arial" size="5" color="blue">Sample</font></td>
	<td><font face="Times New Roman" size="3" color="black">font face="Modern,Arial" size="5" color="blue</font>"</td>
</tr>
</table> 
<br>
<center><font size="3" Face="Modern,Arial" color="green">Compiled January 17, 2003</font></center>

</body>
</html>
"""


HTML1 = """
<html>
    <head>
            <style>
                        @font-face {
                                        font-family: "font number one";
                                        src: url("file:///root/font1.otf");
                                                    }
                                                                @font-face {
                                                                                font-family: "font number two";
                                                                                src: url("file:///root/font2.ttf");
   }
           </style>
               </head>
                   <body>
                   <span style="font-family: 'font number one'; color: blue;">
                               hello world!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                                       </span>
                                           </body>
                                           </html>
                                           """

HTML2 = """
                                                                                <html>
                                                                                    <head>
   <style>
               @font-face {
                               font-family: "font number one";
                               src: url("file:///root/font1.otf");
                                           }
                                                       @font-face {
                                                                       font-family: "font number two";
                                                                       src: url("file:///root/font2.ttf");
                                                                                   }
  </style>
      </head>
          <body>
          <span style="font-family: 'font number two'; color: red;">
                      HELLO MARS
                              </span>
                                  </body>
                                  </html>
                                  """
                                                                       

HTML4 = """
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html>
<head>
<title>HTML Fonts</title>
<link rel="shortcut icon" href="/favicon.ico" type="image/x-icon" />
<meta name="Keywords" content="HTML, Tutorials, Learning, Beginners, HTML Basics, Hyper Text Markup Language, HTML Tutorial, HTML tutorials," />
<meta name="description" content="HTML Fonts -  Learning HTML in simple and easy steps. Its a beginner's tutorial containing complete knowledge of HTML and how to begin HTML coding to build up your own webpage." />
<meta http-equiv="pragma" content="no-cache" />
<meta http-equiv="cache-control" content="no-cache" />
<base href="http://www.tutorialspoint.com" />
<link rel="stylesheet" type="text/css" href="tp.css" />
<script src="http://www.google-analytics.com/urchin.js" type="text/javascript">
</script>
<script type="text/javascript">
_uacct = "UA-232293-6";
urchinTracker();
</script>
</head>
<body>
<center>
<a name="top"></a>
<table border="0" class="main" cellpadding="2" cellspacing="0">
<tr>
<td class="content" valign="top" align="left">
<a href="index.htm"><img src="images/tp-logo.gif" border="0"  alt="Tutorials Point" /></a>
<br />
<br />
<div  class="search">
<!-- SiteSearch Google -->
<form method="get" action="http://www.google.com/custom" target="google_window">
<table border="0">
<tr><td nowrap="nowrap" valign="top" align="center" height="32">
<a href="http://www.google.com/">
<img src="http://www.google.com/logos/Logo_25wht.gif" border="0" alt="Google" align="middle"></img></a>
</td>
</tr>
<tr>
<td nowrap="nowrap">
<input type="hidden" name="domains" value="www.tutorialspoint.com"></input>
<input type="text" name="q" size="20" maxlength="255" value=""></input>
<input type="submit" name="sa" style="width:20px;font-weight:bold" value="..."></input>
</td></tr>
<tr>
<td nowrap="nowrap">
<table>
<tr>
<td>
<input type="radio" name="sitesearch" value="" checked="checked"></input>
<font size="-1" color="#000000">Web</font>
</td>
<td>
<input type="radio" name="sitesearch" value="www.tutorialspoint.com"></input>
<font size="-1" color="#000000">This Site</font>
</td>
</tr>
</table>
<input type="hidden" name="client" value="pub-7133395778201029"></input>
<input type="hidden" name="forid" value="1"></input>
<input type="hidden" name="ie" value="ISO-8859-1"></input>
<input type="hidden" name="oe" value="ISO-8859-1"></input>
<input type="hidden" name="cof" value="GALT:#008000;GL:1;DIV:#336699;VLC:663399;AH:center;BGC:FFFFFF;LBGC:336699;ALC:0000FF;LC:0000FF;T:000000;GFNT:0000FF;GIMP:0000FF;FORID:1"></input>
<input type="hidden" name="hl" value="en"></input>
</td></tr></table>
</form>
<!-- SiteSearch Google -->
</div>
<br />
&nbsp;&nbsp;<b>HTML Tutorial</b>
<div  class="left">
<ul class="menu">
<li><a class="left" target="_top" href="/html/index.htm" >HTML Home</a></li>
<li><a class="left" target="_top" href="/html/what_is_html.htm" >HTML Introduction</a></li>
<li><a class="left" target="_top" href="/html/html_basic_tags.htm" >HTML Basic Tags</a></li>
<li><a class="left" target="_top" href="/html/html_meta_tags.htm" >HTML Meta Tags</a></li>
<li><a class="left" target="_top" href="/html/html_attributes.htm" >HTML Attributes</a></li>
<li><a class="left" target="_top" href="/html/html_formatting.htm" >HTML Formatting</a></li>
<li><a class="left" target="_top" href="/html/html_phrase_elements.htm" >HTML Phrase Tags</a></li>
<li><a class="left" target="_top" href="/html/html_comments.htm" >HTML Comments</a></li>
<li><a class="left" target="_top" href="/html/html_fonts.htm" ><b>HTML Fonts</b></a></li>
<li><a class="left" target="_top" href="/html/html_marquees.htm" >HTML Marquees</a></li>
<li><a class="left" target="_top" href="/html/html_images.htm" >HTML Images</a></li>
<li><a class="left" target="_top" href="/html/html_text_links.htm" >HTML Text Links</a></li>
<li><a class="left" target="_top" href="/html/html_image_links.htm" >HTML Image Links</a></li>
<li><a class="left" target="_top" href="/html/html_email_links.htm" >HTML Email Links</a></li>
<li><a class="left" target="_top" href="/html/html_tables.htm" >HTML Tables</a></li>
<li><a class="left" target="_top" href="/html/html_frames.htm" >HTML Frames</a></li>
<li><a class="left" target="_top" href="/html/html_lists.htm" >HTML Lists</a></li>
<li><a class="left" target="_top" href="/html/html_layouts.htm" >HTML Layouts</a></li>
<li><a class="left" target="_top" href="/html/html_colors.htm" >HTML Colors</a></li>
<li><a class="left" target="_top" href="/html/html_forms.htm" >HTML Forms</a></li>
<li><a class="left" target="_top" href="/html/html_backgrounds.htm" >HTML Backgrounds</a></li>
<li><a class="left" target="_top" href="/html/html_embed_multimedia.htm" >HTML Embed Multimedia</a></li>
<li><a class="left" target="_top" href="/html/html_styles.htm" >HTML Styles</a></li>
<li><a class="left" target="_top" href="/html/html_scripts.htm" >HTML Scripts</a></li>
</ul>
</div>
<br />
&nbsp;&nbsp;<b>HTML References</b>
<div  class="left">
<ul class="menu">
<li><a class="left" target="_top" href="/html/html_quick_guide.htm" >HTML Quick Guide</a></li>
<li><a class="left" target="_top" href="/html/html_tags_ref.htm" >HTML Tags Ref</a></li>
<li><a class="left" target="_top" href="/html/html_deprecated_tags.htm" >HTML Deprecated Tags</a></li>
<li><a class="left" target="_top" href="/html/what_is_new_in_html4.htm" >New in HTML 4.0</a></li>
<li><a class="left" target="_top" href="/html/html_ascii_codes.htm" >HTML ASCII Codes</a></li>
<li><a class="left" target="_top" href="/html/html_attributes_ref.htm" >HTML Attributes Ref</a></li>
<li><a class="left" target="_top" href="/html/html_character_encodings.htm" >HTML Character Encodings</a></li>
<li><a class="left" target="_top" href="/html/html_color_names.htm" >HTML Color Names</a></li>
<li><a class="left" target="_top" href="/html/html_entities.htm" >HTML Entities</a></li>
<li><a class="left" target="_top" href="/html/html_fonts_ref.htm" >HTML Fonts Ref</a></li>
<li><a class="left" target="_top" href="/html/html_events_ref.htm" >HTML Events Ref </a></li>
<li><a class="left" target="_top" href="/html/mime_media_types.htm" >MIME Media Types</a></li>
<li><a class="left" target="_top" href="/html/html_url_encoding.htm" >HTML URL Encoding</a></li>
<li><a class="left" target="_top" href="/html/ascii_table_lookup.htm">ASCII Table Lookup</a></li>
<li><a class="left" target="_top" href="/html/language_iso_codes.htm" >Language Codes</a></li>
</ul>
</div>
<br />
&nbsp;&nbsp;<b>HTML Tools</b>
<div  class="left">
<ul class="menu">
<li><a class="left" target="_top" href="/html/html_color_code_builder.htm" >Color Code Builder</a></li>
<li><a class="left" target="_top" href="/html/html_editor.htm" >HTML Online Editor</a></li>
</ul>
</div>
<br />
&nbsp;&nbsp;<b>HTML Resources</b>
<div  class="left">
<ul class="menu">
<li><a class="left" target="_top" href="/html/html_useful_resources.htm" >HTML Useful Resources</a></li>
</ul>
</div>
<br />
&nbsp;&nbsp;<b>Selected Reading</b>
<div  class="left">
<ul class="menu">
<li><a class="left" target="_top" href="computer_glossary.htm">Computer Glossary</a></li>
<li><a class="left" target="_top" href="computer_whoiswho.htm">Who is Who</a></li>
</ul>
</div>
<br /><br />
<p>Copyright &copy; 2014 by tutorialspoint</p>
</td>
<td valign="top" width="100%">
<br /><br />
<table cellpadding="0" cellspacing="0">
<tr valign="middle">
<td  style="width:20px;"></td>
<td  class="top">
&nbsp;&nbsp;<a class="top" href="index.htm">Home</a>&nbsp;&nbsp;
</td>
<td  style="width:20px;"></td>
<td  class="top">
&nbsp;&nbsp;<a class="top" href="references.htm">References</a>&nbsp;&nbsp;
</td>
<td  style="width:20px;"></td>
<td  class="top">
&nbsp;&nbsp;<a class="top" href="/forums/index.php">Discussion Forums</a>&nbsp;&nbsp;
</td>
<td  style="width:20px;"></td>
<td  class="top">
&nbsp;&nbsp;<a class="top" href="/about/index.htm">About TP</a>&nbsp;&nbsp;
</td>
</tr>
</table>
<table class="middle" cellpadding="5" cellspacing="0">
<tr>
<td>
<!-- PRINTING STARTS HERE -->
<h1>HTML Fonts</h1>
<hr />
<a href="/html/html_comments.htm"><img alt="previous" border="0" src="/images/previous.gif" /></a>
<a href="/html/html_marquees.htm"><img alt="next" border="0" src="/images/next.gif" /></a>
<!-- AddThis Bookmark Button BEGIN -->
<a href="http://www.addthis.com/bookmark.php" rel="nofollow" onclick="addthis_url   = location.href; addthis_title = document.title; return addthis_click(this);" target="_blank"><img src="/images/add-this.gif" border="0" alt="AddThis Social Bookmark Button" /></a> <script type="text/javascript">var addthis_pub = 'tutorialspoint';</script><script type="text/javascript" src="http://s9.addthis.com/js/widget.php?v=10"></script>
<!-- AddThis Bookmark Button END -->
<br />
<br />
<hr/><div style="padding-bottom:5px;padding-left:10px;">Advertisements</div>
<hr />
<p>Font face and color depends entirely on the computer and browser that is being used to view your page. But the &lt;font&gt; tag is used to add style, size, and color to the text on your site. You can use a &lt;basefont&gt; tag to set all of your text to the same size, face, and color.</p>
<p> The font tag is having three attributes called size, color, and face to customize your fonts.</p>
<p>To change any of the font attributes at any time within your page, simply use the &lt;font&gt; tag. The text that follows will remain changed until you close with the &lt;/font&gt; tag. You can change any or all of the font attributes at the one time, by including all the required changes within the one &lt;font&gt; tag.</p>
<p><b>NOTE: The font and basefont tags are deprecated and it is supposed to be removed in a future version of HTML. So it should not be used. Its is suggested to use css styles to manipulate your font.</b></p>
<h1>Font Size:</h1>
<p>You can set the size of your font with size attribute. The range of accepted values is from 1(smallest) to 7(largest). The default size of a font is 3.</p>
<p>Example:</p>
<table class="src" cellpadding="5"><tr><td><pre>
&lt;font size="1"&gt;Font size="1"&lt;/font&gt;
&lt;font size="2"&gt;Font size="2"&lt;/font&gt;
&lt;font size="3"&gt;Font size="3"&lt;/font&gt;
&lt;font size="4"&gt;Font size="4"&lt;/font&gt;
&lt;font size="5"&gt;Font size="5"&lt;/font&gt;
&lt;font size="6"&gt;Font size="6"&lt;/font&gt;
&lt;font size="7"&gt;Font size="7"&lt;/font&gt;
</pre></td></tr></table>
<p>This will produce following result:</p>
<table class="src" cellpadding="5"><tr><td>
<font size="1">Font size="1"</font><br />
<font size="2">Font size="2"</font><br />
<font size="3">Font size="3"</font><br />
<font size="4">Font size="4"</font><br />
<font size="5">Font size="5"</font><br />
<font size="6">Font size="6"</font><br />
<font size="7">Font size="7"</font>
</td></tr></table>
<p><b>SPECIFY THE RELATIVE FONT SIZE. &lt;font size="+n"&gt; or &lt;font size="-n"&gt;:</b><br/>
You can specify how many sizes larger or how many sizes smaller than the preset font size should be.</p>
<p>Example:</p>
<table class="src" cellpadding="5"><tr><td><pre>
&lt;font size="-1"&gt;Font size="-1"&lt;/font&gt;
&lt;font size="+1"&gt;Font size="+1"&lt;/font&gt;
&lt;font size="+2"&gt;Font size="+2"&lt;/font&gt;
&lt;font size="+3"&gt;Font size="+3"&lt;/font&gt;
&lt;font size="+4"&gt;Font size="+4"&lt;/font&gt;
</pre></td></tr></table>
<p>This will produce following result:</p>
<table class="src" cellpadding="5"><tr><td>
<font size="-1">Font size="-1"</font><br />
<font size="+1">Font size="+1"</font><br />
<font size="+2">Font size="+2"</font><br />
<font size="+3">Font size="+3"</font><br />
<font size="+4">Font size="+4"</font>
</td></tr></table>
<h1>Font Face:</h1>
<p>You can set any font you like using <i>face</i> attribute but be aware that if the user viewing the page doesn't have the font installed, they will not be able to see it. Instead they will default to Times New Roman of your font with size attribute. See below few examples on using different font face</p>
<p>Example:</p>
<table class="src" cellpadding="5"><tr><td><pre>
&lt;font face="Times New Roman" size="5"&gt;Times New Roman&lt;/font&gt;
&lt;font face="Verdana" size="5"&gt;Verdana&lt;/font&gt;
&lt;font face="Comic sans MS" size="5"&gt;Comic Sans MS&lt;/font&gt;
&lt;font face="WildWest" size="5"&gt;WildWest&lt;/font&gt;
&lt;font face="Bedrock" size="5"&gt;Bedrock&lt;/font&gt;
</pre></td></tr></table>
<p>This will produce following result:</p>
<table class="src" cellpadding="5"><tr><td>
<font face="Times New Roman" size="5">Times New Roman</font><br />
<font face="Verdana" size="5">Verdana</font><br />
<font face="Comic sans MS" size="5">Comic Sans MS</font><br />
<font face="WildWest" size="5">WildWest</font><br />
<font face="Bedrock" size="5">Bedrock</font>
</td></tr></table>
<h2>Specify alternate font faces:</h2>
<p>A visitor will only be able to see your font if they have that font installed on their computer. So, it is possible to specify two or more font face alternatives by listing the font face names, separated by a comma. </p>
<p>Example:</p>
<table class="src" cellpadding="5"><tr><td><pre>
&lt;font face="arial,helvetica"&gt;
&lt;font face="Lucida Calligraphy,Comic Sans MS,Lucida Console&gt;
</pre></td></tr></table>
<p>When your page is loaded, their browser will display the first font face that it has available. If none of your selections are installed....then it will display the default font face <i>Times New Roman</i>.</p>
<p>Check a complete list of <a href="/html/html_fonts_ref.htm">HTML Standard Fonts</a>.</p>
<h1>Font Color:</h1>
<p>You can set any font color you like using <i>color</i> attribute. You can specify the color that you want by either the color name or hexadecimal code for that color. Check a complete list of <a href="/html/html_color_names.htm">HTML Color Name with Codes</a>.</p>
<p>Example:</p>
<table class="src" cellpadding="5"><tr><td><pre>
&lt;font color="#FF00FF"&gt;This text is hexcolor #FF00FF&lt;/font&gt;
&lt;font color="red"&gt;This text is red&lt;/font&gt;
</pre></td></tr></table>
<p>This will produce following result:</p>
<table class="src" cellpadding="5"><tr><td>
<font color="#FF00FF">This text is hexcolor #FF00FF</font><br />
<font color="red">This text is red</font>
</td></tr></table>
<p>To Become more comfortable - <a href="/cgi-bin/practice.cgi?file=html_font_tag" target="_blank">Do Online Practice</a></p>
<h1>The &lt;basefont&gt; Element:</h1>
<p>The &lt;basefont&gt; element is supposed to set a default font size, color, and typeface for any parts of the document that are not otherwise contained within a &lt;font&gt; element. You can then use the &lt;font&gt; elements to override the &lt;basefont&gt; settings.</p>
<p>The attributes that the &lt;basefont&gt; element takes are exactly the same as for the &lt;font&gt; element. You can also set the size of fonts relative to the size of the &lt;basefont&gt; by giving them a value of +1 for a size larger or -2 for two sizes smaller</p>
<p><b>NOTE:</b> This element is deprecated in HTML 4 and will be removed from HTML, the preferred option is to use CSS styles. Your browser may not have support for this tag.</p>
<h1>Example:</h1>
<table class="src" cellpadding="5"><tr><td><pre>
&lt;basefont face="arial, verdana, sans-serif" size="2" color="#ff0000"&gt;
&lt;p&gt;This is the page's default font.&lt;/p&gt;
&lt;h2&gt;Example of the &amp;lt;basefont&amp;gt; Element&lt;/h2&gt;
&lt;p&gt;&lt;font size="+2" color="darkgray"&gt;Here is some darkgray text
two sizes larger&lt;/font&gt;&lt;/p&gt;
&lt;p&gt;&lt;font face="courier" size="-1" color="#000000"&gt;Here is a courier
font, a size smaller, in black&lt;/font&gt;&lt;/p&gt;
</pre></td></tr></table>
<p>This will produce following result:</p>
<table class="src" cellpadding="5"><tr><td>
<basefont face="arial, verdana, sans-serif" size="2" color="#ff0000" /><br />
<p>This is the page's default font.</p>
<h2>Example of the &lt;basefont&gt; Element</h2>
<p><font size="+2" color="darkgray">Here is some darkgray text<br />
two sizes larger</font></p>
<p><font face="courier" size="-1" color="#000000">Here is a courier<br />
font, a size smaller, in black</font></p>
</td></tr></table>
<p>As you can see, the default font now takes on the properties specified in the &lt;basefont&gt; element. It is red, size 2, and uses the Arial typeface.</p>
<p>The paragraph after the &lt;h2&gt; element uses a font size two sizes larger than the default size and is gray text, whereas the following paragraph uses a font one size smaller than the default font. You can also see that the color of this font is black (overriding the default).</p>
<br />
<hr />
<a href="/html/html_comments.htm"><img alt="previous" border="0" src="/images/previous.gif" /></a>
<a href="/html/html_marquees.htm"><img alt="next" border="0" src="/images/next.gif" /></a>
<a href='/cgi-bin/printversion.cgi?tutorial=html&amp;file=html_fonts.htm' target="_blank">
<img src="/images/print.gif" alt="Printer Friendly" border="0" /></a>
<br />
<hr />
<!-- PRINTING ENDS HERE -->
<div style="padding-bottom:5px;padding-left:10px;">Advertisements</div>
</td></tr>
</table>
</td>
<td class="right" align="center" valign="top">
<br /><br />
<table width="100%" cellpadding="0" cellspacing="0">
<tr valign="middle">
<td  class="top" style="border:0px;">
&nbsp;&nbsp;
</td>
</tr>
</table>
<table class="right"  cellpadding="0" cellspacing="0">
<tr>
<td align="center" class="right">
<p>
<u>Advertisements</u>
</p>
<br /><br /></td></tr></table>
</td></tr></table>
</center>
</body>
</html>
"""

def request(image_server, data, iter, timing):
    try:
        data = {'format': '',
                'html': data,
                'output': '',
                'width': 500,
                'height': 110}
        start = time.time()
        response = requests.post(image_server, data=data)
        end = time.time()
        timing['elapsed'] += (end-start)
        timing['requests'] += 1
        response.raise_for_status()
        print image_server, response.headers, response.content
        
    except urllib2.HTTPError, e:
        print 'HTTP Error code', e.code
    except Exception, e:
        print 'Exception', e

    return 0

def main(argv):
    """
    Track log replay. Used for timing and volume during the test
    """
    parser = argparse.ArgumentParser(description='replays track requests')
    parser.add_argument('--concurrency', type=int, default=1, help='Number of request threads to run')
    parser.add_argument('--no-delay',dest='schedule',action='store_false', default=True, help="Do not wait between requests") 
    parser.add_argument('--iterations',dest='iterations', type=int, default=10, help="Number of iterations") 
    parser.add_argument('--http', dest='image_server', help="http image server", required=True)
    options = parser.parse_args()

    timing = {'elapsed': 0,
              'requests': 0}
    data = [HTML1,HTML2]   
    delta = None
    pool = gevent.pool.Pool(options.concurrency)
    for x in range(options.iterations):
        pool.spawn(request, options.image_server, random.choice(data), x, timing)
    pool.join()

    print "elapsed (waiting for requests): %s" % timing['elapsed']
    print "total requests: %s" % timing['requests']
    print "avg request time: %s" % (timing['elapsed']/timing['requests'])


if __name__ == '__main__':
    sys.exit(main(sys.argv))
