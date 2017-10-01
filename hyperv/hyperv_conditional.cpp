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

#include <sstream>
#include "hyperv_conditional.h"
#include "hyperv_utils.h"
#include "hyperv.h"


hyperv_conditional::hyperv_conditional(pi_p4info_t *p4info,
                                       uint8_t stage_id,
                                       uint8_t inst_id) {
    this->p4info = p4info;
    this->stage_id = stage_id;
    this->inst_id = inst_id;
    this->branch3_action = nullptr;
    this->branch1_action = nullptr;
    this->branch2_action = nullptr;
    std::ostringstream ostr;

    this->table_id = pi_p4info_table_id_from_name(p4info,
                                                  get_table_name().c_str());
    this->branch1_table_id = pi_p4info_table_id_from_name(p4info,
                                                          get_branch1_table_name().c_str());
    this->branch2_table_id = pi_p4info_table_id_from_name(p4info,
                                                          get_branch2_table_name().c_str());
    this->branch3_table_id = pi_p4info_table_id_from_name(p4info,
                                                          get_branch3_table_name().c_str());

}

void
hyperv_conditional::set_expr_header_op_const(uint32_t byte_offset,
                                             uint16_t mask,
                                             uint16_t r_expr) {
    pi_p4_id_t a_id =
            pi_p4info_action_id_from_name(p4info,
                                          "set_expr_header_op_const");
    this->action = new ActionData(p4info, a_id);

    pi_p4_id_t p =
            pi_p4info_action_param_id_from_name(p4info,
                                                a_id,
                                                "l_expr_offset");
    this->action->set_arg(p, byte_offset*8);

    p = pi_p4info_action_param_id_from_name(p4info,
                                            a_id,
                                            "l_expr_mask");
    this->action->set_arg(p, mask);

    p = pi_p4info_action_param_id_from_name(p4info,
                                            a_id,
                                            "r_expr");
    this->action->set_arg(p, r_expr);
}

MatchKey
hyperv_conditional::get_match() {
    pi_p4_id_t t_id = this->table_id;
    MatchKey key(p4info, t_id);
    {
        pi_p4_id_t p = pi_p4info_table_match_field_id_from_name(p4info,
                                                                t_id,
                                                                "vdp_metadata.inst_id");
        key.set_exact(p, inst_id);
    }
    {
        pi_p4_id_t p = pi_p4info_table_match_field_id_from_name(p4info,
                                                                t_id,
                                                                "vdp_metadata.stage_id");

        key.set_exact(p, stage_id);
    }

    return key;
}

MatchKey
hyperv_conditional::get_branch1_match() {
    pi_p4_id_t t_id = this->branch1_table_id;
    MatchKey key(p4info, t_id);
    {
        pi_p4_id_t p = pi_p4info_table_match_field_id_from_name(p4info,
                                                                t_id,
                                                                "vdp_metadata.inst_id");
        key.set_exact(p, inst_id);
    }
    {
        pi_p4_id_t p = pi_p4info_table_match_field_id_from_name(p4info,
                                                                t_id,
                                                                "vdp_metadata.stage_id");

        key.set_exact(p, stage_id);
    }

    return key;
}


MatchKey
hyperv_conditional::get_branch2_match() {
    pi_p4_id_t t_id = this->branch2_table_id;
    MatchKey key(p4info, t_id);
    {
        pi_p4_id_t p = pi_p4info_table_match_field_id_from_name(p4info,
                                                                t_id,
                                                                "vdp_metadata.inst_id");
        key.set_exact(p, inst_id);
    }
    {
        pi_p4_id_t p = pi_p4info_table_match_field_id_from_name(p4info,
                                                                t_id,
                                                                "vdp_metadata.stage_id");

        key.set_exact(p, stage_id);
    }

    return key;
}

MatchKey
hyperv_conditional::get_branch3_match() {
    pi_p4_id_t t_id = this->branch3_table_id;
    MatchKey key(p4info, t_id);
    {
        pi_p4_id_t p = pi_p4info_table_match_field_id_from_name(p4info,
                                                                t_id,
                                                                "vdp_metadata.inst_id");
        key.set_exact(p, inst_id);
    }
    {
        pi_p4_id_t p = pi_p4info_table_match_field_id_from_name(p4info,
                                                                t_id,
                                                                "vdp_metadata.stage_id");

        key.set_exact(p, stage_id);
    }

    return key;
}


void
hyperv_conditional::set_next_stage(int branch_id,
                                   uint8_t match_bitmap,
                                   uint8_t next_stage,
                                   uint16_t next_prog) {
    pi_p4_id_t a_id = pi_p4info_action_id_from_name(p4info, SET_NEXT_STAGE);
    auto action = new ActionData(p4info, a_id);

    pi_p4_id_t p = pi_p4info_action_param_id_from_name(p4info, a_id, "match_bitmap");
    action->set_arg(p, match_bitmap);

    p = pi_p4info_action_param_id_from_name(p4info, a_id, "next_stage");
    action->set_arg(p, next_stage);

    p = pi_p4info_action_param_id_from_name(p4info, a_id, "next_prog");
    action->set_arg(p, next_prog);

    if(branch_id == 1) {
        this->branch1_action = action;
    } else if(branch_id == 2) {
        this->branch2_action = action;
    } else {
        this->branch3_action = action;
    }
}

void
hyperv_conditional::set_action_id_direct(int branch_id,
                                         uint64_t action_id,
                                         uint64_t action_bitmap,
                                         uint8_t match_bitmap,
                                         uint8_t next_stage, uint16_t next_prog) {
    pi_p4_id_t a_id = pi_p4info_action_id_from_name(p4info, SET_ACTION_ID_DIRECT);
    auto action = new ActionData(p4info, a_id);

    char data[6];
    byte_buffer bytes((uint8_t*) data, 6, 6);
    bytes.set_value(0, action_id, 6);


    pi_p4_id_t p = pi_p4info_action_param_id_from_name(p4info, a_id, "action_id");
    action->set_arg(p, data, 6);

    bytes.set_value(0, action_bitmap, 6);

    p = pi_p4info_action_param_id_from_name(p4info, a_id, "action_bitmap");
    action->set_arg(p, data, 6);

    p = pi_p4info_action_param_id_from_name(p4info, a_id, "match_bitmap");
    action->set_arg(p, match_bitmap);

    p = pi_p4info_action_param_id_from_name(p4info, a_id, "next_stage");
    action->set_arg(p, next_stage);

    p = pi_p4info_action_param_id_from_name(p4info, a_id, "next_prog");
    action->set_arg(p, next_prog);

    if(branch_id == 1) {
        this->branch1_action = action;
    } else if(branch_id == 2) {
        this->branch2_action = action;
    } else {
        this->branch3_action = action;
    }
}


void
hyperv_conditional::end(int branch_id,
                        uint8_t next_prog) {
    pi_p4_id_t a_id = pi_p4info_action_id_from_name(p4info, "end");
    auto action = new ActionData(p4info, a_id);

    pi_p4_id_t p = pi_p4info_action_param_id_from_name(p4info, a_id, "next_prog");
    action->set_arg(p, next_prog);

    if(branch_id == 1) {
        this->branch1_action = action;
    } else if(branch_id == 2) {
        this->branch2_action = action;
    } else {
        this->branch3_action = action;
    }
}

void hyperv_conditional::set_action_id(int branch_id,
                                       uint64_t match_result,
                                       uint64_t action_bitmap,
                                       uint8_t match_bitmap,
                                       uint8_t next_stage, uint16_t next_prog) {
    pi_p4_id_t a_id = pi_p4info_action_id_from_name(p4info, SET_ACTION_ID);
    auto action = new ActionData(p4info, a_id);

    pi_p4_id_t p;
    {
        char data[6];
        byte_buffer bytes((uint8_t *) data, 6, 6);
        bytes.set_value(0, match_result, 6);

        p = pi_p4info_action_param_id_from_name(p4info, a_id, "match_result");
        action->set_arg(p, data, 6);
    }

    {
        char data[6];
        byte_buffer bytes((uint8_t *) data, 6, 6);
        bytes.set_value(0, action_bitmap, 6);

        p = 2; //pi_p4info_action_param_id_from_name(p4info, a_id, "match_result");

        action->set_arg(p, data, 6);

    }

    p = pi_p4info_action_param_id_from_name(p4info, a_id, "match_bitmap");
    action->set_arg(p, match_bitmap);

    p = pi_p4info_action_param_id_from_name(p4info, a_id, "next_stage");
    action->set_arg(p, next_stage);

    p = pi_p4info_action_param_id_from_name(p4info, a_id, "next_prog");
    action->set_arg(p, next_prog);

    if(branch_id == 1) {
        this->branch1_action = action;
    } else if(branch_id == 2) {
        this->branch2_action = action;
    } else {
        this->branch3_action = action;
    }
}

void
hyperv_conditional::set_expr_counter_op_const(uint16_t r_expr) {
    pi_p4_id_t a_id =
            pi_p4info_action_id_from_name(p4info,
                                          "set_expr_header_op_const");
    this->action = new ActionData(p4info, a_id);

    this->action->set_arg(GET_ACTION_PARAM_ID("r_expr"),
                          r_expr);
}