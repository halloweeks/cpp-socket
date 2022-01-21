#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>

#define ADDRESS "127.0.0.1"
#define PORT 8888
#define MAXDATASIZE 2000

int main(int argc, char const *argv[]){
	int sock = 0, valread;
	struct sockaddr_in serv_addr;
  char buffer[MAXDATASIZE] = {0};
  
  // socket
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    std::cout << "[ERROR] " << "Socket creating failure" << std::endl;
  } else {
      std::cout << "[NOTE] " << "Socket created" << std::endl;
  }
  
  // assign IP, PORT
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);
  
  // Convert IPv4 and IPv6 addresses from text to binary form
  
  if(inet_pton(AF_INET, ADDRESS, &serv_addr.sin_addr)<=0){
    std::cout << "[ERROR] " << "Invalid address/ Address not supported" << std::endl;
  } else {
  	std::cout << "[NOTE] " << "Address is supported" << std::endl;
  }
  
  // connect the client socket to server socket
  if(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
    std::cout << "[ERROR] " << "Unable to connect the server" << std::endl;
  } else {
      std::cout << "[NOTE] " << "We are connected to the server" << std::endl;
  }
    
   while(true) {
  std::string msg="";
  std::cout << "\n[TCP]> ";
  getline(std::cin,msg);
  std::cout << std::endl;
  
  // if user input is blank to set default
  if(msg.compare("")==0){
  	msg = "Default message";
  } else if (msg.compare("exit")==0) {
  	break;
  }
  
  if(send(sock, msg.c_str(), 2000, 0)){
    std::cout << "[SEND] " << msg << std::endl;
  } else {
    std::cout << "[ERROR] " << "Unable to send message on the server" << std::endl;
  }
  
  
  if(recv(sock, buffer, 2000, 0)>0){
    std::cout << "[RECEIVE] " << buffer << std::endl;
    memset(buffer, 0, 2000);
  } else {
    std::cout << "[ERROR] " << "Unable to receive message on the server" << std::endl;
  }
    
  }
  
  close(sock);
  
  return 0; 
}