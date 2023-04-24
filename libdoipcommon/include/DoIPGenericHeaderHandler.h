#ifndef DOIPGENERICHEADERHANDLER_H
#define DOIPGENERICHEADERHANDLER_H

#include <stdint.h>

const int _GenericHeaderLength = 8;
const int _NACKLength = 1;

const unsigned char _IncorrectPatternFormatCode = 0x00;
const unsigned char _UnknownPayloadTypeCode = 0x01;
const unsigned char _InvalidPayloadLengthCode = 0x04;

enum PayloadType {
    NEGATIVEACK,
    ROUTINGACTIVATIONREQUEST,
    ROUTINGACTIVATIONRESPONSE,
    VEHICLEIDENTREQUEST,
    VEHICLEIDENTRESPONSE,
    DIAGNOSTICMESSAGE,
    DIAGNOSTICPOSITIVEACK,
    DIAGNOSTICNEGATIVEACK,
    ALIVECHECKRESPONSE,
};

struct GenericHeaderAction {
    PayloadType type;
    unsigned char value;
    unsigned long payloadLength;
};

GenericHeaderAction parseGenericHeader(unsigned char* data, int dataLenght);
unsigned char* createGenericHeader(PayloadType type, uint32_t length);


struct __attribute__((packed)) DoIPHeader {
    uint8_t protocolVersion;
    uint8_t inverseProtocolVersion;
    uint16_t payloadType;
    uint32_t length;
};

struct __attribute__ ((packed)) DiagnosticMessageNegativeResponse {
    uint8_t requestSID;
    uint8_t nrc;
};

struct __attribute__ ((packed)) DiagnosticMessageF181Response {
    uint16_t requestID;
    uint8_t  data[64]; // TBD-length
};

struct __attribute__ ((packed)) DiagnosticMessageResponse {
    struct DoIPHeader header;
    uint16_t sourceAddress;
    uint16_t targetAddress;
    uint8_t  sidAndReplyFlags;
    union {
        struct DiagnosticMessageNegativeResponse negative;
        struct DiagnosticMessageF181Response f181;
        uint8_t positive[64]; // TBD - length
    } payload;
};

// fixed size would be source address and target address
#define DIAGNOSTIC_MESSAGE_RESPONSE_FIXED_SIZE          (sizeof(uint16_t) + sizeof(uint16_t))
// note that pld_length is the length in the DoIPHeader
#define DIAGNOSTIC_MESSAGE_RESPONSE_PAYLOAD_SIZE(pld_length) (pld_length - DIAGNOSTIC_MESSAGE_RESPONSE_FIXED_SIZE)

// flag indicates if diagnostic response is successful
#define DIAGNOSTIC_MESSAGE_RESPONSE_SUCCESS	    (0x40)
#define DIAGNOSTIC_MESSAGE_NEGATIVE_RESPONSE        (0x7F)
#define DiagnosticMessageResponseSuccess(rid, respValue)    ((rid + DIAGNOSTIC_MESSAGE_RESPONSE_SUCCESS) == respValue)

#endif /* DOIPGENERICHEADERHANDLER_H */

