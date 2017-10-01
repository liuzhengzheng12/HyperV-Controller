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

#include "hyperv_hitter.h"

void
hyperv_hitter::activate(bool init, bool special) {
pi_p4info_t * p4info = controller->get_p4info();
    if (init) {
        controller->add_init(dev_id,
                             this->vdp_id,
                             this->inst_id,
                             this->start_stage_id,
                             BIT_MASK_HEADER, special);
    }
    {

        hyperv_conditional conditional(p4info,
                                       CONST_CONDITIONAL_STAGE_2,
                                       this->inst_id);

        conditional.set_expr_counter_op_const(500);

        conditional.set_next_stage(1,
                                   next_stage_match,
                                   next_stage,
                                   next_inst);
        conditional.set_next_stage(2,
                                   next_stage_match,
                                   next_stage,
                                   next_inst);
        conditional.set_next_stage(3,
                                   next_stage_match,
                                   next_stage,
                                   next_inst);

        controller->add_conditional(dev_id, &conditional);
    }
}

void
hyperv_hitter::install(uint32_t src_ip,
                       uint32_t dst_ip) {
    pi_p4info_t * p4info = controller->get_p4info();
    // Start stage
    {
        uint64_t match_result =
                controller->get_hedaer_match_result(dev_id,
                                                    start_stage_id);
        {

            header_match header(p4info,
                                this->inst_id,
                                start_stage_id);

            header.set_value(IP_SRC_BYTE_OFFSET, src_ip);
            header.set_value(IP_DST_BYTE_OFFSET, dst_ip);
            header.set_action_id(match_result,
                                 BIT_MASK_COUNTER,
                                 0,
                                 NEXT_CONDITIONAL_STAGE(start_stage_id),
                                 this->inst_id);

            controller->add_match(dev_id, &header);

            {
                counter action(p4info,
                               start_stage_id,
                               match_result);

                action.do_packet_count(allocate_index());
                controller->add_action(dev_id, &action);
            }

        }

        {
            header_match header(p4info,
                                this->inst_id,
                                start_stage_id);

            header.set_value(IP_DST_BYTE_OFFSET, src_ip);
            header.set_value(IP_SRC_BYTE_OFFSET, dst_ip);

            header.set_action_id(match_result,
                                 BIT_MASK_COUNTER,
                                 0,
                                 NEXT_CONDITIONAL_STAGE(start_stage_id),
                                 this->inst_id);

            controller->add_match(dev_id, &header);

        }

    }

}

void
hyperv_hitter::deactivate() {

}