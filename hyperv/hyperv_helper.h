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

#ifndef CONTROLLER_HYPERV_HELPER_H
#define CONTROLLER_HYPERV_HELPER_H


#include "hyperv_controller.h"

class hyperv_helper {
public:
    static void do_forward(hyperv_controller * controller,
                           pi_dev_id_t dev_id,
                           uint8_t stage_id,
                           uint64_t match_result,
                           uint16_t port);
    static void do_drop(hyperv_controller * controller,
                        pi_dev_id_t dev_id,
                        uint8_t stage_id,
                        uint64_t match_result);
    static void do_loopback(hyperv_controller * controller,
                           pi_dev_id_t dev_id,
                           uint8_t stage_id,
                           uint64_t match_result);
};


#endif //CONTROLLER_HYPERV_HELPER_H
