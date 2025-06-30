/* toralize.c */
#include "toralize.h"


// ./toralize [ip_addrress] [port] ->

//This is the function actually just makes the request in the format table that we have defined in header file
Req* request(struct sockaddr_in *sock2){
  
  Req* req;
  req = malloc(reqsize);
  req->vn=4; //ipv4
  req->cd=1;
  req->dstport=sock2->sin_port;
  req->dstip=sock2->sin_addr.s_addr;
  strncpy((char *)req->userid,USERNAME,8); // The username is of 7char and the strncpy will add a null-terminator

  return req;
};

  
int connect(int s2, const struct sockaddr* sock2, socklen_t addrlen){
  
  //char *host;
  int s; // The file descriptor for connect sys call 
  Req* req;
  Res* res;
  char buf[ressize]; // The buffer is the response string
  int success;
  char temp[512];
  //Making a function pointer of connect using its signature
  int (*p)(int, const struct sockaddr*, socklen_t);


// why is this not needed after creating a wrapper around the connect 

 /* if (argc<3){
    fprintf(stderr,"Usage: %s <host> <port>\n",argv[0]);
    return -1;
  }


  host=argv[1];
  port=atoi(argv[2]); */


 /*
Here we ar intercepting the connect sys call to add more functionality the p will contain the original connect 
  */
  p=dlsym("RTLD_NEXT","connect");



  //Defining the socket
  s=socket(AF_INET,SOCK_STREAM,0);
  if(s<0){
    perror("socket");
    return -1;

  }


  struct sockaddr_in sock;
  sock.sin_family=AF_INET;
  sock.sin_port=htons(PROXYPORT);
  sock.sin_addr.s_addr = inet_addr(PROXY);



 if( p(s,(struct sockaddr*)&sock,sizeof(sock))){
  perror("connect");
    return -1;
  }
 
  printf("Connected to proxy\n");

  // Request prepares the request to be sent by write
  req= request((struct sockaddr_in *)&sock2);
  //we will use the write sys call to send the request in the format that is defiened by the request function 
  write(s,req,reqsize); //Send the request 
  memset(buf,0,ressize);// Make sure that the response_buffer does not contain anything from our previous computations so we will set it to 0;
  /*
    Reading the buffer means getting the response back 
    */

  if(read(s,buf,ressize)<1){
    perror("read");
    free(req);
    close(s);
    return -1;
  }
// Explained in the res!!!!
  res=(Res *)buf;

//Creating a predicate[an integer value that is returned by the function, for the purpose of telling the status of the code]

  success=(res->cd==90);
  if(!success){

  fprintf(stderr,"Unable to transverse"
          "the proxy, error code: %d\n",res->cd);
    close(s);
    free(req);
    return -1;

  }

  printf("Successfully connected through the proxy\n");




  free(req);
  return 0;


}
