v0.12.6 (unreleased)
--------------------
* **#3953**: fix TOC and other special pages not present in output PDF (#3962)
* **#3242**: **[qt]** fix regression from #2353 in setLineDash for Canvas (qt#35)

v0.12.5 (2018-06-11)
--------------------
* fixed build without patched Qt and integrate with Travis CI and AppVeyor
* **#1524**: **[qt]** partial fix for overlapping text when repeating headers/footers in table (qt#29)
* **#1914**: **[qt]** rotated elements not rendered on thead on second page and beyond
* **#1870**: fixed sending of duplicate cookies (#3257)
* **#2609**: add viewportSize property to C API (#3440)
* **#2711**: fix differences between "counting" and "printing" phases (causing crashes/blank pages)
* **#3001**: fix static builds not working with updated OpenSSL
* **#3090**: added support for SSL client certificates (#3206)
* **#3241**: fix for DPI regression on OS X (#3386)
* **#3363**: added error message for network errors related to non-media files
* **#3364**: new option `verbosity` to allow different log levels in both CLI and API
* **#3476**: fix application name in libwkhtmltox (#3878)
* **#3672**: **[qt]** fix broken PDF with fonts having spaces in name (qt#38)
* **#3680**: **[qt]** fix build on macOS 10.13
* **#3900**: **[qt]** properly encode URLs with unicode characters in links
* add outlineDepth property to C API (#3291)
* add wkhtmltoimage_destroy_global_settings method to C API (#3324)
* switch to self-hosted website and downloads as gna.org is shutting down (#3390)
* fix conversion of read-only form fields (#3550)
* flush stderr to allow reading progress updates (#3583)
* fix handling of errors for media URLs with query string (#3593)
* add setting to perform hostname lookup via proxy (#3628)
* correctly pass --copies to underlying QPrinter (#3747)
* minor documentation updates and fixing compiler warnings
* add various Qt patches to support OpenSSL 1.1 and latest MinGW/macOS environments
* removed build scripts in favor of separate repo (wkhtmltopdf/packaging)

v0.12.4 (2016-11-22)
--------------------
* add support for building with VS2015
* update OpenSSL to 1.0.2j and libpng to 1.6.26
* **[qt]** change screen size in headless mode to a more sane value (1366x768)
* **[qt]** allow screen size in headless mode to be overriden via environment variables
* **[qt]** fix compilation failure on OS X 10.11 due to usage of deprecated APIs
* **#1768**: allow using long argument list (without hardcoded limits)
* **#2004**: **[qt]** fix rendering of form fields in PDF output
* **#2353**: **[qt]** context.setLineDash doesn't work in Canvas
* **#2416**: make background transparent by default for headers/footers
* **#2463**: **[qt]** standardize rendering DPI to 96
* **#2673**: fail immediately in the build script if the Qt source is not available
* **#2684**: **[qt]** fix crash in WebKit (RenderBlock::removeFloatingObjectsBelow)
* **#2885**: enforce UTF-8 encoding for input strings in the API
* **#2887**: improve docs to make examples of header/footer variables more explicit
* **#3077**: use correct install path for man pages

v0.12.3 (2016-01-20)
--------------------
* **[qt]** upgrade the patched Qt to the last Qt 4.8 commit
* removed support for CentOS 5 builds
* update OpenSSL to 1.0.2e and xz to 5.2.2 and libjpeg to 9b
* downgrade libpng to 1.2.56
* **#1843**: add --keep-relative-links and --resolve-relative-links to resolve relative links
* **#1981**: fix temporary PDF files not being removed when writing to stdout
* **#2104**: renamed COPYING to LICENSE
* **#2183**: apply --javascript-delay settings for non-main resource loaders
* **#2190**: do not depend on ICU even if it is already installed
* **#2194**: **[qt]** fix debugging of Qt/WebKit due to missing debug information
* **#2214**: **[qt]** fix broken rendering of transparent images when converting to PDF
* **#2252**: fix ownership of /usr/local being changed during installation on OS X
* **#2280**: do not allow data URIs for --header-html or --footer-html
* **#2322**: fix broken debug builds with MSVC
* **#2355**: add support for proxy bypass for specific hosts with --bypass-proxy-for
* **#2418**: fix build error on OS X
* **#2560**: allow using email address as proxy username
* **#2572**: fix incorrect version detection when building distro-specific packages

v0.12.2.1 (2015-01-19)
----------------------
* update OpenSSL to 1.0.1l
* **#2084**: **[qt]** make background transparent by default when rendering to PDF
* **#2142**: fix wkhtmltoimage ignoring --javascript-delay and --window-status
* **#2151**: **[qt]** fix PDFs rendering without embedded fonts on 64-bit Cocoa build

v0.12.2 (2015-01-09)
--------------------
* external libraries updated: OpenSSL to 1.0.1k, libpng to 1.5.21 and xz to 5.2.0
* fix build failure with unpatched Qt >= 5.3
* **#1539**: **[qt]** using OpenType fonts now results in selectable text on Windows
* **#1638**: **[qt]** fix incorrect rendering of JPEG images on Windows Server 2008 x64
* **#1639**: block access to local files on Windows when an invalid URL is used
* **#1640**: **[qt]** make table page-break logic opt-in via CSS at the row level
* **#1676**: do not allow overriding the header/footer settings for cover pages
* **#1676**: set page margins correctly via computed header/footer heights for multiple URLs
* **#1758**: fix corrupt image when output is specified as "-" in wkhtmltoimage on Windows
* **#1722**: **[qt]** fix broken hyphenation with soft-hyphens
* **#1769**: fixed unicode URLs in links
* **#1772**: added variable 'isodate' for substitution in headers/footers
* **#1790**: fix handling of temporary files during PDF conversion via API
* **#1808**: fix [sitepage] and [sitepages] not working without HTML headers/footers
* **#1825**: fix handling of non-ASCII characters in command-line arguments
* **#1863**: **[qt]** blank page or crash with low DPI on Windows
* **#1892**: --window-status causes endless loop with --header-html
* **#1906**: fix wrong comparison when parsing list parameters
* **#1944**: **[breaking change]** fix the reflection subsystem to fix the non-functional API
* **#1949**: fix generation of tarball in the posix-local build
* **#1955**: installer does not work on 32-bit OS X (10.6.x or 10.7.x)
* **#1961**: add explicit dependency on minimal font packages for all linux targets
* **#1988**: fix footer spacing being ignored (and header spacing being used instead)
* **#1992**: add support for CentOS 7 target
* **#2055**: **[qt]** rendering of Thai symbols is very slow

v0.12.1 (2014-06-26)
--------------------
* removed support for PostScript output which is not supported in QT5
* use OpenSSL 1.0.1h for the Windows builds
* **#1502**: fix spurious "exit with code 1 due to http error: 1xxx" errors
* **#1504**: **[qt]** fix rendering issues for JPEG with XMP metadata
* **#1507**: **[qt]** fix crash when CSS widows are specified
* **#1509**: fix TOC support which was broken after upgrade to latest Qt
* **#1512**: **[qt]** fix crashes when rendering JPEG images on Windows Server 2008 x64
* **#1544**: use UTF-8 encoding for the HTML generated by the TOC XSL
* **#1545**: PDFs rendering without embedded fonts on OS X
* **#1546**: fix problem with non-ASCII characters in output path on Windows
* **#1585**: delete existing files explicitly in windows installer
* **#1586**: **[qt]** suppress blank page generated if the first cell spans multiple pages
* **#1631**: **[qt]** crash when hiding header for a specific page
* **#1686**: **[qt]** --user-style-sheet does not work on Windows
* **#1757**: fix parsing of ColorMode and PrinterMode in the API
* **#1764**: include text codecs for Chinese (Simplified/Traditional), Japanese and Korean
* fix compilation failures when not building with patched Qt/WebKit
* support building with non-patched Qt4/Qt5 in either shared or static mode
* implement a --license command line option
* update URLs in the internal documentation
* generate API documentation for libwkhtmltox (on the website)
* display version in compiled binary properly under various scenarios
* complete revamp of the build system, which now generates native packages on all platforms

v0.12.0 (2014-02-06)
--------------------
* **#443**: add "sitepage" and "sitepages" support for headers
* **#175**: add ability to auto calculate header/footer heights
* **#1086**: don't return freed memory; instead use a string cache
* **#648**: delete infinite warnings into console about window status
* **#550**: enable tunneling to use a HTTP proxy for HTTPS
* **#1277**: avoid crash due to premature deletion of ResourceObject
* lots of fixes/enhancements for memory and error management
* add *--quiet* option to wkhtmltoimage
* add *--cache-dir* option for configuring web cache directory
* add *--load-media-error-handling* to handle errors for media files
* add *--viewport-size* to specify viewport size
* reserve heights when not using HTML header/footer
* lots of improvements in the build system
* lots of typos were fixed in various places
* **[qt]** rebase patch series on QT 4.8.5 (now hosted on github)
* **[qt]** fixes to get static builds working for MSVC 2010 (32/64 bit)
* **[qt]** prevent page breaks in table rows
* **[qt]** repeat THEAD/TFOOT when table contains page breaks
* **[qt]** improve font kerning as per Webkit bug 93263
* **[qt]** various table page-break improvements
* **[qt]** fix rendering due to incorrect page height computation
* **[qt]** implement "page-break-inside: avoid" for non-floating block elements
* **[qt]** enable WOFF file support
* **[qt]** add support for CSS widows and orphans for block elements
