/*
 * Copyright (C) 2017 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef NOS_CITADELD_PROXY_CLIENT_H
#define NOS_CITADELD_PROXY_CLIENT_H

#include <cstdint>
#include <vector>

#include <nos/NuggetClient.h>

#include <android/hardware/citadel/ICitadeld.h>

namespace nos {

/**
 * Implementation of NuggetClient to proxy calls via the citadeld synchronizing
 * daemon which coordinates communication between the HALs and Citadel.
 */
class CitadeldProxyClient : public NuggetClient {
    ::android::sp<::android::hardware::citadel::ICitadeld> _citadeld;

public:
    CitadeldProxyClient() = default;
    ~CitadeldProxyClient() override;

    void open() override;
    void close() override;
    bool isOpen() override;
    uint32_t callApp(uint32_t appId, uint16_t arg,
                     const std::vector<uint8_t>& request,
                     std::vector<uint8_t>& response) override;
};

} // namespace nos

#endif // NOS_CITADELD_PROXY_CLIENT_H
