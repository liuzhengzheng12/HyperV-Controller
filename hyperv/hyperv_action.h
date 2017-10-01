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

#ifndef CONTROLLER_HYPERV_ACTION_H
#define CONTROLLER_HYPERV_ACTION_H


#include <PI/pi.h>
#include <PI/frontends/generic/pi.h>
#include <PI/frontends/cpp/tables.h>

#include <string>
#include <sstream>

#include "hyperv.h"
#include "hyperv_utils.h"
#include "hyperv_match.h"

using std::string;

using namespace pi;

class hyperv_action : public hyperv_context {
protected:
    enum hyperv_action_type {
        FORWARD = 0
    } action_type;

    uint64_t action_id;
    pi_p4_id_t key_action_id;
    ActionData * action_data;
public:
    hyperv_action(pi_p4info_t * p4info,
                  uint8_t stage_id,
                  uint64_t action_id,
                  hyperv_action_type type): hyperv_context() {
        this->action_type = type;
        this->action_id = action_id;
        this->stage_id = stage_id;
        this->p4info = p4info;
        this->action_data = nullptr;
   }
    enum hyperv_action_type get_action_type() {
        return this->action_type;
    }
    virtual string get_table_name() = 0;

    virtual MatchKey get_match_key() {
        this->table_id =
                pi_p4info_table_id_from_name(p4info,
                                             get_table_name().c_str());
        this->key_action_id =
                pi_p4info_table_match_field_id_from_name(p4info,
                                                         this->table_id,
                                                         "vdp_metadata.action_chain_id");
        char data [6];
        byte_buffer bytes((uint8_t*) data, 6, 6);
        bytes.set_value(0, this->action_id, 6);
        MatchKey key(p4info, this->table_id);
        error_code_t t = key.set_exact(key_action_id, data, 6);
        return key;
    }

    virtual uint64_t get_action_bitmap() = 0;

    ActionData get_action_data() {
        return *this->action_data;
    }
};


class mod_std_meta : public hyperv_action {
public:
    mod_std_meta(pi_p4info_t * p4info,
                       uint8_t stage_id,
                       uint64_t action_id):
            hyperv_action(p4info,
                          stage_id,
                          action_id,
                          hyperv_action::hyperv_action_type::FORWARD) {

    }

    uint64_t get_action_bitmap() override {
        return BIT_MASK_MOD_STD_META;
    }


    string get_table_name() override {
        std::ostringstream ostr;
        ostr << "table_mod_std_meta_stage";
        ostr << stage_id;
        return ostr.str();
    }

    void set_forward(uint16_t port);
    void set_loopback();
    void set_drop();
    void set_multicast(uint16_t multi_grp);
    void set_queue(uint8_t qid);
};



class add_header : hyperv_action {
public:
    add_header(pi_p4info_t * p4info,
                   uint8_t stage_id,
                   uint64_t action_id):
            hyperv_action(p4info,
                          stage_id,
                          action_id,
                          hyperv_action::hyperv_action_type::FORWARD) {

    }

    uint64_t  get_action_bitmap() override {
        return BIT_MASK_ADD_HEADER;
    }

};

class copy_header : hyperv_action {
public:
    copy_header(pi_p4info_t * p4info,
                      uint8_t stage_id,
                      uint64_t action_id):
            hyperv_action(p4info,
                          stage_id,
                          action_id,
                          hyperv_action::hyperv_action_type::FORWARD) {

    }

    uint64_t  get_action_bitmap() override {
        return BIT_MASK_COPY_HEADER;
    }
};

class remove_header : hyperv_action {
public:
    remove_header(pi_p4info_t * p4info,
                       uint8_t stage_id,
                       uint64_t action_id):
            hyperv_action(p4info,
                          stage_id,
                          action_id,
                          hyperv_action::hyperv_action_type::FORWARD) {

    }

    uint64_t get_action_bitmap() override {
        return BIT_MASK_REMOVE_HEADER;
    }
};

class mod_header_with_const : public hyperv_action {
private:
    uint8_t hdr[HDR_SIZE] = {0};
    uint8_t hdr_mask[HDR_SIZE] = {0};
    int space;
public:
    mod_header_with_const(pi_p4info_t * p4info,
                         uint8_t stage_id,
                         uint64_t action_id):
            hyperv_action(p4info,
                          stage_id,
                          action_id,
                          hyperv_action::hyperv_action_type::FORWARD) {
        space = DEFAULT_HEADER_SPACE;
    }

    uint64_t get_action_bitmap() override {
        return BIT_MASK_MOD_HEADER_WITH_CONST;
    }

    string get_table_name() override {
        std::ostringstream ostr;
        ostr << "table_mod_header_with_const_stage";
        ostr << stage_id;
        return ostr.str();
    }

    void set_value(int offset, uint64_t value, int width);
    void set_value(int, unsigned int);
    void set_value(int, uint16_t);
    void set_value(int, uint8_t);

    void do_mod_header_with_const();
};

#define META_SIZE 48
class mod_meta_with_const : public hyperv_action {
private:
    uint8_t meta[META_SIZE] = {0};
    uint8_t meta_mask[META_SIZE] = {0};
public:
    mod_meta_with_const(pi_p4info_t * p4info,
                          uint8_t stage_id,
                          uint64_t action_id):
            hyperv_action(p4info,
                          stage_id,
                          action_id,
                          hyperv_action::hyperv_action_type::FORWARD) {

    }

    uint64_t get_action_bitmap() override {
        return BIT_MASK_MOD_META_WITH_CONST;
    }

    string get_table_name() override {
        std::ostringstream ostr;
        ostr << "table_mod_meta_with_const_stage";
        ostr << stage_id;
        return ostr.str();
    }

    void set_value(int, unsigned int);
    void set_value(int, uint16_t);
    void set_value(int, uint8_t);

    void do_mod_meta_with_const();

};

class mod_header_with_meta : public hyperv_action {
public:
    mod_header_with_meta(pi_p4info_t * p4info,
                        uint8_t stage_id,
                        uint64_t action_id):
            hyperv_action(p4info,
                          stage_id,
                          action_id,
                          hyperv_action::hyperv_action_type::FORWARD) {

    }

    uint64_t get_action_bitmap() override {
        return BIT_MASK_MOD_HEADER_WITH_META;
    }

    string get_table_name() override {
        std::ostringstream ostr;
        ostr << "table_mod_header_with_meta_stage";
        ostr << stage_id;
        return ostr.str();
    }


};

class mod_header_with_header : public hyperv_action {
public:
    mod_header_with_header(pi_p4info_t * p4info,
                         uint8_t stage_id,
                         uint64_t action_id):
            hyperv_action(p4info,
                          stage_id,
                          action_id,
                          hyperv_action::hyperv_action_type::FORWARD) {

    }

    uint64_t get_action_bitmap() override {
        return BIT_MASK_MOD_HEADER_WITH_HEADER;
    }
};

class mod_meta_with_meta : public hyperv_action {
public:
    mod_meta_with_meta(pi_p4info_t * p4info,
                         uint8_t stage_id,
                         uint64_t action_id):
            hyperv_action(p4info,
                          stage_id,
                          action_id,
                          hyperv_action::hyperv_action_type::FORWARD) {

    }

    uint64_t get_action_bitmap() override {
        return BIT_MASK_MOD_META_WITH_META;
    }
};

class mod_meta_with_header : public hyperv_action {
public:
    mod_meta_with_header(pi_p4info_t * p4info,
                       uint8_t stage_id,
                       uint64_t action_id):
            hyperv_action(p4info,
                          stage_id,
                          action_id,
                          hyperv_action::hyperv_action_type::FORWARD) {

    }

    uint64_t get_action_bitmap() override {
        return BIT_MASK_MOD_META_WITH_HEADER;
    }
};

class substarct : public hyperv_action {
private:
    uint8_t meta[META_SIZE] = {0};
    uint8_t meta_mask[META_SIZE] = {0};
    uint8_t hdr[HDR_SIZE] = {0};
    uint8_t hdr_mask[HDR_SIZE] = {0};
    int space;
public:
    substarct(pi_p4info_t * p4info,
    uint8_t stage_id,
            uint64_t action_id):
    hyperv_action(p4info,
            stage_id,
            action_id,
            hyperv_action::hyperv_action_type::FORWARD) {
        space = DEFAULT_HEADER_SPACE;
    }

    string get_table_name() override {
        std::ostringstream ostr;
        ostr << "table_subtract_stage";
        ostr << stage_id;
        return ostr.str();
    }

    uint64_t get_action_bitmap() override {
        return BIT_MASK_SUBTRACT;
    }

    /**
    *
    */
    void set_header_value(int offset, uint64_t value, int width);
    void set_header_value(int, unsigned int);
    void set_header_value(int, uint16_t);
    void set_header_value(int, uint8_t);

    void set_meta_value(int, unsigned int);
    void set_meta_value(int, uint16_t);
    void set_meta_value(int, uint8_t);

    void do_subtract_const_from_header();
    void do_subtract_const_from_meta();
};

class generate_digest : public hyperv_action {
public:
    generate_digest(pi_p4info_t * p4info,
    uint8_t stage_id,
            uint64_t action_id):
    hyperv_action(p4info,
            stage_id,
            action_id,
            hyperv_action::hyperv_action_type::FORWARD) {

    }

    string get_table_name() override {
        std::ostringstream ostr;
        ostr << "table_generate_digest_stage";
        ostr << stage_id;
        return ostr.str();
    }

    uint64_t get_action_bitmap() override {
        return BIT_MASK_GENERATE_DIGIST;
    }

    /**
     *
     */
    void do_gen_digest();
};

class counter : public hyperv_action {
public:
    counter(pi_p4info_t * p4info,
                    uint8_t stage_id,
                    uint64_t action_id):
            hyperv_action(p4info,
                          stage_id,
                          action_id,
                          hyperv_action::hyperv_action_type::FORWARD) {

    }

    string get_table_name() override {
        std::ostringstream ostr;
        ostr << "table_counter_stage";
        ostr << stage_id;
        return ostr.str();
    }

    uint64_t get_action_bitmap() override {
        return BIT_MASK_COUNTER;
    }

    /**
     *
     */
    void do_packet_count(uint32_t index);
};




#endif //CONTROLLER_HYPERV_ACTION_H
