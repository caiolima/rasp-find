//
//  network_utils.c
//  rasp-find
//
//  Created by Caio Lima on 27/03/13.
//  Copyright (c) 2013 Five ITs. All rights reserved.
//

#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <ifaddrs.h>
#include <string.h>
#include "network_utils.h"

char* getBroadcastIP(){
   
    struct ifaddrs *interfaces = NULL;
    struct ifaddrs *temp_addr = NULL;
    int success = 0;
    
    // retrieve the current interfaces - returns 0 on success
    success = getifaddrs(&interfaces);
    char* address;
    if (success == 0)
    {
        // Loop through linked list of interfaces
        temp_addr = interfaces;
        while(temp_addr != NULL)
        {
            if(temp_addr->ifa_addr->sa_family == AF_INET)
            {
            
                // Check if interface is en0 which is the wifi connection on the iPhone
                if(strcmp(temp_addr->ifa_name, "en1")==0)
                {
            
                    address = inet_ntoa(((struct sockaddr_in *)temp_addr->ifa_dstaddr)->sin_addr);
                    break;
                }
            }
            temp_addr = temp_addr->ifa_next;
        }
    }
    
    // Free memory
    freeifaddrs(interfaces);
    return address;
    
}

void waitForResponse(char* response){
    int socket_desc , client_sock , c , read_size;
    struct sockaddr_in server , client;
    char client_message[2000];
    
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8000 );
    
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        return;
    }
    
    
    //Listen
    listen(socket_desc , 3);
    
    c = sizeof(struct sockaddr_in);
    
    //accept connection from an incoming client
    client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
    
    
        //Receive a message from client
    while( (read_size = recv(client_sock , client_message , 2000 , 0)) > 0 ){
        strcpy(response, client_message);
    }
    
}
