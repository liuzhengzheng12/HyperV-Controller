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

#include "hyperv_qos.h"




void
hyperv_qos::activate(bool init, bool special) {
    if(init) {
        controller->add_init(dev_id,
                             this->vdp_id,
                             this->inst_id,
                             this->start_stage_id,
                             get_match_bitmap(),
                             special);
    }
}

void
hyperv_qos::install(uint32_t src_ip,
                    uint32_t dst_ip,
                    uint8_t qid) {

    pi_p4info_t * p4info = controller->get_p4info();

    {
        auto match_result =
                controller->get_hedaer_match_result(dev_id,
                                                    start_stage_id);
        header_match header(p4info, this->inst_id, start_stage_id);

        header.set_ip_src(src_ip);
        header.set_ip_dst(dst_ip);

        header.set_action_id(match_result,
                             BIT_MASK_MOD_STD_META,
                             next_stage_match,
                             next_stage,
                             next_inst);

        controller->add_match(dev_id, &header);

        // MOD_STD_META
        {
            mod_std_meta action(p4info, start_stage_id, match_result);
            action.set_queue(qid);
            controller->add_action(dev_id, &action);
        }
    }

}

void
hyperv_qos::deactivate() {

}