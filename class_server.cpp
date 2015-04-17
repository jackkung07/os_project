//Che Yi Kung
//HW6 Server
#include <stdlib.h>
#include <iostream>
#include <string>

#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#include <stdio.h>
#include <math.h>
#include <sstream>
#include <sys/time.h>

#define UNUSED -999
using namespace std;

const int MAXPENDING = 10;
const int MAXSEAT = 5;

pthread_mutex_t c_mutex;
pthread_mutex_t s_mutex;
pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond2 = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond3 = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond4 = PTHREAD_COND_INITIALIZER;

struct ThreadArgs {
    int clientSock; // socket to communicate with client
};

struct classInfo{
    string name;
    int year;
}classInfo[MAXSEAT];


void processClient(int clientSock){
    
    int bytesRecv, bytesSent, strSize, year;
    
    string name;
    char *buffer;
    char c;
    
    //receive string size of name
    bytesRecv = recv(clientSock, (void *) &strSize, sizeof(int), 0);
    if(bytesRecv < 0){
        cout << "fail to receive string size of the name" << endl;
        exit(-1);
    }
    
    
    //receive name
    int counter = 0;
    do{
        bytesRecv = recv(clientSock, (void*) &c, 1, 0);
        if(bytesRecv < 0){
            cout << "fail to receive name" << endl;
            exit(-1);
        }
        name = name + c;
        counter++;
    }while(counter != strSize);
    
    //receive year
    bytesRecv = recv(clientSock, (void *) &year, sizeof(int), 0);
    if(bytesRecv < 0){
        cout << "fail to receive year" << endl;
        exit(-1);
    }
    
    //Thread to wait for a condition
    if(year == 1)
    {
        pthread_cond_wait( &cond1, &c_mutex );
    }
    if(year == 2)
    {
        pthread_cond_wait( &cond2, &c_mutex );
    }
    if(year == 3)
    {
        pthread_cond_wait( &cond3, &c_mutex );
    }
    if(year == 4)
    {
        pthread_cond_wait( &cond4, &c_mutex );
    }
    
    //update class information
   
    pthread_mutex_lock(&s_mutex);
    int count = 0;
  for(int i = 0; i < MAXSEAT; i++){
        if(classInfo[i].year == UNUSED){
            break;
        }
        count++;
    }
    
    if(count < MAXSEAT){
        classInfo[count].name = name;
        classInfo[count].year = year;
    }
    pthread_mutex_unlock(&s_mutex);
    
    //send seat message
    //successful
    if(count < MAXSEAT)
    {
        string sucMsg1 = "Congratulations! ";
        string sucMsg2 = " You are in class!!";
        stringstream out;
        string outMsg;
        out << endl;
        out << sucMsg1 << name << sucMsg2 << endl;
        outMsg = out.str();
        strSize = outMsg.length();
        char *msg = new char[strSize];
        buffer = (char *) &strSize;
        bytesSent = send(clientSock, buffer, sizeof(int), 0);
        if(bytesSent < 0){
            cout << "fail to send string size of message" << endl;
            exit(-1);
        }
        
        strcpy(msg, outMsg.c_str());
        
        bytesSent = send(clientSock, (void *) msg, strSize, 0);
        if(bytesSent < 0){
            cout << "fail to send status message" << endl;
            exit(-1);
        }
        
        delete [] msg;
    }
    //unsuccessful
    else
    {
        string failMsg1 = "Sorry! ";
        string failMsg2 = " You are not in class!!";
        stringstream out;
        string outMsg;
        out << endl;
        out << failMsg1 << name << failMsg2 << endl;
        outMsg = out.str();
        strSize = outMsg.length();
        char *msg = new char[strSize];
        buffer = (char *) &strSize;
        bytesSent = send(clientSock, buffer, sizeof(int), 0);
        if(bytesSent < 0){
            cout << "fail to send string size of message" << endl;
            exit(-1);
        }
        
        strcpy(msg, outMsg.c_str());
        
        bytesSent = send(clientSock, (void *) msg, strSize, 0);
        if(bytesSent < 0){
            cout << "fail to send message" << endl;
            exit(-1);
        }
        delete [] msg;
    }
    
    //send current class information
    string classMsg;
    stringstream msg;
    int place;
    
    msg << endl;
    
    for(int i = 0; i < MAXSEAT; i++){
        if(classInfo[i].year != UNUSED){
            place = i + 1;
            msg << place << ". " << classInfo[i].name << " " << classInfo[i].year << endl;
        }
    }
    
    classMsg = msg.str();
    strSize = classMsg.length();
    buffer = (char *) &strSize;
    
    bytesSent = send(clientSock, buffer, sizeof(int), 0);
    if(bytesSent < 0){
        cout << "fail to send class info string size" << endl;
        exit(-1);
    }
    
    counter = 0;
    do{
        c = classMsg.at(counter);
        bytesSent = send(clientSock, (void*) &c, 1, 0);
        if(bytesSent < 0){
            cout << "fail to send leaderboard message" << endl;
            exit(-1);
        }
        counter++;
    }while(counter != strSize);
}


void *threadMain(void *threadArgs)
{
    // Insures thread resources are deallocated on return
    pthread_detach(pthread_self());
    // Extract socket file descriptor from argument
    int clientSock;
    clientSock = ((ThreadArgs *) threadArgs) -> clientSock;
    // Communicate with client
    processClient(clientSock);
    // Close client socket
    close(clientSock);
    return NULL;
}

void *threadCtr(void *ptr)
{
    // Insures thread resources are deallocated on return
    pthread_detach(pthread_self());
    // Extract socket file descriptor from argument
    time_t start = time(0);
    double seconds_since_start;
    while(true)
    {
        seconds_since_start = difftime( time(0), start);
        
        if(seconds_since_start > 5)
        {
            pthread_mutex_lock(&c_mutex);
            pthread_cond_broadcast(&cond4);
            pthread_mutex_unlock(&c_mutex);
        }
        
        if(seconds_since_start > 10)
        {
            pthread_mutex_lock(&c_mutex);
            pthread_cond_broadcast(&cond3);
            pthread_mutex_unlock(&c_mutex);
        }
        if(seconds_since_start > 15)
        {
            pthread_mutex_lock(&c_mutex);
            pthread_cond_broadcast(&cond2);
            pthread_mutex_unlock(&c_mutex);
        }
        if(seconds_since_start > 20)
        {
            pthread_mutex_lock(&c_mutex);
            pthread_cond_broadcast(&cond1);
            pthread_mutex_unlock(&c_mutex);
        }
    }
    return NULL;
}

int main(int argc, char *argv[]){
    
    //check for argument
    if(argc < 2 || argc > 2){
        cout << "invalid arguments" << endl;
        exit(-1);
    }
    
    //initialize mutex
    pthread_mutex_init(&c_mutex, NULL);
    pthread_mutex_init(&s_mutex, NULL);  
    //create a socket
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    if(sock < 0){
        cout << "fail to create a tcp socket" << endl;
        exit(-1);
    }
    
    //assign port number
    //port 10900
    unsigned short servPort = (unsigned short) strtoul(argv[1], NULL, 0);
    
    //clear out address struct
    struct sockaddr_in servAddr;
    memset(&servAddr, 0, sizeof(servAddr));
    
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(servPort);
    
    int status = bind(sock, (struct sockaddr *) &servAddr, sizeof(servAddr));
    
    if(status < 0){
        cout << "fail to bind the port" << endl;
        exit(-1);
    }
    
    //set socket to listen
    status = listen(sock, MAXPENDING);
    
    if(status < 0){
        cout << "fail to listen for incoming request" << endl;
        exit(-1);
    }
    
    //initialize struct
    for(int i = 0; i < MAXSEAT; i++)
    {
        classInfo[i].year = UNUSED;
    }
    
    //create thread to keep track of seat
    pthread_t tid_bak;
    status = pthread_create(&tid_bak, NULL, threadCtr, (void *) NULL);
    
    if(status != 0){
        cout << "fail to create a thread" << endl;
        exit(-1);
    }
    
    //accept connection
    while(true){
        //accept connection from client
        struct sockaddr_in clientAddr;
        socklen_t addrLen = sizeof(clientAddr);
        int clientSock = accept(sock,(struct sockaddr *) &clientAddr, &addrLen);
        if(clientSock < 0){
            cout << "fail to make connection" << endl;
            exit(-1);
        }
        
        ThreadArgs *targs = new ThreadArgs;
        targs -> clientSock = clientSock;
        
        pthread_t tid;
        status = pthread_create(&tid, NULL, threadMain, (void *) targs);
        
        if(status != 0){
            cout << "fail to create a thread" << endl;
            exit(-1);
        }
    }
    
    return 0;
}
