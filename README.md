# http.h
simple and small http library for C99,
just include header in your project and you ready to GET.

for example, GET / from suckless.org on port 80:
```c
#include "http.h"

int
main(void)
{
	char *response; long long int size;
	if(!(size = httpGET("suckless.org", 80, "/", &response))) return -1;
	printf("%s", response);
	/* or if you don't want http header: printf("%s", truncateHeader(response)); */
	free(response);
	return 0;
}
```
## compatibility:
any POSIX 7 compliant system: yes

windows: no

## additional features:
if you want to change maximal request length (*default 1024*), `#define MAX_REQUEST_LEN` as new request length __before__ `#including "http.h"`
