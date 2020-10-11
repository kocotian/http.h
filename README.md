# http.h
simple and small http library for C99,
just include header in your project and you ready to GET.

for example, GET / from suckless.org on port 80:
```c
#include "http.h"

int
main()
{
	char *response; long long int size;
	if(!(size = httpGET("suckless.org", 80, "/", &response))) return -1;
	printf("%s", response);
	free(response);
	return 0;
}
```
same as higher, but without HTTP header:
```c
#include "http.h"

int
main()
{
	char *response, *data; long long int size;
	if(!(size = httpGET("suckless.org", 80, "/", &response))) return -1;
	data = strstr(response, "\r\n\r\n") + 4;
	printf("%s", data);
	free(response);
	return 0;
}
```
## compatibility:
any POSIX 7 compliant system: yes

windows: no

## additional features:
if you want to change maximal request length (*default 1024*), `#define MAX_REQUEST_LEN` as new request length __before__ `#including "http.h"`
