This is a generic INSTALL file for utilities distributions.
If this package does not come with, e.g., installable documentation or
data files, please ignore the references to them below.

To compile this package:

1.  Configure the package for your system.  In the directory that this
file is in, type `./configure'.  If you're using `csh' on an old
version of System V, you might need to type `sh configure' instead to
prevent `csh' from trying to execute `configure' itself.

The `configure' shell script attempts to guess correct values for
various system-dependent variables used during compilation, and
creates the Makefile(s) (one in each subdirectory of the source
directory).  In some packages it creates a C header file containing
system-dependent definitions.  It also creates a file `config.status'
that you can run in the future to recreate the current configuration.

Running `configure' takes a minute or two.  While it is running, it
prints some messages that tell what it is doing.  If you don't want to
see the messages, run `configure' with its standard output redirected
to `/dev/null'; for example, `./configure >/dev/null'.

To compile the package in a different directory from the one
containing the source code, you must use a version of `make' that
supports the VPATH variable, such as GNU `make'.  `cd' to the directory
where you want the object files and executables to go and run
`configure'.  `configure' automatically checks for the source code in
the directory that `configure' is in and in `..'.  If for some reason
`configure' is not in the source code directory that you are
configuring, then it will report that it can't find the source code.
In that case, run `configure' with the option `--srcdir=DIR', where
DIR is the directory that contains the source code.

By default, `make install' will install the package's files in
/usr/local/bin, /usr/local/lib, /usr/local/man, etc.  You can specify
an installation prefix other than /usr/local by giving `configure' the
option `--prefix=PATH'.  Alternately, you can do so by giving a value
for the `prefix' variable when you run `make', e.g.,
	make prefix=/usr/gnu

You can specify separate installation prefixes for
architecture-specific files and architecture-independent files.  If
you give `configure' the option `--exec-prefix=PATH' or set the
`make' variable `exec_prefix' to PATH, the package will use PATH as
the prefix for installing programs and libraries.  Data files and
documentation will still use the regular prefix.  Normally, all files
are installed using the regular prefix.

Another `configure' option is useful mainly in `Makefile' rules for
updating `config.status' and `Makefile'.  The `--no-create' option
figures out the configuration for your system and records it in
`config.status', without actually configuring the package (creating
`Makefile's and perhaps a configuration header file).  Later, you can
run `./config.status' to actually configure the package.  You can also
give `config.status' the `--recheck' option, which makes it re-run
`configure' with the same arguments you used before.  This option is
useful if you change `configure'.

Some packages pay attention to `--with-PACKAGE' options to `configure',
where PACKAGE is something like `gnu-libc' or `x' (for X windows).
The README should mention any --with- options that the package recognizes.

`configure' ignores any other arguments that you give it.

If your system requires unusual options for compilation or linking
that `configure' doesn't know about, you can give `configure' initial
values for some variables by setting them in the environment.  In
Bourne-compatible shells, you can do that on the command line like
this:
	CC='gcc -traditional' DEFS=-D_POSIX_SOURCE ./configure

The `make' variables that you might want to override with environment
variables when running `configure' are:

(For these variables, any value given in the environment overrides the
value that `configure' would choose:)
CC		C compiler program.
		Default is `cc', or `gcc' if `gcc' is in your PATH.
INSTALL		Program to use to install files.
		Default is `install' if you have it, `cp' otherwise.

(For these variables, any value given in the environment is added to
the value that `configure' chooses:)
DEFS		Configuration options, in the form `-Dfoo -Dbar ...'
		Do not use this variable in packages that create a
		configuration header file.
LIBS		Libraries to link with, in the form `-lfoo -lbar ...'

If you need to do unusual things to compile the package, we encourage
you to figure out how `configure' could check whether to do them, and
mail diffs or instructions to the address given in the README so we
can include them in the next release.

2.  Type `make' to compile the package.  If you want, you can override
the `make' variables CFLAGS and LDFLAGS like this:

	make CFLAGS=-O2 LDFLAGS=-s

3.  If the package comes with self-tests and you want to run them,
type `make check'.  If you're not sure whether there are any, try it;
if `make' responds with something like
	make: *** No way to make target `check'.  Stop.
then the package does not come with self-tests.

4.  Type `make install' to install programs, data files, and
documentation.

5.  You can remove the program binaries and object files from the
source directory by typing `make clean'.  To also remove the
Makefile(s), the header file containing system-dependent definitions
(if the package uses one), and `config.status' (all the files that
`configure' created), type `make distclean'.

The file `configure.in' is used as a template to create `configure' by
a program called `autoconf'.  You will only need it if you want to
regenerate `configure' using a newer version of `autoconf'.
