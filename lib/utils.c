
/*
 *  utils.c
 *
 *  Copyright (C) 2006 Alex deVries
 *
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "afp.h"
#include "utils.h"

struct afp_path_header_long {
	unsigned char type;
	unsigned char len;
}  __attribute__((__packed__)) ;

struct afp_path_header_unicode {
	uint8_t type;
	uint32_t hint;
	uint16_t unicode;
}  __attribute__((__packed__)) ;

int translate_path(struct afp_volume * volume,
	char *incoming, char * outgoing)
{
	return 0;
}

unsigned short utf8_to_string(char * dest, char * buf, unsigned short maxlen)
{
	return copy_from_pascal_two(dest,buf+4,maxlen);
}

unsigned char unixpath_to_afppath(
	struct afp_server * server,
	char * buf)
{
	unsigned char encoding = server->path_encoding;
	char *p =NULL, *end;
	unsigned short *len_p=NULL,len;

	switch (encoding) {
	case kFPUTF8Name:
		len_p = (void *) buf + 5;
		p=buf+7;
		break;
	case kFPLongName:
		len_p = (void *) buf + 1;
		p=buf+3;
	}
	len=ntohs(*len_p);
	end=p+len;

	while (p<end) {
		if (*p=='/') *p='\0';
		p++;
	}
	return 0;
}


unsigned char copy_from_pascal(char *dest, char *pascal,unsigned int max_len) 
{

	unsigned char len;

	if (!pascal) return 0;
	len=*pascal;

	if (max_len<len) len=max_len;

	bzero(dest,max_len);
	memcpy(dest,pascal+1,len);
	return len;
}

unsigned short copy_from_pascal_two(char *dest, char *pascal,unsigned int max_len) 
{

	unsigned short * len_p = (unsigned short *) pascal;
	unsigned short len = ntohs(*len_p);

	if (max_len<len) len=max_len;
	if (len==0) return 0;
	bzero(dest,max_len);
	memcpy(dest,pascal+2,len);
	return len;
}

unsigned char copy_to_pascal(char *dest, const char *src) 
{
	unsigned char len = (unsigned char) strlen(src);
	dest[0]=len;

	memcpy(dest+1,src,len);
	return len;
}

unsigned short copy_to_pascal_two(char *dest, const char *src) 
{
	unsigned short * sendlen = (void *) dest;
	char * data = dest + 2;

	unsigned short len ;
	if (!src) {
		dest[0]=0;
		dest[1]=0;
		return 2;
	}
	len = (unsigned short) strlen(src);
	*sendlen=htons(len);
	memcpy(data,src,len);
	return len;
}

unsigned char sizeof_path_header(struct afp_server * server)
{
	switch (server->path_encoding) {
	case kFPUTF8Name:
		return(sizeof(struct afp_path_header_unicode));
	case kFPLongName:
		return(sizeof(struct afp_path_header_long));
	}
	return 0;
}


void copy_path(struct afp_server * server, char * dest, const char * pathname, unsigned char len)
{

	char tmppathname[255];
	unsigned char encoding = server->path_encoding;
	struct afp_path_header_unicode * header_unicode = (void *) dest;
	struct afp_path_header_long * header_long = (void *) dest;
	unsigned char offset, header_len, namelen;

	switch (encoding) {
	case kFPUTF8Name:
		header_unicode->type=encoding;
		header_unicode->hint=htonl(0x08000103);
		offset = 5;
		header_len = sizeof(struct afp_path_header_unicode);
		namelen=copy_to_pascal_two(tmppathname,pathname);
		memcpy(dest+offset,tmppathname,namelen+2);
		break;
	case kFPLongName:
		header_long->type=encoding;
		offset = 1;
		header_len = sizeof(struct afp_path_header_long);
		namelen=copy_to_pascal(tmppathname,pathname) ;
		memcpy(dest+offset,tmppathname,namelen+1);
	}
}

int invalid_filename(struct afp_server * server, const char * filename) 
{

	unsigned int maxlen=0;

	if ((strlen(filename)==1) && (*filename=='/')) return 0;

	/* From p.34, each individual file can be 255 chars for > 30
	   for Long or short names.  UTF8 is "virtually unlimited" */

	if (server->using_version->av_number < 30) 
		maxlen=31; 
	else
		if (server->path_encoding==kFPUTF8Name) 
			maxlen=1024;
		else 
			maxlen=255;

	return ((strlen(filename)>maxlen));

}


unsigned char is_netatalk(struct afp_server * server) 
{
	if (strcmp(server->machine_type,"Netatalk")==0) 
		return 1;
	else
		return 0;
}

