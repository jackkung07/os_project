//Che Yi Kung
//HW6 Client

#include <iostream>
#include <string>

#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <cstdlib>
#include <netdb.h>


using namespace std;

int main(int argc, char *argv[]){

  //check for argument
  if(argc < 5 || argc > 5){
	cout << "invalid arguments" << endl;
	cout << "argument example: 127.0.0.1 10900 abc 1" << endl;
	exit(-1);
  }
  
  //create a tcp socket
  int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if(sock < 0){
	cout << "fail to create a tcp socket" << endl;
	exit(-1);
  }

  //establish connection
  char *IPAddr = argv[1];
  unsigned short servPort = (unsigned short) strtoul(argv[2], NULL, 0);

  //convert dotted decimal address to int
  unsigned long servIP;
  int status = inet_pton(AF_INET, IPAddr, &servIP);
  if(status <= 0){
	if(status == 0)
	  cout << "invalid address" << endl;
	else
	  cout << "failed to convert dotted decimal address to int" << endl;
	exit(-1);
  }

  //clear out address struct
  struct sockaddr_in servAddr;
  memset(&servAddr, 0, sizeof(servAddr));

  //set the fields
  servAddr.sin_family = AF_INET; // always AF_INET
  servAddr.sin_addr.s_addr = servIP;
  servAddr.sin_port = htons(servPort);
  
  //establish connection
  status =  connect(sock, (struct sockaddr *) &servAddr, sizeof(servAddr));
  if(status < 0){
	cout << "connection failed" << endl;
	exit(-1);
  }

  //communicate
  string temp;
  int bytesRecv;
  char *buffer;
  char c;

  //ask for name
  cout << endl << endl;
  //cout << "Welcome" << endl;
  //cout << "Please Enter Your Name" << endl;
  //cin >> temp;
  temp = argv[3];
  int strSize = temp.length();
  
  //send string size of name
  buffer = (char *) &strSize;
  int bytesSent = send(sock, buffer, sizeof(int), 0);
  if(bytesSent < 0){
	cout << "fail to send the string size of the name" << endl;
	exit(-1);
  }

  //send name char
  int counter = 0;
  do{
	c = temp.at(counter);
	bytesSent = send(sock, (void*) &c, 1, 0);
	if(bytesSent < 0){
	  cout << "fail to send the message" << endl;
	  exit(-1);
	}
	counter ++;
  }while(counter != strSize);  
  //send year
  int year;
  //cout << "Enter your year (1, 2, 3 or 4)" << endl;
  //cin >> year;
  year = atoi(argv[4]);
  buffer = (char *) &year;
  bytesSent = send(sock, buffer, sizeof(int), 0);
  if(bytesSent < 0){
          cout << "fail to send year" << endl;
          exit(-1);
  }

  //receive seat message
  bytesRecv = recv(sock, (void *) &strSize, sizeof(int), 0);
  if(bytesRecv < 0){
	cout << "fail to receive string size of message" << endl;
	exit(-1);
  }
    
  char *msg = new char[strSize];
  
  bytesRecv = recv(sock, (void *) msg, strSize, 0);
  if(bytesRecv < 0){
	cout << "fail to receive status message" << endl;
	exit(-1);
  }

  cout << endl << msg << endl;

  delete [] msg;

  //receive current class information
  bytesRecv = recv(sock, &strSize, sizeof(int), 0);
  if(bytesRecv < 0){
	cout << "fail to receive leaderboard string size" << endl;
	exit(-1);
  }

  string leaderMsg;
  if(strSize != 0)
{
  counter = 0;
  do{
	bytesRecv = recv(sock, (void*) &c, 1, 0);
	if(bytesRecv < 0){
	  cout << "fail to receive name" << endl;
	  exit(-1);
	}
	leaderMsg = leaderMsg + c;
	counter ++;
  }while(counter != strSize);
  
  cout << leaderMsg << endl;
 }
else
{
	cout << "empty class" << endl;
} 
  //close connection
  close(sock);
  
  return 0;
}
