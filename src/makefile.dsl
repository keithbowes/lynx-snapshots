OBJS= UCdomap.o UCAux.o UCAuto.o \
LYClean.o LYShowInfo.o LYEdit.o LYStrings.o \
LYMail.o HTAlert.o GridText.o LYGetFile.o \
LYMain.o LYMainLoop.o LYCurses.o LYBookmark.o LYUtils.o \
LYOptions.o LYReadCFG.o LYSearch.o LYHistory.o \
LYForms.o LYPrint.o LYrcFile.o LYDownload.o LYNews.o LYKeymap.o \
HTML.o HTFWriter.o HTInit.o DefaultStyle.o LYLocal.o LYUpload.o \
LYLeaks.o LYexit.o LYJump.o LYList.o LYCgi.o LYTraversal.o \
LYEditmap.o LYCharSets.o LYCharUtils.o LYMap.o LYCookie.o LYExtern.o \
LYStyle.o LYHash.o LYPrettySrc.o TRSTable.o

CFLAGS= $(MCFLAGS) $(INTLFLAGS) -I. -I.. $(SLANGINC)

# comment this line to suppress DIRED support
DIRED_DEFS = -DDIRED_SUPPORT -DOK_UUDECODE -DOK_TAR -DOK_GZIP -DOK_ZIP

CC = gcc
MCFLAGS = -O2 -DDISP_PARTIAL -DUSE_ZLIB -DUSE_EXTERNALS \
-DWATT32 \
$(DIRED_DEFS) \
-DSOURCE_CACHE -DUSE_PSRC \
-DUSE_SLANG -DDJGPP_KEYHANDLER -DACCESS_AUTH -DNO_CUSERID \
-DNOUSERS -DDOSPATH -DNO_TTYTYPE -DNO_UTMP -I../WWW/Library/Implementation \
-I../djgpp/watt32/inc -I./chrtrans -I../djgpp/watt32/inc/sys
WWWLIB = ../WWW/Library/djgpp/libwww.a ../djgpp/watt32/lib/libwatt.a
LIBS= -lslang -lz # -lintl
CHRTR= ./chrtrans/
#INTLFLAGS = -DHAVE_GETTEXT -DHAVE_LIBINTL_H

all: lynx.exe

lynx.exe:   message $(OBJS) $(WWWLIB)
	@echo "Linking and creating Lynx executable"
	$(CC) $(CFLAGS) -o lynx.exe  $(OBJS) $(WWWLIB) $(SLANGLIB) $(LIBS)
	@echo "Welcome to Lynx!"

message:
	@echo "Compiling Lynx sources"

dbg:    $(OBJS) $(WWWLIB)
	@echo "Making Lynx code"
	$(CC) $(OBJS) $(CFLAGS) $(WWWLIB) $(SLANGLIB) $(LIBS)

lint:
	lint *.c  > ../lint.out

clean:
	rm -f lynx.exe core *.[ob]

DefaultStyle.o:	../userdefs.h
HTFWriter.o:	../userdefs.h
LYBookmark.o:	../userdefs.h
LYCharSets.o:	../userdefs.h
LYCharUtils.o:	../userdefs.h
LYCookie.o:	../userdefs.h
LYDownload.o:	../userdefs.h
LYEditmap.o:	../userdefs.h
LYExtern.o:	../userdefs.h
LYGetFile.o:	../userdefs.h
LYHistory.o:	../userdefs.h
LYKeymap.o:	../userdefs.h
LYMain.o:	../userdefs.h
LYMainLoop.o:	../userdefs.h
LYOptions.o:	../userdefs.h
LYReadCFG.o:	../userdefs.h
LYReadCFG.o:	../userdefs.h
LYShowInfo.o:	../userdefs.h
LYStrings.o:	../userdefs.h
LYTraversal.o:	../userdefs.h
LYUtils.o:	../userdefs.h
