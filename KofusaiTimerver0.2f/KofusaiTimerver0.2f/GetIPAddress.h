#pragma once

#include <Siv3D.hpp>
#include <iostream>
#include <string>
#include <winsock.h>


class IPtable {
public:
    size_t callBackFunk(char* ptr, size_t size, size_t nmemb, std::string* stream) {
        int realsize = size * nmemb;
        stream->append(ptr, realsize);
        return realsize;
    }
    std::string getIPAddress() {
        WSADATA wsadata;
        if (WSAStartup(MAKEWORD(1, 1), &wsadata) != 0) {
            return "error";
        }
        char hostname[256];
        if (gethostname(hostname, sizeof(hostname)) != 0) {
            return "error";
        }
        HOSTENT* hostend = gethostbyname(hostname);
        if (hostend == NULL) {
            return "error";
        }
        IN_ADDR inaddr;
        memcpy(&inaddr, hostend->h_addr_list[0], 4);
        char ip[256];
        strcpy_s(ip, inet_ntoa(inaddr));
        WSACleanup();
        std::string ipaddr = ip;
        return ipaddr;
    }
};