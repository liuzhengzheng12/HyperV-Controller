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

#include "hyperv_firewall.h"
#include "../../hyperv_helper.h"


void
hyperv_firewall::activate(bool init, bool special) {
    pi_p4info_t *p4info = controller->get_p4info();

    if(init) {
        controller->add_init(dev_id,
                             this->vdp_id,
                             this->inst_id,
                             this->start_stage_id,
                             BIT_MASK_STD_META,
                             special);
    }

    // Stage 2
    {
        header_match match(p4info,
                           this->inst_id,
                           start_stage_id);

        match.set_eth_type(0x0800);
        match.set_ip_proto(0x11);
        match.set_priority(0);
        match.set_next_stage(BIT_MASK_HEADER,
                             (uint8_t) (start_stage_id + 1),
                             this->inst_id);
        controller->add_match(dev_id,
                              &match);
    }


    {
        header_match match(p4info, this->inst_id, start_stage_id);
        match.set_eth_type(0x0800);
        match.set_ip_proto(0x06);
        match.set_priority(1);
        match.set_next_stage(BIT_MASK_HEADER,
                             (uint8_t) (start_stage_id + 1),
                             this->inst_id);
        controller->add_match(dev_id,
                              &match);
    }

    {
        header_match match(p4info, this->inst_id, start_stage_id);
        match.set_eth_type(0x0800);
        match.set_ip_proto(0x01);
        match.set_priority(1);
        match.set_next_stage(BIT_MASK_HEADER,
                             (uint8_t) (start_stage_id + 1),
                             this->inst_id);
        controller->add_match(dev_id,
                              &match);
    }

    {
        header_match match(p4info,
                           this->inst_id, start_stage_id);
        match.set_priority(100);
        match.end(END_INST);
        controller->add_match(dev_id,
                              &match);
    }

    // Stage 3

    {
        this->drop_match_result =
                controller->get_hedaer_match_result(0,
                                                    (uint8_t) (start_stage_id + 1));
        hyperv_helper::do_drop(controller,
                               dev_id,
                               (uint8_t)(this->start_stage_id + 1),
                               this->drop_match_result);

    }

}


void
hyperv_firewall::install_black_list(uint32_t src_ip,
                                    uint32_t dst_ip,
                                    int priority) {
    pi_p4info_t *p4info = controller->get_p4info();
    {
        header_match match(p4info,
                           this->inst_id,
                           (uint8_t) (this->start_stage_id + 1));

        match.set_ip_dst(dst_ip);
        match.set_ip_src(src_ip);
        match.set_priority(priority);

        match.set_action_id(this->drop_match_result,
                            BIT_MASK_MOD_STD_META,
                            0,
                            0,
                            END_INST);
        controller->add_match(dev_id, &match);
    }
}

void
hyperv_firewall::install_white_list(uint32_t src_ip,
                                    uint32_t dst_ip,
                                    int priority) {
    pi_p4info_t *p4info = controller->get_p4info();
    {
        header_match match(p4info,
                           this->inst_id,
                           (uint8_t) (this->start_stage_id + 1));

        match.set_ip_dst(dst_ip);
        match.set_ip_src(src_ip);
        match.set_priority(priority);

        match.set_next_stage(this->next_stage_match,
                             this->next_stage,
                             this->next_inst);
        controller->add_match(dev_id, &match);
    }
}

void
hyperv_firewall::deactivate() {

}
