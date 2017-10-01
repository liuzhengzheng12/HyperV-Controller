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

#include "hyperv_multicast.h"

void
hyperv_multicast::activate(bool init, bool special) {
    if (init) {
        controller->add_init(dev_id,
                             this->vdp_id,
                             this->inst_id,
                             this->start_stage_id,
                             get_match_bitmap(), special);
    }
}

void
hyperv_multicast::deactivate() {

}


void
hyperv_multicast::install(uint16_t dst_mac,
                          uint16_t mcast_grp) {

    pi_p4info_t * p4info = controller->get_p4info();

    {
        uint64_t match_result = controller->get_hedaer_match_result(dev_id,
                                                                    start_stage_id);
        header_match header(p4info, this->inst_id, start_stage_id);
        header.set_value(0, dst_mac);
        header.set_action_id(match_result,
                             BIT_MASK_MOD_STD_META,
                             next_stage_match,
                             next_stage,
                             next_inst);
        controller->add_match(0, &header);

        // MOD_STD_META
        {
            mod_std_meta action(p4info, start_stage_id, match_result);
            action.set_multicast(mcast_grp);
            controller->add_action(0, &action);
        }
    }

}