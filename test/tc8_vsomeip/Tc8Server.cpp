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
#include <sys/wait.h>
#include <arpa/inet.h>
#include <errno.h>


#include "NESomeIPLog.h"


enum TESTCASE_NO
{
    SOMEIPSRV_SETUP_01= 0x1,
    SOMEIPSRV_SETUP_02,
    SOMEIPSRV_FORMAT_01,
    SOMEIPSRV_FORMAT_02,
    SOMEIPSRV_FORMAT_03,
    SOMEIPSRV_FORMAT_04,
    SOMEIPSRV_FORMAT_05,
    SOMEIPSRV_FORMAT_06,
    SOMEIPSRV_FORMAT_07,
    SOMEIPSRV_FORMAT_08,
    SOMEIPSRV_FORMAT_09,
    SOMEIPSRV_FORMAT_10,
    SOMEIPSRV_FORMAT_11,
    SOMEIPSRV_FORMAT_12,
    SOMEIPSRV_FORMAT_13,
    SOMEIPSRV_FORMAT_14,
    SOMEIPSRV_FORMAT_15,
    SOMEIPSRV_FORMAT_16,
    SOMEIPSRV_FORMAT_17,
    SOMEIPSRV_FORMAT_18,
    SOMEIPSRV_FORMAT_19,
    SOMEIPSRV_FORMAT_20,
    SOMEIPSRV_FORMAT_21,
    SOMEIPSRV_FORMAT_22,
    SOMEIPSRV_FORMAT_23,
    SOMEIPSRV_FORMAT_24,
    SOMEIPSRV_FORMAT_25,
    SOMEIPSRV_FORMAT_26,
    SOMEIPSRV_FORMAT_27,
    SOMEIPSRV_FORMAT_28,
    SOMEIPSRV_OPTION_01,
    SOMEIPSRV_OPTION_02,
    SOMEIPSRV_OPTION_03,
    SOMEIPSRV_OPTION_04,
    SOMEIPSRV_OPTION_05,
    SOMEIPSRV_OPTION_06,
    SOMEIPSRV_OPTION_07,
    SOMEIPSRV_OPTION_08,
    SOMEIPSRV_OPTION_09,
    SOMEIPSRV_OPTION_10,
    SOMEIPSRV_OPTION_11,
    SOMEIPSRV_OPTION_12,
    SOMEIPSRV_OPTION_13,
    SOMEIPSRV_OPTION_14,
    SOMEIPSRV_OPTION_15,
    SOMEIPSRV_SDMESSAEG_01,
    SOMEIPSRV_SDMESSAEG_02,
    SOMEIPSRV_SDMESSAEG_03,
    SOMEIPSRV_SDMESSAEG_04,
    SOMEIPSRV_SDMESSAEG_05,
    SOMEIPSRV_SDMESSAEG_06,
    SOMEIPSRV_SDMESSAEG_07,
    SOMEIPSRV_SDMESSAEG_08,
    SOMEIPSRV_SDMESSAEG_09,
    SOMEIPSRV_SDMESSAEG_10,
    SOMEIPSRV_SDMESSAEG_11,
    SOMEIPSRV_SDMESSAEG_12,
    SOMEIPSRV_SDMESSAEG_13,
    SOMEIPSRV_SDMESSAEG_14,
    SOMEIPSRV_SDMESSAEG_15,
    SOMEIPSRV_SDMESSAEG_16,
    SOMEIPSRV_SDMESSAEG_17,
    SOMEIPSRV_SDMESSAEG_18,
    SOMEIPSRV_SDMESSAEG_19,
    SOMEIPSRV_BEHAVOR_01,
    SOMEIPSRV_BEHAVOR_02,
    SOMEIPSRV_BEHAVOR_03,
    SOMEIPSRV_BEHAVOR_04,
    SOMEIPSRV_BEHAVOR_05,
    SOMEIPSRV_BEHAVOR_06,
    SOMEIPSRV_BASIC_01,
    SOMEIPSRV_BASIC_02,
    SOMEIPSRV_BASIC_03,
    SOMEIPSRV_ONWIRE_01,
    SOMEIPSRV_ONWIRE_02,
    SOMEIPSRV_ONWIRE_03,
    SOMEIPSRV_ONWIRE_04,
    SOMEIPSRV_ONWIRE_05,
    SOMEIPSRV_ONWIRE_06,
    SOMEIPSRV_ONWIRE_07,
    SOMEIPSRV_ONWIRE_08,
//    SOMEIPSRV_ONWIRE_09, // the lastest someip version have no this test
    SOMEIPSRV_ONWIRE_10,
    SOMEIPSRV_ONWIRE_11,
    SOMEIPSRV_ONWIRE_12,
    SOMEIPSRV_RPC_01 ,
    SOMEIPSRV_RPC_02,
    SOMEIPSRV_RPC_03,
    SOMEIPSRV_RPC_04,
    SOMEIPSRV_RPC_05,
    SOMEIPSRV_RPC_06,
    SOMEIPSRV_RPC_07,
    SOMEIPSRV_RPC_08,
    SOMEIPSRV_RPC_09,
    SOMEIPSRV_RPC_10,
    SOMEIPSRV_RPC_11,
    SOMEIPSRV_RPC_12,
    SOMEIPSRV_RPC_13,
    SOMEIPSRV_RPC_14,
    SOMEIPSRV_RPC_15,
    SOMEIPSRV_RPC_16,
    SOMEIPSRV_RPC_17,
    SOMEIPSRV_RPC_18,
    SOMEIPSRV_RPC_19,
    SOMEIPSRV_RPC_20,
};

namespace nutshell {

    static pid_t someip_pid = 0;

    bool stopSomeipDaemon()
    {
        system("killall tc8vsomeip");
        someip_pid = 0;
        NESOMEIP_DEBUG("----------stopSomeipDaemon--------------------------------");
        return true;
    }
    bool startSomeipDaemon()
    {

        NESOMEIP_DEBUG("----------startSomeipDaemon--------------------");
        bool res = false;
        someip_pid = fork();
        if (someip_pid < 0) {
            NESOMEIP_ERROR("fork failed\n");
        }
        else {
            if (someip_pid == 0) {
                char* argv[] = {NULL};
                if (execv("./build/tc8vsomeip", argv) < 0) {
                    NESOMEIP_ERROR("execv failed\n");
                    exit(-1);
                }
    
            }
            else {
                signal(SIGCHLD, SIG_IGN);
                usleep(100*1000);
                if (kill(someip_pid, 0) != 0) {
                    NESOMEIP_ERROR("start failed");
                }
                else {
                    res = true;
                }
            }
        }
        return res;
    }

    #define SERVER_PORT 10000
    #define BUFFER_SIZE 100
    #define SERVER_QUEUE 20
    bool sendtosomeip(int socketid, uint8_t *buffer, int len, const struct sockaddr *dest_addr)
    {
        usleep(100*1000);
        // send to someip daemon
        int sockClient = socket(AF_INET, SOCK_DGRAM, 0);
        if (sockClient < 0) {
            NESOMEIP_ERROR("socket failed\n");
            exit(1);
        }

        int on = 1;
        if (setsockopt(sockClient, SOL_SOCKET, SO_REUSEADDR| SO_BROADCAST, &on, sizeof(on))) {
            NESOMEIP_ERROR("setsockopt SO_REUSEADDR error, %s \n", strerror(errno));
            close(sockClient);
            exit(1);
        }


        struct sockaddr_in sendAddr;
        memset(&sendAddr, 0, sizeof(sendAddr));
        sendAddr.sin_family = AF_INET;
        sendAddr.sin_port = htons(10001);
        sendAddr.sin_addr.s_addr = inet_addr("127.0.0.0");
        //inet_aton("127.0.0.0" , &sendAddr.sin_addr);
        if (sendto(sockClient, buffer, len , 0 , (struct sockaddr *) &sendAddr, sizeof(sendAddr))==-1) {
            NESOMEIP_DEBUG("sendto failed,%s\n", strerror(errno));
        }

        // wait someip daemon response
        uint8_t buffer_tmp[BUFFER_SIZE] = { 0 };
        NESOMEIP_DEBUG("start recvfrom message \n");

        memset(buffer_tmp, 0, sizeof(buffer_tmp));
        int n = recvfrom(sockClient, buffer_tmp, sizeof(buffer_tmp), 0, NULL, 0);
        NESOMEIP_DEBUG("recvfrom message length [%d] \n", n);

        // forward response canoe
        sendto(socketid, buffer_tmp, 16, 0, dest_addr, sizeof(struct sockaddr));

        close(sockClient);
        usleep(100*1000);
        return 0;
    }




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

    int SomeipReveiveControl(int socketid, uint8_t *buffer, int len, const struct sockaddr *dest_addr) {
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
        static int castNo = 1;
        NESOMEIP_DEBUG("SomeipUt.GID =0x%x", SomeipUt.GID);
        if (SomeipUt.GID == 127) {

            NESOMEIP_DEBUG("hello word");
            switch (SomeipUt.PID) {
            case 248: {
                if (SomeipUt.Length == 12) {
                    NESOMEIP_DEBUG("OfferService");
                    sendtosomeip(socketid, buffer, len, dest_addr);
#if 0
                    buffer[7] = 8;
                    buffer[14] = 128;
                    ServiceId = (buffer[16] << 8) | buffer[17];
                    InstaceNum = (buffer[18] << 8) | buffer[19];
                    NESOMEIP_DEBUG("offer serviec request command receive ,service: 0x%x, instance number: 0x%x",
                        ServiceId, InstaceNum);
                    sendto(socketid, buffer, 16, 0, dest_addr, sizeof(struct sockaddr));
                    OfferService(ServiceId, InstaceNum);
#endif
                    
                    return 10;
                } else {
                    NESOMEIP_ERROR("Length error: 0x%x", SomeipUt.Length);
                    return -1;
                }
            }
            case 247: {
                if (SomeipUt.Length == 10) {
                    sendtosomeip(socketid, buffer, len, dest_addr);
#if 0
                    buffer[7] = 8;
                    buffer[14] = 128;
                    ServiceId = (buffer[16] << 8) | buffer[17];
                    //StopService(ServiceId);
                    NESOMEIP_DEBUG("stop serviec request command receive ,service: 0x%x\n", ServiceId);
                    sendto(socketid, buffer, 16, 0, dest_addr, sizeof(struct sockaddr));
#endif
                    return 20;
                } else {
                    NESOMEIP_DEBUG("Length error: 0x%x", SomeipUt.Length);
                    return -1;
                }
            }
            case 246: {
                if (SomeipUt.Length == 14) {
                    sendtosomeip(socketid, buffer, len, dest_addr);
                    buffer[7] = 8;
                    buffer[14] = 128;

                    ServiceId = (buffer[16] << 8) | buffer[17];
                    EventGroup = (buffer[18] << 8) | buffer[19];
                    EventId = (buffer[20] << 8) | buffer[21];
                    sendto(socketid, buffer, 16, 0, dest_addr, sizeof(struct sockaddr));

                    //TriggerEvent(ServiceId, EventGroup, EventId);
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
                    if (castNo == SOMEIPSRV_SETUP_01) {
                        system("cp vsomeip.json-most-ok vsomeip.json");
                    }
                    else if (castNo == SOMEIPSRV_OPTION_14) {
                        system("cp vsomeip.json-option14-test vsomeip.json");
                    }
                    else if (castNo == SOMEIPSRV_SDMESSAEG_07) { // SD_MESSAGE_07 start
                        system("cp vsomeip.json-sdmsg_07_ok vsomeip.json");
                    }
                    else if (castNo == SOMEIPSRV_BEHAVOR_03) {
                        //system("cp vsomeip.json-behavior03-ok vsomeip.json");
                    }
                    else if (castNo == SOMEIPSRV_RPC_01) {
                        system("cp vsomeip.json-rpc1_12-ok vsomeip.json");
                    }
                    else if (castNo == SOMEIPSRV_RPC_12) {
                        system("cp vsomeip.json-rpc12-ok vsomeip.json");
                    }
                    else if (castNo == SOMEIPSRV_RPC_13) { // RPC_13 start
                        system("cp vsomeip.json_rpc13-ok vsomeip.json");
                    }
                    else if (castNo == SOMEIPSRV_RPC_15) { // RPC_13 start
                        system("cp vsomeip.json-rpc15-ok vsomeip.json");
                    }
                    else if (castNo == SOMEIPSRV_RPC_16) { // RPC_13 start
                        system("cp vsomeip.json-rpc15-ok vsomeip.json");
                    }
                    else {
                        system("cp vsomeip.json-most-ok vsomeip.json");
                    }
                    system("sync");
                    NESOMEIP_DEBUG("---------start testcase[%d] request command----", castNo++);
                    startSomeipDaemon();
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
                stopSomeipDaemon();
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
#if 0
    for (int i = 0; i < 5; i++) {
        nutshell::startSomeipDaemon();
        sleep(1);
        nutshell::stopSomeipDaemon();
    }
#endif
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
        nutshell::SomeipReveiveControl(sockServer, buffer, n , (struct sockaddr *)&clientAddr);
    }

    close(sockServer);
    return 0;
}
/* EOF */

