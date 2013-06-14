/*
 * Copyright (C) 2013 The Android Open Source Project
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

#define LOG_TAG "scrypt_test"
#include <utils/Log.h>
#include <utils/UniquePtr.h>

#include <gtest/gtest.h>

#include <fstream>
#include <iostream>

extern "C" {
#include <crypto_scrypt.h>
}

namespace android {

typedef struct scrypt_test_setting_t {
        const char *pw, *salt;
        uint32_t Nfactor, rfactor, pfactor;
} scrypt_test_setting;

static const scrypt_test_setting post_settings[] = {
        {"", "", 16, 1, 1},
        {"password", "NaCl", 1024, 8, 16},
        {"pleaseletmein", "SodiumChloride", 16384, 8, 1},
        {0, 0, 0, 0, 0}
};

static const uint8_t post_vectors[][64] = {
        {0x77,0xd6,0x57,0x62,0x38,0x65,0x7b,0x20,0x3b,0x19,0xca,0x42,0xc1,0x8a,0x04,0x97,
         0xf1,0x6b,0x48,0x44,0xe3,0x07,0x4a,0xe8,0xdf,0xdf,0xfa,0x3f,0xed,0xe2,0x14,0x42,
         0xfc,0xd0,0x06,0x9d,0xed,0x09,0x48,0xf8,0x32,0x6a,0x75,0x3a,0x0f,0xc8,0x1f,0x17,
         0xe8,0xd3,0xe0,0xfb,0x2e,0x0d,0x36,0x28,0xcf,0x35,0xe2,0x0c,0x38,0xd1,0x89,0x06},
        {0xfd,0xba,0xbe,0x1c,0x9d,0x34,0x72,0x00,0x78,0x56,0xe7,0x19,0x0d,0x01,0xe9,0xfe,
         0x7c,0x6a,0xd7,0xcb,0xc8,0x23,0x78,0x30,0xe7,0x73,0x76,0x63,0x4b,0x37,0x31,0x62,
         0x2e,0xaf,0x30,0xd9,0x2e,0x22,0xa3,0x88,0x6f,0xf1,0x09,0x27,0x9d,0x98,0x30,0xda,
         0xc7,0x27,0xaf,0xb9,0x4a,0x83,0xee,0x6d,0x83,0x60,0xcb,0xdf,0xa2,0xcc,0x06,0x40},
        {0x70,0x23,0xbd,0xcb,0x3a,0xfd,0x73,0x48,0x46,0x1c,0x06,0xcd,0x81,0xfd,0x38,0xeb,
         0xfd,0xa8,0xfb,0xba,0x90,0x4f,0x8e,0x3e,0xa9,0xb5,0x43,0xf6,0x54,0x5d,0xa1,0xf2,
         0xd5,0x43,0x29,0x55,0x61,0x3f,0x0f,0xcf,0x62,0xd4,0x97,0x05,0x24,0x2a,0x9a,0xf9,
         0xe6,0x1e,0x85,0xdc,0x0d,0x65,0x1e,0x40,0xdf,0xcf,0x01,0x7b,0x45,0x57,0x58,0x87},
};

class ScryptTest : public ::testing::Test {
};

TEST_F(ScryptTest, TestVectors) {
    int i;

    for (i = 0; post_settings[i].pw != NULL; i++) {
        uint8_t output[64];

        scrypt_test_setting_t s = post_settings[i];
        ASSERT_EQ(0,
                crypto_scrypt((const uint8_t*) s.pw, strlen(s.pw), (const uint8_t*) s.salt,
                        strlen(s.salt), s.Nfactor, s.rfactor, s.pfactor, output, sizeof(output)))
                << "scrypt call should succeed for " << i << "; error=" << strerror(errno);
        ASSERT_EQ(0, memcmp(post_vectors[i], output, sizeof(output)))
                << "Should match expected output";
    }
}

}
