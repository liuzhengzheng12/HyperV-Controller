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

#ifndef CONTROLLER_HYPERV_APP_H
#define CONTROLLER_HYPERV_APP_H


#include "../hyperv_controller.h"

#include "../hyperv_control.h"


class hyperv_app {
protected:
    uint16_t vdp_id;
    uint8_t inst_id;
    uint8_t start_stage_id;
    uint8_t next_stage;
    uint8_t next_stage_match;
    uint8_t next_inst;
    pi_dev_id_t dev_id;
    hyperv_controller * controller;
public:
    hyperv_app(pi_dev_id_t dev_id,
               uint16_t vdp_id,
               uint8_t inst_id,
               uint8_t start_stage_id,
               hyperv_controller * controller) {
        this->controller = controller;
        this->inst_id = inst_id;
        this->dev_id = dev_id;
        this->vdp_id = vdp_id;
        this->start_stage_id = start_stage_id;
        this->next_inst = END_INST;
        this->next_stage = END_STAGE;
        this->next_stage_match = 0;

        if (this->inst_id == 0) {
            this->inst_id = controller->allocate_inst();
        }

    }


    hyperv_app & operator>>(hyperv_app & app) {
        this->set_next_inst(app);
        return app;
    }

    /**
     *
     * @param start_stage_id
     */
    void set_start_stage(uint8_t start_stage_id) {
        this->start_stage_id = start_stage_id;
    }

    /**
     *
     * @return
     */
    uint8_t get_inst_id() {
        return inst_id;
    }

    /**
     *
     * @return
     */
    uint8_t get_start_stage() {
        return start_stage_id;
    }



    /**
     *
     * @param next_inst
     * @param next_stage
     * @param next_stage_match
     */
    void set_next_inst(uint8_t next_inst,
                       uint8_t next_stage,
                       uint8_t next_stage_match) {
        this->next_stage = next_stage;
        this->next_stage_match = next_stage_match;
        this->next_inst = next_inst;
    }

    /**
     * Set the next Instance
     * @param app HyperV Application Instance
     */
    void set_next_inst(hyperv_app & app) {
        this->next_stage = app.start_stage_id;
        this->next_stage_match = app.get_match_bitmap();
        this->next_inst = app.inst_id;
    }
    /**
     *
     * @return
     */
    virtual uint8_t get_stage_num() = 0;

    /**
     *
     * @return
     */
    virtual uint8_t get_match_bitmap() = 0;

    /**
     *
     */
    virtual void activate(bool init, bool special) = 0;

    /**
     *
     */
    virtual void deactivate() = 0;
};


#endif //CONTROLLER_HYPERV_APP_H
