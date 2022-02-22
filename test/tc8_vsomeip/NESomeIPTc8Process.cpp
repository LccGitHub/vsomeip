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

#include <unistd.h>
#include <thread>
#include <functional>
#include <vsomeip/vsomeip.hpp>

#include "NESomeIPLog.h"
#include "NESomeIPTc8Process.h"

#define NESOMEIP_UINT8_TO_CHAR(ptr) reinterpret_cast<char*>(const_cast<uint8_t*>(ptr))
#define byte_t uint8_t

namespace nutshell {

    bool initComplete = false;
    std::shared_ptr<vsomeip::runtime> runtimePointer = NULL;
    std::shared_ptr<vsomeip::application> someipObject = NULL;

    uint16_t instaceId01 = 0x0001;
    uint16_t instaceId02 = 0x0002;


    uint8_t majorVersion = 1;
    uint8_t minorVersion = 0;

    void messageHandlerTest1(const std::shared_ptr<vsomeip::message>& message) {
        NESOMEIP_DEBUG("--------------messageHandlerTest1:::: Recv message on serviceId [0x%x], instanceId [0x%x], methodId [0x%x]",
            message->get_service(), message->get_instance(), message->get_method());
        NESOMEIP_DEBUG("messageHandlerTest1:::: MsgType [0x%x], interfaceVer [0x%x], returnCode [0x%x]",
            message->get_message_type(), message->get_interface_version(), message->get_return_code());

        std::shared_ptr<vsomeip::payload> tempPayload = message->get_payload();
        std::shared_ptr<vsomeip::message> responseMessage = runtimePointer->create_response(message);

        if (vsomeip::message_type_e::MT_REQUEST_NO_RETURN == message->get_message_type()) {
            NESOMEIP_DEBUG("no need to response");
            return;
        }

        if (0x01 != message->get_protocol_version()) {
            NESOMEIP_DEBUG("response error protocol version");
            responseMessage->set_return_code(vsomeip::return_code_e::E_WRONG_PROTOCOL_VERSION);
            someipObject->send(responseMessage);
            return;
        }

        // SOMEIPSRV_RPC_09, SOMEIPSRV_RPC_18, SOMEIPSRV_RPC_19, SOMEIPSRV_RPC_20
        if (0x7b == message->get_method()) {
            responseMessage->set_message_type(vsomeip::message_type_e::MT_ERROR);
            responseMessage->set_return_code(vsomeip::return_code_e::E_UNKNOWN_METHOD);
            someipObject->send(responseMessage);
            return;
        }

        if (0xc0 == (int)message->get_return_code()) {
            responseMessage->set_return_code(vsomeip::return_code_e::E_OK);
            NESOMEIP_DEBUG("error return code !!!!!");
        }
#if 0
        // case SOMEIPSRV_ONWIRE_09
        if (0x15 == message->get_method()) {
            responseMessage->set_message_type(vsomeip::message_type_e::MT_REQUEST_ACK);
            someipObject->send(responseMessage);
            return;
        }
#endif
        responseMessage->set_return_code(vsomeip::return_code_e::E_OK);

        
        if (NULL != tempPayload) {
            const byte_t* tempData = tempPayload->get_data();
            vsomeip::length_t tempLength = tempPayload->get_length();
            NESOMEIP_DEBUG("messageHandlerTest1:::: tempdata length [%d], content [%s]",
                tempLength, NESOMEIP_UINT8_TO_CHAR(tempData));

            NESOMEIP_DEBUG("messageHandlerTest1:::: start response");
            if (NULL == responseMessage) {
                NESOMEIP_DEBUG("messageHandlerTest1::::create response message error");
                return;
            }

            responseMessage->set_payload(tempPayload);
            //responseMessage->setMessageLength(8 + tempLength);

            someipObject->send(responseMessage);
        } else {
            NESOMEIP_DEBUG("messageHandlerTest1::::messageHandlerTest1:::: tempPayload IS Null");
        }
    }

    void messageHandlerTest2(const std::shared_ptr<vsomeip::message>& message) {
        NESOMEIP_DEBUG("messageHandlerTest2:::: Recv message on serviceId [0x%x], instanceId [0x%x], methodId [0x%x]",
            message->get_service(), message->get_instance(), message->get_method());
        NESOMEIP_DEBUG("messageHandlerTest2:::: MsgType [0x%x], interfaceVer [0x%x], returnCode [0x%x]",
            message->get_message_type(), message->get_interface_version(), message->get_return_code());

        std::shared_ptr<vsomeip::payload> tempPayload = message->get_payload();
        if (NULL != tempPayload) {
            const byte_t* tempData = tempPayload->get_data();
            vsomeip::length_t tempLength = tempPayload->get_length();
            NESOMEIP_DEBUG("messageHandlerTest2:::: tempdata length [%d], content [%s]",
                tempLength, NESOMEIP_UINT8_TO_CHAR(tempData));
        } else {
            NESOMEIP_DEBUG("messageHandlerTest2:::: tempPayload IS Null");
        }
    }


    void messageHandlerTest3(const std::shared_ptr<vsomeip::message>& message) {
        NESOMEIP_DEBUG("messageHandlerTest3:::: Recv message on serviceId [0x%x], instanceId [0x%x], methodId [0x%x]",
            message->get_service(), message->get_instance(), message->get_method());
        NESOMEIP_DEBUG("messageHandlerTest3:::: MsgType [0x%x], interfaceVer [0x%x], returnCode [0x%x]",
            message->get_message_type(), message->get_interface_version(), message->get_return_code());

        std::shared_ptr<vsomeip::payload> tempPayload = message->get_payload();
        if (NULL != tempPayload) {
            const byte_t* tempData = tempPayload->get_data();
            vsomeip::length_t tempLength = tempPayload->get_length();
            NESOMEIP_DEBUG("messageHandlerTest3:::: tempdata length [%d], content [%s]",
                tempLength, NESOMEIP_UINT8_TO_CHAR(tempData));
        } else {
            NESOMEIP_DEBUG("messageHandlerTest3:::: tempPayload IS Null");
        }

        NESOMEIP_DEBUG("messageHandlerTest3:::: send requst ack");
        std::shared_ptr<vsomeip::message> responseAck = runtimePointer->create_response(message);
        if (NULL == responseAck) {
            NESOMEIP_DEBUG("create response ack error");
            return;
        }

        responseAck->set_message_type(vsomeip::message_type_e::MT_REQUEST_ACK);
        //responseAck->setMessageLength(8);

        someipObject->send(responseAck);

        NESOMEIP_DEBUG("messageHandlerTest3:::: send requst response");
        std::shared_ptr<vsomeip::message> responseMessage = runtimePointer->create_response(message);
        if (NULL == responseMessage) {
            NESOMEIP_DEBUG("messageHandlerTest3:::: create response message error");
            return;
        }

        byte_t tempData[2] = { 0 };
        NESOMEIP_DEBUG("messageHandlerTest3:::: send data [%s] [%ld]", tempData, sizeof(tempData));
        std::shared_ptr<vsomeip::payload> tempPayloadResponse
            = runtimePointer->create_payload(tempData, sizeof(tempData));
        if (NULL == tempPayloadResponse) {
            NESOMEIP_ERROR("messageHandlerTest3:::: tempPayloadResponse NULL");
            return;
        }

        responseMessage->set_payload(tempPayloadResponse);
        //responseMessage->setMessageLength(8 + sizeof(tempData));
        someipObject->send(responseMessage);
    }

    void on_state(vsomeip::state_type_e _state) {
        if (_state == vsomeip::state_type_e::ST_REGISTERED) {
            NESOMEIP_DEBUG("Application is %s registered ", someipObject->get_name().c_str());
        }
        else {
            NESOMEIP_DEBUG("Application is %s deregistered ", someipObject->get_name().c_str());
        }

    }

    void start_runable(int n)
    {
        NESOMEIP_DEBUG("start_thread start");
        someipObject->start();
        if (1) {
            NESOMEIP_ERROR("someipObject start error");
        }
        NESOMEIP_DEBUG("start_thread start finish");

    }

    bool on_subscription_cbk(vsomeip::client_t _client,
        vsomeip::uid_t _uid, vsomeip::gid_t _gid, bool _is_available) 
{
    NESOMEIP_DEBUG("~~~on_subscription_cbk  _client: %d, _uid=%d, gid=%d, avaiable=%d", _client, _uid, _gid, _is_available);
    return true;
}

    void offerEvent()
    {
        
        // ----------------UDP -------------------
        uint16_t serviceid1 = 0x7D0;
        std::set<vsomeip::eventgroup_t> its_groups1;
        its_groups1.insert(0x01);
        someipObject->offer_event(
                        serviceid1, instaceId01,
                        0x8002, its_groups1,
                        vsomeip::event_type_e::ET_EVENT, std::chrono::milliseconds(1000),
                        //vsomeip::event_type_e::ET_EVENT, std::chrono::milliseconds::zero(),
                        false, true, nullptr, vsomeip::reliability_type_e::RT_UNKNOWN);
        someipObject->offer_event(
                        serviceid1, instaceId01,
                        0x8005, its_groups1,
                        vsomeip::event_type_e::ET_FIELD, std::chrono::milliseconds::zero(),
                        false, true, nullptr, vsomeip::reliability_type_e::RT_UNKNOWN);

#if 1
        // ----------------TCP ----------------
        uint16_t serviceid2 = 0xBB8;
        std::set<vsomeip::eventgroup_t> its_groups5;
        its_groups5.insert(0x5);
        someipObject->offer_event(
                        serviceid2, instaceId01,
                        0x0006, its_groups5,
                        vsomeip::event_type_e::ET_EVENT, std::chrono::milliseconds::zero(),
                        false, true, nullptr, vsomeip::reliability_type_e::RT_UNKNOWN);

#endif
    uint8_t notifyData[] = "1111";
    std::shared_ptr<vsomeip::payload> notifyPayload
        = runtimePointer->create_payload(notifyData, sizeof(notifyData));

    someipObject->notify(0x7D0, instaceId01, 0x8002, notifyPayload);
        NESOMEIP_DEBUG("offerEvent");
    }

    void stopOfferEvent()
    {
        // ----------------UDP -------------------
        uint16_t serviceid1 = 0x7D0;
        std::set<vsomeip::eventgroup_t> its_groups1;
        its_groups1.insert(0x1);
        someipObject->stop_offer_event(serviceid1, instaceId01, 0x8002);
        someipObject->stop_offer_event(serviceid1, instaceId01, 0x8005);


#if 1
        // ----------------TCP ----------------
        uint16_t serviceid2 = 0xBB8;
        std::set<vsomeip::eventgroup_t> its_groups5;
        its_groups5.insert(0x5);
        someipObject->stop_offer_event(serviceid2, instaceId01, 0x0006);

#endif
        NESOMEIP_DEBUG("offerEvent");
    }


    bool init() {
        NESOMEIP_DEBUG("init");


        runtimePointer =vsomeip::runtime::get();
        if (NULL == runtimePointer) {
            NESOMEIP_ERROR("runtimePointer NULL");
            return false;
        }

        someipObject = runtimePointer->create_application(std::string("service-sample"));
        if (NULL == someipObject) {
            NESOMEIP_ERROR("someipObject NULL");
            return false;
        }

        bool ret = someipObject->init();
        if (!ret) {
            NESOMEIP_ERROR("someipObject init error");
            return false;
        }

        someipObject->register_state_handler(on_state);

        NESOMEIP_DEBUG("START REGISTER MESSSAGE HANDLER");


        someipObject->register_message_handler(0x7D0, vsomeip::ANY_INSTANCE, vsomeip::ANY_METHOD, messageHandlerTest1);


        someipObject->register_message_handler(3000, vsomeip::ANY_INSTANCE, vsomeip::ANY_METHOD, messageHandlerTest1);

        someipObject->register_subscription_handler(vsomeip::ANY_SERVICE, vsomeip::ANY_INSTANCE, 0x01,
            std::bind(&on_subscription_cbk, std::placeholders::_1, std::placeholders::_2,
            std::placeholders::_3, std::placeholders::_4));

        offerEvent();

        NESOMEIP_DEBUG("END REGISTER MESSSAGE HANDLER");

        initComplete = true;

        std::thread start_thread(std::bind(&start_runable, NULL));
        start_thread.detach();



        NESOMEIP_DEBUG("init success");
        return true;
    }

    bool deinit() {
        NESOMEIP_DEBUG("deinit");
        someipObject->stop();
        someipObject = NULL;
        runtimePointer = NULL;
        system("sudo rm /tmp/vsomeip*");
        initComplete = false;

        NESOMEIP_DEBUG("deinit success");
        return true;
    }


    int OfferService(uint16_t service, uint16_t NumInstance) {
        NESOMEIP_DEBUG("start, serviceId [0x%x], NumInstance [%d]",
            service, NumInstance);
        if (!initComplete) {
            return -1;
        }

        if (NULL == someipObject) {
            NESOMEIP_ERROR("someipObject NULL");
            return -1;
        }
        offerEvent();


        if (1 == NumInstance) {
            someipObject->offer_service(service, instaceId01, majorVersion, minorVersion);
            return 0;
        } else if (2 == NumInstance) {
            someipObject->offer_service(service, instaceId01, majorVersion, minorVersion);
            someipObject->offer_service(service, instaceId02, majorVersion, minorVersion);
            return 0;
        } else {
            NESOMEIP_ERROR("NumInstance [%d] error", NumInstance);
            return -1;
        }
    }

    int StopService(uint16_t service) {
        NESOMEIP_DEBUG("start, serviceId [0x%x]", service);
        if (!initComplete) {
            return -1;
        }

        if (NULL == someipObject) {
            NESOMEIP_ERROR("someipObject NULL");
            return -1;
        }


        someipObject->stop_offer_service(service, instaceId01, majorVersion, minorVersion);
        someipObject->stop_offer_service(service, instaceId02, majorVersion, minorVersion);

        someipObject->stop_offer_service(0xBB8, instaceId01, majorVersion, minorVersion);

        //stopOfferEvent();
        return 0;
    }

    void notifyThread()
    {
        for (int i = 0; i < 10; i++) {
            uint8_t notifyData[] = "zhongguo";
            std::shared_ptr<vsomeip::payload> notifyPayload
                = runtimePointer->create_payload(notifyData, sizeof(notifyData));

            someipObject->notify(0x7D, instaceId01, 0x8005, notifyPayload);

            uint8_t notifyData1[] = "zhongguo11";
            std::shared_ptr<vsomeip::payload> notifyPayload1
                = runtimePointer->create_payload(notifyData1, sizeof(notifyData1));

            someipObject->notify(0x7D, instaceId01, 0x8005, notifyPayload1);
            NESOMEIP_DEBUG("=============notify =============");
        }
    }

    int TriggerEvent(uint16_t service, uint16_t EventGroup, uint16_t EventId) {
        NESOMEIP_DEBUG("==============start, serviceId [0x%x], EventGroup [0x%x], EventId [0x%x]",
            service, EventGroup, EventId);

        if (!initComplete) {
            NESOMEIP_ERROR("init is not completed");
            return false;
        }

        uint8_t notifyData[] = "zhongguo";
        std::shared_ptr<vsomeip::payload> notifyPayload
            = runtimePointer->create_payload(notifyData, sizeof(notifyData));

        someipObject->notify(service, instaceId01, EventId+0x8000, notifyPayload);
        NESOMEIP_DEBUG("=============notify =============");
        //std::thread notifyth(notifyThread);
        //notifyth.detach();
        return 0;
    }
}  // namespace nutshell
/* EOF */
