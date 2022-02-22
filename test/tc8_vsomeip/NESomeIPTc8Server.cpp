/**
 * Copyright @ 2019 iAuto (Shanghai) Co., Ltd.
 * All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are NOT permitted except as agreed by
 * iAuto (Shanghai) Co., Ltd.
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 */

#include <stdint.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "NESomeIPLog.h"
#include "NESomeIPTc8Process.h"

namespace nutshell {
#if 0
    #define SERVER_PORT 10000
#else
    #define SERVER_PORT 10001
#endif
    #define BUFFER_SIZE 100
    #define SERVER_QUEUE 20


    // define the struct for someip message header
    struct UtHeader {
        uint16_t SID;         // service id
        uint8_t GID;          // group id
        uint8_t PID;          // service primitive id
        uint32_t Length;      // the length filed of message
        uint32_t dc;          // reserved
        uint8_t ProtcolVer;   // protocol version
        uint8_t InterfaceVer;  // interface version
        uint8_t TID;          // type id
        uint8_t RID;          // result id
    };

    int SomeipReveiveControl(int socketid, uint8_t *buffer, const struct sockaddr *dest_addr) {
        uint16_t ServiceId, InstaceNum, EventGroup, EventId;
        struct UtHeader SomeipUt;
        memset(&SomeipUt, 0, sizeof(SomeipUt));
        memcpy(&SomeipUt, buffer, sizeof(SomeipUt));
        SomeipUt.SID = ((buffer[0]<< 8) | buffer[1]);
        SomeipUt.Length = ((buffer[4] << 24) | (buffer[5] << 16) | (buffer[6] << 8) | buffer[7]);
        NESOMEIP_DEBUG("SID=0x%x\n GID=0x%x\n PID=0x%x\n Length=0x%x",
            SomeipUt.SID, SomeipUt.GID, SomeipUt.PID, SomeipUt.Length);
        NESOMEIP_DEBUG("ProtcolVer=0x%x\n InterfaceVer=0x%x\n TID=0x%x\n RID=0x%x",
            SomeipUt.ProtcolVer, SomeipUt.InterfaceVer, SomeipUt.TID, SomeipUt.RID);

        if (SomeipUt.SID != 261) {
            NESOMEIP_ERROR("service id error: 0x%x", SomeipUt.SID);
            return -1;
        }

        if (SomeipUt.ProtcolVer != 1) {
            NESOMEIP_ERROR("Protcol Version  error: 0x%x", SomeipUt.ProtcolVer);
            return -1;
        }

        if (SomeipUt.InterfaceVer != 1) {
            NESOMEIP_ERROR("Protcol Version  error: 0x%x", SomeipUt.InterfaceVer);
            return -1;
        }

        if (SomeipUt.RID != 0) {
            NESOMEIP_ERROR("RID  error: 0x%x", SomeipUt.RID);
            return -1;
        }

        NESOMEIP_DEBUG("SomeipUt.GID =0x%x", SomeipUt.GID);
        if (SomeipUt.GID == 127) {
            NESOMEIP_DEBUG("hello word");
            switch (SomeipUt.PID) {
            case 248: {
                if (SomeipUt.Length == 12) {
                    buffer[7] = 8;
                    buffer[14] = 128;
                    ServiceId = (buffer[16] << 8) | buffer[17];
                    InstaceNum = (buffer[18] << 8) | buffer[19];
                    NESOMEIP_DEBUG("offer serviec request command receive ,service: 0x%x, instance number: 0x%x",
                        ServiceId, InstaceNum);
                    sendto(socketid, buffer, 16, 0, dest_addr, sizeof(struct sockaddr));
                    OfferService(ServiceId, InstaceNum);
                    return 10;
                } else {
                    NESOMEIP_ERROR("Length error: 0x%x", SomeipUt.Length);
                    return -1;
                }
            }
            case 247: {
                if (SomeipUt.Length == 10) {
                    buffer[7] = 8;
                    buffer[14] = 128;
                    ServiceId = (buffer[16] << 8) | buffer[17];

                    StopService(ServiceId);
                    NESOMEIP_DEBUG("stop serviec request command receive ,service: 0x%x\n", ServiceId);
                    sendto(socketid, buffer, 16, 0, dest_addr, sizeof(struct sockaddr));
                    return 20;
                } else {
                    NESOMEIP_DEBUG("Length error: 0x%x", SomeipUt.Length);
                    return -1;
                }
            }
            case 246: {
                if (SomeipUt.Length == 14) {
                    buffer[7] = 8;
                    buffer[14] = 128;

                    ServiceId = (buffer[16] << 8) | buffer[17];
                    EventGroup = (buffer[18] << 8) | buffer[19];
                    EventId = (buffer[20] << 8) | buffer[21];
                    sendto(socketid, buffer, 16, 0, dest_addr, sizeof(struct sockaddr));

                    TriggerEvent(ServiceId, EventGroup, EventId);
                    NESOMEIP_DEBUG("event trigge event request command receive ,service: 0x%x", ServiceId);
                    NESOMEIP_DEBUG("Eventgrpoup: 0x%x, eventid : 0x%x", EventGroup, EventId);
                    return 30;
                } else {
                    NESOMEIP_ERROR("Length error: 0x%x", SomeipUt.Length);
                    return -1;
                }
            }
            default: {
                NESOMEIP_ERROR("PID error: 0x%x", SomeipUt.PID);
                return -1;
            }
            }
        } else if (SomeipUt.GID == 0) {
            if (SomeipUt.PID == 2) {
                if (SomeipUt.Length == 8) {
                    buffer[14] = 128;
                    NESOMEIP_DEBUG("start service request command");
                    int ret = sendto(socketid, buffer, 16, 0, dest_addr, sizeof(struct sockaddr));
                    if (ret < 0) {
                        NESOMEIP_ERROR("sendto error [%s]", strerror(errno));
                    }
                    NESOMEIP_DEBUG("sendto success [%d]", ret);
                    return 40;
                } else {
                    NESOMEIP_ERROR("Length error: 0x%x", SomeipUt.Length);
                    return -1;
                }
            } else if (SomeipUt.PID == 3) {
                NESOMEIP_DEBUG("SomeipUt.Length = 0x%x", SomeipUt.Length);
                buffer[7] = 8;
                buffer[14] = 128;
                NESOMEIP_DEBUG("End service request command");
                sendto(socketid, buffer, 16, 0, dest_addr, sizeof(struct sockaddr));
                return 50;
            }
        } else {
            NESOMEIP_ERROR("GID erro 0x%x", SomeipUt.GID);
            return -1;
        }
        return 0;
    }
}  // namespace nutshell

int main() {
    NESOMEIP_DEBUG("start\n");
    int sockServer = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockServer < 0) {
        NESOMEIP_ERROR("socket failed\n");
        exit(1);
    }

    int on = 1;
    if (setsockopt(sockServer, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on))) {
        NESOMEIP_ERROR("setsockopt SO_REUSEADDR error, %s \n", strerror(errno));
        close(sockServer);
        exit(1);
    }

    if (setsockopt(sockServer, SOL_SOCKET, SO_REUSEPORT, reinterpret_cast<char*>(&on), sizeof(on))) {
        NESOMEIP_ERROR("setsockopt SO_REUSEPORT error, %s \n", strerror(errno));
        close(sockServer);
        exit(1);
    }

    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sockServer, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        NESOMEIP_ERROR("sock_server::main, Bind failed %s \n", strerror(errno));
        exit(1);
    }

    struct sockaddr_in clientAddr;
    memset(&clientAddr, 0, sizeof(clientAddr));
    socklen_t length = sizeof(clientAddr);

    NESOMEIP_DEBUG("start init \n");
    bool ret = nutshell::init();
    if (!ret) {
        NESOMEIP_ERROR("start init failed \n");
        exit(1);
    }
    //nutshell::OfferService(2000, 1);
    NESOMEIP_DEBUG("start init success \n");

    uint8_t buffer[BUFFER_SIZE] = { 0 };
    NESOMEIP_DEBUG("start recvfrom message \n");
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int n = recvfrom(sockServer, buffer, sizeof(buffer), 0, (struct sockaddr *)&clientAddr, &length);
        NESOMEIP_DEBUG("recvfrom message length [%d] \n", n);
        if (n > 25) {
            char testtest[100] = { 0 };
            memcpy(testtest, buffer + 25, n - 25);
            NESOMEIP_DEBUG("\n **** [%s] **** \n", testtest);
        }
        nutshell::SomeipReveiveControl(sockServer, buffer, (struct sockaddr *)&clientAddr);
    }

    close(sockServer);
    return 0;
}
/* EOF */

