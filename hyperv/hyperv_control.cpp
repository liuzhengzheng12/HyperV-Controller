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

#include "hyperv_control.h"

hyperv_init::hyperv_init(pi_p4info_t *p4info, uint16_t vdp_id) : p4info(p4info) {
    this->vdp_id = vdp_id;
    this->stage_id = 0;
    this->inst_id = 0;
    this->table_id = pi_p4info_table_id_from_name(p4info, "table_config_at_initial");
    this->key_inst_id = pi_p4info_table_match_field_id_from_name(p4info,
                                                                 this->table_id,
                                                                 "vdp_metadata.inst_id");
    this->key_stage_id = pi_p4info_table_match_field_id_from_name(p4info,
                                                                 this->table_id,
                                                                 "vdp_metadata.stage_id");
    this->key_vdp_id = pi_p4info_table_match_field_id_from_name(p4info,
                                                                 this->table_id,
                                                                 "vdp_metadata.vdp_id");
}

MatchKey
hyperv_init::get_match_key() {
    MatchKey key(p4info, this->table_id);
    key.set_exact(key_stage_id, stage_id);
    key.set_exact(key_inst_id, inst_id);
    key.set_exact(key_vdp_id, vdp_id);
    return key;
}

ActionData
hyperv_init::get_action_data(uint8_t inst_id,
                             uint8_t stage_id,
                             uint8_t match_bitmap,
                             bool special) {
    pi_p4_id_t a_id;
    if (special) {
        a_id = pi_p4info_action_id_from_name(p4info, "set_initial_config_new");
    } else {
        a_id = pi_p4info_action_id_from_name(p4info, "set_initial_config");
    }
    ActionData action_data(p4info, a_id);

    pi_p4_id_t p = pi_p4info_action_param_id_from_name(p4info, a_id, "match_bitmap");
    action_data.set_arg(p, match_bitmap);

    p = pi_p4info_action_param_id_from_name(p4info, a_id, "progid");
    action_data.set_arg(p, inst_id);

    p = pi_p4info_action_param_id_from_name(p4info, a_id, "initstage");
    action_data.set_arg(p, stage_id);

    return action_data;
}


hyperv_end::hyperv_end(pi_p4info_t *p4info, uint16_t vdp_id) : p4info(p4info) {
    this->vdp_id = vdp_id;
    this->stage_id = 0;
    this->inst_id = 0;
    this->table_id = pi_p4info_table_id_from_name(p4info, "table_config_at_end");
    this->key_inst_id = pi_p4info_table_match_field_id_from_name(p4info,
                                                                 this->table_id,
                                                                 "vdp_metadata.inst_id");
    this->key_stage_id = pi_p4info_table_match_field_id_from_name(p4info,
                                                                  this->table_id,
                                                                  "vdp_metadata.stage_id");
    this->key_vdp_id = pi_p4info_table_match_field_id_from_name(p4info,
                                                                this->table_id,
                                                                "vdp_metadata.vdp_id");
}

MatchKey
hyperv_end::get_match_key() {
    MatchKey key(p4info, this->table_id);
    key.set_exact(key_stage_id, stage_id);
    key.set_exact(key_inst_id, inst_id);
    key.set_exact(key_vdp_id, vdp_id);
    return key;
}

ActionData
hyperv_end::get_action_data(uint8_t inst_id) {
    pi_p4_id_t a_id = pi_p4info_action_id_from_name(p4info, "do_resubmit");
    ActionData action_data(p4info, a_id);

    pi_p4_id_t p = pi_p4info_action_param_id_from_name(p4info, a_id, "progid");
    action_data.set_arg(p, inst_id);

    return action_data;
}