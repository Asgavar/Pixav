#ifndef FSOPS_H
#define FSOPS_H

char* pixh_file_name;
char* svg_file_name;

char* basename(char*);
char* temp_folder_name(char*);
void tmpdirloop(void (*func)(char* filename));

#endif
