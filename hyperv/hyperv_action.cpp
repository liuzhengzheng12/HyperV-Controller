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

#include <iostream>
#include "hyperv_action.h"

void
mod_std_meta::set_forward(uint16_t port) {

    pi_p4_id_t a_id = pi_p4info_action_id_from_name(p4info, "do_forward");

    this->action_data = new ActionData(p4info, a_id);

    pi_p4_id_t p = pi_p4info_action_param_id_from_name(p4info, a_id, "port");

    this->action_data->set_arg(p, port);
}

void
mod_std_meta::set_loopback() {
    pi_p4_id_t a_id = pi_p4info_action_id_from_name(p4info, DO_LOOPBACK);
    this->action_data = new ActionData(p4info, a_id);
}

void
mod_std_meta::set_multicast(uint16_t multi_grp) {
    pi_p4_id_t a_id = pi_p4info_action_id_from_name(p4info, "do_multicast");

    this->action_data = new ActionData(p4info, a_id);

    pi_p4_id_t p = pi_p4info_action_param_id_from_name(p4info, a_id, "mcast_grp");

    this->action_data->set_arg(p, multi_grp);
}

void
mod_std_meta::set_queue(uint8_t qid) {
    pi_p4_id_t a_id = pi_p4info_action_id_from_name(p4info, "do_queue");

    this->action_data = new ActionData(p4info, a_id);

    pi_p4_id_t p = pi_p4info_action_param_id_from_name(p4info, a_id, "qid");

    this->action_data->set_arg(p, qid);
}

void
mod_std_meta::set_drop() {
    pi_p4_id_t a_id = pi_p4info_action_id_from_name(p4info, DO_DROP);
    this->action_data = new ActionData(p4info, a_id);
}

void
mod_meta_with_const::do_mod_meta_with_const() {
    pi_p4_id_t a_id = pi_p4info_action_id_from_name(p4info,
                                                    "do_mod_meta_with_const");
    this->action_data = new ActionData(p4info, a_id);
    this->action_data->set_arg(GET_ACTION_PARAM_ID("value"),
                               (char *)this->meta,
                               48);
    this->action_data->set_arg(GET_ACTION_PARAM_ID("mask1"),
                               (char *)this->meta_mask,
                               48);
}

void
mod_meta_with_const::set_value(int offset,
                               unsigned int value) {
    byte_buffer bytes(this->meta, 48, 48);
    bytes.set_value(offset, value);
    byte_buffer mask_bytes(this->meta_mask, 48, 48);
    mask_bytes.set_value(offset, (unsigned int) 0xFFFFFFFF);
}



void
mod_meta_with_const::set_value(int offset,
                               uint16_t value) {
    byte_buffer bytes(this->meta, 48, 48);
    bytes.set_value(offset, value);
    byte_buffer mask_bytes(this->meta_mask, 48, 48);
    mask_bytes.set_value(offset, (uint16_t) 0xFFFF);

}

void
mod_meta_with_const::set_value(int offset,
                               uint8_t value) {
    byte_buffer bytes(this->meta, 48, 48);
    bytes.set_value(offset, value);
    byte_buffer mask_bytes(this->meta_mask, 48, 48);
    mask_bytes.set_value(offset, (uint8_t) 0xFF);
}


void
substarct::set_meta_value(int offset,
                          unsigned int value) {
    byte_buffer bytes(this->meta, 48, 48);
    bytes.set_value(offset, value);
    byte_buffer mask_bytes(this->meta_mask, 48, 48);
    mask_bytes.set_value(offset, (unsigned int) 0xFFFFFFFF);
}



void
substarct::set_meta_value(int offset,
                               uint16_t value) {
    byte_buffer bytes(this->meta, 48, 48);
    bytes.set_value(offset, value);
    byte_buffer mask_bytes(this->meta_mask, 48, 48);
    mask_bytes.set_value(offset, (uint16_t) 0xFFFF);

}

void
substarct::set_meta_value(int offset,
                               uint8_t value) {
    byte_buffer bytes(this->meta, 48, 48);
    bytes.set_value(offset, value);
    byte_buffer mask_bytes(this->meta_mask, 48, 48);
    mask_bytes.set_value(offset, (uint8_t) 0xFF);
}


void
substarct::set_header_value(int offset, uint64_t value, int width) {
    byte_buffer bytes(this->hdr, HDR_SIZE, this->space);
    bytes.set_value(offset, value, width);
    byte_buffer mask_bytes(this->hdr_mask, HDR_SIZE, this->space);
    mask_bytes.set_value(offset, 0xFFFFFFFFFFFFFFFFULL, width);
}

void
substarct::set_header_value(int offset, unsigned int value) {
    byte_buffer bytes(this->hdr, HDR_SIZE, this->space);
    bytes.set_value(offset, value);
    byte_buffer mask_bytes(this->hdr_mask, HDR_SIZE, this->space);
    mask_bytes.set_value(offset, (unsigned int) 0xFFFFFFFF);
}

void
substarct::set_header_value(int offset, uint16_t value) {
    byte_buffer bytes(this->hdr, HDR_SIZE, this->space);
    bytes.set_value(offset, value);
    byte_buffer mask_bytes(this->hdr_mask, HDR_SIZE, this->space);
    mask_bytes.set_value(offset, (uint16_t) 0xFFFF);
}

void
substarct::set_header_value(int offset, uint8_t value) {
    byte_buffer bytes(this->hdr, HDR_SIZE, this->space);
    bytes.set_value(offset, value);
    byte_buffer mask_bytes(this->hdr_mask, HDR_SIZE, this->space);
    mask_bytes.set_value(offset, (uint8_t) 0xFF);
}


void
substarct::do_subtract_const_from_header() {
    pi_p4_id_t a_id = pi_p4info_action_id_from_name(p4info,
                                                    "do_subtract_const_from_header");
    this->action_data = new ActionData(p4info, a_id);
    this->action_data->set_arg(GET_ACTION_PARAM_ID("value1"),
                               (char *) this->hdr,
                               HDR_SIZE);
    this->action_data->set_arg(GET_ACTION_PARAM_ID("mask1"),
                               (char *) this->hdr_mask,
                               HDR_SIZE);
}

void
substarct::do_subtract_const_from_meta() {
    pi_p4_id_t a_id = pi_p4info_action_id_from_name(p4info,
                                                    "do_subtract_const_from_meta");
    this->action_data = new ActionData(p4info, a_id);
    this->action_data->set_arg(GET_ACTION_PARAM_ID("value1"),
                               (char *) this->meta,
                               META_SIZE);
    this->action_data->set_arg(GET_ACTION_PARAM_ID("mask1"),
                               (char *) this->meta_mask,
                               META_SIZE);
}


void
mod_header_with_const::set_value(int offset, uint64_t value, int width) {
    byte_buffer bytes(this->hdr, HDR_SIZE, this->space);
    bytes.set_value(offset, value, width);
    byte_buffer mask_bytes(this->hdr_mask, HDR_SIZE, this->space);
    mask_bytes.set_value(offset, 0xFFFFFFFFFFFFFFFFULL, width);
}

void
mod_header_with_const::set_value(int offset, unsigned int value) {
    byte_buffer bytes(this->hdr, HDR_SIZE, this->space);
    bytes.set_value(offset, value);

    byte_buffer mask_bytes(this->hdr_mask, HDR_SIZE, this->space);
    mask_bytes.set_value(offset, (unsigned int) 0xFFFFFFFF);
}

void
mod_header_with_const::set_value(int offset, uint16_t value) {
    byte_buffer bytes(this->hdr, HDR_SIZE, this->space);
    bytes.set_value(offset, value);
    byte_buffer mask_bytes(this->hdr_mask, HDR_SIZE, this->space);
    mask_bytes.set_value(offset, (uint16_t) 0xFFFF);
}

void
mod_header_with_const::set_value(int offset, uint8_t value) {
    byte_buffer bytes(this->hdr, HDR_SIZE, this->space);
    bytes.set_value(offset, value);
    byte_buffer mask_bytes(this->hdr_mask, HDR_SIZE, this->space);
    mask_bytes.set_value(offset, (uint8_t) 0xFF);
}

void
mod_header_with_const::do_mod_header_with_const() {
    pi_p4_id_t a_id = pi_p4info_action_id_from_name(p4info,
                                                    "do_mod_header_with_const");
    this->action_data = new ActionData(p4info, a_id);
    this->action_data->set_arg(GET_ACTION_PARAM_ID("value"),
                               (char *) this->hdr,
                               HDR_SIZE);
    this->action_data->set_arg(GET_ACTION_PARAM_ID("mask1"),
                               (char *) this->hdr_mask,
                               HDR_SIZE);
}

void
generate_digest::do_gen_digest() {
    pi_p4_id_t a_id = pi_p4info_action_id_from_name(p4info,
                                                    "do_gen_digest");
    this->action_data = new ActionData(p4info, a_id);
}

void
counter::do_packet_count(uint32_t index) {
    pi_p4_id_t a_id = pi_p4info_action_id_from_name(p4info,
                                                    "packet_count");
    this->action_data = new ActionData(p4info, a_id);
    this->action_data->set_arg(GET_ACTION_PARAM_ID("index"),
                               index);
}