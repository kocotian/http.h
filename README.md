# http.h
simple and small http library for C99
just include header in your project and you ready to GET.

for example, GET / from suckless.org on port 80:
```c
#include "http.h"
int
main()
{
    char *response; long long int size;
    if((size = httpGET("suckless.org", 80, "/", &response)) == NULL) return -1;
    printf("%s", response);
}
```
