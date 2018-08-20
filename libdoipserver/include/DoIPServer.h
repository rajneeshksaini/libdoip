#ifndef DOIPSERVER_H
#define DOIPSERVER_H

#include <iostream>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <net/if.h>
#include <unistd.h>
#include "DoIPGenericHeaderHandler.h"
#include "RoutingActivationHandler.h"
#include "VehicleIdentificationHandler.h"
#include "DoIPGenericHeaderHandler.h"
#include "RoutingActivationHandler.h"
#include "DiagnosticMessageHandler.h"

const int _ServerPort = 13400;
const int _MaxDataSize = 64;   

typedef void (*DiagnosticCallback)(unsigned char*, int);

class DoIPServer {
  
    char VIN [18] = "00000000000000000"; //17 chars VIN + /0 char
    unsigned char LogicalAddress [2] = {0x00, 0x00};
    unsigned char EID [6];
    unsigned char GID [6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    unsigned char FurtherActionReq = 0x00;

    public:
	DoIPServer(DiagnosticCallback diag_callback): diag_callback{diag_callback} { };
        
        const DiagnosticCallback diag_callback;
        unsigned char data[_MaxDataSize];
        int sockfd_receiver, sockfd_sender;
        int sockfd_receiver_udp;

        struct sockaddr_in serverAdress, clientAdress;
	unsigned char* routedClientAddress;
              
        void receiveDiagnosticPayload(unsigned char* value, int length);
        void setupSocket();
        void setupUdpSocket();
        void receiveMessage();
        void receiveUdpMessage();
        void sendMessage(unsigned char* message, int messageLenght);
        void sendUdpMessage(unsigned char* message, int messageLength);
        void closeSocket();
        void closeUdpSocket();
        void setEIDdefault();
        void setVIN(const char* VINString);
        void setLogicalAddress(const unsigned int inputLogAdd);
        void setEID(const unsigned long inputEID);
        void setGID(const unsigned long inputGID);
        void setFAR(const unsigned int inputFAR);

};

        








#endif /* DOIPSERVER_H */

