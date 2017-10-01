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

#ifndef CONTROLLER_HYPERV_CONTROLLER_H
#define CONTROLLER_HYPERV_CONTROLLER_H

#include <cstring>

#include <PI/pi.h>
#include <PI/frontends/generic/pi.h>
#include <PI/frontends/cpp/tables.h>
#include <vector>

#include "hyperv_control.h"
#include "hyperv_match.h"
#include "hyperv_action.h"
#include "hyperv_conditional.h"

using namespace pi;

class hyperv_controller {
private:
    uint16_t header_match_result[256][32]={{0}};
    uint16_t meta_match_result[256][32] = {{0}};
    uint16_t intrinsic_match_result[256][32] = {{0}};
    uint8_t inst_counter = 0;
    pi_dev_id_t id_counter = 0;

    std::vector<pi_dev_id_t> dev_id_list;

    pi_p4info_t *p4info;
    pi_session_handle_t sess;

    pi_entry_handle_t  entry_handler;

public:

    /**
     *
     * @param p4info
     */
    explicit hyperv_controller(pi_p4info_t *p4info) ;
    ~hyperv_controller();

    /**
     *
     * @return
     */
    unsigned long get_dev_num() {
        return dev_id_list.size();
    }


    /**
     *
     * @param i
     * @return
     */
    pi_dev_id_t get_dev_id(unsigned long i) {
        return dev_id_list[i];
    }

    /**
     *
     * @param table_name
     * @param dev_id
     * @param action
     */
    void set_default_entry(pi_dev_id_t dev_id,
                           string table_name,
                           ActionData & action);

    /**
     *
     * @param port
     */
    void assign_device(char* port);

    /**
     *
     * @return
     */
    int set_default_entries();

    /**
     *
     * @param dev_id
     * @param table_name
     * @param key
     * @param data
     */
    void add_entry(pi_dev_id_t dev_id, const char* table_name, MatchKey & key, ActionData & data);

    /**
     *
     * @param dev_id
     * @param stage_id
     * @param match
     */
    void add_match(pi_dev_id_t dev_id, hyperv_match * match);

    /**
     *
     * @param dev_id
     * @param stage_id
     * @param action
     */
    void add_action(pi_dev_id_t dev_id, hyperv_action * action);

    /**
     *
     * @param dev_id
     * @param vdp_id
     * @param inst_id
     * @param stage_id
     * @param match_bitmap
     */
    void add_init(pi_dev_id_t dev_id,
                  uint16_t vdp_id,
                  uint8_t inst_id,
                  uint8_t stage_id,
                  uint8_t match_bitmap,
                  bool special);

    /**
     *
     * @param dev_id
     * @param vdp_id
     * @param inst_id
     */
    void add_end(pi_dev_id_t dev_id, uint8_t vdp_id,
                 uint8_t inst_id );

    /**
     *
     * @param dev_id
     * @param conditional
     */
    void add_conditional(pi_dev_id_t dev_id,
                         hyperv_conditional * conditional);

    pi_p4info_t * get_p4info() {
        return p4info;
    }

    /**
     *
     * @param dev_id
     * @param stage_id
     * @return
     */
    uint64_t get_hedaer_match_result(pi_dev_id_t dev_id, uint8_t stage_id) {
        uint64_t i = ++header_match_result[dev_id][stage_id];
        return i << 24;//header_match_result[dev_id][stage_id];
    }

    /**
     *
     * @param dev_id
     * @param stage_id
     * @return
     */
    uint64_t get_meta_match_result(pi_dev_id_t dev_id, uint8_t stage_id) {
        uint64_t i = ++meta_match_result[dev_id][stage_id];
        return  i << 16;//meta_match_result[dev_id][stage_id];
    }

    /**
     *
     * @param dev_id
     * @param stage_id
     * @return
     */
    uint64_t get_intrinsic_match_result(pi_dev_id_t dev_id, uint8_t stage_id) {
        intrinsic_match_result[dev_id][stage_id]++;
        return intrinsic_match_result[dev_id][stage_id];
    }

    /**
     *
     * @return
     */
    uint8_t allocate_inst() {
        return ++this->inst_counter;
    }
};


#endif //CONTROLLER_HYPERV_CONTROLLER_H
