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

#include "hyperv_utils.h"

/**
 *
 * @param buffer
 */
byte_buffer::byte_buffer(uint8_t * buffer, int length, int space) {
    this->buffer = buffer;
    this->buffer_start = length - space;
}

/**
 *
 * @param offset
 * @param value
 */
void
byte_buffer::set_value(int offset, unsigned int value) {
    offset += this->buffer_start;
    this->buffer[offset + 3] = (uint8_t) (value % 256);
    value = value / 256;
    this->buffer[offset + 2] = (uint8_t) (value % 256);
    value = value / 256;
    this->buffer[offset + 1] = (uint8_t) (value % 256);
    value = value / 256;
    this->buffer[offset] = (uint8_t) (value % 256);
}

/**
 *
 * @param offset
 * @param value
 */
void
byte_buffer::set_value(int offset, uint16_t value) {
    offset += this->buffer_start;
    this->buffer[offset + 1] = (uint8_t) (value % 256);
    value = (uint16_t) (value / 256);
    this->buffer[offset] = (uint8_t) (value % 256);
}

/**
 *
 * @param offset
 * @param value
 */
void
byte_buffer::set_value(int offset, uint8_t value) {
    offset += this->buffer_start;
    this->buffer[offset] = value;
}

/**
 *
 * @param offset
 * @param value
 * @param width
 */
void
byte_buffer::set_value(int offset, uint64_t value, int width) {
    int i = width - 1;
    offset += this->buffer_start;
    for (; i >= 0; i--) {
        this->buffer[offset + i] = (uint8_t) (value % 256);
        value = value >> 8;
    }
}