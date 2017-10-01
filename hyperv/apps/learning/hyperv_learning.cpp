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

#include "hyperv_learning.h"

void
hyperv_learning::activate(bool init, bool special) {
pi_p4info_t * p4info = controller->get_p4info();
    if(init) {
        controller->add_init(dev_id,
                             this->vdp_id,
                             this->inst_id,
                             this->start_stage_id,
                             BIT_MASK_HEADER, special);
    }

    {
        uint64_t match_result =
                controller->get_hedaer_match_result(dev_id,
                                                    start_stage_id);

        header_match header(p4info, this->inst_id, start_stage_id);
        header.set_priority(100);
        header.set_action_id(match_result,
                             BIT_MASK_GENERATE_DIGIST,
                             next_stage_match,
                             next_stage,
                             END_INST);

        controller->add_match(dev_id, &header);

        // MOD_STD_META
        {
            generate_digest action(p4info, start_stage_id, match_result);
            action.do_gen_digest();
            controller->add_action(dev_id,
                                   &action);
        }

    }
}

void
hyperv_learning::install(uint64_t mac) {

    pi_p4info_t * p4info = controller->get_p4info();

    {
        header_match match(p4info,
                           this->inst_id,
                           start_stage_id);

        match.set_value(ETH_SRC_BYTE_OFFSET, mac, 6);
        match.set_priority(0);
        match.set_next_stage(next_stage_match,
                             next_stage,
                             next_inst);

        controller->add_match(dev_id, &match);
    }
}

void
hyperv_learning::deactivate() {

}