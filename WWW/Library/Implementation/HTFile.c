/*			File Access				HTFile.c
**			===========
**
**	This is unix-specific code in general, with some VMS bits.
**	These are routines for file access used by browsers.
**	Development of this module for Unix DIRED_SUPPORT in Lynx
**	 regrettably has has been conducted in a manner with now
**	 creates a major impediment for hopes of adapting Lynx to
**	 a newer version of the library.
**
**  History:
**	   Feb 91	Written Tim Berners-Lee CERN/CN
**	   Apr 91	vms-vms access included using DECnet syntax
**	26 Jun 92 (JFG) When running over DECnet, suppressed FTP.
**			Fixed access bug for relative names on VMS.
**	   Sep 93 (MD)  Access to VMS files allows sharing.
**	15 Nov 93 (MD)	Moved HTVMSname to HTVMSUTILS.C
**      27 Dec 93 (FM)  FTP now works with VMS hosts.
**			FTP path must be Unix-style and cannot include
**			 the device or top directory.
*/

#ifndef VMS
/* #define LONG_LIST */ /* Define this for long style unix listings (ls -l) */
/* #define NO_PARENT_DIR_REFERENCE */ /* Define this for no parent links */
#endif /* !VMS */

#ifdef DOSPATH
#define HAVE_READDIR
#include <dirent.h>
#define USE_DIRENT
#include "HTDOS.h"
#endif

#include "HTUtils.h"
#include "tcp.h"
#include "HTFile.h"		/* Implemented here */
#ifdef VMS
#include <stat.h>
#endif /* VMS */

#ifndef VMS
#ifdef LONG_LIST
#include <pwd.h>
#include <grp.h>
#endif /* LONG_LIST */
#endif /* !VMS */
  
#define INFINITY 512		/* file name length @@ FIXME */
#define MULTI_SUFFIX ".multi"   /* Extension for scanning formats */

#define HT_EM_SPACE ((char)2)

#define FREE(x) if (x) {free(x); x = NULL;}

#ifdef VMS
#include "HTVMSUtils.h"
#endif /* VMS */

#include "HTParse.h"
#include "HTTCP.h"
#ifndef DECNET
#include "HTFTP.h"
#endif /* !DECNET */
#include "HTAnchor.h"
#include "HTAtom.h"
#include "HTWriter.h"
#include "HTFWriter.h"
#include "HTInit.h"
#include "HTBTree.h"
#include "HTAlert.h"
#include "HTCJK.h"
#ifdef EXP_CHARTRANS
#include "UCDefs.h"
#include "UCMap.h"
#include "UCAux.h"
#endif /* EXP_CHARTRANS */

#include "LYexit.h"
#include "LYLeaks.h"

typedef struct _HTSuffix {
	char *		suffix;
	HTAtom *	rep;
	HTAtom *	encoding;
	float		quality;
} HTSuffix;

#ifndef NGROUPS
#ifdef NGROUPS_MAX
#define NGROUPS NGROUPS_MAX
#else
#define NGROUPS 32
#endif /* NGROUPS_MAX */
#endif /* NGROUPS */

#include "HTML.h"		/* For directory object building */

#define PUTC(c) (*target->isa->put_character)(target, c)
#define PUTS(s) (*target->isa->put_string)(target, s)
#define START(e) (*target->isa->start_element)(target, e, 0, 0, 0)
#define END(e) (*target->isa->end_element)(target, e, 0)
#define MAYBE_END(e) if (HTML_dtd.tags[e].contents != SGML_EMPTY) \
                        (*target->isa->end_element)(target, e, 0)
#define FREE_TARGET (*target->isa->_free)(target)
struct _HTStructured {
	CONST HTStructuredClass *	isa;
	/* ... */
};

/*
**  Controlling globals.
*/
PUBLIC int HTDirAccess = HT_DIR_OK;

#ifdef DIRED_SUPPORT
PUBLIC int HTDirReadme = HT_DIR_README_NONE;
#define FILES_FIRST 1
#define MIXED_STYLE 2
extern BOOLEAN lynx_edit_mode;
extern BOOLEAN dir_list_style;
#else
PUBLIC int HTDirReadme = HT_DIR_README_TOP;
#endif /* DIRED_SUPPORT */

extern int current_char_set;
extern char *LYchar_set_names[];
extern BOOL HTPassEightBitRaw;
extern HTCJKlang HTCJK;
#ifndef EXP_CHARTRANS
#define UCLYhndl_HTFile_for_unspec 0 /* a dummy define */
#endif

PRIVATE char *HTMountRoot = "/Net/";		/* Where to find mounts */
#ifdef VMS
PRIVATE char *HTCacheRoot = "/WWW$SCRATCH";	/* Where to cache things */
#else
PRIVATE char *HTCacheRoot = "/tmp/W3_Cache_";	/* Where to cache things */
#endif /* VMS */

/*PRIVATE char *HTSaveRoot  = "$(HOME)/WWW/";*/	/* Where to save things */

/*
**  Suffix registration.
*/
PRIVATE HTList * HTSuffixes = 0;
PRIVATE HTSuffix no_suffix = { "*", NULL, NULL, 1.0 };
PRIVATE HTSuffix unknown_suffix = { "*.*", NULL, NULL, 1.0};


#ifdef _WINDOWS
int exists(char *filename)
{
 return (access(filename,0)==0);
}
#endif


/*	To free up the suffixes at program exit.
**	----------------------------------------
*/
PRIVATE void free_suffixes NOPARAMS;

#ifdef LONG_LIST
PRIVATE void LYListFmtParse ARGS5(
	char *,		fmtstr,
	char *,		file,
	HTStructured *,	target,
	char *,		entry,
	char *,		tail)
{
	char c;
	char *s;
	char *end;
	char *start;
	char *str = NULL;
	struct stat st;
	char buf[512];
	char fmt[512];
	char type;
	struct passwd *p;
	struct group *g;
	time_t now;
	char *datestr;
	int len;
#define SEC_PER_YEAR	(60 * 60 * 24 * 365)
	static char *pbits[] = { "---", "--x", "-w-", "-wx",
		"r--", "r-x", "rw-", "rwx", 0 };
	static char *psbits[] = { "--S", "--s", "-wS", "-ws",
		"r-S", "r-s", "rwS", "rws", 0 };
#define PBIT(a, n, s)  (s) ? psbits[((a) >> (n)) & 0x7] : \
	pbits[((a) >> (n)) & 0x7]

	if (lstat(file, &st) < 0)
		fmtstr = "%a";	/* can't stat so just do anchor */
	
	StrAllocCopy(str, fmtstr);
	s = str;
	end = str + strlen(str);
	START(HTML_PRE);
	while (*s) {
		start = s;
		while (*s) {
			if (*s == '%') {
				if (*(s+1) == '%') /* literal % */
					s++;
				else
					break;
			}
			s++;
		}
		/* s is positioned either at a % or at \0 */
		*s = '\0';
		if (s > start) {	/* some literal chars. */
			PUTS(start);
		}
		if (s == end)
			break;
		start = ++s;
		while (isdigit(*s) || *s == '.' || *s == '-')
			s++;
		c = *s;		/* the format char. or \0 */
		*s = '\0';

		switch (c) {
		case '\0':
			break;
		
		case 'A':
		case 'a':	/* anchor */
			HTDirEntry(target, tail, entry);
			sprintf(fmt, "%%%ss", start);
			sprintf(buf, fmt, entry);
			PUTS(buf);
			END(HTML_A);
			if (c != 'A' && (st.st_mode & S_IFMT) == S_IFLNK &&
			    (len = readlink(file, buf, sizeof(buf))) >= 0) {
				PUTS(" -> ");
				buf[len] = '\0';
				PUTS(buf);
			}
			*buf = '\0';
			break;

		case 'd':	/* date */
			now = time(0);
			datestr = ctime(&st.st_mtime);
			if ((now - st.st_mtime) < SEC_PER_YEAR/2)
				/*
				**  MMM DD HH:MM
				*/
				sprintf(buf, "%.12s", datestr + 4);
			else
				/*
				**  MMM DD  YYYY
				*/
				sprintf(buf, "%.7s %.4s ", datestr + 4,
				  datestr + 20);
			sprintf(fmt, "%%%ss", start);
			sprintf(buf, fmt, buf);
			break;

		case 's':	/* size in bytes */
			sprintf(fmt, "%%%sd", start);
			sprintf(buf, fmt, st.st_size);
			break;

		case 'K':	/* size in Kilobytes but not for directories */
			if ((st.st_mode & S_IFMT) == S_IFDIR) {
				sprintf(fmt, "%%%ss ", start);
				sprintf(buf, fmt, "");
				break;
			}
			/* FALL THROUGH */
		case 'k':	/* size in Kilobytes */
			sprintf(fmt, "%%%sdK", start);
			sprintf(buf, fmt, (st.st_size+1023)/1024);
			break;

		case 'p':	/* unix-style permission bits */
			switch(st.st_mode & S_IFMT) {
			case S_IFIFO: type = 'p'; break;
			case S_IFCHR: type = 'c'; break;
			case S_IFDIR: type = 'd'; break;
			case S_IFBLK: type = 'b'; break;
			case S_IFREG: type = '-'; break;
			case S_IFLNK: type = 'l'; break;
#ifdef S_IFSOCK
			case S_IFSOCK: type = 's'; break;
#endif /* S_IFSOCK */
			default: type = '?'; break;
			}
			sprintf(buf, "%c%s%s%s", type,
			  PBIT(st.st_mode, 6, st.st_mode & S_ISUID),
			  PBIT(st.st_mode, 3, st.st_mode & S_ISGID),
			  PBIT(st.st_mode, 0, 0));
			sprintf(fmt, "%%%ss", start);
			sprintf(buf, fmt, buf);
			break;

		case 'o':	/* owner */
			sprintf(fmt, "%%%ss", start);
			p = getpwuid(st.st_uid);
			if (p) {
				sprintf(fmt, "%%%ss", start);
				sprintf(buf, fmt, p->pw_name);
			} else {
				
				sprintf(fmt, "%%%sd", start);
				sprintf(buf, fmt, st.st_uid);
			}
			break;

		case 'g':	/* group */
			g = getgrgid(st.st_gid);
			if (g) {
				sprintf(fmt, "%%%ss", start);
				sprintf(buf, fmt, g->gr_name);
			} else {
				sprintf(fmt, "%%%sd", start);
				sprintf(buf, fmt, st.st_gid);
			}
			break;

		case 'l':	/* link count */
			sprintf(fmt, "%%%sd", start);
			sprintf(buf, fmt, st.st_nlink);
			break;

		default:
			fprintf(stderr,
			"Unknown format character `%c' in list format\n", c);
			break;
		}
		PUTS(buf);

		s++;
	}
	END(HTML_PRE);
	PUTS("\n");
	FREE(str);
}
#endif /* LONG_LIST */

/*	Define the representation associated with a file suffix.
**	--------------------------------------------------------
**
**	Calling this with suffix set to "*" will set the default
**	representation.
**	Calling this with suffix set to "*.*" will set the default
**	representation for unknown suffix files which contain a ".".
**
**	If filename suffix is already defined its previous
**	definition is overridden.
*/
PUBLIC void HTSetSuffix ARGS4(
	CONST char *,	suffix,
	CONST char *,	representation,
	CONST char *,	encoding,
	float,		value)
{
    HTSuffix * suff;

    if (strcmp(suffix, "*") == 0)
        suff = &no_suffix;
    else if (strcmp(suffix, "*.*") == 0)
        suff = &unknown_suffix;
    else {
	HTList *cur = HTSuffixes;

	while (NULL != (suff = (HTSuffix*)HTList_nextObject(cur))) {
	    if (suff->suffix && 0 == strcmp(suff->suffix, suffix))
		break;
	}
	if (!suff) { /* Not found -- create a new node */
	    suff = (HTSuffix *) calloc(1, sizeof(HTSuffix));
	    if (suff == NULL)
	        outofmem(__FILE__, "HTSetSuffix");

	    /*
            **	Memory leak fixed.
	    **	05-28-94 Lynx 2-3-1 Garrett Arch Blythe
	    */	
	    if (!HTSuffixes)	{
		HTSuffixes = HTList_new();
		atexit(free_suffixes);
	    }

	    HTList_addObject(HTSuffixes, suff);
	
	    StrAllocCopy(suff->suffix, suffix);
	}
    }

    suff->rep = HTAtom_for(representation);
   
    /*
    **	Memory leak fixed.
    **	05-28-94 Lynx 2-3-1 Garrett Arch Blythe
    **	Invariant code removed.
    */
    suff->encoding = HTAtom_for(encoding);
    
    suff->quality = value;
}

/*
**	Purpose:	Free all added suffixes.
**	Arguments:	void
**	Return Value:	void
**	Remarks/Portability/Dependencies/Restrictions:
**		To be used at program exit.
**	Revision History:
**		05-28-94	created Lynx 2-3-1 Garrett Arch Blythe
*/
PRIVATE void free_suffixes NOARGS
{
    HTSuffix * suff = NULL;

    /*
    **  Loop through all suffixes.
    */
    while (!HTList_isEmpty(HTSuffixes)) {
	/*
	**  Free off each item and its members if need be.
	*/
	suff = (HTSuffix *)HTList_removeLastObject(HTSuffixes);
	FREE(suff->suffix);
	FREE(suff);
    }
    /*
    **  Free off the list itself.
    */
    HTList_delete(HTSuffixes);
    HTSuffixes = NULL;
}

/*	Send README file.
**	-----------------
**
**  If a README file exists, then it is inserted into the document here.
*/
#ifdef HAVE_READDIR
PRIVATE void do_readme ARGS2(HTStructured *, target, CONST char *, localname)
{ 
    FILE * fp;
    char * readme_file_name = 
	malloc(strlen(localname)+ 1 + strlen(HT_DIR_README_FILE) + 1);
    if (readme_file_name == NULL)
        outofmem(__FILE__, "do_readme");
    strcpy(readme_file_name, localname);
    strcat(readme_file_name, "/");
    strcat(readme_file_name, HT_DIR_README_FILE);
    
    fp = fopen(readme_file_name,  "r");
    
    if (fp) {
	HTStructuredClass targetClass;
	
	targetClass =  *target->isa;	/* (Can't init agregate in K&R) */
	START(HTML_PRE);
	for (;;){
	    char c = fgetc(fp);
	    if (c == (char)EOF) break;
#ifdef NOTDEFINED
	    switch (c) {
	    	case '&':
		case '<':
		case '>':
			PUTC('&');
			PUTC('#');
			PUTC((char)(c / 10));
			PUTC((char) (c % 10));
			PUTC(';');
			break;
/*	    	case '\n':
			PUTC('\r');    
Bug removed thanks to joe@athena.mit.edu */			
		default:
			PUTC(c);
	    }
#else
	    PUTC(c);
#endif /* NOTDEFINED */
	}
	END(HTML_PRE);
	fclose(fp);
    } 
}
#endif /* HAVE_READDIR */

/*	Make the cache file name for a W3 document.
**	-------------------------------------------
**	Make up a suitable name for saving the node in
**
**	E.g.	/tmp/WWW_Cache_news/1234@cernvax.cern.ch
**		/tmp/WWW_Cache_http/crnvmc/FIND/xx.xxx.xx
**
**  On exit:
**	Returns	a malloc'ed string which must be freed by the caller.
*/
PUBLIC char * HTCacheFileName ARGS1(
	CONST char *,	name)
{
    char * access = HTParse(name, "", PARSE_ACCESS);
    char * host = HTParse(name, "", PARSE_HOST);
    char * path = HTParse(name, "", PARSE_PATH+PARSE_PUNCTUATION);
    
    char * result;
    result = (char *)malloc(
	    strlen(HTCacheRoot)+strlen(access)
	    +strlen(host)+strlen(path)+6+1);
    if (result == NULL)
        outofmem(__FILE__, "HTCacheFileName");
    sprintf(result, "%s/WWW/%s/%s%s", HTCacheRoot, access, host, path);
    FREE(path);
    FREE(access);
    FREE(host);
    return result;
}

/*	Open a file for write, creating the path.
**	-----------------------------------------
*/
#ifdef NOT_IMPLEMENTED
PRIVATE int HTCreatePath ARGS1(CONST char *,path)
{
    return -1;
}
#endif /* NOT_IMPLEMENTED */

/*	Convert filenames between local and WWW formats.
**	------------------------------------------------
**	Make up a suitable name for saving the node in
**
**	E.g.	$(HOME)/WWW/news/1234@cernvax.cern.ch
**		$(HOME)/WWW/http/crnvmc/FIND/xx.xxx.xx
**
**  On exit:
**	Returns	a malloc'ed string which must be freed by the caller.
*/
PUBLIC char * HTLocalName ARGS1(
	CONST char *,	name)
{
    char * access = HTParse(name, "", PARSE_ACCESS);
    char * host = HTParse(name, "", PARSE_HOST);
    char * path = HTParse(name, "", PARSE_PATH+PARSE_PUNCTUATION);
    
    HTUnEscape(path);	/* Interpret % signs */
    
    if (0 == strcmp(access, "file")) { /* local file */
        FREE(access);	
	if ((0 == strcasecomp(host, HTHostName())) ||
	    (0 == strcasecomp(host, "localhost")) || !*host) {
	    FREE(host);
	    if (TRACE)
	        fprintf(stderr, "Node `%s' means path `%s'\n", name, path);
#ifdef DOSPATH
		 return(HTDOS_name(path));
#else
	    return(path);
#endif /* DOSPATH */
	} else {
	    char * result = (char *)malloc(
	    			strlen("/Net/")+strlen(host)+strlen(path)+1);
              if (result == NULL)
	          outofmem(__FILE__, "HTLocalName");
	    sprintf(result, "%s%s%s", "/Net/", host, path);
	    FREE(host);
	    FREE(path);
	    if (TRACE)
	        fprintf(stderr, "Node `%s' means file `%s'\n", name, result);
	    return result;
	}
    } else {  /* other access */
	char * result;
#ifdef VMS
        char * home =  getenv("HOME");
	if (!home) 
	    home = HTCacheRoot; 
	else
   	    home = HTVMS_wwwName(home);
#else
        CONST char * home =  (CONST char*)getenv("HOME");
	if (!home)
	    home = "/tmp"; 
#endif /* VMS */
	result = (char *)malloc(
		strlen(home)+strlen(access)+strlen(host)+strlen(path)+6+1);
        if (result == NULL)
            outofmem(__FILE__, "HTLocalName");
	sprintf(result, "%s/WWW/%s/%s%s", home, access, host, path);
	FREE(path);
	FREE(access);
	FREE(host);
	return result;
    }
}

/*	Make a WWW name from a full local path name.
**	--------------------------------------------
**
**  Bugs:
**	At present, only the names of two network root nodes are hand-coded
**	in and valid for the NeXT only. This should be configurable in
**	the general case.
*/
PUBLIC char * WWW_nameOfFile ARGS1(
	CONST char *,	name)
{
    char * result;
#ifdef NeXT
    if (0 == strncmp("/private/Net/", name, 13)) {
	result = (char *)malloc(7+strlen(name+13)+1);
	if (result == NULL)
	    outofmem(__FILE__, "WWW_nameOfFile");
	sprintf(result, "file://%s", name+13);
    } else
#endif /* NeXT */
    if (0 == strncmp(HTMountRoot, name, 5)) {
	result = (char *)malloc(7+strlen(name+5)+1);
	if (result == NULL)
	    outofmem(__FILE__, "WWW_nameOfFile");
	sprintf(result, "file://%s", name+5);
    } else {
        result = (char *)malloc(7+strlen(HTHostName())+strlen(name)+1);
	if (result == NULL)
	    outofmem(__FILE__, "WWW_nameOfFile");
	sprintf(result, "file://%s%s", HTHostName(), name);
    }
    if (TRACE)
        fprintf(stderr, "File `%s'\n\tmeans node `%s'\n", name, result);

    return result;
}

/*	Determine a suitable suffix, given the representation.
**	------------------------------------------------------
**
**  On entry,
**	rep	is the atomized MIME style representation
**
**  On exit:
**	Returns	a pointer to a suitable suffix string if one has been
**	found, else "".
*/
PUBLIC CONST char * HTFileSuffix ARGS1(
	HTAtom*,	rep)
{
    HTSuffix * suff;
    int n;
    int i;

#define NO_INIT  /* don't init anymore since I do it in Lynx at startup */
#ifndef NO_INIT    
    if (!HTSuffixes)
        HTFileInit();
#endif /* !NO_INIT */
    n = HTList_count(HTSuffixes);
    for (i = 0; i < n; i++) {
	suff = (HTSuffix *)HTList_objectAt(HTSuffixes, i);
	if (suff->rep == rep) {
	    return suff->suffix;		/* OK -- found */
	}
    }
    return "";		/* Dunno */
}

/*	Determine file format from file name.
**	-------------------------------------
**
**	This version will return the representation and also set
**	a variable for the encoding.
**
**	It will handle for example  x.txt, x.txt,Z, x.Z
*/
PUBLIC HTFormat HTFileFormat ARGS2(
	CONST char *,	filename,
	HTAtom **,	pencoding)
{
    HTSuffix * suff;
    int n;
    int i;
    int lf;
#ifdef VMS
    char *semicolon = NULL;
#endif /* VMS */
    extern char LYforce_HTML_mode;

    if (LYforce_HTML_mode) {
        return WWW_HTML;
    }

#ifdef VMS
    /*
    **  Trim at semicolon if a version number was
    **  included, so it doesn't interfere with the
    **  code for getting the MIME type. - FM
    */
    if ((semicolon = strchr(filename, ';')) != NULL)
        *semicolon = '\0';
#endif /* VMS */

#ifndef NO_INIT    
    if (!HTSuffixes)
        HTFileInit();
#endif /* !NO_INIT */
    if (pencoding)
	*pencoding = NULL;
    lf  = strlen(filename);
    n = HTList_count(HTSuffixes);
    for (i = 0; i < n; i++) {
        int ls;
	suff = (HTSuffix *)HTList_objectAt(HTSuffixes, i);
	ls = strlen(suff->suffix);
	if ((ls <= lf) && 0 == strcasecomp(suff->suffix, filename + lf - ls)) {
	    int j;
	    if (pencoding)
		*pencoding = suff->encoding;
	    if (suff->rep) {
#ifdef VMS
		if (semicolon != NULL)
		    *semicolon = ';';
#endif /* VMS */
	        return suff->rep;		/* OK -- found */
	    }
	    for (j = 0; j < n; j++) {  /* Got encoding, need representation */
		int ls2;
		suff = (HTSuffix *)HTList_objectAt(HTSuffixes, j);
		ls2 = strlen(suff->suffix);
		if ((ls <= lf) && 0 == strncasecomp(
			suff->suffix, filename + lf - ls -ls2, ls2)) {
		    if (suff->rep) {
#ifdef VMS
			if (semicolon != NULL)
			    *semicolon = ';';
#endif /* VMS */
		        return suff->rep;
		    }
		}
	    }
	    
	}
    }
    
    /* defaults tree */
    
    suff = strchr(filename, '.') ? 	/* Unknown suffix */
    	 ( unknown_suffix.rep ? &unknown_suffix : &no_suffix)
	 : &no_suffix;
	 
    /*
    **  Set default encoding unless found with suffix already.
    */
    if (pencoding && !*pencoding)
        *pencoding = suff->encoding ? suff->encoding
				    : HTAtom_for("binary");
#ifdef VMS
    if (semicolon != NULL)
        *semicolon = ';';
#endif /* VMS */
    return suff->rep ? suff->rep : WWW_BINARY;
}

/*	Revise the file format in relation to the Lynx charset. - FM
**	-------------------------------------------------------
**
**	This checks the format associated with an anchor for
**	an extended MIME Content-Type, and if a charset is
**	indicated, sets Lynx up for proper handling in relation
**	to the currently selected character set. - FM
*/
PUBLIC HTFormat HTCharsetFormat ARGS3(
	HTFormat,		format,
	HTParentAnchor *,	anchor,
	int,			default_LYhndl)
{
    char *cp = NULL, *cp1, *cp2, *cp3 = NULL, *cp4;
    BOOL chartrans_ok = FALSE;
    int chndl = -1;
    int i;

    FREE(anchor->charset);
    StrAllocCopy(cp, format->name);
    for (i = 0; cp[i]; i++)
	cp[i] = TOLOWER(cp[i]);
    if (((cp1 = strchr(cp, ';')) != NULL) &&
	(cp2 = strstr(cp1, "charset")) != NULL) {
	if (TRACE)
	    fprintf(stderr,
	    	    "HTCharsetFormat: Extended MIME Content-Type is %s\n",
		    format->name);
	cp2 += 7;
	while (*cp2 == ' ' || *cp2 == '=')
	    cp2++;
#ifdef EXP_CHARTRANS
	StrAllocCopy(cp3, cp2); /* copy to mutilate more */
	for (cp4 = cp3; (*cp4 != '\0' && *cp4 != '"' &&
			 *cp4 != ';'  && *cp4 != ':' &&
			 !WHITE(*cp4)); cp4++) {
	    ; /* do nothing */
	}
	*cp4 = '\0';
	cp4 = cp3;
	chndl = UCGetLYhndl_byMIME(cp3);
	if (chndl < 0) {
	    if (!strcmp(cp4, "cn-big5")) {
		cp4 += 3;
		chndl = UCGetLYhndl_byMIME(cp4);
	    } else if (!strncmp(cp4, "cn-gb", 5)) {
		StrAllocCopy(cp3, "gb2312");
		cp4 = cp3;
		chndl = UCGetLYhndl_byMIME(cp4);
	    }
	}
	if (UCCanTranslateFromTo(chndl, current_char_set)) {
	    chartrans_ok = YES;
	    *cp1 = '\0';
	    format = HTAtom_for(cp);
	    StrAllocCopy(anchor->charset, cp4);
	    HTAnchor_setUCInfoStage(anchor, chndl,
				    UCT_STAGE_MIME, UCT_SETBY_MIME);
	} else if (chndl < 0) {
	    /*
	    **  Got something but we don't recognize it.
	    */
	    chndl = UCLYhndl_for_unrec;
	    if (UCCanTranslateFromTo(chndl, current_char_set)) {
		chartrans_ok = YES;
		HTAnchor_setUCInfoStage(anchor, chndl,
					UCT_STAGE_MIME, UCT_SETBY_DEFAULT);
	    }
	}
	FREE(cp3);
	if (chartrans_ok) {
	    LYUCcharset *p_in = HTAnchor_getUCInfoStage(anchor,
							UCT_STAGE_MIME);
	    LYUCcharset *p_out = HTAnchor_setUCInfoStage(anchor,
							 current_char_set,
							 UCT_STAGE_HTEXT,
							 UCT_SETBY_DEFAULT);
	    if (!p_out) {
		/*
		**  Try again.
		*/
		p_out = HTAnchor_getUCInfoStage(anchor, UCT_STAGE_HTEXT);
	    }
	    if (!strcmp(p_in->MIMEname, "x-transparent")) {
		HTPassEightBitRaw = TRUE;
		HTAnchor_setUCInfoStage(anchor,
					HTAnchor_getUCLYhndl(anchor,
							     UCT_STAGE_HTEXT),
					UCT_STAGE_MIME,
					UCT_SETBY_DEFAULT);
	    }
	    if (!strcmp(p_out->MIMEname, "x-transparent")) {
		HTPassEightBitRaw = TRUE;
		HTAnchor_setUCInfoStage(anchor,
					HTAnchor_getUCLYhndl(anchor,
							     UCT_STAGE_MIME),
					UCT_STAGE_HTEXT,
					UCT_SETBY_DEFAULT);
	    }
	    if (!(p_in->enc & UCT_ENC_CJK) &&
		(p_in->codepoints & UCT_CP_SUBSETOF_LAT1)) {
		HTCJK = NOCJK;
	    } else if (chndl == current_char_set) {
		HTPassEightBitRaw = TRUE;
	    }
	/*
	**  Fall through to old behavior.
	*/
	} else
#endif /* EXP_CHARTRANS */
	    if (!strncmp(cp2, "us-ascii", 8) ||
	    !strncmp(cp2, "iso-8859-1", 10)) {
	    *cp1 = '\0';
	    format = HTAtom_for(cp);
	    StrAllocCopy(anchor->charset, "iso-8859-1");
	    HTCJK = NOCJK;
	} else if (!strncmp(cp2, "iso-8859-2", 10) &&
		   !strncmp(LYchar_set_names[current_char_set],
			    "ISO Latin 2", 11)) {
	    *cp1 = '\0';
	    format = HTAtom_for(cp);
	    StrAllocCopy(anchor->charset, "iso-8859-2");
	    HTPassEightBitRaw = TRUE;
	} else if (!strncmp(cp2, "iso-8859-", 9) &&
		   !strncmp(LYchar_set_names[current_char_set],
			    "Other ISO Latin", 15)) {
	    /*
	    **  Hope it's a match, for now. - FM
	    */
	    *cp1 = '\0';
	    format = HTAtom_for(cp);
	    StrAllocCopy(anchor->charset, "iso-8859- ");
	    anchor->charset[9] = cp2[9];
	    HTPassEightBitRaw = TRUE;
	    HTAlert(anchor->charset);
	} else if (!strncmp(cp2, "koi8-r", 6) &&
		   !strncmp(LYchar_set_names[current_char_set],
			    "KOI8-R character set", 20)) {
	    *cp1 = '\0';
	    format = HTAtom_for(cp);
	    StrAllocCopy(anchor->charset, "koi8-r");
	    HTPassEightBitRaw = TRUE;
	} else if (!strncmp(cp2, "euc-jp", 6) &&
		   HTCJK == JAPANESE) {
	    *cp1 = '\0';
	    format = HTAtom_for(cp);
	    StrAllocCopy(anchor->charset, "euc-jp");
	} else if (!strncmp(cp2, "shift_jis", 9) &&
		   HTCJK == JAPANESE) {
	    *cp1 = '\0';
	    format = HTAtom_for(cp);
	    StrAllocCopy(anchor->charset, "shift_jis");
	} else if (!strncmp(cp2, "iso-2022-jp", 11) &&
		   HTCJK == JAPANESE) {
	    *cp1 = '\0';
	    format = HTAtom_for(cp);
	    StrAllocCopy(anchor->charset, "iso-2022-jp");
	} else if (!strncmp(cp2, "iso-2022-jp-2", 13) &&
		   HTCJK == JAPANESE) {
	    *cp1 = '\0';
	    format = HTAtom_for(cp);
	    StrAllocCopy(anchor->charset, "iso-2022-jp-2");
	} else if (!strncmp(cp2, "euc-kr", 6) &&
		   HTCJK == KOREAN) {
	    *cp1 = '\0';
	    format = HTAtom_for(cp);
	    StrAllocCopy(anchor->charset, "euc-kr");
	} else if (!strncmp(cp2, "iso-2022-kr", 11) &&
		   HTCJK == KOREAN) {
	    *cp1 = '\0';
	    format = HTAtom_for(cp);
	    StrAllocCopy(anchor->charset, "iso-2022-kr");
	} else if ((!strncmp(cp2, "big5", 4) ||
		    !strncmp(cp2, "cn-big5", 7)) &&
		   HTCJK == TAIPEI) {
	    *cp1 = '\0';
	    format = HTAtom_for(cp);
	    StrAllocCopy(anchor->charset, "big5");
	} else if (!strncmp(cp2, "euc-cn", 6) &&
		   HTCJK == CHINESE) {
	    *cp1 = '\0';
	    format = HTAtom_for(cp);
	    StrAllocCopy(anchor->charset, "euc-cn");
	} else if ((!strncmp(cp2, "gb2312", 6) ||
		    !strncmp(cp2, "cn-gb", 5)) &&
		   HTCJK == CHINESE) {
	    *cp1 = '\0';
	    format = HTAtom_for(cp);
	    StrAllocCopy(anchor->charset, "gb2312");
	} else if (!strncmp(cp2, "iso-2022-cn", 11) &&
		   HTCJK == CHINESE) {
	    *cp1 = '\0';
	    format = HTAtom_for(cp);
	    StrAllocCopy(anchor->charset, "iso-2022-cn");
	}
    } else if (cp1 != NULL) {
	/*
	**  No charset parameter is present.
	**  Ignore all other parameters, as
	**  we do when charset is present. - FM
	*/
	*cp1 = '\0';
	format = HTAtom_for(cp);
    }
    FREE(cp);

#ifdef EXP_CHARTRANS
    if (!chartrans_ok && !anchor->charset && default_LYhndl >= 0) {
	HTAnchor_setUCInfoStage(anchor, default_LYhndl,
				UCT_STAGE_MIME, UCT_SETBY_DEFAULT);
    }
    HTAnchor_copyUCInfoStage(anchor,
			    UCT_STAGE_PARSER, UCT_STAGE_MIME,
			    -1);
#endif

    return format;
}

/*	Determine value from file name.
**	-------------------------------
**
*/
PUBLIC float HTFileValue ARGS1(
	CONST char *,	filename)
{
    HTSuffix * suff;
    int n;
    int i;
    int lf = strlen(filename);

#ifndef NO_INIT    
    if (!HTSuffixes)
        HTFileInit();
#endif /* !NO_INIT */
    n = HTList_count(HTSuffixes);
    for (i = 0; i < n; i++) {
        int ls;
	suff = (HTSuffix *)HTList_objectAt(HTSuffixes, i);
	ls = strlen(suff->suffix);
	if ((ls <= lf) && 0==strcmp(suff->suffix, filename + lf - ls)) {
	    if (TRACE)
	        fprintf(stderr, "File: Value of %s is %.3f\n",
			        filename, suff->quality);
	    return suff->quality;		/* OK -- found */
	}
    }
    return 0.3;		/* Dunno! */
}

/*	Determine write access to a file.
**	---------------------------------
**
**  On exit:
**	Returns YES if file can be accessed and can be written to.
**
**  Bugs:
**	1.	No code for non-unix systems.
**	2.	Isn't there a quicker way?
*/
#ifdef VMS
#define NO_GROUPS
#endif /* VMS */
#ifdef NO_UNIX_IO
#define NO_GROUPS
#endif /* NO_UNIX_IO */
#ifdef PCNFS
#define NO_GROUPS
#endif /* PCNFS */
#ifdef NOUSERS
#define NO_GROUPS
#endif /* PCNFS */

PUBLIC BOOL HTEditable ARGS1(
	CONST char *,	filename)
{
#ifdef NO_GROUPS
    return NO;		/* Safe answer till we find the correct algorithm */
#else
#ifdef NeXT
    int		groups[NGROUPS];
#else
    gid_t	groups[NGROUPS];
#endif /* NeXT */
    uid_t	myUid;
    int		ngroups;			/* The number of groups  */
    struct stat	fileStatus;
    int		i;
        
    if (stat(filename, &fileStatus))		/* Get details of filename */
    	return NO;				/* Can't even access file! */

    ngroups = getgroups(NGROUPS, groups);	/* Groups to which I belong  */
    myUid = geteuid();				/* Get my user identifier */

    if (TRACE) {
        int i;
	fprintf(stderr, 
	    "File mode is 0%o, uid=%d, gid=%d. My uid=%d, %d groups (",
    	    (unsigned int) fileStatus.st_mode, fileStatus.st_uid,
	    fileStatus.st_gid,
	    myUid, ngroups);
	for (i = 0; i < ngroups; i++)
	    fprintf(stderr, " %d", groups[i]);
	fprintf(stderr, ")\n");
    }
    
    if (fileStatus.st_mode & 0002)		/* I can write anyway? */
    	return YES;
	
    if ((fileStatus.st_mode & 0200)		/* I can write my own file? */
     && (fileStatus.st_uid == myUid))
    	return YES;

    if (fileStatus.st_mode & 0020)		/* Group I am in can write? */
    {
   	for (i = 0; i < ngroups; i++) {
            if (groups[i] == fileStatus.st_gid)
	        return YES;
	}
    }
    if (TRACE)
        fprintf(stderr, "\tFile is not editable.\n");
    return NO;					/* If no excuse, can't do */
#endif /* NO_GROUPS */
}

/*	Make a save stream.
**	-------------------
**
**	The stream must be used for writing back the file.
**	@@@ no backup done
*/
PUBLIC HTStream * HTFileSaveStream ARGS1(
	HTParentAnchor *,	anchor)
{
    CONST char * addr = HTAnchor_address((HTAnchor*)anchor);
    char *  localname = HTLocalName(addr);

    FILE* fp = fopen(localname, "w");
    if (!fp)
        return NULL;

    return HTFWriter_new(fp);
}

/*      Output one directory entry.
**	---------------------------
*/
PUBLIC void HTDirEntry ARGS3(
	HTStructured *,	target,
	CONST char *,	tail,
	CONST char *,	entry)
{
    char * relative = NULL;
    char * escaped = NULL;
    int len;

    if (0 == strcmp(entry,"../")) {
        /*
	**  Undo slash appending for anchor creation.
	*/
	StrAllocCopy(escaped,"..");
    } else {
	escaped = HTEscape(entry, URL_XPALPHAS);
	if (((len = strlen(escaped)) > 2) &&
	    escaped[(len - 3)] == '%' &&
	    escaped[(len - 2)] == '2' &&
	    TOUPPER(escaped[(len - 1)]) == 'F') {
	    escaped[(len - 3)] = '\0';
	}
    }

    if (tail == NULL || *tail == '\0') {
        /*
	**  Handle extra slash at end of path.
	*/
        HTStartAnchor(target, NULL, (escaped[0] != '\0' ? escaped : "/"));
    } else {
        /*
	**  If empty tail, gives absolute ref below.
	*/
        relative = (char*)malloc(strlen(tail) + strlen(escaped)+2);
        if (relative == NULL)
	    outofmem(__FILE__, "HTDirEntry");
        sprintf(relative, "%s%s%s",
			   tail,
			   (*escaped != '\0' ? "/" : ""),
			   escaped);
        HTStartAnchor(target, NULL, relative);
        FREE(relative);
    }
    FREE(escaped);
}
 
/*      Output parent directory entry.
**	------------------------------
**
**    This gives the TITLE and H1 header, and also a link
**    to the parent directory if appropriate.
**
**  On exit:
**	Returns TRUE if an "Up to <parent>" link was not created
**	for a readable local directiory because LONG_LIST is defined
**	and NO_PARENT_DIR_REFERENCE is not defined, such that the
**	calling function use LYListFmtParse() to create a link to
**	the parent directory.  Otherwise, it returns FALSE. - FM
*/
PUBLIC BOOL HTDirTitles ARGS3(
	HTStructured *,	target,
	HTAnchor *,	anchor,
	BOOL,		tildeIsTop)
{
    char * logical = HTAnchor_address(anchor);
    char * path = HTParse(logical, "", PARSE_PATH + PARSE_PUNCTUATION);
    char * current;
    char * cp = NULL;
    BOOL need_parent_link = FALSE;
    int i;

#ifdef DOSPATH
	 BOOL local_link = FALSE;
	 if (logical[18] == ':') local_link = TRUE;
#endif
    /*
    **  Check tildeIsTop for treating home directory as Welcome
    **  (assume the tilde is not followed by a username). - FM
    */ 
    if (tildeIsTop && !strncmp(path, "/~", 2)) {
        if (path[2] == '\0') {
	    path[1] = '\0';
	} else {
	    for (i = 0; path[(i + 2)]; i++) {
	        path[i] = path[(i + 2)];
	    }
	    path[i] = '\0';
	}
    }

    /*
    **  Trim out the ;type= parameter, if present. - FM
    */
    if ((cp = strrchr(path, ';')) != NULL) {
	if (!strncasecomp((cp+1), "type=", 5)) {
	    if (TOUPPER(*(cp+6)) == 'D' ||
		TOUPPER(*(cp+6)) == 'A' ||
		TOUPPER(*(cp+6)) == 'I')
		*cp = '\0';
	}
	cp = NULL;
    }
    current = strrchr(path, '/');	/* last part or "" */

    {
      char * printable = NULL;

#ifdef DIRED_SUPPORT
      if (0 == strncasecomp(path, "/%2F", 4))
          StrAllocCopy(printable, (path+1));
      else
          StrAllocCopy(printable, path);
      if (0 == strncasecomp(printable, "/vmsysu%2b", 10) ||
          0 == strncasecomp(printable, "/anonymou.", 10)) {
          StrAllocCopy(cp, (printable+1));
	  StrAllocCopy(printable, cp);
	  FREE(cp);
      }
#else 
      StrAllocCopy(printable, (current ? current + 1 : ""));
#endif /* DIRED_SUPPORT */

      START(HTML_HEAD);
      PUTS("\n");
      HTUnEscape(printable);
      START(HTML_TITLE);
      PUTS(*printable ? printable : "Welcome");
      PUTS(" directory");
      END(HTML_TITLE);
      PUTS("\n");
      END(HTML_HEAD);
      PUTS("\n");

#ifdef DIRED_SUPPORT
      START(HTML_H2);
      PUTS(*printable ? "Current directory is " : "");
      PUTS(*printable ? printable : "Welcome");
      END(HTML_H2);
      PUTS("\n");
#else
      START(HTML_H1);
      PUTS(*printable ? printable : "Welcome");
      END(HTML_H1);
      PUTS("\n");
#endif /* DIRED_SUPPORT */
      if (((0 == strncasecomp(printable, "vmsysu:", 7)) &&
           (cp = strchr(printable, '.')) != NULL &&
	   strchr(cp, '/') == NULL) ||
	  (0 == strncasecomp(printable, "anonymou.", 9) &&
	   strchr(printable, '/') == NULL)) {
	  FREE(printable);
	  FREE(logical);
	  FREE(path);
	  return(need_parent_link);
      }
      FREE(printable);
    }

#ifndef NO_PARENT_DIR_REFERENCE
    /*
    **  Make link back to parent directory.
    */
    if (current && current[1]) {   /* was a slash AND something else too */
        char * parent = NULL;
	char * relative = NULL;

	*current++ = '\0';
        parent = strrchr(path, '/');  /* penultimate slash */

	if ((parent &&
	     (!strcmp(parent, "/..") ||
	      !strncasecomp(parent, "/%2F", 4))) ||
	    !strncasecomp(current, "%2F", 3)) {
	    FREE(logical);
	    FREE(path);
	    return(need_parent_link);
	} 

	relative = (char*) malloc(strlen(current) + 4);
	if (relative == NULL)
	    outofmem(__FILE__, "HTDirTitles");

	sprintf(relative, "%s/..", current);

#ifdef DOSPATH
		if(local_link)
	  if (strlen(parent) == 3 )
		StrAllocCat(relative, "/.");
#endif

#if !defined (VMS)
#ifdef DOSPATH
	if(!local_link)
#endif
	{
	    /*
	    **  On Unix, if it's not ftp and the directory cannot
	    **  be read, don't put out a link.
	    **
	    **  On VMS, this problem is dealt with internally by
	    **  HTVMSBrowseDir().
	    */
	    extern BOOLEAN LYisLocalFile PARAMS((char *logical));
	    DIR  * dp = NULL;

	    if (LYisLocalFile(logical)) {
		/*
		**  We need an absolute file path for the opendir.
		**  We also need to unescape for this test.
		**  Don't worry about %2F now, they presumably have been
		**  dealt with above, and shouldn't appear for local
		**  files anyway...  Assume OS / filesystem will just
		**  ignore superfluous slashes. - KW
		*/
		char * fullparentpath = NULL;

		/*
		**  Path has been shortened above.
		*/
		StrAllocCopy(fullparentpath, *path ? path : "/");

		/*
		**  Guard against weirdness.
		*/
		if (0 == strcmp(current,"..")) {
		    StrAllocCat(fullparentpath,"/../..");
		} else if (0 == strcmp(current,".")) {
		    StrAllocCat(fullparentpath,"/..");
		}

		HTUnEscape(fullparentpath);
	        if ((dp = opendir(fullparentpath)) == NULL) {
	            FREE(fullparentpath);
	            FREE(logical);
	            FREE(relative);
	            FREE(path);
	            return(need_parent_link);
		}
	    	closedir(dp);
		FREE(fullparentpath);
#ifdef LONG_LIST
		need_parent_link = TRUE;
		FREE(logical);
		FREE(path);
		FREE(relative);
		return(need_parent_link);
#endif /* LONG_LIST */
	    }
	}
#endif /* !VMS */
	HTStartAnchor(target, "", relative);
	FREE(relative);

	PUTS("Up to ");
	if (parent) {
	    if ((0 == strcmp(current,".")) ||
	        (0 == strcmp(current,".."))) {
		/*
		**  Should not happen, but if it does,
		**  at least avoid giving misleading info. - KW
		*/
		PUTS("..");
	    } else {
		char * printable = NULL;
		StrAllocCopy(printable, parent + 1);
		HTUnEscape(printable);
		PUTS(printable);
		FREE(printable);
	    }
	} else {
	    PUTS("/");
	}
	END(HTML_A);
    }
#endif /* !NO_PARENT_DIR_REFERENCE */

    FREE(logical);
    FREE(path);
    return(need_parent_link);
}
		
/*	Load a document.
**	----------------
**
**  On entry:
**	addr		must point to the fully qualified hypertext reference.
**			This is the physical address of the file
**
**  On exit:
**	returns		<0		Error has occured.
**			HTLOADED	OK 
**
*/
PUBLIC int HTLoadFile ARGS4(
	CONST char *,		addr,
	HTParentAnchor *,	anchor,
	HTFormat,		format_out,
	HTStream *,		sink)
{
    char * filename = NULL;
    char * access = NULL;
    HTFormat format;
    char * nodename = NULL;
    char * newname = NULL;	/* Simplified name of file */
    HTAtom * encoding;		/* @@ not used yet */
    int status;
#ifdef VMS
    struct stat stat_info;
#else
    extern char *list_format;
#endif /* VMS */
#ifdef USE_ZLIB
    gzFile gzfp;
    BOOL use_gzread = NO;
#endif /* USE_ZLIB */

    /*
    **  Reduce the filename to a basic form (hopefully unique!).
    */
    StrAllocCopy(newname, addr);
    filename=HTParse(newname, "", PARSE_PATH|PARSE_PUNCTUATION);
    nodename=HTParse(newname, "", PARSE_HOST);
    
    /*
    **  If access is ftp, or file is on another host, invoke ftp now.
    */
    access = HTParse(newname, "", PARSE_ACCESS);
    if (strcmp("ftp", access) == 0 ||
       (strcmp("localhost", nodename) != 0 &&
#ifdef VMS
        strcasecomp(nodename, HTHostName()) != 0
#else
        strcmp(nodename, HTHostName()) != 0
#endif /* VMS */
    )) {
        FREE(newname);
	FREE(filename);
	FREE(nodename);
        FREE(access);
        return HTFTPLoad(addr, anchor, format_out, sink);
    } else {
        FREE(newname);
        FREE(access);
    }
#ifdef VMS
    HTUnEscape(filename);
#endif /* VMS */

    /*
    **  Determine the format and encoding mapped to any suffix.
    */
    format = HTFileFormat(filename, &encoding);

    /*
    **  Check the format for an extended MIME charset value, and
    **  act on it if present.  Otherwise, assume the ISO-8859-1
    **  character set for local files.  If it's actually another
    **  charset (e.g., ISO-8859-2 or KOI8-R) and the terminal is
    **  using that, Lynx users should make the current character
    **  set "ISO Latin 1" so that 8-bit characters are passed raw.
    */
    format = HTCharsetFormat(format, anchor, UCLYhndl_HTFile_for_unspec);

#ifdef VMS
    /*
    **  Check to see if the 'filename' is in fact a directory.  If it is
    **  create a new hypertext object containing a list of files and 
    **  subdirectories contained in the directory.  All of these are links
    **  to the directories or files listed.
    */
    if (HTStat(filename, &stat_info) == -1) {
	if (TRACE)
	    fprintf(stderr, "HTLoadFile: Can't stat %s\n", filename);
    } else {
	if (((stat_info.st_mode) & S_IFMT) == S_IFDIR) {
	    if (HTDirAccess == HT_DIR_FORBID) {
		FREE(filename);
		FREE(nodename);
		return HTLoadError(sink, 403,
		"Directory browsing is not allowed.");
	    }

	    if (HTDirAccess == HT_DIR_SELECTIVE) {
		char * enable_file_name = 
		    malloc(strlen(filename)+ 1 +
		    strlen(HT_DIR_ENABLE_FILE) + 1);
		if (enable_file_name == NULL)
		    outofmem(__FILE__, "HTLoadFile");
		strcpy(enable_file_name, filename);
		strcat(enable_file_name, "/");
		strcat(enable_file_name, HT_DIR_ENABLE_FILE);
		if (HTStat(enable_file_name, &stat_info) == -1) {
		    FREE(filename);
		    FREE(nodename);
		    return HTLoadError(sink, 403,
		    "Selective access is not enabled for this directory");
		}
	    }

	    FREE(filename);
	    FREE(nodename);
	    return HTVMSBrowseDir(addr, anchor, format_out, sink);
	}
    }

    /*
    **  Assume that the file is in Unix-style syntax if it contains a '/'
    **  after the leading one. @@
    */
    {
        FILE * fp;
	char * vmsname = strchr(filename + 1, '/') ?
		    HTVMS_name(nodename, filename) : filename + 1;
	fp = fopen(vmsname, "r", "shr=put", "shr=upd");
	
        /*
	**  If the file wasn't VMS syntax, then perhaps it is ultrix.
	*/
	if (!fp) {
	    char ultrixname[INFINITY];
	    if (TRACE)
	        fprintf(stderr, "HTLoadFile: Can't open as %s\n", vmsname);
	    sprintf(ultrixname, "%s::\"%s\"", nodename, filename);
  	    fp = fopen(ultrixname, "r", "shr=put", "shr=upd");
	    if (!fp) {
		if (TRACE)
		    fprintf(stderr, "HTLoadFile: Can't open as %s\n",
		    		    ultrixname);
	    }
	}
        if (fp) {
	    int len;
	    char *cp = NULL;
	    char *semicolon = NULL;

	    if (HTEditable(vmsname)) {
		HTAtom * put = HTAtom_for("PUT");
		HTList * methods = HTAnchor_methods(anchor);
		if (HTList_indexOf(methods, put) == (-1)) {
	   	    HTList_addObject(methods, put);
	        }
	    }
	    /*
	    **  Trim vmsname at semicolon if a version number was
	    **  included, so it doesn't interfere with the check
	    **  for a compressed file. - FM
	    */
	    if ((semicolon = strchr(vmsname, ';')) != NULL)
	        *semicolon = '\0';
	    /*
	    **  Fake a Content-Encoding for compressed files. - FM
	    */
	    if ((len = strlen(vmsname)) > 2) {
	        if ((vmsname[len - 1] == 'Z') &&
		    (vmsname[len - 2] == '.' ||
		     vmsname[len - 2] == '-' ||
		     vmsname[len - 2] == '_') &&
		    vmsname[len - 3] != ']' &&
		    vmsname[len - 3] != ':') {
		    StrAllocCopy(cp, vmsname);
		    cp[len - 2] = '\0';
		    format = HTFileFormat(cp, &encoding);
		    FREE(cp);
		    format = HTCharsetFormat(format, anchor,
					     UCLYhndl_HTFile_for_unspec);
		    StrAllocCopy(anchor->content_type, format->name);
		    StrAllocCopy(anchor->content_encoding, "x-compress");
		    format = HTAtom_for("www/compressed");
		} else if ((len > 3) &&
			   !strcasecomp((char *)&vmsname[len - 2], "gz")) {
		    if (vmsname[len - 3] == '.' ||
		        vmsname[len - 3] == '-' ||
			vmsname[len - 3] == '_') {
			StrAllocCopy(cp, vmsname);
			cp[len - 3] = '\0';
			format = HTFileFormat(cp, &encoding);
			FREE(cp);
			format = HTCharsetFormat(format, anchor,
						 UCLYhndl_HTFile_for_unspec);
			StrAllocCopy(anchor->content_type, format->name);
			StrAllocCopy(anchor->content_encoding, "x-gzip");
#ifdef USE_ZLIB
			if (strcmp(format_out->name, "www/download") != 0) {
			    fclose(fp);
			    gzfp = gzopen(localname, "rb");

			    if (TRACE)
				fprintf(stderr,
				       "HTLoadFile: gzopen of `%s' gives %p\n",
					localname, (void*)gzfp);
			    use_gzread = YES;
			}
#else  /* USE_ZLIB */
			format = HTAtom_for("www/compressed");
#endif  /* USE_ZLIB */
		    }
		}
	    }
	    if (semicolon != NULL)
	        *semicolon = ';';
	    FREE(filename);
	    FREE(nodename);
#ifdef USE_ZLIB
	    if (use_gzread) {
		if (gzfp) {
		    char * sugfname = NULL;
		    if (anchor->SugFname) {
			StrAllocCopy(sugfname, anchor->SugFname);
		    } else {
			char * anchor_path = HTParse(anchor->address, "",
						     PARSE_PATH + PARSE_PUNCTUATION);
			char * lastslash;
			HTUnEscape(anchor_path);
			lastslash = strrchr(anchor_path, '/');
			if (lastslash)
			    StrAllocCopy(sugfname, lastslash + 1);
			FREE(anchor_path);
		    }
		    FREE(anchor->content_encoding);
		    if (sugfname && *sugfname)
			HTCheckFnameForCompression(&sugfname, anchor,
						   TRUE);
		    if (sugfname && *sugfname)
			StrAllocCopy(anchor->SugFname, sugfname);
		    FREE(sugfname);
		    status = HTParseGzFile(format, format_out,
					   anchor,
					   gzfp, sink);
		} else {
		    status = HTLoadError(NULL,
					 -(HT_ERROR),
					 "Could not open file for decompression!");
		}
	    } else
#endif /* USE_GZREAD */
	    {
		status = HTParseFile(format, format_out, anchor, fp, sink);
		fclose(fp);
	    }
	    return status;
        }  /* If successfull open */
	FREE(filename);
    }

#else /* Unix: */

    FREE(filename);
    
    /*
    **  For unix, we try to translate the name into the name of a
    **  transparently mounted file.
    **
    **  Not allowed in secure (HTClienntHost) situations. TBL 921019
    */
#ifndef NO_UNIX_IO
    /*  Need protection here for telnet server but not httpd server. */
	 
    if (!HTSecure) {		/* try local file system */
	char * localname = HTLocalName(addr);
	struct stat dir_info;
	
#ifdef HAVE_READDIR
        /*
	**  Multiformat handling.
	**
	**  If needed, scan directory to find a good file.
	**  Bug:  We don't stat the file to find the length.
	*/
	if ((strlen(localname) > strlen(MULTI_SUFFIX)) &&
	    (0 == strcmp(localname + strlen(localname) - strlen(MULTI_SUFFIX),
	                 MULTI_SUFFIX))) {
	    DIR *dp;

	    STRUCT_DIRENT * dirbuf;
	    float best = NO_VALUE_FOUND;	/* So far best is bad */
	    HTFormat best_rep = NULL;	/* Set when rep found */
	    char * best_name = NULL;	/* Best dir entry so far */

	    char *base = strrchr(localname, '/');
	    int baselen;

	    if (!base || base == localname)
	        goto forget_multi;
	    *base++ = '\0';		/* Just got directory name */
	    baselen = strlen(base)- strlen(MULTI_SUFFIX);
	    base[baselen] = '\0';	/* Chop off suffix */

	    dp = opendir(localname);
	    if (!dp) {
forget_multi:
		FREE(localname);
		FREE(nodename);
		return HTLoadError(sink, 500,
			"Multiformat: directory scan failed.");
	    }
	    
	    while ((dirbuf = readdir(dp)) != NULL) {
		/*
		**  While there are directory entries to be read...
		*/
#ifndef DOSPATH
		if (dirbuf->d_ino == 0)
		    continue;	/* if the entry is not being used, skip it */
#endif
		if ((int)strlen(dirbuf->d_name) > baselen &&     /* Match? */
		    !strncmp(dirbuf->d_name, base, baselen)) {	
		    HTFormat rep = HTFileFormat(dirbuf->d_name, NULL);
		    float filevalue = HTFileValue(dirbuf->d_name);
		    float value = HTStackValue(rep, format_out,
		    				filevalue,
						0L  /* @@@@@@ */);
		    if (value <= 0.0) {
			char * cp = NULL;
			int len = strlen(dirbuf->d_name);
    			if (len > 2 &&
			    dirbuf->d_name[len - 1] == 'Z' &&
			    dirbuf->d_name[len - 2] == '.') {
			    StrAllocCopy(cp, dirbuf->d_name);
			    cp[len - 2] = '\0';
			    format = HTFileFormat(cp, NULL);
			    FREE(cp);
			    value = HTStackValue(format, format_out,
						 filevalue, 0);
			    if (value <= 0.0) {
				format = HTAtom_for("application/x-compressed");
				value = HTStackValue(format, format_out,
						     filevalue, 0);
			    }
			    if (value <= 0.0) {
				format = HTAtom_for("www/compressed");
				value = HTStackValue(format, format_out,
						     filevalue, 0);
			    }
			} else if ((len > 3) &&
				   !strcasecomp((char *)&dirbuf->d_name[len - 2],
						"gz") &&
				   dirbuf->d_name[len - 3] == '.') {
			    StrAllocCopy(cp, dirbuf->d_name);
			    cp[len - 3] = '\0';
			    format = HTFileFormat(cp, NULL);
			    FREE(cp);
			    value = HTStackValue(format, format_out,
						 filevalue, 0);
			    if (value <= 0.0) {
				format = HTAtom_for("application/x-gzip");
				value = HTStackValue(format, format_out,
						     filevalue, 0);
			    }
			    if (value <= 0.0) {
				format = HTAtom_for("www/compressed");
				value = HTStackValue(format, format_out,
						     filevalue, 0);
			    }
			}
		    }
		    if (value != NO_VALUE_FOUND) {
		        if (TRACE)
			    fprintf(stderr,
				 "HTLoadFile: value of presenting %s is %f\n",
				    HTAtom_name(rep), value);
			if  (value > best) {
			    best_rep = rep;
			    best = value;
			    StrAllocCopy(best_name, dirbuf->d_name);
		       }
		    }	/* if best so far */ 		    
		 } /* if match */  
		    
	    } /* end while directory entries left to read */
	    closedir(dp);
	    
	    if (best_rep) {
		format = best_rep;
		base[-1] = '/';		/* Restore directory name */
		base[0] = '\0';
		StrAllocCat(localname, best_name);
                FREE(best_name);
                /* goto open_file; */  /* Nope - might be a directory - kw */
		
	    } else { 			/* If not found suitable file */
		FREE(localname);
		FREE(nodename);
		return HTLoadError(sink, 403,	/* List formats? */
		   "Could not find suitable representation for transmission.");
	    }
	    /*NOTREACHED*/
	} /* if multi suffix */

	/*
	**  Check to see if the 'localname' is in fact a directory.  If it
	**  is create a new hypertext object containing a list of files and 
	**  subdirectories contained in the directory.  All of these are
	**  links to the directories or files listed.
	**  NB This assumes the existance of a type 'STRUCT_DIRENT', which
	**  will hold the directory entry, and a type 'DIR' which is used
	**  to point to the current directory being read.
	*/
#ifdef _WINDOWS
	if (!exists(localname))
#else
	if (stat(localname,&dir_info) == -1)	   /* get file information */
#endif
	{
	                               /* if can't read file information */
	    if (TRACE)
	        fprintf(stderr, "HTLoadFile: can't stat %s\n", localname);

	}  else {		/* Stat was OK */
		
#ifdef _WINDOWS
	if (stat(localname,&dir_info) == -1) dir_info.st_mode = S_IFDIR;
#endif

	    if (((dir_info.st_mode) & S_IFMT) == S_IFDIR) {
		/*
		**  If localname is a directory.
		*/	
		HTStructured *target;		/* HTML object */
		HTStructuredClass targetClass;
		DIR *dp;
		STRUCT_DIRENT * dirbuf;
		char *logical = NULL;
		char *pathname = NULL;
		char *tail = NULL;
		BOOL present[HTML_A_ATTRIBUTES];
		char * tmpfilename = NULL;
                BOOL need_parent_link = FALSE;
		struct stat file_info;
		
		if (TRACE)
		    fprintf(stderr, "%s is a directory\n", localname);
			
		/*
		**  Check directory access.
		**  Selective access means only those directories containing
		**  a marker file can be browsed.
		*/
		if (HTDirAccess == HT_DIR_FORBID) {
		    FREE(localname);
		    FREE(nodename);
		    return HTLoadError(sink, 403,
		    "Directory browsing is not allowed.");
		}


		if (HTDirAccess == HT_DIR_SELECTIVE) {
		    char * enable_file_name = 
			malloc(strlen(localname)+ 1 +
				      strlen(HT_DIR_ENABLE_FILE) + 1);
		    if (enable_file_name == NULL)
			outofmem(__FILE__, "HTLoadFile");
		    strcpy(enable_file_name, localname);
		    strcat(enable_file_name, "/");
		    strcat(enable_file_name, HT_DIR_ENABLE_FILE);
		    if (stat(enable_file_name, &file_info) != 0) {
			FREE(localname);
			FREE(nodename);
			return HTLoadError(sink, 403,
			"Selective access is not enabled for this directory");
		    }
		}

		dp = opendir(localname);
		if (!dp) {
		    FREE(localname);
		    FREE(nodename);
		    return HTLoadError(sink, 403, 
		    		       "This directory is not readable.");
		}

		/*
		**  Directory access is allowed and possible.
		*/
		logical = HTAnchor_address((HTAnchor*)anchor);
        	pathname = HTParse(logical, "", 
					PARSE_PATH + PARSE_PUNCTUATION);

    		if (!strcmp(pathname,"/"))  /* root path */
        	    StrAllocCopy (tail, "/foo/..");
    	    	else {
        	    char *p = strrchr(pathname, '/');  /* find lastslash */
        	    StrAllocCopy(tail, p+1); /* take slash off the beginning */
    		}
    		FREE(pathname);

#ifdef EXP_CHARTRANS
		if (UCLYhndl_HTFile_for_unspec >= 0) {
		    HTAnchor_setUCInfoStage(anchor,
					    UCLYhndl_HTFile_for_unspec,
					    UCT_STAGE_PARSER,
					    UCT_SETBY_DEFAULT);
		}
#endif		
		target = HTML_new(anchor, format_out, sink);
		targetClass = *target->isa;	/* Copy routine entry points */
		    
  		{ int i;
			for (i = 0; i < HTML_A_ATTRIBUTES; i++)
				present[i] = (i == HTML_A_HREF);
		}
		
		/*
		**  The need_parent_link flag will be set if an
		**  "Up to <parent>" link was not created for a
		**  readable parent in HTDirTitles() because
		**  LONG_LIST is defined and NO_PARENT_DIR_REFERENCE
		**  is not defined so that need we to create the
		**  link via an LYListFmtParse() call. - FM
		*/
                need_parent_link = HTDirTitles(target,
					       (HTAnchor *)anchor, FALSE);

#ifdef DIRED_SUPPORT
		HTAnchor_setFormat((HTParentAnchor *) anchor, WWW_DIRED);
		lynx_edit_mode = TRUE;
#endif /* DIRED_SUPPORT */
                if (HTDirReadme == HT_DIR_README_TOP)
		    do_readme(target, localname);
		{
		    HTBTree * bt = HTBTree_new((HTComparer)strcmp);

		    while ((dirbuf = readdir(dp)) != NULL) {
			/*
			**  While there are directory entries to be read...
			*/
		        char * dirname = NULL;
			extern BOOLEAN no_dotfiles, show_dotfiles;

#ifndef DOSPATH
		        if (dirbuf->d_ino == 0)
			    /*
			    **  If the entry is not being used, skip it.
			    */
			    continue;
#endif
			/*
			**  Skip self, parent if handled in HTDirTitles()
			**  or if NO_PARENT_DIR_REFERENCE is not defined,
			**  and any dot files if no_dotfiles is set or
			**  show_dotfiles is not set. - FM
			*/
			if (!strcmp(dirbuf->d_name, ".")   /* self   */ ||
			    (!strcmp(dirbuf->d_name, "..") /* parent */ &&
			     need_parent_link == FALSE) ||
			    ((strcmp(dirbuf->d_name, "..")) &&
			     (dirbuf->d_name[0] == '.' &&
			      (no_dotfiles || !show_dotfiles))))
			    continue;

			dirname = (char *)malloc(strlen(dirbuf->d_name) + 4);
			if (dirname == NULL)
			    outofmem(__FILE__, "HTLoadFile");
			StrAllocCopy(tmpfilename, localname);
			if (strcmp(localname, "/"))
			    /*
			    **  If filename is not root directory.
			    */
			    StrAllocCat(tmpfilename, "/"); 

			StrAllocCat(tmpfilename, dirbuf->d_name);
			stat(tmpfilename, &file_info);
			if (((file_info.st_mode) & S_IFMT) == S_IFDIR)
#ifndef DIRED_SUPPORT
		            sprintf((char *)dirname, "D%s",dirbuf->d_name);
			else
			    sprintf((char *)dirname, "F%s",dirbuf->d_name);
			    /* D & F to have first directories, then files */
#else
			    if (dir_list_style == MIXED_STYLE)
			        sprintf((char *)dirname,
					" %s/", dirbuf->d_name);
			    else if (!strcmp(dirbuf->d_name, ".."))
			        sprintf((char *)dirname,
					"A%s", dirbuf->d_name);
			    else
			        sprintf((char *)dirname,
					"D%s", dirbuf->d_name);
			else if (dir_list_style == MIXED_STYLE)
			    sprintf((char *)dirname, " %s", dirbuf->d_name);
			else if (dir_list_style == FILES_FIRST)
			    sprintf((char *)dirname, "C%s", dirbuf->d_name);
			    /* C & D to have first files, then directories */
			else
			    sprintf((char *)dirname, "F%s", dirbuf->d_name);
#endif /* !DIRED_SUPPORT */
			/*
			**  Sort dirname in the tree bt.
			*/
			HTBTree_add(bt, dirname);
		    }

		    /*
		    **  Run through tree printing out in order.
		    */
		    {
		        HTBTElement * next_element = HTBTree_next(bt,NULL);
			    /* pick up the first element of the list */
			char state;
			    /* I for initial (.. file),
			       D for directory file,
			       F for file */
			
#ifdef DIRED_SUPPORT
			char test;
#endif /* DIRED_SUPPORT */
			state = 'I';

			while (next_element != NULL) {
			    char *entry, *file_extra;

			    StrAllocCopy(tmpfilename,localname);
			    if (strcmp(localname, "/")) 
				/*
				**  If filename is not root directory.
				*/
			        StrAllocCat(tmpfilename, "/"); 

			    StrAllocCat(tmpfilename,
					(char *)HTBTree_object(next_element)+1);
			    /*
			    **  Append the current entry's filename
			    **  to the path.
			    */
			    HTSimplify(tmpfilename);
			    /*
			    **  Output the directory entry.
			    */
			    if (strcmp((char *)
				       (HTBTree_object(next_element)), "D..") &&
			        strcmp((char *)
				       (HTBTree_object(next_element)), "A.."))
			    {			    
#ifdef DIRED_SUPPORT
			        test = (*(char *)(HTBTree_object(next_element))
				        == 'D' ? 'D' : 'F');
				if (state != test) {
#ifndef LONG_LIST
				    if (dir_list_style == FILES_FIRST) {
				       if (state == 'F')
  					  END(HTML_DIR); 
				    } else if (dir_list_style != MIXED_STYLE)
				       if (state == 'D')
  					  END(HTML_DIR); 
#endif /* !LONG_LIST */
				    state = 
				       (*(char *)(HTBTree_object(next_element))
				        == 'D' ? 'D' : 'F');
				    START(HTML_H2);
				    if (dir_list_style != MIXED_STYLE) {
				       START(HTML_EM);
				       PUTS(state == 'D' ?
				          "Subirectories:" : "Files:");
				       END(HTML_EM);
				    }
				    END(HTML_H2);
#ifndef LONG_LIST
				    START(HTML_DIR);
#endif /* !LONG_LIST */
				}
#else
				if (state != *(char *)(HTBTree_object(
							 next_element))) {
#ifndef LONG_LIST
				    if (state == 'D')
				        END(HTML_DIR);
#endif /* !LONG_LIST */
				    state =
				      (*(char *)(HTBTree_object(next_element))
				       == 'D' ? 'D' : 'F');
				    START(HTML_H2);
				    START(HTML_EM);
				    PUTS(state == 'D' ?
				    "Subdirectories:" : "Files:");
				    END(HTML_EM);
				    END(HTML_H2);
#ifndef LONG_LIST
				    START(HTML_DIR);
#endif /* !LONG_LIST */
				}
#endif /* DIRED_SUPPORT */
#ifndef LONG_LIST
			        START(HTML_LI);
#endif /* !LONG_LIST */
			    }
			    entry = (char*)HTBTree_object(next_element)+1;
			    file_extra = NULL;

#ifdef LONG_LIST
			    LYListFmtParse(list_format, tmpfilename, target,
				entry, tail);
#else
			    HTDirEntry(target, tail, entry);
			    PUTS(entry);
			    END(HTML_A);
			    if (file_extra) {
				PUTS(file_extra);
				FREE(file_extra);
			    }
                            MAYBE_END(HTML_LI);
#endif /* LONG_LIST */

			    next_element = HTBTree_next(bt, next_element);
			        /* pick up the next element of the list; 
				 if none, return NULL*/
			}
			if (state == 'I') {
			    START(HTML_P);
			    PUTS("Empty Directory");
			}
#ifndef LONG_LIST
			else
			    END(HTML_DIR);
#endif /* !LONG_LIST */
		    }
		        /* end while directory entries left to read */
		    closedir(dp);
		    FREE(logical);
		    FREE(tmpfilename);
		    FREE(tail);
		    HTBTreeAndObject_free(bt);

		    if (HTDirReadme == HT_DIR_README_BOTTOM)
			  do_readme(target, localname);
		    FREE_TARGET;
		    FREE(localname);
		    FREE(nodename);
		    return HT_LOADED;	/* document loaded */
		}

	    } /* end if localname is directory */
	
	} /* end if file stat worked */
	
/* End of directory reading section
*/
#endif /* HAVE_READDIR */
open_file:
	{
	    FILE * fp = fopen(localname, "r");

	    if (TRACE)
	        fprintf (stderr, "HTLoadFile: Opening `%s' gives %p\n",
				 localname, (void*)fp);
	    if (fp) {		/* Good! */
	        int len;
		char *cp = NULL;

		if (HTEditable(localname)) {
		    HTAtom * put = HTAtom_for("PUT");
		    HTList * methods = HTAnchor_methods(anchor);
		    if (HTList_indexOf(methods, put) == (-1)) {
			HTList_addObject(methods, put);
		    }
		}
		/*
		**  Fake a Content-Encoding for compressed files. - FM
		*/
		if ((len = strlen(localname)) > 2) {
		    if (localname[len - 1] == 'Z' &&
		        localname[len - 2] == '.') {
			StrAllocCopy(cp, localname);
			cp[len - 2] = '\0';
			format = HTFileFormat(cp, &encoding);
			FREE(cp);
			format = HTCharsetFormat(format, anchor,
						 UCLYhndl_HTFile_for_unspec);
			StrAllocCopy(anchor->content_type, format->name);
			StrAllocCopy(anchor->content_encoding, "x-compress");
			format = HTAtom_for("www/compressed");
		    } else if ((len > 3) &&
			       !strcasecomp((char *)&localname[len - 2],
			       		    "gz") &&
			       localname[len - 3] == '.') {
			StrAllocCopy(cp, localname);
			cp[len - 3] = '\0';
			format = HTFileFormat(cp, &encoding);
			FREE(cp);
			format = HTCharsetFormat(format, anchor,
						 UCLYhndl_HTFile_for_unspec);
			StrAllocCopy(anchor->content_type, format->name);
			StrAllocCopy(anchor->content_encoding, "x-gzip");
#ifdef USE_ZLIB
			if (strcmp(format_out->name, "www/download") != 0) {
			    fclose(fp);
			    gzfp = gzopen(localname, "rb");

			    if (TRACE)
				fprintf(stderr,
				       "HTLoadFile: gzopen of `%s' gives %p\n",
					localname, (void*)gzfp);
			    use_gzread = YES;
			}
#else  /* USE_ZLIB */
			format = HTAtom_for("www/compressed");
#endif  /* USE_ZLIB */
		    }
		}
		FREE(localname);
		FREE(nodename);
#ifdef USE_ZLIB
		if (use_gzread) {
		    if (gzfp) {
			char * sugfname = NULL;
			if (anchor->SugFname) {
			    StrAllocCopy(sugfname, anchor->SugFname);
			} else {
			    char * anchor_path = HTParse(anchor->address, "",
							 PARSE_PATH + PARSE_PUNCTUATION);
			    char * lastslash;
			    HTUnEscape(anchor_path);
			    lastslash = strrchr(anchor_path, '/');
			    if (lastslash)
				StrAllocCopy(sugfname, lastslash + 1);
			    FREE(anchor_path);
			}
			FREE(anchor->content_encoding);
			if (sugfname && *sugfname)
			    HTCheckFnameForCompression(&sugfname, anchor,
						       TRUE);
			if (sugfname && *sugfname)
			    StrAllocCopy(anchor->SugFname, sugfname);
			FREE(sugfname);
			status = HTParseGzFile(format, format_out,
					       anchor,
					       gzfp, sink);
		    } else {
			status = HTLoadError(NULL,
					     -(HT_ERROR),
				     "Could not open file for decompression!");
		    }
		} else
#endif /* USE_GZREAD */
		{
		    status = HTParseFile(format, format_out, anchor, fp, sink);
		    fclose(fp);
		}
		return status;
	    }  /* If succesfull open */
	}    /* scope of fp */
    }  /* local unix file system */    
#endif /* !NO_UNIX_IO */
#endif /* VMS */

#ifndef DECNET
    /*
    **  Now, as transparently mounted access has failed, we try FTP.
    */
    {
	/*
	**  Deal with case-sensitivity differences on VMS verus Unix.
	*/
#ifdef VMS
        if (strcasecomp(nodename, HTHostName()) != 0)
#else
	if (strcmp(nodename, HTHostName()) != 0)
#endif /* VMS */
	{
	    FREE(nodename);
	    if (!strncmp(addr, "file://localhost", 16)) {
	        return -1;  /* never go to ftp site when URL
			     * is file://localhost
			     */
	    } else {
	        return HTFTPLoad(addr, anchor, format_out, sink);
	    }
	}
	FREE(nodename);
    }
#endif /* !DECNET */

    /*
    **  All attempts have failed.
    */
    {
    	if (TRACE)
	    fprintf(stderr, "Can't open `%s', errno=%d\n", addr, SOCKET_ERRNO);

	return HTLoadError(sink, 403, "Can't access requested file.");
    }
    
 
}

/*
**	Protocol descriptors
*/
#ifdef GLOBALDEF_IS_MACRO
#define _HTFILE_C_1_INIT { "ftp", HTLoadFile, 0 }
GLOBALDEF (HTProtocol,HTFTP,_HTFILE_C_1_INIT);
#define _HTFILE_C_2_INIT { "file", HTLoadFile, HTFileSaveStream }
GLOBALDEF (HTProtocol,HTFile,_HTFILE_C_2_INIT);
#else
GLOBALDEF PUBLIC HTProtocol HTFTP  = { "ftp", HTLoadFile, 0 };
GLOBALDEF PUBLIC HTProtocol HTFile = { "file", HTLoadFile, HTFileSaveStream };
#endif /* GLOBALDEF_IS_MACRO */
