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

#ifndef CONTROLLER_HYPERV_H
#define CONTROLLER_HYPERV_H

#include <PI/pi.h>
#include <PI/frontends/generic/pi.h>
#include <PI/frontends/cpp/tables.h>

#include <string>
#include <cstring>
#include <iostream>
#include <sstream>

#include "hyperv.h"
#include "hyperv_utils.h"

using std::string;

using namespace pi;

class hyperv_match: public hyperv_context {
protected:
    enum hyperv_match_type {
        HEADER_MATCH = BIT_MASK_HEADER,
        METADATA_MATCH = BIT_MASK_USER_META,
        INTRINSIC_MATCH = BIT_MASK_STD_META,
        MATCH_RESULT = 0,
    } match_type;
    uint8_t inst_id;
    pi_p4_id_t key_inst_id;
    pi_p4_id_t key_stage_id;
    ActionData * action_data;

public:
    hyperv_match(pi_p4info_t * p4info,
                 uint8_t inst_id,
                 uint8_t stage_id,
                 enum hyperv_match_type type) {
        this->p4info = p4info;
        this->match_type = type;
        this->inst_id = inst_id;
        this->stage_id = stage_id;
        this->table_id =
                pi_p4info_table_id_from_name(p4info,
                                             get_table_name().c_str());
        this->key_inst_id =
                pi_p4info_table_match_field_id_from_name(p4info, this->table_id,
                                             "vdp_metadata.inst_id");
        this->key_stage_id =
                pi_p4info_table_match_field_id_from_name(p4info, this->table_id,
                                             "vdp_metadata.stage_id");
        this->action_data = nullptr;
    }

    ~hyperv_match(){
        if (action_data != nullptr) {
            delete action_data;
        }
    }

    enum hyperv_match_type get_match_type() {
        return match_type;
    }

    string get_table_name() {
        std::ostringstream ostr;
        if (this->match_type == HEADER_MATCH) {
            ostr << HEADER_MATCH_TABLE_NAME;
        } else if(this->match_type == METADATA_MATCH) {
            ostr << METADATA_MATCH_TABLE_NAME;
        } else if(this->match_type == INTRINSIC_MATCH) {
            ostr << INTRINSIC_MATCH_TABLE_NAME;
        } else {
            ostr << MATCH_RESULT_TABLE_NAME;
        }
        ostr << "stage";
        ostr << this->stage_id;
        return ostr.str();
    }

    virtual MatchKey get_match_key() = 0;

    ActionData get_action_data() {
        return *this->action_data;
    }


    /**
     *
     * @param priority
     */
    void set_priority(int priority) {
        this->priority = priority;
    }

    /**
     *
     * @param match_result
     */
    void set_match_result(uint64_t match_result);

    /**
     *
     * @param match_result
     * @param action_bitmap
     * @param match_bitmap
     * @param next_stage
     * @param next_prog
     */
    void set_action_id(uint64_t match_result,
                       uint64_t action_bitmap,
                       uint8_t match_bitmap,
                       uint8_t next_stage,
                       uint16_t next_prog);

    /**
     *
     * @param action_id
     * @param action_bitmap
     * @param match_bitmap
     * @param next_stage
     * @param next_prog
     */
    void set_action_id_direct(uint64_t action_id,
                              uint64_t action_bitmap,
                              uint8_t match_bitmap,
                              uint8_t next_stage,
                              uint16_t next_prog);

    /**
     *
     * @param match_bitmap
     * @param next_stage
     * @param next_prog
     */
    void set_next_stage(uint8_t match_bitmap,
                        uint8_t next_stage,
                        uint16_t next_prog);

    /**
     *
     * @param next_prog
     */
    void end(uint8_t next_prog);
};

const size_t HDR_SIZE = 100;
const int DEFAULT_HEADER_SPACE = 30;
class header_match: public hyperv_match{

private:
    uint8_t hdr[HDR_SIZE] = {0};
    uint8_t hdr_mask[HDR_SIZE] = {0};
    int space;
    pi_p4_id_t key_hdr_id;

public:
    /**
     *
     * @param p4info
     * @param inst_id
     * @param stage_id
     */
    header_match(pi_p4info_t * p4info,
                 uint8_t inst_id,
                 uint8_t stage_id):
            hyperv_match(p4info,
                         inst_id,
                         stage_id,
                         hyperv_match::hyperv_match_type::HEADER_MATCH) {
        key_hdr_id =
                pi_p4info_table_match_field_id_from_name(p4info,
                                                         this->table_id,
                                                         "user_metadata.load_header");
        space = DEFAULT_HEADER_SPACE;
    }

    int get_space() {
        return space;
    }
    /**
     *
     */
    void set_value(int, uint64_t, int);

    /**
     *
     */
    void set_value(int, unsigned int);

    /**
     *
     */
    void set_value(int, uint16_t);


    /**
     *
     * @param type
     */
    void set_eth_type(uint16_t type);

    /**
     *
     * @param proto
     */
    void set_ip_proto(uint8_t proto);


    /**
     *
     * @param src
     */
    void set_ip_src(uint32_t src);


    /**
     *
     * @param dst
     */
    void set_ip_dst(uint32_t dst);

    /**
     *
     */
    void set_value(int, uint8_t);

    MatchKey get_match_key() override;
};

const size_t META_SIZE = 48;
class metadata_match: public hyperv_match {

private:
    uint8_t meta[META_SIZE] = {0};
    uint8_t meta_mask[META_SIZE] = {0};
    pi_p4_id_t key_meta_id;
public:
    /**
     *
     * @param p4info
     * @param inst_id
     * @param stage_id
     */
    metadata_match(pi_p4info_t * p4info,
                   uint8_t inst_id,
                   uint8_t stage_id):
            hyperv_match(p4info,
                         inst_id,
                         stage_id,
                         hyperv_match::hyperv_match_type::METADATA_MATCH) {
        key_meta_id =
                pi_p4info_table_match_field_id_from_name(p4info,
                                                         this->table_id,
                                                         "user_metadata.meta");

    }

    void set_value(int, unsigned int);
    void set_value(int, uint16_t);
    void set_value(int, uint8_t);

    MatchKey get_match_key() override ;
};

class intrinsic_match: public hyperv_match {

private:
    uint16_t iport;
    uint16_t eport;
    unsigned int instance_type;
    uint16_t iport_mask;
    uint16_t eport_mask;
    unsigned int instance_type_mask;

    pi_p4_id_t key_iport;
    pi_p4_id_t key_eport;
    pi_p4_id_t key_instance_type;
public:

    /**
     *
     * @param p4info
     * @param inst_id
     * @param stage_id
     */
    intrinsic_match(pi_p4info_t * p4info, uint8_t inst_id, uint8_t stage_id):
            hyperv_match(p4info, inst_id, stage_id, hyperv_match::hyperv_match_type::INTRINSIC_MATCH) {
        key_iport = pi_p4info_table_match_field_id_from_name(p4info,
                                                             this->table_id,
                                                             "standard_metadata.ingress_port");
        key_eport = pi_p4info_table_match_field_id_from_name(p4info,
                                                             this->table_id,
                                                             "standard_metadata.egress_spec");
        key_instance_type = pi_p4info_table_match_field_id_from_name(p4info,
                                                                     this->table_id,
                                                                     "standard_metadata.instance_type");


        this->iport = 0;
        this->iport_mask = 0;
        this->eport = 0;
        this->eport_mask = 0;
        this->instance_type = 0;
        this->instance_type_mask = 0;
    }

    /**
     *
     * @param iport
     */
    void set_iport_value(uint16_t iport) {
        this->iport = iport;
        this->iport_mask = 0xFFFF;
    }

    /**
     *
     * @param eport
     */
    void set_eport_value(uint16_t eport) {
        this->eport = eport;
        this->eport_mask = 0xFFFF;
    }

    /**
     *
     * @param instance_type
     */
    void set_instance_type_value(unsigned int instance_type) {
        this->instance_type = instance_type;
        this->instance_type_mask = 0xFFFFFFFF;
    }

    MatchKey get_match_key() override;
};

class hyperv_match_result : public hyperv_match {

private:
    pi_p4_id_t key_match_result;
    uint64_t match_result;
    pi_p4info_t * p4info;
public:
    /**
     *
     * @param p4info
     * @param inst_id
     * @param stage_id
     */
    hyperv_match_result(pi_p4info_t * p4info,
                        uint8_t inst_id,
                        uint8_t stage_id,
                        uint64_t match_result):
            hyperv_match(p4info, inst_id, stage_id, hyperv_match::hyperv_match_type::MATCH_RESULT) {
        this->p4info = p4info;
        this->match_result = match_result;
        this->stage_id = stage_id;
        this->key_match_result = pi_p4info_table_match_field_id_from_name(p4info,
                                                                          this->table_id,
                                                                          "vdp_metadata.match_chain_result");
    }

    MatchKey get_match_key() override {
        MatchKey key(p4info, this->table_id);

        char data[6];
        byte_buffer bytes((uint8_t*) data, 6, 6);

        bytes.set_value(0, match_result, 6);

        key.set_exact(key_match_result, data, 6);

        return key;
    }
};




#endif //CONTROLLER_HYPERV_H
