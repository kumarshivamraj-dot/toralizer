/* toralize.h */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <dlfcn.h>

#define PROXY     "127.0.0.1"
#define PROXYPORT  9050
#define reqsize sizeof(struct proxy_request)
#define ressize sizeof(struct proxy_response)
#define USERNAME "toraliz"

typedef unsigned char int8;
typedef unsigned short int int16;
typedef unsigned int int32;

/*


   +----+----+----+----+----+----+----+----+----+----+....+----+
   | VN | CD | DSTPORT |      DSTIP        | USERID       |NULL|
   +----+----+----+----+----+----+----+----+----+----+....+----+
#    1    1      2              4           variable       1


*/


struct proxy_request{
  int8 vn;
  int8 cd;
  int16 dstport;
  int32 dstip;
  char userid[8]; //user id containes the null character 
};
typedef struct proxy_request Req;
/*
 
		+----+----+----+----+----+----+----+----+
		| VN | CD | DSTPORT |      DSTIP        |
		+----+----+----+----+----+----+----+----+
 #	  1    1      2              4
*/

struct proxy_response{
  int8 vn;
  int8 cd;
  int16 _;
  int32 __;
};
typedef struct proxy_response Res;


// Creating a funcion of return type Req*
//Req* request(const char,const int);

int connect (int,const struct sockaddr*, socklen_t);
