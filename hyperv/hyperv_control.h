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

#ifndef CONTROLLER_HYPERV_CONTROL_H
#define CONTROLLER_HYPERV_CONTROL_H


#include <PI/pi.h>
#include <PI/frontends/generic/pi.h>
#include <PI/frontends/cpp/tables.h>

#include <string>
#include <cstring>

#include "hyperv.h"

using std::string;

using namespace pi;

class hyperv_init {
private:
    pi_p4info_t * p4info;
    pi_p4_id_t table_id;
    pi_p4_id_t key_vdp_id;
    pi_p4_id_t key_inst_id;
    pi_p4_id_t key_stage_id;
    uint16_t vdp_id;
    uint8_t stage_id;
    uint8_t inst_id;
public:
    /**
     *
     * @param p4info
     * @param vdp_id
     */
    hyperv_init(pi_p4info_t * p4info, uint16_t vdp_id);

    /**
     *
     * @return
     */
    string get_table_name() {
        return "table_config_at_initial";
    }


    /**
     *
     * @return
     */
    MatchKey get_match_key();
    ActionData get_action_data(uint8_t inst_id, uint8_t stage_id, uint8_t match_bitmap, bool special);

};

class hyperv_end {
private:
    pi_p4info_t * p4info;
    pi_p4_id_t table_id;
    pi_p4_id_t key_vdp_id;
    pi_p4_id_t key_inst_id;
    pi_p4_id_t key_stage_id;
    uint16_t vdp_id;
    uint8_t stage_id;
    uint8_t inst_id;

public:

    /**
     *
     * @param p4info
     * @param vdp_id
     */
    hyperv_end(pi_p4info_t * p4info, uint16_t vdp_id);

    string get_table_name() {
        return "table_config_at_end";
    }

    /**
     *
     * @return
     */
    MatchKey get_match_key();

    /**
     *
     * @param inst_id
     * @return
     */
    ActionData get_action_data(uint8_t inst_id);
};

#endif //CONTROLLER_HYPERV_CONTROL_H
