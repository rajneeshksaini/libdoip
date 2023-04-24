
#ifndef DOIPCLIENT_H
#define DOIPCLIENT_H
#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>
#include <cstddef>
#include <stdlib.h>
#include <cstdlib>
#include <cstring>

#include "DiagnosticMessageHandler.h"
#include "DoIPGenericHeaderHandler.h"

const int _serverPortNr=13400;
const int _maxDataSize=64;

#define ROUTING_ACTIVATION_SUCCESS (0x10)
#define MAX_CONNECTION_RETRIES	(3)

class DoIPClient{
    
public:
    void startTcpConnection();   
    void startUdpConnection();
    ssize_t sendRoutingActivationRequest();
    void sendVehicleIdentificationRequest(const char* address);
    bool receiveRoutingActivationResponse();
    void receiveUdpMessage();
    ssize_t receiveMessage(void *buffer, ssize_t size);
    ssize_t sendDiagnosticMessage(uint16_t sourceAddress, uint16_t targetAddress,
          unsigned char* userData, int userDataLength);
    void sendAliveCheckResponse();
    void setSourceAddress(unsigned char* address);
    void setSourceAddress(uint16_t address);
    void displayVIResponseInformation();
    void closeTcpConnection();
    void closeUdpConnection();
    void reconnectServer();

    int getSockFd();
    int getConnected();

    DoIPClient();
    DoIPClient(std::string &);
    DoIPClient(std::string &, bool);
    DoIPClient(std::string &, int);
    ~DoIPClient();

    bool isConnected() const;
    bool requestRoutingActivation(uint16_t address);
    
private:
    unsigned char _receivedData[_maxDataSize];
    int _sockFd, _sockFd_udp, _connected;
    int broadcast = 1;
    struct sockaddr_in _serverAddr, _clientAddr; 
    unsigned char sourceAddress [2];
    
    unsigned char VINResult [17];
    unsigned char LogicalAddressResult [2];
    unsigned char EIDResult [6];
    unsigned char GIDResult [6];
    unsigned char FurtherActionReqResult;

    std::string _ecuIPAddress;
    
    const std::pair<int, unsigned char*>* buildRoutingActivationRequest();
    const std::pair<int, unsigned char*>* buildVehicleIdentificationRequest();
    void parseVIResponseInformation(unsigned char* data);
    
    int emptyMessageCounter = 0;
    bool verbose;
    bool validateDiagnosticAction(uint8_t *, ssize_t length, PayloadType);
    int retries = MAX_CONNECTION_RETRIES;
};


struct __attribute__ ((packed)) DoIPRequestActivationResponse {
  struct DoIPHeader header;
  uint16_t logical_address_tester;
  uint16_t logical_address_doip_entity;
  uint8_t activation_response_code; // 0x10 is success
  uint8_t reserved[4];
  uint8_t oem_specific[4];

};


#endif /* DOIPCLIENT_H */

