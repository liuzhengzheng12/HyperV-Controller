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

#include "hyperv_router.h"

void
hyperv_router::activate(bool init, bool special) {
    pi_p4info_t * p4info = controller->get_p4info();

    if (init) {
        controller->add_init(dev_id,
                             this->vdp_id,
                             this->inst_id,
                             this->start_stage_id,
                             get_match_bitmap(),
                             special);
    }

    // Stage 1
    {
        header_match match(p4info,
                           this->inst_id,
                           start_stage_id);

        match.set_value(ETH_TYPE_BYTE_OFFSET, (uint16_t) 0x0800);
        match.set_next_stage(BIT_MASK_HEADER,
                             NEXT_CONDITIONAL_STAGE(start_stage_id),
                             this->inst_id);

        controller->add_match(dev_id, &match);

    }

    // Stage 1
    {
        header_match match(p4info, this->inst_id, start_stage_id);

        match.set_priority(100);

        match.end(0xFF);

        controller->add_match(dev_id, &match);

    }

    // Conditional Stage 2
    {
        hyperv_conditional conditional(p4info,
                                       NEXT_CONDITIONAL_STAGE(start_stage_id),
                                       this->inst_id);

        conditional.set_expr_header_op_const(TTL_BYTE_OFFSET, 0xFF, 0);
        conditional.end(1, END_INST);
        conditional.end(3, END_INST);
        conditional.set_next_stage(2,
                                   BIT_MASK_HEADER,
                                   NEXT_STAGE(start_stage_id),
                                   this->inst_id);
        controller->add_conditional(dev_id, &conditional);
    }

}

void
hyperv_router::install(uint32_t dst_ip,
                       uint16_t dst_mac,
                       uint64_t src_mac,
                       uint16_t port) {
    pi_p4info_t * p4info = controller->get_p4info();
    uint8_t stage_id = NEXT_STAGE(start_stage_id);

    // Stage 2
    {
        uint64_t match_result =
                controller->get_hedaer_match_result(dev_id,
                                                    stage_id);
        header_match header(p4info, this->inst_id, stage_id);

        header.set_value(IP_DST_BYTE_OFFSET, dst_ip);

        header.set_action_id(match_result,
                             BIT_MASK_MOD_META_WITH_CONST | BIT_MASK_SUBTRACT,
                             BIT_MASK_HEADER,
                             NEXT_STAGE(stage_id),
                             this->inst_id);

        controller->add_match(dev_id, &header);

        // MOD_STD_META
        {
            mod_meta_with_const action(p4info, stage_id, match_result);
            action.set_value(ETH_DST_BYTE_OFFSET, dst_mac);
            action.do_mod_meta_with_const();
            controller->add_action(dev_id, &action);
        }

        {
            substarct action(p4info, stage_id, match_result);
            action.set_header_value(TTL_BYTE_OFFSET, (uint8_t) 0x01);
            action.do_subtract_const_from_header();
            controller->add_action(dev_id, &action);
        }
    }

    stage_id = NEXT_STAGE(stage_id);
    // Stage 3
    {
        uint64_t match_result = controller->get_hedaer_match_result(dev_id,
                                                                    stage_id);
        header_match header(p4info, this->inst_id, stage_id);

        header.set_value(IP_DST_BYTE_OFFSET, dst_ip);

        header.set_action_id(match_result,
                             BIT_MASK_MOD_STD_META | BIT_MASK_MOD_HEADER_WITH_CONST,
                             BIT_MASK_STD_META,
                             NEXT_STAGE(stage_id),
                             this->inst_id);

        controller->add_match(dev_id, &header);
        {
            mod_std_meta action(p4info, stage_id, match_result);
            action.set_forward(port);
            controller->add_action(dev_id, &action);
        }
        {
            mod_header_with_const action(p4info, stage_id, match_result);
            action.set_value(ETH_SRC_BYTE_OFFSET, src_mac, 6);
            action.do_mod_header_with_const();
            controller->add_action(dev_id, &action);
        }
    }

    stage_id = NEXT_STAGE(stage_id);
    // Stage 4
    {
        uint64_t match_result = controller->get_intrinsic_match_result(dev_id,
                                                                       stage_id);
        intrinsic_match match(p4info, this->inst_id, stage_id);

        match.set_eport_value(port);

        match.set_action_id(match_result,
                            BIT_MASK_MOD_HEADER_WITH_CONST,
                            next_stage_match,
                            next_stage,
                            next_inst);

        controller->add_match(dev_id, &match);

        {
            mod_header_with_const action(p4info, stage_id, match_result);
            action.set_value(ETH_SRC_BYTE_OFFSET, src_mac, 6);
            action.do_mod_header_with_const();
            controller->add_action(dev_id, &action);
        }
    }
}

void
hyperv_router::deactivate() {

}