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

#include "hyperv_match.h"

void
hyperv_match::set_match_result(uint64_t match_result) {
    pi_p4_id_t action_id = pi_p4info_action_id_from_name(p4info, SET_MATCH_RESULT);
    pi_p4_id_t p1 = pi_p4info_action_param_id_from_name(p4info, action_id, "match_result");
    this->action_data = new ActionData(p4info, action_id);
    char data[6];
    byte_buffer bytes((uint8_t*) data, 6, 6);
    bytes.set_value(0, match_result, 6);
    this->action_data->set_arg(p1, data, 6);
}

void
hyperv_match::set_action_id(uint64_t match_result,
                            uint64_t action_bitmap,
                            uint8_t match_bitmap,
                            uint8_t next_stage,
                            uint16_t next_prog) {
    pi_p4_id_t a_id = pi_p4info_action_id_from_name(p4info, SET_ACTION_ID);
    this->action_data = new ActionData(p4info, a_id);

    pi_p4_id_t p;
    {
        char data[6];
        byte_buffer bytes((uint8_t *) data, 6, 6);
        bytes.set_value(0, match_result, 6);

        p = pi_p4info_action_param_id_from_name(p4info, a_id, "match_result");
        this->action_data->set_arg(p, data, 6);
    }

    {
        char data[6];
        byte_buffer bytes((uint8_t *) data, 6, 6);
        bytes.set_value(0, action_bitmap, 6);

        p = 2; //pi_p4info_action_param_id_from_name(p4info, a_id, "match_result");

        this->action_data->set_arg(p, data, 6);

    }

    p = pi_p4info_action_param_id_from_name(p4info, a_id, "match_bitmap");
    this->action_data->set_arg(p, match_bitmap);

    p = pi_p4info_action_param_id_from_name(p4info, a_id, "next_stage");
    this->action_data->set_arg(p, next_stage);

    p = pi_p4info_action_param_id_from_name(p4info, a_id, "next_prog");
    this->action_data->set_arg(p, next_prog);
}
void
hyperv_match::set_action_id_direct(uint64_t action_id,
                                   uint64_t action_bitmap,
                                   uint8_t match_bitmap,
                                   uint8_t next_stage,
                                   uint16_t next_prog) {
    pi_p4_id_t a_id = pi_p4info_action_id_from_name(p4info,
                                                    SET_ACTION_ID_DIRECT);
    this->action_data = new ActionData(p4info, a_id);

    char data[6];
    byte_buffer bytes((uint8_t*) data, 6, 6);
    bytes.set_value(0, action_id, 6);


    pi_p4_id_t p = pi_p4info_action_param_id_from_name(p4info, a_id, "action_id");
    this->action_data->set_arg(p, data, 6);

    bytes.set_value(0, action_bitmap, 6);

    p = pi_p4info_action_param_id_from_name(p4info, a_id, "action_bitmap");
    this->action_data->set_arg(p, data, 6);

    p = pi_p4info_action_param_id_from_name(p4info, a_id, "match_bitmap");
    this->action_data->set_arg(p, match_bitmap);

    p = pi_p4info_action_param_id_from_name(p4info, a_id, "next_stage");
    this->action_data->set_arg(p, next_stage);

    p = pi_p4info_action_param_id_from_name(p4info, a_id, "next_prog");
    this->action_data->set_arg(p, next_prog);
}


void
hyperv_match::set_next_stage(uint8_t match_bitmap,
                                  uint8_t next_stage,
                                  uint16_t next_prog) {
    pi_p4_id_t a_id = pi_p4info_action_id_from_name(p4info, SET_NEXT_STAGE);
    this->action_data = new ActionData(p4info, a_id);

    pi_p4_id_t p = pi_p4info_action_param_id_from_name(p4info, a_id, "match_bitmap");
    this->action_data->set_arg(p, match_bitmap);

    p = pi_p4info_action_param_id_from_name(p4info, a_id, "next_stage");
    this->action_data->set_arg(p, next_stage);

    p = pi_p4info_action_param_id_from_name(p4info, a_id, "next_prog");
    this->action_data->set_arg(p, next_prog);
}

void
hyperv_match::end(uint8_t next_prog) {
    pi_p4_id_t a_id = pi_p4info_action_id_from_name(p4info, "end");
    this->action_data = new ActionData(p4info, a_id);

    pi_p4_id_t p = pi_p4info_action_param_id_from_name(p4info, a_id, "next_prog");
    this->action_data->set_arg(p, next_prog);
}

void
header_match::set_value(int offset, uint64_t value, int width) {
    byte_buffer bytes(this->hdr, HDR_SIZE, this->space);
    bytes.set_value(offset, value, width);
    byte_buffer mask_bytes(this->hdr_mask, HDR_SIZE, this->space);
    mask_bytes.set_value(offset, 0xFFFFFFFFFFFFFFFFULL, width);
}

void
header_match::set_value(int offset, unsigned int value) {
    byte_buffer bytes(this->hdr, HDR_SIZE, this->space);
    bytes.set_value(offset, value);
    byte_buffer mask_bytes(this->hdr_mask, HDR_SIZE, this->space);
    mask_bytes.set_value(offset, (unsigned int) 0xFFFFFFFF);
}

void
header_match::set_value(int offset, uint16_t value) {
    byte_buffer bytes(this->hdr, HDR_SIZE, this->space);
    bytes.set_value(offset, value);
    byte_buffer mask_bytes(this->hdr_mask, HDR_SIZE, this->space);
    mask_bytes.set_value(offset, (uint16_t) 0xFFFF);
}

void
header_match::set_value(int offset, uint8_t value) {
    byte_buffer bytes(this->hdr, HDR_SIZE, this->space);
    bytes.set_value(offset, value);
    byte_buffer mask_bytes(this->hdr_mask, HDR_SIZE, this->space);
    mask_bytes.set_value(offset, (uint8_t) 0xFF);
}


void
header_match::set_eth_type(uint16_t type) {
    set_value(ETH_TYPE_BYTE_OFFSET, type);
}

void
header_match::set_ip_proto(uint8_t proto) {
    set_value(IP_PROTO_BYTE_OFFSET, proto);
}

void
header_match::set_ip_dst(uint32_t dst) {
    set_value(IP_DST_BYTE_OFFSET, dst);
}

void
header_match::set_ip_src(uint32_t src) {
    set_value(IP_SRC_BYTE_OFFSET, src);
}

MatchKey
header_match::get_match_key() {
    MatchKey key(p4info, this->table_id);
    key.set_exact(key_stage_id, stage_id);
    key.set_exact(key_inst_id, inst_id);
    key.set_ternary(key_hdr_id,
                    (const char*) hdr,
                    (const char*) hdr_mask,
                    100);
    key.set_priority(this->priority);
    return key;
}

void
metadata_match::set_value(int offset, unsigned int value) {
    byte_buffer bytes(this->meta, 48, 48);
    bytes.set_value(offset, value);
    byte_buffer mask_bytes(this->meta_mask, 48, 48);
    mask_bytes.set_value(offset, (unsigned int) 0xFFFFFFFF);
}



void
metadata_match::set_value(int offset, uint16_t value) {
    byte_buffer bytes(this->meta, 48, 48);
    bytes.set_value(offset, value);
    byte_buffer mask_bytes(this->meta_mask, 48, 48);
    mask_bytes.set_value(offset, (uint16_t) 0xFFFF);

}

void
metadata_match::set_value(int offset, uint8_t value) {
    byte_buffer bytes(this->meta, 48, 48);
    bytes.set_value(offset, value);
    byte_buffer mask_bytes(this->meta_mask, 48, 48);
    mask_bytes.set_value(offset, (uint8_t) 0xFF);
}


MatchKey
metadata_match::get_match_key() {

    MatchKey key(p4info, this->table_id);
    key.set_exact(key_stage_id, stage_id);
    key.set_exact(key_inst_id, inst_id);
    key.set_ternary(key_meta_id,
                    (const char*)meta,
                    (const char*)meta_mask,
                    META_SIZE);
    key.set_priority(this->priority);
    return key;
}


MatchKey
intrinsic_match::get_match_key() {
    pi_p4_id_t t_id = pi_p4info_table_id_from_name(p4info, get_table_name().c_str());

    MatchKey key(p4info, t_id);
    key.set_exact(key_stage_id, stage_id);
    key.set_exact(key_inst_id, inst_id);
    key.set_ternary(key_eport,this->eport, this->eport_mask);
    key.set_ternary(key_iport,this->iport, this->iport_mask);
    key.set_ternary(key_instance_type,
                    this->instance_type,
                    this->instance_type_mask);
    key.set_priority(this->priority);
    return key;
}