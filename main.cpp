//
//  main.cpp
//  rasp-find
//
//  Created by Caio Lima on 27/03/13.
//  Copyright (c) 2013 Five ITs. All rights reserved.
//

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <string.h>
#include "network_utils.h"

using namespace std;


int main(int argc,char *argv[]){
    int sock;
    struct sockaddr_in destination;
    size_t echolen;
    int broadcast = 1;
    /* Create the UDP socket */
    if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0){
        std::cout<<"Error to open socket";
    }
    
    char* ip=getBroadcastIP();
    
    /* Construct the server sockaddr_in structure */
    memset(&destination, 0, sizeof(destination));
    
    /* Clear struct */
    destination.sin_family = AF_INET;
    
    /* Internet/IP */
    destination.sin_addr.s_addr =inet_addr(ip);
    
    /* IP address */
    destination.sin_port = htons(8000);
    destination.sin_len = sizeof(destination);
    
    /* server port */
    setsockopt(sock,
               IPPROTO_IP,
               IP_MULTICAST_IF,
               &destination,
               sizeof(destination));
    
    const char *cmsg = "where-is-pi";
    echolen = strlen(cmsg);
    
    // this call is what allows broadcast packets to be sent:
    if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof broadcast) == -1){
        cout<<"Unable to send boradcast messages in this network";
        return 0;
    }
    
    if (sendto(sock, cmsg,echolen, 0, (struct sockaddr *) &destination, sizeof(destination)) != echolen){
        cout<<"The message couldn't be sent...";
        return 0;
    }
    char* response=NULL;
    waitForResponse(response);
    
    cout<<"\n"<<response;
    
    return 0;
}
