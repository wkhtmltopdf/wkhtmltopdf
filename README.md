wkhtmltopdf and wkthmltoimage
-----------------------------

wkhtmltopdf and wkhtmltoimage are a command line tools to render HTML into PDF
and various image formats using the QT Webkit rendering engine. This tool runs
entirely "headless" and does not require a display or display service.


Usage
-----

    wkhtmltopdf [options...] [objects...] <output file>

    wkhtmltoimage [options...] <input url/file name> <output file>

wkhtmltopdf is able to put several objects into the output file. An object is
a single webpage, a cover webpage, or a table of contents. The objects are
put into the output document in the order they are specified on the command
line. Options can be specified on a per object basis or in the global options
area.

    ['page'] <input url/file name> [page options...]

A page objects puts the content of a singe webpage into the output document.

options for the page object can be placed in the global options and the page
options areas. The applicable options can be found in the Page Options and 
Headers And Footer Options sections.

    cover <input url/file name> [page option...]

A cover objects puts the content of a singe webpage into the output document,
the page does not appear in the table of content, and does not have headers and
footers.

All options that can be specified for a page object can also be specified for a
cover.

    toc [toc option...]

A table of content object inserts a table of content into the output document.

All options that can be specified for a page object can also be specified for a
toc, further more the options from the TOC Options section can also be applied.
The table of content is generated via XSLT which means that it can be styled to
look however you want it to look. To get an aide of how to do this you can dump
the default xslt document by supplying the --dump-default-toc-xsl, and the
outline it works on by supplying --dump-outline, see the Outline Options
section.

Examples
--------

To convert a remote webpage to PDF:

    wkhtmltopdf http://example.com example.pdf

To convert a local HTML file to PDF:

    wkhtmltopdf my.html my.pdf

You can also convert to PS files if you like:

    wkhtmltopdf my.html my.ps

Printing a book with a table of content:

    wkhtmltopdf -H cover cover.html toc chapter1.html chapter2.html chapter3.html book.pdf

To convert a remote webpage to PNG:

    wkhtmltoimage http://example.com example.png

To convert a local HTML file to JPEG:

    wkhtmltoimage my.html my.jpg
