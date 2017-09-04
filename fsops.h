#ifndef FSOPS_H
#define FSOPS_H

/* Global variable, sort of anti-pattern as far as I know
 * but it should do the trick */
char* pixh_file_name;
char* basename(char*);
char* temp_folder_name(char*);
void tmpdirloop(void (*func)(char* filename));

#endif
