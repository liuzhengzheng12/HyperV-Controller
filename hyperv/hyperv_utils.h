/* Copyright 2016-present NetArch Lab, Tsinghua University.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef CONTROLLER_HYPERV_UTILS_H
#define CONTROLLER_HYPERV_UTILS_H


#include <cstdint>

class byte_buffer {
private:
    uint8_t * buffer;
    int buffer_start;

public:
    /**
     *
     * @param buffer
     * @param lenght
     * @param space
     */
    byte_buffer(uint8_t * buffer, int length, int space);

    void set_value(int, unsigned int);
    void set_value(int, uint16_t);
    void set_value(int, uint8_t);
    void set_value(int, uint64_t, int);
};

#endif //CONTROLLER_HYPERV_UTILS_H
