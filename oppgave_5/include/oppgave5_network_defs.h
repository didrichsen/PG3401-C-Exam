#ifndef PG3401_V24_38_COMMON_NETWORK_HEADERS_H
#define PG3401_V24_38_COMMON_NETWORK_HEADERS_H

#define MAGIC_NUMBER 117
#define INET_ADDRSTRLEN 16
#define MAX_CLIENTS 2

#pragma pack(1)
struct TASK5_CONNECT_MESSAGE{
    int iMagicNumber;
    int iMessageSize;
    int iIpAddress;
    int iPhoneNumber;
};
#pragma pack()

#pragma pack(1)
typedef struct _TASK5_ACCEPT_MESSAGE{
    int iAcceptFlag;
}ACCEPT_MESSAGE;
#pragma pack()

#pragma pack(1)
struct TASK5_CLIENT_DATA{
    int iIpAddress;
    int iPhoneNumber;
};
#pragma pack()

#pragma pack(1)
typedef struct _TASK5_SEND_MESSAGE{

    int iMagicNumber;
    int iMessageSize;
    char szMessage[];

}SEND_MESSAGE;
#pragma pack()

#endif //PG3401_V24_38_COMMON_NETWORK_HEADERS_H
