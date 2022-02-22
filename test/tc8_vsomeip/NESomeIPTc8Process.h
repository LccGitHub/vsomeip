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
/**
 * @file NESomeIPTc8Process.h
 * @brief Declaration file of class NESomeIPTc8Process.
 */

#ifndef TC8TEST_NESOMEIPTC8PROCESS_H_
#define TC8TEST_NESOMEIPTC8PROCESS_H_

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation(use a .cpp suffix)
#endif

#include <stdint.h>

namespace nutshell {
    bool init();
    bool deinit();
    int OfferService(uint16_t service, uint16_t NumInstance);
    int StopService(uint16_t service);
    int TriggerEvent(uint16_t service, uint16_t EventGroup, uint16_t EventId);
}  // namespace nutshell
#endif  // TC8TEST_NESOMEIPTC8PROCESS_H_
/* EOF */
