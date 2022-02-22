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
 * @file NESomeIPLog.h
 * @brief Declaration file of class NESomeIPTc8Process.
 */

#ifndef TC8TEST_NESOMEIPTC8LOG_H_
#define TC8TEST_NESOMEIPTC8LOG_H_

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation(use a .cpp suffix)
#endif

#include <stdio.h>

#define NESOMEIP_DEBUG(...) printf("%s,%d:", __func__, __LINE__),fprintf(stdout, __VA_ARGS__), fprintf(stdout, "\n")
#define NESOMEIP_INFO(...) printf("%s,%d:", __func__, __LINE__),fprintf(stdout, __VA_ARGS__), fprintf(stdout, "\n")
#define NESOMEIP_WARNING(...) printf("%s,%d:", __func__, __LINE__),fprintf(stdout, __VA_ARGS__), fprintf(stdout, "\n")
#define NESOMEIP_DEBUG(...) printf("%s,%d:", __func__, __LINE__),fprintf(stdout, __VA_ARGS__), fprintf(stdout, "\n")
#define NESOMEIP_ERROR(...) printf("%s,%d:", __func__, __LINE__),fprintf(stderr, __VA_ARGS__), fprintf(stderr, "\n")



#endif  // TC8TEST_NESOMEIPTC8PROCESS_H_
/* EOF */
