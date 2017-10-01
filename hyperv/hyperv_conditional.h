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


#ifndef CONTROLLER_HYPERV_CONDITIONAL_H
#define CONTROLLER_HYPERV_CONDITIONAL_H

#include <PI/pi_base.h>
#include <PI/frontends/cpp/tables.h>
#include <string>
#include "hyperv.h"

using namespace pi;
using namespace std;

class hyperv_conditional {
private:
    uint8_t stage_id;
    uint8_t inst_id;
    pi_p4info_t * p4info;
    ActionData * action;
    ActionData * branch1_action;
    ActionData * branch2_action;
    ActionData * branch3_action;

    pi_p4_id_t table_id;
    pi_p4_id_t branch1_table_id;
    pi_p4_id_t branch2_table_id;
    pi_p4_id_t branch3_table_id;

public:
    /**
     *
     * @param p4info
     * @param stage_id
     * @param inst_id
     */
    hyperv_conditional(pi_p4info_t * p4info,
                       uint8_t stage_id,
                       uint8_t inst_id);


    string get_table_name() {
        std::ostringstream ostr;
        ostr<<"table_get_expression_stage"<<((stage_id%STAGE_NUM));
        return ostr.str();
    }

    string get_branch1_table_name() {
        std::ostringstream ostr;
        ostr<<"table_branch_1_stage"<<((stage_id%STAGE_NUM));
        return ostr.str();
    }


    string get_branch2_table_name() {
        std::ostringstream ostr;
        ostr<<"table_branch_2_stage"<<((stage_id%STAGE_NUM));
        return ostr.str();
    }


    string get_branch3_table_name() {
        std::ostringstream ostr;
        ostr<<"table_branch_3_stage"<<((stage_id%STAGE_NUM));
        return ostr.str();
    }


    /**
     *
     * @param offset
     * @param mask
     * @param r_expr
     */
    void set_expr_header_op_const(uint32_t offset,
                                  uint16_t mask,
                                  uint16_t r_expr);

    /**
     *
     * @param r_expr
     */
    void set_expr_counter_op_const(uint16_t r_expr);

    MatchKey get_match() ;
    ActionData get_action() {
        return * this->action;
    }

    /**
     *
     * @param action_id
     * @param action_bitmap
     * @param match_bitmap
     * @param next_stage
     * @param next_prog
     */
    void set_action_id(int branch_id,
                       uint64_t match_result,
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
    void set_action_id_direct(int branch_id,
                              uint64_t action_id,
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
    void set_next_stage(int branch_id,
                        uint8_t match_bitmap,
                        uint8_t next_stage,
                        uint16_t next_prog);



    /**
     *
     * @param next_prog
     */
    void end(int branch_id,
             uint8_t next_prog);


    MatchKey get_branch1_match();
    MatchKey get_branch2_match();
    MatchKey get_branch3_match();

    ActionData get_branch1_action() {
        return * this->branch1_action;
    }

    ActionData get_branch2_action() {
        return * this->branch2_action;
    }

    ActionData get_branch3_action() {
        return * this->branch3_action;
    }


};


#endif //CONTROLLER_HYPERV_CONDITIONAL_H
