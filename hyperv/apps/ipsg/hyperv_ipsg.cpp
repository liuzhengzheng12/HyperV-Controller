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

#include "hyperv_ipsg.h"
#include "../../hyperv_helper.h"

void
hyperv_ipsg::activate(bool init, bool special) {
    pi_p4info_t * p4info = controller->get_p4info();

    if (init) {
        controller->add_init(dev_id,
                             this->vdp_id,
                             this->inst_id,
                             this->start_stage_id,
                             BIT_MASK_HEADER | BIT_MASK_STD_META,
                             special);
    }

    {
        uint64_t match_result = controller->get_hedaer_match_result(dev_id, start_stage_id) |
                                controller->get_intrinsic_match_result(dev_id, start_stage_id);

        hyperv_match_result match(p4info,
                                  inst_id,
                                  start_stage_id,
                                  0);
        match.set_action_id_direct(match_result,
                                   BIT_MASK_MOD_STD_META,
                                   0,
                                   0,
                                   END_INST);

        ActionData action = match.get_action_data();
        controller->set_default_entry(dev_id,
                                      match.get_table_name(),
                                      action);
        hyperv_helper::do_drop(controller,
                               dev_id,
                               start_stage_id,
                               match_result);
    }
}

void
hyperv_ipsg::install(uint32_t src_ip,
                     uint16_t iport) {

    pi_p4info_t * p4info = controller->get_p4info();
    {
        uint64_t match_result = 0;

        {
            match_result = controller->get_hedaer_match_result(0,
                                                               start_stage_id);
            header_match header(p4info,
                                this->inst_id,
                                start_stage_id);

            header.set_value(IP_SRC_BYTE_OFFSET, src_ip);

            header.set_match_result(match_result);

            controller->add_match(dev_id, &header);
        }

        {
            uint64_t mr =
                    controller->get_intrinsic_match_result(0,
                                                           start_stage_id);
            intrinsic_match match(p4info,
                                  this->inst_id,
                                  start_stage_id);
            match.set_iport_value(iport);
            match.set_match_result(mr);
            controller->add_match(dev_id, &match);

            match_result |= mr;
        }

        {
            hyperv_match_result match(p4info,
                                      this->inst_id,
                                      start_stage_id,
                                      match_result);
            match.set_next_stage(next_stage_match,
                                 next_stage,
                                 next_inst);
            controller->add_match(dev_id, &match);
        }
    }
}

void
hyperv_ipsg::deactivate() {

}