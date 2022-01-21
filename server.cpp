/*

c++ socket server example, handles multiple clients using threads

Compile gcc server.c -lpthread -o server

*/

// Included headers files
#include<string> //strlen string
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h> //write
#include<pthread.h> //for threading , link with lpthread
#include<iostream> // input output
 
//the thread function

void *connection_handler(void *);

int main(int argc , char *argv[]){
	
  std::cout << "[SERVER] [NOTE] Starting up the SERVER" << std::endl;
  std::cout << "[SERVER] [NOTE] SERVER Port Number 8888" << std::endl;
  std::cout << "[SERVER] [NOTE] Protocol TCP" << std::endl;
  
  int socket_desc , client_sock, c;

  struct sockaddr_in server , client;
  
  // Create a TCP socket
  socket_desc = socket(AF_INET, SOCK_STREAM, 0);
  
  if (socket_desc==-1) {
    std::cout << "[SERVER] [ERROR] Could not create socket" << std::endl;
    return 1;
  }else{
    std::cout << "[SERVER] [NOTE] Socket has been created" << std::endl;
  }
  
  //Prepare the sockaddr_in structure

  server.sin_family = AF_INET;

  server.sin_addr.s_addr = INADDR_ANY;

  server.sin_port = htons(8888);


  // Bind the address and port
  if (bind(socket_desc,(struct sockaddr *)&server, sizeof(server)) < 0) {
    //std::cout << "[SERVER] [ERROR] Unable to bind" << std::endl;
    perror("[SERVER] [ERROR] bind failed");
    return -1;
  } else {
    std::cout << "[SERVER] [NOTE] Bind done" << std::endl;
  }
  
  //Listen on the socket, with 40 max connection requests queued
  if (listen(socket_desc, 3)==0) {
    std::cout << "[SERVER] [NOTE] Listening" << std::endl;
  }else{
    std::cout << "[SERVER] [ERROR] Failed to listening" << std::endl;
    return 1;
  }
  
  //Accept and incoming connection
  std::cout << "[SERVER] [NOTE] Waiting for incoming connections..." << std::endl;
  
  c = sizeof(struct sockaddr_in);

	pthread_t thread_id;
	
	
	while ((client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) ) {
	  
	  if (client_sock < 0) {
	    std::cout << "[SERVER] [WARNING] Client connection declined" << std::endl;
	  } else {
	    std::cout << "[SERVER] [NOTE] Client connected" << std::endl;
	  }
	  
	  if (pthread_create(&thread_id, NULL,  connection_handler,(void*)&client_sock) < 0) {
	    std::cout << "[SERVER] [ERROR] could not create thread" << std::endl;
	    return 1;
	  } else {
	    //Now join the thread , so that we dont terminate before the thread
	    //pthread_join( thread_id , NULL);
        std::cout << "[SERVER] [NOTE] thread created" << std::endl;
      }
	}
	return 0;
}


/*

 * This will handle connection for each client

 * */

void *connection_handler(void *socket_desc){

  //Get the socket descriptor

  int sock = *(int*)socket_desc;

  int read_size;

  char client_message[2000];

  

  //Receive a message from client

  while((read_size = recv(sock, client_message,2000,0))>0){
    //end of string marker
		client_message[read_size] = '\0';
		
		
		//Send the message back to client
    //send(sock, client_message, strlen(client_message),0);

         
    write(sock , client_message , strlen(client_message));

    

    std::cout << "[SERVER] [REQUEST] " << client_message << std::endl;
    
		std::cout << "[SERVER] [RESPONSE] " << client_message << std::endl;
		//clear the message buffer
		memset(client_message, 0, 2000);
  }
  
  
  if(read_size == 0) {
    std::cout << "[SERVER] [NOTE] Client disconnected" << std::endl;
    //fflush(stdout);
  } else if(read_size == -1) {
    std::cout << "[SERVER] [ERROR] " << "receive failed" << std::endl;
  }
  
  return 0;
  
}