
#ifndef LYLIST_H
#define LYLIST_H

extern int showlist PARAMS((char **newfile, BOOLEAN titles));
extern void printlist PARAMS((FILE *fp, BOOLEAN titles));

#define LIST_PAGE_TITLE  "Lynx List Page"

#endif /* LYLIST_H */
