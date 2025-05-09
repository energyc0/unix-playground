#ifndef CLIENTLIST
#define CLIENTLIST

void clientlist_add(char* name, int fd); //add client entry to the list
void clientlist_erase(char* name); // erase client entry from the list
int clientlist_is_busy(char* name); // check whether there is an entry of the client
void clientlist_clear(); // clear the whole list
void clientlist_debug(); //print all nodes

#endif