#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include "list.h"

#define PORT 9001
#define ACK "ACK: "

int main(int argc, char const *argv[]) {
    int n, val, idx;
    
    // Create server socket
    int servSockD = socket(AF_INET, SOCK_STREAM, 0);
    if (servSockD == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    char buf[1024];
    char sbuf[1024];
    char *token;
    
    struct sockaddr_in servAddr;
    list_t *mylist;

    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(PORT);
    servAddr.sin_addr.s_addr = INADDR_ANY;

    // Bind socket to the specified IP and port
    if (bind(servSockD, (struct sockaddr*)&servAddr, sizeof(servAddr)) == -1) {
        perror("Bind failed");
        exit(1);
    }

    // Listen for connections
    listen(servSockD, 1);
    
    // Accept client connection
    int clientSocket = accept(servSockD, NULL, NULL);
    if (clientSocket == -1) {
        perror("Accept failed");
        exit(1);
    }

    // Allocate the linked list
    mylist = list_alloc(); 

    while (1) {
        // Receive message from client socket
        n = recv(clientSocket, buf, sizeof(buf), 0);
        if (n > 0) {
            buf[n] = '\0';
            token = strtok(buf, " ");
            
            if (token == NULL) {
                memset(buf, '\0', 1024);
                continue;
            }

            if (strcmp(token, "exit") == 0) {
                list_free(mylist);
                close(clientSocket);
                close(servSockD);
                exit(0);
            } 
            else if (strcmp(token, "get_length") == 0) {
                val = list_length(mylist);
                sprintf(sbuf, "Length = %d", val);
            } 
            else if (strcmp(token, "add_front") == 0) {
                token = strtok(NULL, " ");
                if (token != NULL) {
                    val = atoi(token);
                    list_add_to_front(mylist, val);
                    sprintf(sbuf, "%s%d", ACK, val);
                } else {
                    sprintf(sbuf, "Error: Missing value for add_front");
                }
            } 
            else if (strcmp(token, "add_back") == 0) {
                token = strtok(NULL, " ");
                if (token != NULL) {
                    val = atoi(token);
                    list_add_to_back(mylist, val);
                    sprintf(sbuf, "%s%d", ACK, val);
                } else {
                    sprintf(sbuf, "Error: Missing value for add_back");
                }
            } 
            else if (strcmp(token, "add_position") == 0) {
                token = strtok(NULL, " ");
                if (token != NULL) {
                    idx = atoi(token);
                    token = strtok(NULL, " ");
                    if (token != NULL) {
                        val = atoi(token);
                        list_add_at_index(mylist, val, idx);
                        sprintf(sbuf, "%s index %d, value %d", ACK, idx, val);
                    } else {
                        sprintf(sbuf, "Error: Missing value for add_position");
                    }
                } else {
                    sprintf(sbuf, "Error: Missing index for add_position");
                }
            } 
            else if (strcmp(token, "remove_front") == 0) {
                val = list_remove_from_front(mylist);
                sprintf(sbuf, "Removed: %d", val);
            } 
            else if (strcmp(token, "remove_back") == 0) {
                val = list_remove_from_back(mylist);
                sprintf(sbuf, "Removed: %d", val);
            } 
            else if (strcmp(token, "remove_position") == 0) {
                token = strtok(NULL, " ");
                if (token != NULL) {
                    idx = atoi(token);
                    val = list_remove_at_index(mylist, idx);
                    sprintf(sbuf, "Removed: %d at index %d", val, idx);
                } else {
                    sprintf(sbuf, "Error: Missing index for remove_position");
                }
            } 
            else if (strcmp(token, "get") == 0) {
                token = strtok(NULL, " ");
                if (token != NULL) {
                    idx = atoi(token);
                    val = list_get_elem_at(mylist, idx);
                    sprintf(sbuf, "Element at index %d = %d", idx, val);
                } else {
                    sprintf(sbuf, "Error: Missing index for get");
                }
            } 
            else if (strcmp(token, "print") == 0) {
                char *list_str = listToString(mylist);
                if (list_str != NULL) {
                    sprintf(sbuf, "%s", list_str);
                    free(list_str);
                } else {
                    sprintf(sbuf, "List is empty or error printing");
                }
            } 
            else {
                sprintf(sbuf, "Unknown command");
            }

            // Send response back to the client socket
            send(clientSocket, sbuf, sizeof(sbuf), 0);
        }
        memset(buf, '\0', 1024);
        memset(sbuf, '\0', 1024);
    }

    return 0;
}