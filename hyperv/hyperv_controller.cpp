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

#include "hyperv_controller.h"

#include <iostream>

using namespace std;

hyperv_controller::hyperv_controller(pi_p4info_t *p4info) {
    this->p4info = p4info;
    pi_session_init(&sess);
}

hyperv_controller::~hyperv_controller() {
    pi_session_cleanup(sess);
}

void
hyperv_controller::assign_device(char *port) {
    pi_assign_extra_t assign_options[2];
    memset(assign_options, 0, sizeof(assign_options));
    {
        pi_assign_extra_t *rpc_port = &assign_options[0];
        rpc_port->key = "port";
        rpc_port->v = port;
    }
    assign_options[1].end_of_extras = true;

    this->dev_id_list.push_back(this->id_counter);

    pi_assign_device(this->id_counter++, p4info, assign_options);
}

int
hyperv_controller::set_default_entries() {
    vector<pi_dev_id_t>::iterator i;
    for(i = dev_id_list.begin(); i != dev_id_list.end(); i++) {
        pi_dev_tgt_t dev_tgt = {*i, 0xFFFF};
        {
            pi_p4_id_t t_id = pi_p4info_table_id_from_name(p4info, "dh_deparse");
            pi_p4_id_t a_id = pi_p4info_action_id_from_name(p4info, "do_dh_deparse");

            pi::ActionData action_data(p4info, a_id);
            pi::MatchTable mt(sess, dev_tgt, p4info, t_id);

            if (mt.default_entry_set(action_data) != PI_STATUS_SUCCESS) {
                std::cout << "Error when adding default entry to 'ipv4_lpm'\n";
            }
        }
        {
            // TODO
        }
    }
}

void
hyperv_controller::set_default_entry(pi_dev_id_t dev_id,
                                     string table_name,
                                     ActionData & action) {
    pi_dev_tgt_t dev_tgt = {dev_id, 0xffff};
    pi_p4_id_t t_id = pi_p4info_table_id_from_name(p4info, table_name.c_str());
    MatchTable table(sess, dev_tgt, p4info, t_id);
    table.default_entry_set(action);
}

void
hyperv_controller::add_entry(pi_dev_id_t dev_id,
                             const char *table_name,
                             MatchKey & key,
                             ActionData & data) {

    pi_p4_id_t t_id = pi_p4info_table_id_from_name(p4info, table_name);
    pi_dev_tgt_t dev_tgt = { dev_id, 0xFFFF};

    MatchTable table(sess, dev_tgt, p4info, t_id);

    table.entry_add(key, data, true, &entry_handler);
}


void
hyperv_controller::add_match(pi_dev_id_t dev_id,
                             hyperv_match *match) {

    MatchKey key = match->get_match_key();
    ActionData data = match->get_action_data();

    this->add_entry(dev_id, match->get_table_name().c_str(), key, data);
}

void
hyperv_controller::add_action(pi_dev_id_t dev_id,
                              hyperv_action *action) {
    MatchKey key = action->get_match_key();
    ActionData data = action->get_action_data();
    this->add_entry(dev_id, action->get_table_name().c_str(), key, data);
}


void
hyperv_controller::add_init(pi_dev_id_t dev_id,
                            uint16_t vdp_id,
                            uint8_t inst_id,
                            uint8_t stage_id,
                            uint8_t match_bitmap, bool special) {
    hyperv_init init(p4info, vdp_id);
    MatchKey key = init.get_match_key();

    ActionData data = init.get_action_data(inst_id,
                                           stage_id,
                                           match_bitmap,
                                           special);
    this->add_entry(dev_id, init.get_table_name().c_str(), key, data);

}


void
hyperv_controller::add_end(pi_dev_id_t dev_id,
                           uint8_t vdp_id,
                           uint8_t inst_id) {
    hyperv_end end(p4info, vdp_id);
    MatchKey key = end.get_match_key();
    ActionData data = end.get_action_data(inst_id);
    this->add_entry(dev_id, end.get_table_name().c_str(),
                    key,
                    data);
}

void
hyperv_controller::add_conditional(pi_dev_id_t dev_id,
                                   hyperv_conditional *conditional) {
    {
        MatchKey key = conditional->get_match();
        ActionData action = conditional->get_action();

        this->add_entry(dev_id,
                        conditional->get_table_name().c_str(),
                        key,
                        action);
    }
    {
        MatchKey key = conditional->get_branch1_match();
        ActionData action = conditional->get_branch1_action();

        this->add_entry(dev_id,
                        conditional->get_branch1_table_name().c_str(),
                        key,
                        action);
    }
    {
        MatchKey key = conditional->get_branch2_match();
        ActionData action = conditional->get_branch2_action();

        this->add_entry(dev_id,
                        conditional->get_branch2_table_name().c_str(),
                        key,
                        action);
    }
    {
        MatchKey key = conditional->get_branch3_match();
        ActionData action = conditional->get_branch3_action();

        this->add_entry(dev_id,
                        conditional->get_branch3_table_name().c_str(),
                        key,
                        action);
    }
}
