#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 

typedef struct charlist{ 
unsigned char sum; 
struct charlist *next; 
struct charlist *prev; 
} charlist; 

typedef struct listline{ 
struct charlist *head; 
struct charlist *tail; 
int len; 
} listline; 

listline *CreateListLine(); 
void PushBack(listline *list, unsigned char sum); 
unsigned char PopBack(listline *list); 
void PushFront(listline *list, unsigned char sum); 
unsigned char PopFront(listline *list); 

char *GetCommands(FILE *file); 
charlist* getptr(listline* mlist, charlist *list, int dir); 

#define INC 1 
#define DEC 2 
#define MOVL 3 
#define MOVR 4 
#define PRINT 5 
#define GET 6 
#define PRINTC 7 
#define BEGIN 8 
#define END 9 

int main(int argc, char *argv[]) 
{ 
int f=5; 
FILE *file; 
char *commands=0, *c, *filename; 
charlist *ptr=0; 
listline *line=0; 

if (argc > 1){ 
if ((file = fopen(argv[1], "r")) == 0){ 
printf ("Enter correct file name!\n"); 
return 0; 
} 
} 

else { 
filename = (char*)malloc(500*sizeof(char)); 
printf("File name\n"); 
fgets(filename, 500*sizeof(char), stdin); 
c = filename; 
while ((*(c))!='\n') ++c; 
*c = '\0'; 
while ((file = fopen(filename, "r")) == 0){ 
printf("Enter correct file name!\n"); 
fgets(filename, 500*sizeof(char), stdin); 
c = filename; 
while ((*(c))!='\n') ++c; 
*c = '\0'; 
} 
} 

line = CreateListLine(); 
commands = GetCommands(file); 
PushBack(line, 0); 
c = commands; 
ptr = line->head; 

while (f){ 
switch (*c){ 
case INC:{ 
if (ptr->sum<255) 
++ptr->sum; 
else { 
printf("%s", "Overflow\n"); 
ptr->sum=0; 
} 
++c; 
break; 
} 
case DEC:{ 
if (ptr->sum>0) 
—ptr->sum; 
else { 
printf("%s", "Overflow2\n"); 
ptr->sum=255; 
} 
++c; 
break; 
} 
case MOVL:{ 
getptr(line, ptr, MOVL); 
++c; 
break; 
} 
case MOVR:{ 
getptr(line, ptr, MOVR); 
++c; 
break; 
} 
case PRINT:{ 
printf("%d", (int)ptr->sum); 
++c; 
break; 
} 
case GET:{ 
printf("Enter symbol: \n"); 
ptr->sum = getchar(); 
fflush(stdin); 
++c; 
break; 
} 
case PRINTC:{ 
printf("%c", ptr->sum); 
++c; 
break; 
} 
case BEGIN:{ 
if (ptr->sum == 0) 
while ((*c)!=END) 
++c; 
++c; 
break; 
} 
case END:{ 
if (ptr->sum!=0) 
while ((*c)!=BEGIN) 
—c; 
else 
++c; 
break; 
} 
case 0:{ 
return 0; 
break; 
} 
} 
} 

printf("\n"); 

return 0; 
} 

charlist* getptr(listline *mlist, charlist *list, int dir){ 
if (dir == MOVR){ 
if (list->next == 0){ 
PushBack(mlist, 0); 
} 
return (list->next); 
} 

else if (dir == MOVL){ 
if (list->prev == 0) 
PushFront(mlist, 0); 
return (list->prev); 
} 

return 0; 
} 

char *GetCommands(FILE *file){ 
char c, *commands = 0, temp[256]; 
int i, j, f = 0, len = 0; 
listline *templist = 0; 
templist = CreateListLine(); 

while ((c = fgetc(file))!=EOF){ 
if (c == '\n'){ 
if (f == 2) 
++len; 
f = 0; 
continue; 
} 

if (c == '*') f = 1; 
if (f == 1) continue; 
if ((!(c == ' '))&&(!(c == '\t'))) f = 2; 
} 

fseek(file, ftell(file)-2, SEEK_SET); 
c= fgetc(file); 
if ((c == '\n')&&(fgetc(file) == EOF)); 
else ++len; 

commands = (char*)malloc((len+1)char); 
commands[len] = 0; 
fseek(file, SEEK_SET, 0); 
f = 0; 
i = 0; 

while ((c = fgetc(file))!=EOF){ 
if (c == '\n'){ 
if (f == 2){ 
for (j=0; templist->len; ++j){ 
temp[j] = PopFront(templist); 
} 

temp[j] = '\0'; 

if (!strcmp("inc", temp)) commands[i] = INC; 
if (!strcmp("dec", temp)) commands[i] = DEC; 
if (!strcmp("movl", temp)) commands[i] = MOVL; 
if (!strcmp("movr", temp)) commands[i] = MOVR; 
if (!strcmp("get", temp)) commands[i] = GET; 
if (!strcmp("print", temp)) commands[i] = PRINT; 
if (!strcmp("printc", temp)) commands[i] = PRINTC; 
if (!strcmp("begin", temp)) commands[i] = BEGIN; 
if (!strcmp("end", temp)) commands[i] = END; 
++i; 
} 
f = 0; 
continue; 
} 

if (c == '*') f = 1; 
if (f == 1) continue; 
if ((c == ' ')||(c == '\t')) continue; 
else f=2; 
PushBack(templist, c); 
} 

return commands; 
} 

listline * CreateListLine(){ 
listline *temp =
(listline*)malloc(sizeof(listline)); 
temp->len = 0; 
temp->head = temp->tail = 0; 
return temp; 
} 

void PushBack(listline *list, unsigned char c){ 
charlist *temp = (charlist*)malloc(sizeof(charlist)); 
if (temp == 0)exit(1); 
temp->sum = c; 
temp->next = 0; 
temp->prev = list->tail; 
if (list->tail) 
list->tail->next = temp; 

list->tail = temp; 

if (list->head == 0) 
list->head = temp; 
++list->len; 
} 

void PushFront(listline *list, unsigned char c){ 
charlist *temp = (charlist*)malloc(sizeof(charlist)); 
if (temp == 0) exit(1); 
temp->sum = c; 
temp->next = list->head; 
temp->prev = 0; 

if (list->tail == 0) 
list->tail = temp; 
++list->len; 
} 

unsigned char PopFront(listline *list){ 
unsigned char c; 
charlist *prev; 

if (list->head == 0) 
return 0; 

prev = list->head; 
list->head = list->head->next; 

if (list->head){ 
list->head->prev = 0; 
} 

if (prev == list->tail){ 
list->tail = 0; 
} 

c = prev->sum; 
free(prev); 
—list->len; 
return c; 
} 

unsigned char PopBack(listline *list){ 
unsigned char c; 
charlist *next; 
if (list->tail == 0) 
return 0; 

next = list->tail; 
list->tail = list->tail->prev; 

if (list->tail) 
list->tail->next = 0; 
if (next == list->head) 
list->head = 0; 

c = next->sum; 
free(next); 
—list->len; 
return c; 
}
