##makefile for lynx

SHELL = /bin/sh

##uncomment the next line if your make fails to set $(MAKE)
#MAKE= make

##this is the name of the directory the lynx source code is in.
##(e.g. lynx2-7, not the full path)
lynxdir= lynx2-7

##this is the filename for .zip, .tar and .tar.Z archives.
lynxname= lynx2-7

##change the next line if you want lynx installed somewhere
##besides /usr/local/bin
exec= /usr/local/bin

##change the next line if you want the lynx man file installed somewhere
##besides /usr/local/man/man1
doc= /usr/local/man/man1

##change the next line if you want the lynx.cfg file installed somewhere
##besides /usr/local/lib
cfg= /usr/local/lib

installbin= install -c -s -m 555
installdoc= install -c -m 444

##set the relative location of the WWW library Implementation directory,
##from this directory
##do not set this to be an absolute path!!!
WWWINC= WWW/Library/Implementation

# !!!!!!!!!!! SUN resolv LIBRARY !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
# To include resolv in the LIBS="" list for SUN 3, 4 or Solaris OS,
# point RESOLVLIB to that library.  You need this if you get the message
# "Alert!:  Unable to connect to remote host" whenever you try to
# access a host other than localhost and have made Lynx without it.
# See the PROBLEMS file for more information.
#RESOLVLIB= -lresolv

# !!!!!!!!!!! Alternate socket functions for SOCKS !!!!!!!!!!!!!!!!!!
# To make a SOCKSified lynx, include -DSOCKS in your SITE_LYDEFS and
# SITE_DEFS, below, and point SOCKSLIB to your SOCKS library.  Note
# that you may have problems accessing FTP servers.  Also, instead of
# SOCKSifying lynx for use behind a firewall, you are better off if
# you make it normally, and set it up to use a proxy server.  You can
# SOCKSify the proxy server, and it will handle all clients, not just
# Lynx (see the INSTALLATION file in this distribution).  If your SOCKS
# server was compiled to use the short version of Rbind, also include
# -DSHORTENED_RBIND in your SITE_LYDEFS and SITE_DEFS.  If you do
# SOCKSify lynx, you can turn off SOCKS proxy usage via a -nosocks
# command line switch.
#SOCKSLIB= /usr/local/lib/libsocks.a

# !!!!!!!!!!!!!  DIRECT WAIS ACCESS !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
# If you are adding direct wais access you must fill in the
# location of wais.a, client.a and the standard math library, libm.a.
# You can pick up the most current freeWAIS sources from
#	ftp://ftp.cnidr.org/pub/NIDR.tools/freewais
# If you are using freeWAIS-0.202 or older, include -DDECLARE_WAIS_LOGFILES
# in your SITE_DEFS.
# Plese put an ABSOLUTE address here if you want it to work!
#WAISLIB= ../../freeWAIS-0.202/bin/client.a ../../freeWAIS-0.202/bin/wais.a /usr/lib/libm.a

# !!!!!!!!!!!!! SLANG Support (For color version of Lynx). !!!!!!!!!!!!!!!!
# The slang library is an alternative to curses, developed by John E. Davis
# (davis@space.mit.edu).  Version 0.99-27 or higher is available from
#	ftp://space.mit.edu/pub/davis
# It provides color support for Lynx.  If you wish to try it and are not
# on a supported system (see list, below), include -DUSE_SLANG in your
# SITE_DEFS and define the locations of the slang library (libslang.a)
# and headers (slang.h and slcurses.h) here.  They may be left commented
# out if the library and headers are in their canonical locations (usr/lib,
# and usr/include).
#SLANGINC = -I../../slang/src#              location of slang.h and slcurses.h
#SLANGLIB = -L../../slang/src/$(ARCH)objs#  location of libslang.a
#SLANGRRLIB = -R../../slang/src/$(ARCH)objs# for solaris

# !!!!!!!!!!! Lynx Local Directory Listing Formats !!!!!!!!!!!!!!!!!!!!!!!!!!!
# Lynx supports "ls -l" format for local directory listings on Unix if the
# LONG_LIST compilation symbol is defined, and includes links to the parent
# directory if the compilation symbol NO_PARENT_DIR_REFERENCE is not defined.
# If "ls -l" format is enabled here, the listing format can be modifed via a
# LIST_FORMAT symbol in userdefs.h and/or lynx.cfg.  Otherwise, only file and
# directory names will be included in the listings.
#
# If you are not including DirEd Support (see below) and wish to disable
# "ls -l" emulation in local directory listings, prepend "#" to this line:
DIR_LONG_LIST =   -DLONG_LIST
#
# To disable "Up to" parent links in local directory listings,
# remove "#" from this line:
#DIR_PARENT =  -DNO_PARENT_DIR_REFERENCE

# !!!!!!!!!!! Lynx DirEd Support !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
# Lynx now supports directory editing (DirEd) for local directories.
# This allows users to do things like view, copy, & remove files using
# a tabular display of the directory & single-keystroke commands instead
# of using the command line.  From inside Lynx, the keystroke sequence
# "g.<enter>" switches Lynx to DirEd mode on the current directory.
# If you're building a Lynx that is to be used as a kind of restricted
# shell for users who do not have access to the command line & should
# not have access to equivalent capabilities, you need to disable DirEd
# here.  You can also disable some DirEd functions while allowing others.
#
# DirEd functions are disabled by inserting or removing "#" signs at
# the beginnings of lines that contain variable assignments here.
# All DirEd functions that were enabled on compilation can be disabled
# or modified at run time via DIRED_MENU symbols in lynx.cfg.
#
# To disable DirEd completely, prepend "#" to this line:
DIRED_SUPPORT     =   -DDIRED_SUPPORT
#(If you have disabled DirEd completely, you can ignore all the
#other DirEd stuff here.)
#
# To prevent DirEd from extracting files from an archive file,
# remove "#" from this line:
#DIRED_ARCHIVE   =  -DARCHIVE_ONLY
#
# To prevent DirEd from using the tar program, prepend "#" to this line:
DIRED_TAR   =   -DOK_TAR
#
# To prevent DirEd from using zip & unzip, prepend "#" to this line:
DIRED_ZIP   =   -DOK_ZIP
#
# To prevent DirEd from using gzip & gunzip, prepend "#" to this line:
DIRED_GZIP   =   -DOK_GZIP
#
# To prevent DirEd from using uudecode, prepend "#" to this line:
DIRED_UUDECODE   =   -DOK_UUDECODE
#
# Lynx users can customize their keymaps by creating private versions
# of lynx.cfg & modifying them, overriding the default keymap.
# To prevent DirEd keymap overriding, prepend "#" to this line:
DIRED_OVERRIDE   =   -DOK_OVERRIDE
#
# To prevent DirEd from changing the permissions on directories or files
# (i.e. from doing what the Unix chmod command or the DOS attrib command
# does), prepend "#" to this line:
DIRED_PERMIT   =   -DOK_PERMIT
#
# If you do not comment out the DIRED_PERMIT line, but want to restrict
# changes of the eXecute permission to directories (i.e., not allow it to
# be changed for files), remove "#" from this line (if you don't remove it,
# you can still block changes of the eXecute permission for files but not
# directories via the "change_exec_perms" command line restriction):
#DIRED_NOXPERMS   =   -DNO_CHANGE_EXECUTE_PERMS

# !!!!!!!!!!! NOTE:  NetBSD and FreeBSD !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
# On NetBSD 1.1 and earlier, as well as FreeBSD 2.0.5 and earlier,
# the system make program uses different syntax for including files
# than the GNU make program.  If you get the following error --
#   * "Makefile", line 27: Need an operator *
# you have several choices to run GNU make:
#
# * Install GNU make as `/usr/bin/make', after moving /usr/bin/make
#   to /usr/bin/old-make
#
# * Install GNU make in a different directory which is earlier in
#   your search path.
#
# * Install GNU make as gmake, and change occurrences of `make' to
#   `gmake' in any Makefile used by your install.  You won't need to
#   change all of the Makefiles, just the top-level one, and the one
#   in WWW/Library/???bsd

# !!!!!!!!!!! Lynx Wishful Thinking !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
# If you are on one of the supported systems (see all: list below)
# you should not have to modify anything from here down (but check
# it out for your system, anyway)!
# If you are not, then you should edit the generic: entry below.
#
# If you need ncurses, the latest version can always be found
# in ftp://ftp.netcom.com/pub/zm/zmbenhal/ncurses/
# (you may need -DNCURSESHEADER in addition to -DNCURSES for old
# versions of ncurses; see below).
#
# Also be sure to check out the compilation symbols in userdefs.h
# before building Lynx!

all:
	@echo
	@echo "Please run make with one of the following arguments"
	@echo "aix4       -- for RS/6000 running AIX 4.n and some 3.2"
	@echo "aix32      -- for RS/6000 running AIX 3.2"
	@echo "aix31      -- for RS/6000 running AIX 3.1 with curses bug"
	@echo "apollo     -- apollo systems (untested)"
	@echo "bsdi       -- BSD Interactive"
	@echo "bsdi-ncurses -- BSD/OS(v2.0 or later) using ncurses(v1.9.4 or later) package."
	@echo "clix       -- for Intergraph CLIX"
	@echo "convex     -- for Convex C-series"
	@echo "convex-ncurses -- for Convex C-series with ncurses package"
	@echo "dgux       -- for DGUX "
	@echo "freebsd    -- for FreeBSD"
	@echo "freebsd-ncurses -- for FreeBSD using ncurses package"
	@echo "freebsd-slang -- for FreeBSD with color slang package"
	@echo "generic    -- for generic UNIX"
	@echo "isc        -- for Interactive Machines (untested)"
	@echo "linux      -- for PC linux"
	@echo "linux-ncurses -- for PC linux using ncurses package"
	@echo "linux-slang -- for linux with color slang package"
	@echo "mips       -- for MIPS, svr3/bsd flavour (untested)"
	@echo "netbsd     -- for NetBSD"
	@echo "netbsd-ncurses -- for NetBSD using ncurses package"
	@echo "news       -- for SONY NEWS-OS 4.2.1R"
	@echo "news-ncurses -- for SONY NEWS-OS 4.2.1R using ncurses package"
	@echo "news-slang -- for SONY NEWS-OS 4.2.1R with color slang package"
	@echo "next       -- for NeXT OS"
	@echo "next-ncurses  -- for NeXT OS using ncurses package"
	@echo "osf        -- for Alpha OSF/1"
	@echo "osf-slang  -- for Alpha OSF/1 with color slang package"
	@echo "ptx        -- for Sequent's DYNIX/ptx, version 4, (current version)"
	@echo "ptx-slang  -- for Sequent's DYNIX/ptx, version 4, with color slang package"
	@echo "ptx        -- for Sequent's DYNIX/ptx, current version"
	@echo "ptx2       -- for Sequent's DYNIX/ptx, version 2"
	@echo "riscos     -- Control Data Unix machine running EP/IX"
	@echo "sco        -- for SCO machines"
	@echo "sco5       -- for SCO OpenServer Release 5 machines"
	@echo "sgi        -- for SGI machines"
	@echo "sun3       -- for SUN 3 OS with System V curses"
	@echo "sun4       -- for SUN 4 OS with System V curses"
	@echo "sun4-ncurses -- for SUN 4 OS using ncurses package"
	@echo "sun4-pure  -- for SUN 4 OS with PURIFY and System V curses"
	@echo "sun4-slang -- for SUN 4 OS with color slang package"
	@echo "svr4       -- for SVR4"
	@echo "snake      -- for HP-UX lt 9.01 (gcc)"
	@echo "snake2     -- for HP-UX gte 9.01 (gcc)"
	@echo "snake2-slang -- for HP-UX gte 9.01 (gcc) with color slang"
	@echo "snake3     -- for HP-UX (purchased compiler)"
	@echo "snake3-slang -- for HP-UX (purchased compiler) with color slang"
	@echo "solaris2   -- for solaris 2.x"
	@echo "solaris2cc -- for Solaris 2.x using Sun's unbundled C compiler"
	@echo "solaris2-slang -- for Solaris 2 and color slang package"
	@echo "solaris2-slangcc -- for Solaris 2 slang using Sun's unbundled C compiler"
	@echo "ultrix     -- for DEC Ultrix"
	@echo "decstation -- for DEC Ultrix (same as ultrix)"
	@echo "ultrix-slang     -- for DEC Ultrix and color slang package"
	@echo "decstation-slang -- for DEC Ultrix and color slang (same as ultrix-slang)"
	@echo "umaxv   -- for Encore's UMAXV (SVR3.x) "
	@echo "umaxv-slang   -- for Encore's UMAXV and color slang package"
	@echo "unixware   -- for Novell's Unixware"
	@echo "univell    -- for Novell's Unixware"
	@echo "unixware-slang -- for Unixware and color slang package"
	@echo "univell-slang  -- for Unixware and color slang package"
	@echo "clean      -- removes all '.o' and 'core' files"
	@echo "tar        -- runs clean, removes executable, and tars the whole directory"
	@echo "compress   -- runs tar, then compresses the result"
	@echo "zip        -- runs clean, removes executable, and zips the whole directory"
	@echo

# Map some extra commands to existing ones
unix: generic
decstation: ultrix
decstation-slang: ultrix-slang
univell: unixware
univell-slang: unixware-slang

DIR_LYDEFS = $(DIRED_SUPPORT) $(DIR_LONG_LIST) $(DIR_PARENT)

DIR_DEFS = $(DIRED_SUPPORT) $(DIRED_ARCHIVE) $(DIRED_TAR) $(DIRED_ZIP) $(DIRED_GZIP) $(DIRED_UUDECODE) $(DIRED_OVERRIDE) $(DIRED_PERMIT) $(DIRED_NOXPERMS)

# If you apply patches which require linking to site-specific libraries,
# uncomment the following and point SITE_LIBS to those libraries.
#SITE_LIBS= # Your libraries here (remove the "#")

# Set SITE_LYDEFS to one or more of the defines for the WWW Library:
SITE_LYDEFS = $(DIR_LYDEFS) # Your defines here

# Set SITE_DEFS to one or more of the defines for lynx below:
SITE_DEFS = $(DIR_DEFS) # Your defines here

# if you are compiling on a previously unsupported system, modify
# this generic entry!!
#
# defines include:
# -DNO_CUSERID     if you don't have cuserid()
# -DNO_UTMP	   if you don't have utmp capibilities
# -Dvfork=fork     if you don't have vfork()
# -DMMDF	   if you use MMDF instead of sendmail
# -DFANCY_CURSES   enables fancy curses (bold, underline, reverse)
# -DNCURSES        for ncurses support (also indicate the LIBS path)
# -DNCURSESHEADER  seek ncurses.h instead of ncurses/curses.h in LYCurses.h
# -DNO_KEYPAD      if you don't have keypad() and related defines
# -DNO_SIZECHANGE  Disables window sizechange code
# -DNO_CPU_LIMIT   Disables Max CPU timeout (includes CLOCKS_PER_SECOND)
# -DNO_PUTENV      Define if you don't have a putenv call.
# -DNO_CBREAK      if you don't have cbreak() (use crmode() instead)
# -DUNDERLINE_LINKS  Define if you want links underlined instead of bold.
# -DIGNORE_CTRL_C  Define if Control-C shouldn't exit lynx.
# -DHP_TERMINAL    For DIM workaround to REVERSE problems on HP terminals.
# -DLOCALE         For LOCALE support for international characters.
# -DSOCKS	   For making a SOCKSified lynx.
# -DSHORTENED_RBIND  For a SOCKSified lynx with the short version of Rbind.
# -DNO_UNISTD_H    if you don't have <unistd.h>
# -DNOPORT         if you must use PASV instead of PORT for FTP
# -DNO_TTYTYPE	   if your system lacks a ttytype variable
# -DNSL_FORK	   For fork-based name server lookups that can be 'z'apped.
#
# if you are linking to freeWAIS-0.202 or older, you should define this
# in MCFLAGS (SITE_DEFS)
#
# -DDECLARE_WAIS_LOGFILES
#
# if you have the reverse clear screen problem of some SUN4 systems you
# should define this in MCFLAGS (SITE_DEFS)
#
# -DREVERSE_CLEAR_SCREEN_PROBLEM
#
# if you have an SVR4 system with the "type any key" problem, try defining
# this in LYFLAGS (SITE_LYDEFS) and MCFLAGS (SITE_DEFS)
#
# -DSVR4_BSDSELECT
#
# Old Data General systems may need this in their SITE_LYDEFS
# for their inet_addr(host) calls.
#
# -DDGUX_OLD
#
# if you would like logging of client requests via syslog(), you should
# define this in MCFLAGS (SITE_DEFS)
#
# -DSYSLOG_REQUESTED_URLS
#
# if you would like to enable code to detect memory leaks define this in both
# LYFLAGS (SITE_LYDEFS) and MCFLAGS (SITE_DEFS)
#
# -DLY_FIND_LEAKS
#
# for installation of local execution links, please see the file userdefs.h

#generic machines with original Berkeley curses
generic:
	@echo "You must first compile the WWW library in WWW/Library"
	cd WWW/Library/unix; $(MAKE) LYFLAGS="$(SITE_LYDEFS)"
	cd src; $(MAKE) all CC="cc" MCFLAGS="-O -DFANCY_CURSES -DUNIX \
		-I../$(WWWINC) $(SITE_DEFS)" \
		LIBS="-lcurses -ltermcap \
		$(WAISLIB) $(SOCKSLIB) $(SITE_LIBS)" \
		WWWLIB="../WWW/Library/unix/libwww.a"

# Contributed by Alex Matulich (matuli_a@marlin.navsea.navy.mil).
# Note that some of the online help files in this distribution need
# to be edited and renamed to conform with the Intergraph CLIX 14
# character file name limit.
clix:
	cd WWW/Library/clix; $(MAKE) LYFLAGS="$(SITE_LYDEFS)"
	cd src; $(MAKE) all CC="cc" MCFLAGS="-O -DFANCY_CURSES -DUNIX -DUSG \
		-DCLIX -DNO_UTMP \
		-I../$(WWWINC) $(SITE_DEFS)" \
		LIBS="-lmalloc -lcurses -ltermcap -lbsd -lc_s \
		$(WAISLIB) $(SOCKSLIB) $(SITE_LIBS)" \
		WWWLIB="../WWW/Library/clix/libwww.a"

# Tested on C3 series under ConvexOS 10.1, CC V5.0;
# Should work on other C-series
convex:
	cd WWW/Library/convex; $(MAKE) LYFLAGS="$(SITE_LYDEFS)"
	cd src; $(MAKE) all CC="cc" MCFLAGS="-O \
		-D__STDC__ \
		-DNO_PUTENV -DNO_CBREAK -DNO_KEYPAD -DUSE_DIRENT -DUNIX \
		-DSTDC_HEADERS -I../$(WWWINC) $(SITE_DEFS)" \
		LIBS="-lcurses -ltermcap \
		$(WAISLIB) $(SOCKSLIB) $(SITE_LIBS)" \
		WWWLIB="../WWW/Library/convex/libwww.a"

convex-ncurses:
	cd WWW/Library/convex; $(MAKE) LYFLAGS="$(SITE_LYDEFS)"
	cd src; $(MAKE) all CC="cc" MCFLAGS="-O \
		-I/usr/local/include \
		-DNCURSES -DFANCY_CURSES \
		-D__STDC__ \
		-DNO_PUTENV -DNO_CBREAK -DUSE_DIRENT -DUNIX \
		-DSTDC_HEADERS -I../$(WWWINC) $(SITE_DEFS)" \
		LIBS="-L/usr/local/lib -lncurses \
		$(WAISLIB) $(SOCKSLIB) $(SITE_LIBS)" \
		WWWLIB="../WWW/Library/convex/libwww.a"

# You may need to remove -DUTMPX_FOR_UTMP for some versions of DGUX.
dgux:
	cd WWW/Library/unix; $(MAKE) LYFLAGS="-DDGUX $(SITE_LYDEFS)"
	cd src; $(MAKE) all CC="cc" MCFLAGS="-O -DFANCY_CURSES -DUNIX \
		-I../$(WWWINC) -DDGUX -DUTMPX_FOR_UTMP $(SITE_DEFS)" \
		LIBS="-lcurses -ltermcap -lsocket -lnsl \
		$(WAISLIB) $(SOCKSLIB) $(SITE_LIBS)" \
		WWWLIB="../WWW/Library/unix/libwww.a"

mips:
	cd WWW/Library/mips; $(MAKE) LYFLAGS="$(SITE_LYDEFS)"
	cd src; $(MAKE) all CC="cc" MCFLAGS="-systype svr3 -O \
		-DFANCY_CURSES -DUNIX -I/svr3/usr/include/bsd \
		-I../$(WWWINC) -DNO_UTMP $(SITE_DEFS)" \
		LIBS="-lcurses -lnsl -lbsd \
		$(WAISLIB) $(SOCKSLIB) $(SITE_LIBS)" \
		WWWLIB="../WWW/Library/mips/libwww.a"

# FreeBSD doesn't have or need ranlib. (ignore the error message about that :)
freebsd:
	cd WWW/Library/freebsd; $(MAKE) LYFLAGS="$(SITE_LYDEFS)"
	cd src; $(MAKE) all CC="cc" MCFLAGS="-O -DUNIX -DNO_KEYPAD -DNO_CUSERID \
		-I../$(WWWINC) $(SITE_DEFS)"\
		LIBS="-lcurses -ltermcap \
		$(WAISLIB) $(SOCKSLIB) $(SITE_LIBS)" \
		WWWLIB="../WWW/Library/freebsd/libwww.a"

# FreeBSD doesn't have or need ranlib. (ignore the error message about that :)
freebsd-ncurses:
	cd WWW/Library/freebsd; $(MAKE) LYFLAGS="$(SITE_LYDEFS)"
	cd src; $(MAKE) all MCFLAGS="$(CFLAGS) -DFANCY_CURSES -DNCURSES \
		-DNCURSESHEADER -DUNIX -DNO_TTYTYPE -DNO_CUSERID -DLOCALE \
		-I../$(WWWINC) $(SITE_DEFS)" \
		LIBS="-lncurses -lmytinfo \
		$(WAISLIB) $(SOCKSLIB) $(SITE_LIBS)" \
		WWWLIB="../WWW/Library/freebsd/libwww.a"

# FreeBSD doesn't have or need ranlib. (ignore the error message about that :)
freebsd-slang:
	cd WWW/Library/freebsd; $(MAKE) LYFLAGS="$(SITE_LYDEFS)"
	cd src; $(MAKE) all MCFLAGS="$(CFLAGS) -DUSE_SLANG -DUNIX \
		-DNO_TTYTYPE -DNO_CUSERID -DLOCALE \
		-I../$(WWWINC) $(SITE_DEFS)" \
		LIBS="$(WAISLIB) $(SOCKSLIB) $(SITE_LIBS)" \
		WWWLIB="../WWW/Library/freebsd/libwww.a" \
		SLANGLIB="$(SLANGLIB) -lslang -ltermcap -lm" \
		SLANGINC="$(SLANGINC)"

# NetBSD doesn't have or need ranlib. (ignore the error message about that :)
netbsd:
	cd WWW/Library/netbsd; $(MAKE) LYFLAGS="$(SITE_LYDEFS)"
	cd src; $(MAKE) all CC="cc" MCFLAGS="-O -DUNIX -DNO_KEYPAD -DNO_CUSERID \
		-I../$(WWWINC) $(SITE_DEFS)" \
		LIBS="-lcurses -ltermcap \
		$(WAISLIB) $(SOCKSLIB) $(SITE_LIBS)" \
		WWWLIB="../WWW/Library/netbsd/libwww.a"

# NetBSD doesn't have or need ranlib. (ignore the error message about that :)
netbsd-ncurses:
	cd WWW/Library/netbsd; $(MAKE) LYFLAGS="$(SITE_LYDEFS)"
	cd src; $(MAKE) all CC="cc" MCFLAGS="-O -DFANCY_CURSES -DNCURSES \
		-DUNIX -DNO_CUSERID -I../$(WWWINC) $(SITE_DEFS) \
		-I/usr/include/ncurses" \
		LIBS="-lncurses -lcompat \
		$(WAISLIB) $(SOCKSLIB) $(SITE_LIBS)" \
		WWWLIB="../WWW/Library/netbsd/libwww.a"

# Tested on DN 3500 & 4500's running Domain O/S 10.4 BSD
# -Acpu,mathchip may have to be changed on other machines - see man cpuhep
apollo:
	cd WWW/Library/apollo_m68k; $(MAKE) LYFLAGS="$(SITE_LYDEFS)"
	cd src; $(MAKE) all CC="cc" MCFLAGS="-O -DUNIX -DNO_KEYPAD -DNO_CUSERID \
		-D_BUILTINS -W0,-opt,4 -I../$(WWWINC) $(SITE_DEFS)" \
		LIBS="-lcurses -ltermcap \
		$(WAISLIB) $(SOCKSLIB) $(SITE_LIBS)" \
		WWWLIB="../WWW/Library/apollo_m68k/libwww.a $(MYWWWLIB)"

riscos:
	cd WWW/Library/unix; $(MAKE) CC="gcc" LYFLAGS="$(SITE_LYDEFS)"
	cd src; $(MAKE) all CC="gcc" MCFLAGS="-O -DFANCY_CURSES -DUNIX \
		-DNO_GETCWD -DNO_PUTENV -Dremove=unlink \
		-I../$(WWWINC) $(SITE_DEFS)" \
		LIBS="-lcurses -ltermcap \
		$(WAISLIB) $(SOCKSLIB) $(SITE_LIBS)" \
		WWWLIB="../WWW/Library/unix/libwww.a"

# Lynx will not compile with the "free" cc compiler supplied
# with HP-UX.  You must use a purchased cc compiler to compile Lynx.
snake:
	cd WWW/Library/snake; $(MAKE) CC="gcc" LYFLAGS="$(SITE_LYDEFS)"
	cd src; $(MAKE) all CC="gcc" MCFLAGS="-O -DFANCY_CURSES -DUNIX \
		-DSNAKE -I../$(WWWINC) $(SITE_DEFS)" \
		LIBS="-lcurses -ltermcap \
		$(WAISLIB) $(SOCKSLIB) $(SITE_LIBS)" \
		WWWLIB="../WWW/Library/snake/libwww.a"

snake2:
	cd WWW/Library/snake; $(MAKE) CC="gcc" LYFLAGS="-D_INCLUDE_HPUX_SOURCE \
		-D_INCLUDE_POSIX_SOURCE -D_INCLUDE_AES_SOURCE \
		-D_INCLUDE_XOPEN_SOURCE $(SITE_LYDEFS)"
	cd src; $(MAKE) all CC="gcc" MCFLAGS="-O -DFANCY_CURSES -DUNIX \
		-DSNAKE -I../$(WWWINC) -D_INCLUDE_HPUX_SOURCE \
		-D_INCLUDE_POSIX_SOURCE -D_INCLUDE_AES_SOURCE \
		-D_INCLUDE_XOPEN_SOURCE $(SITE_DEFS)" \
		LIBS="-lcurses -ltermcap \
		$(WAISLIB) $(SOCKSLIB) $(SITE_LIBS)" \
		WWWLIB="../WWW/Library/snake/libwww.a"

snake2-slang:
	cd WWW/Library/snake; $(MAKE) CC="gcc" LYFLAGS="-D_INCLUDE_HPUX_SOURCE \
		-D_INCLUDE_POSIX_SOURCE -D_INCLUDE_AES_SOURCE \
		-D_INCLUDE_XOPEN_SOURCE $(SITE_LYDEFS)"
	cd src; $(MAKE) all CC="gcc" MCFLAGS="-O -DUNIX \
		-DUSE_SLANG -DSNAKE -I../$(WWWINC) -D_INCLUDE_HPUX_SOURCE \
		-D_INCLUDE_POSIX_SOURCE -D_INCLUDE_AES_SOURCE \
		-D_INCLUDE_XOPEN_SOURCE $(SITE_DEFS)" \
		LIBS="-lcurses -ltermcap \
		$(WAISLIB) $(SOCKSLIB) $(SITE_LIBS)" \
		WWWLIB="../WWW/Library/snake/libwww.a" \
		SLANGLIB="$(SLANGLIB) -lslang" SLANGINC="$(SLANGINC)"

# Lynx will compile with the purchased cc compiler added to HP-UX.
# contributed by Andy Finkenstadt (genie@panix.com)
# Note that HP/UX 10.10 apparently has a broken select() in libcurses.a
# and will malfunction with Lynx.  See the PROBLEMS file.
snake3:
	cd WWW/Library/snake; $(MAKE) CC="cc" LYFLAGS="$(SITE_LYDEFS)"
	cd src; $(MAKE) all CC="cc" MCFLAGS="-O -Ae -DFANCY_CURSES -DUNIX \
		-DSNAKE -I../$(WWWINC) $(SITE_DEFS)" \
		LIBS="-lcurses -ltermcap \
		$(WAISLIB) $(SOCKSLIB) $(SITE_LIBS)" \
		WWWLIB="../WWW/Library/snake/libwww.a"

snake3-slang:
	cd WWW/Library/snake; $(MAKE) CC="cc" LYFLAGS="$(SITE_LYDEFS)"
	cd src; $(MAKE) all CC="cc" MCFLAGS="-O -Ae -DUSE_SLANG -DUNIX \
		-DSNAKE -I../$(WWWINC) $(SITE_DEFS)" \
		LIBS="-lcurses -ltermcap \
		$(WAISLIB) $(SOCKSLIB) $(SITE_LIBS)" \
		WWWLIB="../WWW/Library/snake/libwww.a" \
		SLANGLIB="$(SLANGLIB) -lslang" SLANGINC="$(SLANGINC)"

svr4:
	cd WWW/Library/svr4; $(MAKE) LYFLAGS="-DSVR4_BSDSELECT $(SITE_LYDEFS)"
	cd src; $(MAKE) all CC="cc" MCFLAGS="-O -DFANCY_CURSES -DUNIX \
		-I../$(WWWINC) -DUTMPX_FOR_UTMP -DSVR4 -DSVR4_BSDSELECT \
		-DTRUE=1 -DFALSE=0 $(SITE_DEFS)" \
		LIBS="-lcurses -lsocket -lnsl \
		$(WAISLIB) $(SOCKSLIB) $(SITE_LIBS)" \
		WWWLIB="../WWW/Library/svr4/libwww.a"

# SEQUENT doesn't have or need ranlib. (ignore the error message about that :)
ptx:
	cd WWW/Library/ptx; $(MAKE) $(MFLAGS) LYFLAGS="-DNO_BCOPY \
		-DSVR4_BSDSELECT -DNO_FILIO_H $(SITE_LYDEFS)"
	cd src; $(MAKE) $(MFLAGS) all CC="cc" MCFLAGS="-O -DUNIX -DSVR4 \
		-DFANCY_CURSES -DNO_FILIO_H -DUTMPX_FOR_UTMP -DHAVE_TERMIOS_H \
		-I../$(WWWINC) $(SITE_DEFS)" \
		LIBS="-lcurses -lsocket -linet -lnsl \
		$(WAISLIB) $(SOCKSLIB) $(SITE_LIBS)" \
		WWWLIB="../WWW/Library/ptx/libwww.a"

# SEQUENT doesn't have or need ranlib. (ignore the error message about that :)
ptx-slang:
	cd WWW/Library/ptx; $(MAKE) $(MFLAGS) LYFLAGS="-DNO_BCOPY \
		-DSVR4_BSDSELECT -DNO_FILIO_H $(SITE_LYDEFS)"
	cd src; $(MAKE) $(MFLAGS) all CC="cc" MCFLAGS="-O -DUNIX -DSVR4 \
		-DUSE_SLANG -DNO_FILIO_H -DUTMPX_FOR_UTMP -DHAVE_TERMIOS_H \
		-I../$(WWWINC) $(SITE_DEFS)" \
		LIBS="-lcurses -lsocket -linet -lnsl \
		$(WAISLIB) $(SOCKSLIB) $(SITE_LIBS)" \
		WWWLIB="../WWW/Library/ptx/libwww.a" \
		SLANGLIB="$(SLANGLIB) -lslang" SLANGINC="$(SLANGINC)"

# SEQUENT doesn't have or need ranlib. (ignore the error message about that :)
ptx2:
	cd WWW/Library/ptx; $(MAKE) LYFLAGS="-DPTX2 -DNO_BCOPY -DNOPORT \
		-DSVR4_BSDSELECT -DNO_FILIO_H $(SITE_LYDEFS)"
	cd src; $(MAKE) all CC="cc" MCFLAGS="-O -DPTX2 -DFANCY_CURSES -DUNIX \
		-DNO_FILIO_H -DUTMPX_FOR_UTMP -DSVR4 \
		 -I../$(WWWINC) $(SITE_DEFS)" \
		LIBS="-lcurses -lsocket -linet -lnsl \
		$(WAISLIB) $(SOCKSLIB) $(SITE_LIBS)" \
		WWWLIB="../WWW/Library/ptx/libwww.a"

# Contributed by Thanh Ma (tma@encore.com).
umaxv:
	cd WWW/Library/umaxv-m88k; $(MAKE) LYFLAGS="$(SITE_LYDEFS)"
	cd src; $(MAKE) all CC="cc" MCFLAGS="-O -DFANCY_CURSES -DUNIX \
		-D_SYSV3 -DHAVE_TERMIOS -DUSE_DIRENT -DNO_UTMP \
		-I../$(WWWINC) $(SITE_DEFS)" \
		LIBS="-lcurses \
		$(WAISLIB) $(SOCKSLIB) $(SITE_LIBS)" \
		WWWLIB="../WWW/Library/umaxv-m88k/libwww.a"

# Contributed by Thanh Ma (tma@encore.com).
umaxv-slang:
	cd WWW/Library/umaxv-m88k; $(MAKE) LYFLAGS="$(SITE_LYDEFS)"
	cd src; $(MAKE) all CC="cc" MCFLAGS="-O -DUSE_SLANG -DUNIX \
		-D_SYSV3 -DHAVE_TERMIOS -DUSE_DIRENT -DNO_UTMP \
		-I../$(WWWINC) $(SITE_DEFS)" \
		LIBS="$(WAISLIB) $(SOCKSLIB) $(SITE_LIBS)" \
		WWWLIB="../WWW/Library/umaxv-m88k/libwww.a" \
		SLANGLIB="$(SLANGLIB) -lslang -lm" SLANGINC="$(SLANGINC)"

unixware:
	cd WWW/Library/svr4; $(MAKE) LYFLAGS="-DNO_BCOPY -DUNIXWARE \
		$(SITE_LYDEFS)"
	cd src; $(MAKE) all CC="cc" MCFLAGS="-O -DFANCY_CURSES -DUNIX \
		-I../$(WWWINC) -DUTMPX_FOR_UTMP -DSVR4 -DTRUE=1 -DFALSE=0 \
		-DNO_BCOPY -DUNIXWARE $(SITE_DEFS)" \
		LIBS="-lcurses -lsocket -lnsl \
		$(WAISLIB) $(SOCKSLIB) $(SITE_LIBS)" \
		WWWLIB="../WWW/Library/svr4/libwww.a"

unixware-slang:
	cd WWW/Library/svr4; $(MAKE) LYFLAGS="-DNO_BCOPY -DUNIXWARE \
		$(SITE_LYDEFS)"
	cd src; $(MAKE) all CC="cc" MCFLAGS="-O -DUNIX -DUSE_SLANG \
		-DCURS_PERFORMANCE \
		-I../$(WWWINC) -DUTMPX_FOR_UTMP -DSVR4 -DTRUE=1 -DFALSE=0 \
		-DNO_BCOPY -DUNIXWARE $(SITE_DEFS)" \
		LIBS="-lsocket -lnsl \
		$(WAISLIB) $(SOCKSLIB) $(SITE_LIBS)" \
		WWWLIB="../WWW/Library/svr4/libwww.a" \
		SLANGLIB="$(SLANGLIB) -lslang -lm" SLANGINC="$(SLANGINC)"

# SCO doesn't have or need ranlib. (ignore the error message about that :)
#     can build lynx with its native rcc compiler.  (if you replace "gcc"
#     with "rcc" for CC= :)
#     has a const/volatile bug in its native cc compiler. (don't use it ;)
sco:
	cd WWW/Library/sco; $(MAKE) CC="gcc" LYFLAGS="-DNO_FILIO_H -DUNIX \
		-DSCO $(SITE_LYDEFS)"
	cd src; $(MAKE) all CC="gcc" MCFLAGS="-O -s -DNO_FILIO_H -DUNIX -DSCO \
		-DFANCY_CURSES -DMMDF -DNO_UTMP -DNO_SIZECHANGE \
		-I../$(WWWINC) $(SITE_DEFS)" \
		LIBS="-lcurses -lsocket -ltermcap -lmalloc \
		$(WAISLIB) $(SOCKSLIB) $(SITE_LIBS)" \
		WWWLIB="../WWW/Library/sco/libwww.a"

sco5:
	cd WWW/Library/sco; $(MAKE) CC="cc -belf" LYFLAGS="-DNO_FILIO_H \
		-DUNIX -DSCO $(SITE_LYDEFS)"
	cd src; $(MAKE) all CC="cc -belf" MCFLAGS="-O -s -DNO_FILIO_H -DUNIX \
		-DSCO -DFANCY_CURSES -DMMDF -DUTMPX_FOR_UTMP \
		-I../$(WWWINC) $(SITE_DEFS)" \
		LIBS="-lcurses -lsocket \
		$(WAISLIB) $(SOCKSLIB) $(SITE_LIBS)" \
		WWWLIB="../WWW/Library/sco/libwww.a"

bsdi:
	cd WWW/Library/Implementation; $(MAKE) -f BSDI_Makefile \
		LYFLAGS="-DBSDI $(SITE_LYDEFS)"
	cd src; $(MAKE) all CC="cc" MCFLAGS="-O -DNO_CUSERID -DUNIX \
		-DNO_FILIO_H \
		-I../$(WWWINC) -DNO_UTMP -DSVR4 -DNO_KEYPAD $(SITE_DEFS)" \
		LIBS="-lcurses -ltermcap \
		$(WAISLIB) $(SOCKSLIB) $(SITE_LIBS)" \
		WWWLIB="../WWW/Library/svr4/libwww.a"

#IMPORTANT: To compile you need ncurses package (version 1.9.4 or later)
#     installed on your system. You can download ncurses package from
#     ftp://ftp.netcom.com/pub/zm/zmbenhal/ncurses
bsdi-ncurses:
	cd WWW/Library/Implementation; $(MAKE) -f BSDI_Makefile CC="gcc" \
		LYFLAGS="-DBSDI $(SITE_LYDEFS)"
	cd src; $(MAKE) all CC="gcc" MCFLAGS="-O -DNO_CUSERID -DUNIX -DNCURSES \
		-DFANCY_CURSES -DBSDI -DSVR4 \
		-I/usr/include/ncurses -I../$(WWWINC) $(SITE_DEFS)" \
		LIBS="-lncurses \
		$(WAISLIB) $(SOCKSLIB) $(SITE_LIBS)" \
		WWWLIB="../WWW/Library/svr4/libwww.a"

# define RESOLVLIB (above) for the "LIBS" entry if needed
# Solaris2 doesn't have or need ranlib. (ignore the error message about that :)
# (Solaris 2.5 and later actually have a ranlib program - but it does 
#  nothing and is only there to make Makefiles like this one happy)
solaris2:
	cd WWW/Library/solaris2; $(MAKE) CC="gcc" LYFLAGS="$(SITE_LYDEFS)"
	cd src; $(MAKE) all CC="gcc" MCFLAGS="-O -DFANCY_CURSES -DUNIX -DSVR4 \
		-DSOLARIS2 -DCURS_PERFORMANCE -DUTMPX_FOR_UTMP -DUSE_DIRENT \
		-DLOCALE -DHAVE_TERMIOS_H \
		-I../$(WWWINC) $(SITE_DEFS)" \
		LIBS="-L/usr/ccs/lib -R/usr/ccs/lib -lcurses -lnsl -lsocket \
		$(RESOLVLIB) $(WAISLIB) $(SOCKSLIB) $(SITE_LIBS)" \
		WWWLIB="../WWW/Library/solaris2/libwww.a"

# Solaris 2 using Sun's unbundled C compiler
# define RESOLVLIB (above) for the "LIBS" entry if needed
# Solaris2 doesn't have or need ranlib. (ignore the error message about that :)
solaris2cc:
	cd WWW/Library/solaris2; $(MAKE) CC="cc" LYFLAGS="$(SITE_LYDEFS)"
	cd src; $(MAKE) all CC="cc" MCFLAGS="-O -DFANCY_CURSES -DUNIX -DSVR4 \
		-DSOLARIS2 -DCURS_PERFORMANCE -DUTMPX_FOR_UTMP -DUSE_DIRENT \
		-DLOCALE -DHAVE_TERMIOS_H \
		-I../$(WWWINC) $(SITE_DEFS)" \
		LIBS="-L/usr/ccs/lib -R/usr/ccs/lib -lcurses -lnsl -lsocket \
		$(RESOLVLIB) $(WAISLIB) $(SOCKSLIB) $(SITE_LIBS)" \
		WWWLIB="../WWW/Library/solaris2/libwww.a"

# define RESOLVLIB (above) for the "LIBS" entry if needed
# Solaris2 doesn't have or need ranlib. (ignore the error message about that :)
solaris2-slang:
	cd WWW/Library/solaris2; $(MAKE) CC="gcc" LYFLAGS="$(SITE_LYDEFS)"
	cd src; $(MAKE) all CC="gcc" MCFLAGS="-O -DUNIX -DSVR4 -DSOLARIS2 \
		-DUSE_SLANG -DCURS_PERFORMANCE -DUTMPX_FOR_UTMP -DUSE_DIRENT \
		-DLOCALE -DHAVE_TERMIOS_H \
		-I../$(WWWINC) $(SITE_DEFS)" \
		LIBS="-L/usr/ccs/lib -R/usr/ccs/lib -lcurses -lnsl -lsocket \
		$(RESOLVLIB) $(WAISLIB) $(SOCKSLIB) $(SITE_LIBS)" \
		WWWLIB="../WWW/Library/solaris2/libwww.a" \
		SLANGLIB="$(SLANGLIB) $(SLANGRRLIB) -lslang -lm" \
		SLANGINC="$(SLANGINC)"

# Solaris 2 slang using Sun's unbundled C compiler
# define RESOLVLIB (above) for the "LIBS" entry if needed
# Solaris2 doesn't have or need ranlib. (ignore the error message about that :)
solaris2-slangcc:
	cd WWW/Library/solaris2; $(MAKE) CC="cc" LYFLAGS="$(SITE_LYDEFS)"
	cd src; $(MAKE) all CC="cc" MCFLAGS="-O -DUNIX -DSVR4 -DSOLARIS2 \
		-DUSE_SLANG -DCURS_PERFORMANCE -DUTMPX_FOR_UTMP -DUSE_DIRENT \
		-DLOCALE -DHAVE_TERMIOS_H \
		-I../$(WWWINC) $(SITE_DEFS)" \
		LIBS="-L/usr/ccs/lib -R/usr/ccs/lib -lcurses -lnsl -lsocket \
		$(RESOLVLIB) $(WAISLIB) $(SOCKSLIB) $(SITE_LIBS)" \
		WWWLIB="../WWW/Library/solaris2/libwww.a" \
		SLANGLIB="$(SLANGLIB) $(SLANGRRLIB) -lslang -lm" \
		SLANGINC="$(SLANGINC)"
  
osf:
	cd WWW/Library/osf; $(MAKE) LYFLAGS="-Olimit 2000 $(SITE_LYDEFS)"
	cd src; $(MAKE) all CC="cc" MCFLAGS="-O -Olimit 4000 -DUNIX \
		-DFANCY_CURSES $(ADDFLAGS) \
		-I../$(WWWINC) $(SITE_DEFS)" \
		LIBS="-lcurses -ltermcap \
		$(WAISLIB) $(SOCKSLIB) $(SITE_LIBS)" \
		WWWLIB="../WWW/Library/osf/libwww.a"

osf-slang:
	cd WWW/Library/osf; $(MAKE) LYFLAGS="-Olimit 2000 $(SITE_LYDEFS)"
	cd src; $(MAKE) all CC="cc" MCFLAGS="-O -Olimit 4000 -DUNIX \
		-DUSE_SLANG $(ADDFLAGS) \
		-I../$(WWWINC) $(SITE_DEFS)" \
		LIBS="-lcurses -ltermcap \
		$(WAISLIB) $(SOCKSLIB) $(SITE_LIBS)" \
		WWWLIB="../WWW/Library/osf/libwww.a" \
		SLANGLIB="$(SLANGLIB) -lslang -lm" SLANGINC="$(SLANGINC)"

# for some reason loc_t isn't defined when compiling for debug on my system.
# needed for NLchar.h
dbg:
	cd WWW/Library/osf; $(MAKE) CC="gcc" LYFLAGS="-DDIRED_SUPPORT \
		-DLY_FIND_LEAKS"
	cd src; $(MAKE) all CC="gcc" MCFLAGS="-O -Wall $(ADDFLAGS) \
		-DFANCY_CURSES -DLY_FIND_LEAKS \
		-Dloc_t=_LC_locale_t -D_locp=__lc_locale\
		-DDIRED_SUPPORT -DOK_TAR -DOK_GZIP -DOK_OVERRIDE \
		-DUNIX -I../$(WWWINC) -DEXEC_LINKS \
		-DALLOW_USERS_TO_CHANGE_EXEC_WITHIN_OPTIONS $(SITE_DEFS)" \
		LIBS="-lcurses -ltermcap \
		$(WAISLIB) $(SOCKSLIB) $(SITE_LIBS)" \
		WWWLIB="../WWW/Library/osf/libwww.a"

sgi:
# SGI doesn't have or need ranlib. (ignore the error message about that :)
	cd WWW/Library/sgi; $(MAKE) LYFLAGS="$(SITE_LYDEFS)"
	cd src; $(MAKE) all CC="cc" MCFLAGS="-O -cckr -DFANCY_CURSES \
		-DNO_CUSERID -DUNIX -DSGI -Dvfork=fork \
		-I../$(WWWINC) -DNO_UTMP $(SITE_DEFS)" \
		LIBS="-lcurses -ltermcap -DSYSV -DPERFORMANCE \
		$(WAISLIB) $(SOCKSLIB) $(SITE_LIBS)" \
		WWWLIB="../WWW/Library/sgi/libwww.a"

linux:
	cd WWW/Library/unix; $(MAKE) CC="gcc" LYFLAGS="-DLINUX $(SITE_LYDEFS)"
	cd src; $(MAKE) all CC="gcc" MCFLAGS="-O -DUNIX -DLINUX \
		-DNO_KEYPAD -DNO_TTYTYPE \
		-I../$(WWWINC) $(SITE_DEFS)" \
		LIBS="-lcurses -ltermcap \
		$(WAISLIB) $(SOCKSLIB) $(SITE_LIBS)" \
		WWWLIB="../WWW/Library/unix/libwww.a"

linux-ncurses:
	cd WWW/Library/unix; $(MAKE) CC="gcc" LYFLAGS="-DLINUX $(SITE_LYDEFS)"
	cd src; $(MAKE) all CC="gcc" MCFLAGS="-O -DUNIX -DLINUX -DNCURSES \
		-DFANCY_CURSES -DNO_TTYTYPE \
		-I/usr/include/ncurses -I../$(WWWINC) $(SITE_DEFS)" \
		LIBS="-lncurses \
		$(WAISLIB) $(SOCKSLIB) $(SITE_LIBS)" \
		WWWLIB="../WWW/Library/unix/libwww.a"

linux-slang:
	cd WWW/Library/unix; $(MAKE) CC="gcc" LYFLAGS="-DLINUX $(SITE_LYDEFS)"
	cd src; $(MAKE) all CC="gcc" MCFLAGS="-O -DUNIX -DLINUX -DUSE_SLANG \
		-DNO_KEYPAD -DNO_TTYTYPE \
		-I../$(WWWINC) $(SITE_DEFS)" \
		LIBS="$(WAISLIB) $(SOCKSLIB) $(SITE_LIBS)" \
		WWWLIB="../WWW/Library/unix/libwww.a" \
		SLANGLIB="$(SLANGLIB) -lslang -lm" SLANGINC="$(SLANGINC)"

##Various AIX environments
aix:
	@echo "Run make again and use \"make aix4\" or \"make aix32\""

# For AIX 4.n, and AIX 3.2.5 with POWER2 architectures, L2 caches,
# and APAR#IX58890
aix4:
	cd WWW/Library/rs6000; $(MAKE) LYFLAGS="-D_BSD=44 \
		$(SITE_LYDEFS)"
	cd src; $(MAKE) all CC="cc" MCFLAGS="-O -DFANCY_CURSES -DUNIX -DAIX4 \
		-D_BSD=44 -I../$(WWWINC) $(SITE_DEFS)" \
		LIBS="-lcurses -lbsd \
		$(WAISLIB) $(SOCKSLIB) $(SITE_LIBS)" \
		WWWLIB="../WWW/Library/rs6000/libwww.a"

aix32:
	cd WWW/Library/rs6000; $(MAKE) LYFLAGS="$(SITE_LYDEFS)"
	cd src; $(MAKE) all CC="cc" MCFLAGS="-O -DFANCY_CURSES -DUNIX \
		-I../$(WWWINC) $(SITE_DEFS)" \
		LIBS="-lcurses -lbsd \
		$(WAISLIB) $(SOCKSLIB) $(SITE_LIBS)" \
		WWWLIB="../WWW/Library/rs6000/libwww.a"

aix31:
	cd WWW/Library/rs6000; $(MAKE) LYFLAGS="$(SITE_LYDEFS)"
	cd src; $(MAKE) all CC="cc" MCFLAGS="-O -DUNIX \
		-I../$(WWWINC) $(SITE_DEFS)" \
		LIBS="-lcurses -lbsd \
		$(WAISLIB) $(SOCKSLIB) $(SITE_LIBS)" \
		WWWLIB="../WWW/Library/rs6000/libwww.a"

##ultrix - DEC Ultrix
ultrix:
	cd WWW/Library/decstation; $(MAKE) LYFLAGS="$(SITE_LYDEFS)"
	cd src; $(MAKE) all CC="cc" MCFLAGS="-O -Olimit 600 -DFANCY_CURSES \
		-DUNIX -DULTRIX -I../$(WWWINC) $(SITE_DEFS)" \
		LIBS="-lcursesX -ltermcap \
		$(WAISLIB) $(SOCKSLIB) $(SITE_LIBS)" \
		WWWLIB="../WWW/Library/decstation/libwww.a"

ultrix-slang:
	cd WWW/Library/decstation; $(MAKE) LYFLAGS="$(SITE_LYDEFS)"
	cd src; $(MAKE) all CC="cc" MCFLAGS="-O -Olimit 600 -DFANCY_CURSES \
		-DUSE_SLANG -DUNIX -DULTRIX -I../$(WWWINC) $(SITE_DEFS)" \
		LIBS="-lcursesX -ltermcap \
		$(WAISLIB) $(SOCKSLIB) $(SITE_LIBS)" \
		WWWLIB="../WWW/Library/decstation/libwww.a" \
		SLANGLIB="$(SLANGLIB) -lslang -lm" SLANGINC="$(SLANGINC)"

##Various SunOS environments
sun:
	@echo "Run make again and use \"make sun3\" or \"make sun4\""

# define RESOLVLIB (above) for the "LIBS" entry if needed
sun3:
	cd WWW/Library/sun3; $(MAKE) LYFLAGS="$(SITE_LYDEFS)" CC="gcc"
	cd src; $(MAKE) all CC="gcc" MCFLAGS="-O -DUNIX -DFANCY_CURSES \
		-I../$(WWWINC) -DSUN -DLOCALE -I/usr/5include $(SITE_DEFS)"\
		LIBS="-lcurses -ltermcap -L/usr/5lib \
		$(RESOLVLIB) $(WAISLIB) $(SOCKSLIB) $(SITE_LIBS)" \
		WWWLIB="../WWW/Library/sun3/libwww.a"

# define RESOLVLIB (above) for the "LIBS" entry if needed
sun4:
	cd WWW/Library/sun4; $(MAKE) CC="gcc" LYFLAGS="$(CCFLAGS) \
		$(SITE_LYDEFS)"
	cd src; $(MAKE) all CC="gcc" MCFLAGS="$(CCFLAGS) -O -DUNIX -DSUN \
		-DSUN4 -DFANCY_CURSES -DLOCALE \
		-I../$(WWWINC) -I/usr/5include \
		$(SITE_DEFS)" \
		LIBS="-lcurses -ltermcap -L/usr/5lib \
		$(RESOLVLIB) $(WAISLIB) $(SOCKSLIB) $(SITE_LIBS)" \
		WWWLIB="../WWW/Library/sun4/libwww.a"

# define RESOLVLIB (above) for the "LIBS" entry if needed
sun4-ncurses:
	cd WWW/Library/sun4; $(MAKE) CC="gcc" LYFLAGS="$(SITE_LYDEFS)"
	cd src; $(MAKE) all CC="gcc" \
		MCFLAGS="-O -DUNIX -DFANCY_CURSES -DNCURSES \
		-I/usr/local/include -I../$(WWWINC) -DSUN -DSUN4 \
		-DLOCALE $(SITE_DEFS)" \
		LIBS="-L/usr/local/lib -lncurses \
		$(RESOLVLIB) $(WAISLIB) $(SOCKSLIB) $(SITE_LIBS)" \
		WWWLIB="../WWW/Library/sun4/libwww.a"

# use the holiest of holiest --- PURIFY
# define RESOLVLIB (above) for the "LIBS" entry if needed
sun4-pure:
	cd WWW/Library/sun4; $(MAKE) CC="gcc" LYFLAGS="$(SITE_LYDEFS)"
	cd src; $(MAKE) all CC="purify -cache-dir=/home/purify-cache \
		-always-use-cache-dir=yes -optimize-save-o7=no gcc" \
		MCFLAGS="-O -DUNIX -DFANCY_CURSES \
		-I../$(WWWINC) -DSUN -DSUN4 -DLOCALE -I/usr/5include \
		$(SITE_DEFS)" \
		LIBS="-lcurses -ltermcap -L/usr/5lib \
		$(RESOLVLIB) $(WAISLIB) $(SOCKSLIB) $(SITE_LIBS)" \
		WWWLIB="../WWW/Library/sun4/libwww.a"

# define RESOLVLIB (above) for the "LIBS" entry if needed
sun4-slang:
	cd WWW/Library/sun4; $(MAKE) CC="gcc" LYFLAGS="$(CCFLAGS) \
		$(SITE_LYDEFS)"
	cd src; $(MAKE) all CC="gcc" MCFLAGS="-O -DUNIX -DSUN -DSUN4 \
		-DUSE_SLANG -DLOCALE \
		-I../$(WWWINC) -I/usr/5include \
		$(SITE_DEFS)" \
		LIBS="-lcurses -ltermcap -L/usr/5lib \
		$(RESOLVLIB) $(WAISLIB) $(SOCKSLIB) $(SITE_LIBS)" \
		WWWLIB="../WWW/Library/sun4/libwww.a" \
		SLANGLIB="$(SLANGLIB) -lslang -lm" SLANGINC="$(SLANGINC)"

# news - SONY NEWS-OS 4.2.1R
news:
	@echo "You must first compile the WWW library in WWW/Library"
	cd WWW/Library/unix; $(MAKE) LYFLAGS="$(SITE_LYDEFS)"
	cd src; $(MAKE) all CC="gcc -O" MCFLAGS="-O -DUNIX -DNO_KEYPAD \
		-DNO_CUSERID -DLOCALE \
		-I../$(WWWINC) $(SITE_DEFS)" \
		LIBS="-ljcurses -ltermcap $(WAISLIB) $(SOCKSLIB)" \
		WWWLIB="../WWW/Library/unix/libwww.a"

news-ncurses:
	@echo "You must first compile the WWW library in WWW/Library"
	cd WWW/Library/unix; $(MAKE) LYFLAGS="$(SITE_LYDEFS)"
	cd src; $(MAKE) all CC="gcc -O" MCFLAGS="-O -DUNIX \
		-DNO_CUSERID -DNCURSES -DFANCY_CURSES \
		-I../$(WWWINC) $(SITE_DEFS)" \
		LIBS="-lncurses $(WAISLIB) $(SOCKSLIB)" \
		WWWLIB="../WWW/Library/unix/libwww.a"

news-slang:
	@echo "You must first compile the WWW library in WWW/Library"
	cd WWW/Library/unix; $(MAKE) LYFLAGS="$(SITE_LYDEFS)"
	cd src; $(MAKE) all CC="gcc -O" MCFLAGS="-O -DUNIX -DNO_KEYPAD \
		-DNO_CUSERID -DUSE_SLANG -DLOCALE \
		-I../$(WWWINC) $(SITE_DEFS)" \
		LIBS="$(WAISLIB) $(SOCKSLIB)" \
		WWWLIB="../WWW/Library/unix/libwww.a" \
		SLANGLIB="$(SLANGLIB) -lslang" SLANGINC="$(SLANGINC)"

# next - Nextstep
next:
	cd WWW/Library/next; $(MAKE) LYFLAGS="$(SITE_LYDEFS)"
	cd src; $(MAKE) all CC="cc" MCFLAGS="-O -DUNIX -DNEXT -DNO_UNISTD_H \
		-DNO_CUSERID -DNO_GETCWD -DNO_PUTENV -DNO_KEYPAD \
		-I../$(WWWINC) $(SITE_DEFS)" \
		LIBS="-lcurses -ltermcap \
		$(WAISLIB) $(SOCKSLIB) $(SITE_LIBS)" \
		WWWLIB="../WWW/Library/next/libwww.a"

next-ncurses:
	cd WWW/Library/next; $(MAKE) LYFLAGS="$(SITE_LYDEFS)"
	cd src; $(MAKE) all CC="cc" MCFLAGS="-O -DUNIX -DNEXT -DNO_UNISTD_H \
		-DNCURSES -DFANCY_CURSES \
		-DNO_CUSERID -DNO_GETCWD -DNO_PUTENV \
		-I/usr/local/include -I../$(WWWINC) $(SITE_DEFS)" \
		LIBS="-L/usr/local/lib -lncurses \
		$(WAISLIB) $(SOCKSLIB) $(SITE_LIBS)" \
		WWWLIB="../WWW/Library/next/libwww.a"

# isc - Interactive Unix for i386 and i486 machines
# According to Robert Salter (salter1@mster.nsbf.nasa.gov),
#  cc should be used instead of gcc.
# According to Walter Skorski (walt@clyde.jmp.tju.edu),
#  -DPOSIX_JC is necessary, else LYBookmark.c never finds out
#  what a mode_t is.
isc:
	cd WWW/Library/isc; $(MAKE) CC="cc" LYFLAGS="$(SITE_LYDEFS)"
	cd src; $(MAKE) all CC="cc" MCFLAGS="-O -DISC -DFANCY_CURSES -DUNIX \
		-DNO_UTMP -Dvfork=fork -DPOSIX_JC -I../$(WWWINC) $(SITE_DEFS)" \
		LIBS="-lcurses -ltermcap -linet -lnsl_s -lcposix \
		$(WAISLIB) $(SOCKSLIB) $(SITE_LIBS)" \
		WWWLIB="../WWW/Library/isc/libwww.a"

##Miscellaneous actions
clean:
	rm -f WWW/Library/*/*.[aob]
	rm -f WWW/Library/*/.created
	cd src; $(MAKE) clean
	rm -f *.b src/lynx core Lynx.leaks

tar:  clean
	rm -f lynx
	$(MAKE) save

turnover:	clean
	$(MAKE) save

zipcompress: compress zip
	echo "done!"

zip:  clean
	rm -f src/lynx
	rm -f lynx
	rm -f src/a.out
	rm -f ../$(lynxname).zip
	cd ..; rm -f $(lynxname).zip; zip -r $(lynxname).zip $(lynxdir)

save:
	cd ..; rm -f $(lynxname).tar; tar -cf - $(lynxdir) > $(lynxname).tar

compress: tar
	cd ..; rm -f $(lynxname).tar.Z; compress -f -v $(lynxname).tar

install:
	-mv -f $(exec)/lynx $(exec)/lynx.old
	-mv -f $(cfg)/lynx.cfg $(cfg)/lynx.oldcfg
	$(installbin) lynx $(exec)/lynx
	$(installdoc) lynx.man $(doc)/lynx.1
	$(installdoc) lynx.cfg $(cfg)/lynx.cfg
