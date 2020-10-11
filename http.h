#ifndef HTTP_H
#define HTTP_H

#define _XOPEN_SOURCE 700

/*
 * http.h
 * simple and small http library for C99
 * version 0.1.2
 * creator: kocotian
 */

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#ifndef MAX_REQUEST_LEN
#define MAX_REQUEST_LEN 1024
#endif

long long int httpGET(char *hostname, unsigned short port, char *path, char **buffer);
int getResponseStatus(char *response);
int parseResponseLine(char *response, char *value, char **buffer);
char *truncateHeader(char *response);
int getHeaderLength(char *response);

long long int
httpGET(char *hostname, unsigned short port, char *path, char **buffer)
{
	char tmpbuf[BUFSIZ];
	struct hostent *hostent;
	struct sockaddr_in sockaddr_in;
	in_addr_t in_addr;
	int sockfd;
	char *request_template = "GET %s HTTP/1.0\r\nHost: %s\r\n\r\n";
	char request[MAX_REQUEST_LEN];
	int request_length;
	int byteswrote = 0;
	long long int totalbytes = 0;
	int writeiter = 0;
	request_length = snprintf(request, MAX_REQUEST_LEN, request_template,
			path, hostname);
	if((hostent = gethostbyname(hostname)) == NULL)
	{
		fprintf(stderr, "gethostbyname(%s) error", hostname);
		return NULL;
	}
	if((sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
	{
		perror("socket opening error");
		return NULL;
	}
	in_addr = inet_addr(inet_ntoa(*(struct in_addr*)*(hostent -> h_addr_list)));
	sockaddr_in.sin_port          = htons(port);
	sockaddr_in.sin_addr.s_addr   = in_addr;
	sockaddr_in.sin_family        = AF_INET;
	if(connect(sockfd, (struct sockaddr*)&sockaddr_in, sizeof(sockaddr_in)) == -1)
	{
		perror("socket connecting error");
		return NULL;
	}
	if((write(sockfd, request, request_length)) == -1)
	{
		perror("sending request error");
		return NULL;
	}
	*buffer = malloc(BUFSIZ);
	while((byteswrote = read(sockfd, tmpbuf, BUFSIZ)) > 0)
	{
		totalbytes += byteswrote;
		strncat(*buffer, tmpbuf, byteswrote);
		*buffer = realloc(*buffer, BUFSIZ * ((++writeiter) + 1));
	}
	close(sockfd);
	return totalbytes;
}

int
getResponseStatus(char *response)
{
	char *firstline = strtok(response, "\r"); char *status;
	firstline = strtok(response, "\r");
	status = strtok(firstline, " ");
	status = strtok(NULL, " ");
	return atoi(status);
}

int
parseResponseLine(char *response, char *value, char **buffer)
{
	char *tmpbuf = strtok(response, "\r");
	char *val;
	if(!strcmp(value, "HTTP"))
	{
		*buffer = tmpbuf;
		return 0;
	}
	while(strcmp(tmpbuf, "\n"))
	{
		tmpbuf = strtok(NULL, "\r");
		unsigned short iter = -1;
		char allOk = 1;
		while(value[++iter])
		{
			if(value[iter] != tmpbuf[iter + 1])
			{
				allOk = 0;
				break;
			}
			else allOk = 1;
		}
		if(allOk)
		{
			iter += 2;
			int i2 = 1;
			*buffer = malloc(i2);
			while(tmpbuf[iter])
			{
				(*buffer)[i2 - 1] = tmpbuf[++iter];
				*buffer = realloc(*buffer, ++i2);
			}
			(*buffer)[i2 - 2] = 0;
			return 0;
		}
	}
	return -1;
}

char *truncateHeader(char *response)
{
	return strstr(response, "\r\n\r\n") + 4;
}

int getHeaderLength(char *response)
{
	return strstr(response, "\r\n\r\n") - response;
}

#endif
