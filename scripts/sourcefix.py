#!/usr/bin/python
#
# Copyright 2010 wkhtmltopdf authors
#
# This file is part of wkhtmltopdf.
#
# wkhtmltopdf is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# wkhtmltopdf is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with wkhtmltopdf.  If not, see <http:#www.gnu.org/licenses/>.

from sys import argv, exit
import re
from datetime import date
import os
import difflib

cdate = re.compile(r"Copyright ([0-9 ,]*) wkhtmltopdf authors")
ifdef = re.compile(r"^[\n\r \t]*#ifndef __(.*)__[\t ]*\n#define __(\1)__[\t ]*\n")
endif = re.compile(r"#endif.*[\r\n \t]*$")
ws = re.compile(r"[ \t]*[\r\n]")
branchspace = re.compile(r"([ \t\r\n])(for|if|while|switch|foreach)[\t \r\n]*\(")
hangelse = re.compile(r"}[\r\n\t ]*(else)")
braceup = re.compile(r"(\)|else)[\r\n\t ]*{")
include = re.compile(r"(#include (\"[^\"]*\"|<[^>]*>)\n)+")

def includesort(x):
	return "\n".join(sorted(x.group(0)[:-1].split("\n"))+[""])

changes=False
progname="wkhtmltopdf"
for path in argv[1:]:
	if path.split("/")[0] == "include": continue
	try:
		data = file(path).read()
	except:
		continue

	mo = cdate.search(data)
	years = set(mo.group(1).split(", ")) if mo else set()
	years.add(str(date.today().year))

	ext = path.rsplit(".",2)[-1]
	header = ""
	cc = "//"
	if ext in ["hh","h","c","cc","cpp","inl", "inc"]:
		header += """// -*- mode: c++; tab-width: 4; indent-tabs-mode: t; eval: (progn (c-set-style "stroustrup") (c-set-offset 'innamespace 0)); -*-
// vi:set ts=4 sts=4 sw=4 noet :
//
"""
	elif ext in ["sh"]:
		header += "#!/bin/bash\n#\n"
		cc = "#"
	elif ext in ["py"]:
		header += "#!/usr/bin/python\n#\n"
		cc = "#"
	elif ext in ["pro","pri"]:
		cc = "#"
	else:
		continue

	header += """// Copyright %(years)s %(name)s authors
//
// This file is part of %(name)s.
//
// %(name)s is free software: you can redistribute it and/or modify
// it under the terms of the GNU %(lesser)sGeneral Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// %(name)s is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU %(lesser)sGeneral Public License
// along with %(name)s.  If not, see <http://www.gnu.org/licenses/>.

"""%{"years": (", ".join(sorted(list(years)))),"name":progname, "lesser": ("Lesser " if path.startswith("src/lib/") else "")}

	if ext in ["c", "h", "inc"]:
		header = "/*" + header[2:-1] + " */\n\n"
		cc = " *"
		hexp = re.compile(r"^/\*([^*]*(\*[^/]))*[^*]*\*/[ \t\n]*");
	else:
		#Strip away generated header
		hexp = re.compile("^(%s[^\\n]*\\n)*"%(cc))
	ndata = hexp.sub("", data,1)
	ndata = ws.sub("\n", ndata)+"\n"
	if ext in ["hh","h","inl"]:
		s=0
		e=-1
		while ndata[s] in ['\r','\n',' ','\t']: s+=1
		while ndata[e] in ['\r','\n',' ','\t']: e-=1
		#Strip away generated ifdef
		if ifdef.search(ndata):
			ndata = endif.sub("",ifdef.sub("",ndata,1),1)
	s=0
	e=-1
	while ndata[s] in ['\r','\n',' ','\t']: s+=1
	while ndata[e] in ['\r','\n',' ','\t']: e-=1
	ndata=ndata[s:e+1].replace("	",'\t')
	if ext in ["hh","h","c","cc","cpp","inl"]:
		ndata = branchspace.sub(r"\1\2 (",ndata)
		ndata = hangelse.sub("} else",ndata)
		ndata = braceup.sub(r"\1 {",ndata)
		ndata = include.sub(includesort, ndata)

	if ext in ["hh","h","inl"]:
		n = os.path.split(path)[-1].replace(".","_").replace(" ","_").upper()
		ndata = """#ifndef __%s__
#define __%s__
%s
#endif %s__%s__%s"""%(n,n,ndata, "//" if ext != "h" else "/*", n, "" if ext != "h" else "*/")
	ndata = header.replace("//",cc)+ndata+"\n"
	if ndata != data:
		for x in difflib.unified_diff(data.split("\n"),ndata.split("\n"), "a/"+path, "b/"+path):
			print x
 		changes=True
		file(path, "w").write(ndata)

if changes: exit(1)
