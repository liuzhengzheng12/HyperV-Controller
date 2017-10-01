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

#ifndef CONTROLLER_HYPERV_HITTER_H
#define CONTROLLER_HYPERV_HITTER_H


#include "../hyperv_app.h"

class hyperv_hitter : public hyperv_app {
private:
    uint32_t counter_index = 0;

protected:
    uint32_t allocate_index() {
        return counter_index++;
    }
public:
    hyperv_hitter(pi_dev_id_t dev_id,
                  uint16_t vdp_id,
                  uint8_t inst_id,
                  uint8_t start_stage_id,
                  hyperv_controller * controller):
                  hyperv_app(dev_id,
                             vdp_id,
                             inst_id,
                             start_stage_id,
                             controller) {

    }

    /**
     *
     * @param src_ip
     * @param dst_ip
     */
    void install(uint32_t src_ip,
                 uint32_t dst_ip);

    uint8_t get_stage_num() override  {
        return 2;
    }

    uint8_t get_match_bitmap() override {
        return BIT_MASK_HEADER;
    }

    void activate(bool init, bool special) override ;
    void deactivate() override ;

};


#endif //CONTROLLER_HYPERV_HITTER_H
