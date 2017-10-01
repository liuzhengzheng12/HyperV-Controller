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
#include "apps/switch/hyperv_switch.h"
#include "apps/router/hyperv_router.h"
#include "apps/firewall/hyperv_firewall.h"
#include "apps/learning/hyperv_learning.h"
#include "apps/qos/hyperv_qos.h"
#include "apps/ipsg/hyperv_ipsg.h"
#include "apps/hitter/hyperv_hitter.h"

namespace hyperv {
    /**
     *
     * @param controller
     */
    void policy1(hyperv_controller * controller) {
        controller->assign_device((char *) "9090");
        controller->set_default_entries();

        hyperv_switch s(0,
                        1, // vdp-id
                        1, // inst-id
                        1, // start-stage-id
                        controller);
        s.activate(true, false);
        s.install(0x0001,
                  1);
        s.install(0x0002,
                  2);

    }

    /**
     *
     * @param controller
     */
    void policy2(hyperv_controller * controller) {
        uint8_t stage_id = CONST_STAGE_1;
        controller->assign_device((char *) "9090");
        controller->set_default_entries();

        hyperv_learning learning(0,
                                 1,
                                 0,
                                 stage_id,
                                 controller);

        stage_id += learning.get_stage_num();

        hyperv_switch s(0,
                        1,
                        0,
                        stage_id,
                        controller);

        learning.set_next_inst(s);

        // Learning
        {
            learning.activate(true, false);
            learning.install(0x802200010001);
            learning.install(0x802200010002);
        }
        {
            s.activate(false, false);
            s.install(0x0001, 1);
            s.install(0x0002, 2);
        }

    }

    /**
     *
     * @param controller
     */
    void policy3(hyperv_controller * controller) {
        uint8_t stage_id = CONST_STAGE_1;
        controller->assign_device((char *) "9090");
        controller->set_default_entries();

        hyperv_learning learning(0,
                                 1,
                                 0,
                                 stage_id,
                                 controller);

        stage_id += learning.get_stage_num();

        hyperv_switch l2sw(0,
                           1,
                           0,
                           stage_id,
                           controller);

        learning.set_next_inst(l2sw);

        stage_id += l2sw.get_stage_num();

        hyperv_qos qos(0,
                       1,
                       0,
                       stage_id,
                       controller);

        l2sw.set_next_inst(qos);

        // Learning
        {
            learning.activate(true, false);
            learning.install(0x802200010001);
            learning.install(0x802200010002);
        }
        {
            l2sw.activate(false, false);
            l2sw.install(0x0001, 1);
            l2sw.install(0x0002, 2);
        }
        {
            qos.activate(false, false);
            qos.install(0x0a000001, 0x0a000002, 1);
            qos.install(0x0a000002, 0x0a000001, 1);
        }
    }

    /**
     *
     * @param controller
     */
    void policy4(hyperv_controller * controller) {
        uint8_t stage_id = CONST_STAGE_1;
        controller->assign_device((char *) "9090");
        controller->set_default_entries();

        hyperv_router router(0,
                             1,
                             0,
                             stage_id,
                             controller);

        {
            router.activate(true, false);
            router.install(0x0a000001, 0x0001, 0x802200010002, 1);
            router.install(0x0a000002, 0x0002, 0x802200010001, 2);
        }
    }

    /**
     *
     * @param controller
     */
    void policy5(hyperv_controller * controller) {
        uint8_t stage_id = CONST_STAGE_1;
        controller->assign_device((char *) "9090");
        controller->set_default_entries();

        hyperv_router router(0, 1, 0, stage_id, controller);

        stage_id += router.get_stage_num();

        hyperv_firewall firewall(0,
                                 1,
                                 0,
                                 stage_id,
                                 controller);

        router.set_next_inst(firewall);

        {
            router.activate(true, false);
            router.install(0x0a000001,
                           0x0001,
                           0x802200010002,
                           1);
            router.install(0x0a000002,
                           0x0002,
                           0x802200010001,
                           2);
        }
        {
            firewall.activate(false, false);
            firewall.install_white_list(0x0a000001,
                                        0x0a000002,
                                        10);
            firewall.install_white_list(0x0a000002,
                                        0x0a000001,
                                        10);
        }
    }

    void policy6(hyperv_controller * controller) {
        uint8_t stage_id = CONST_STAGE_1;
        controller->assign_device((char *) "9090");
        controller->set_default_entries();

        hyperv_router router(0,
                             1,
                             0,
                             stage_id,
                             controller);

        stage_id += router.get_stage_num();

        hyperv_ipsg ipsg(0,
                         1,
                         0,
                         stage_id,
                         controller);

        stage_id += ipsg.get_stage_num();

        hyperv_firewall firewall(0,
                                 1,
                                 0,
                                 stage_id,
                                 controller);

        router.set_next_inst(ipsg);
        ipsg.set_next_inst(firewall);


        {
            router.activate(true, false);
            router.install(0x0a000001,
                           0x0001,
                           0x802200010002,
                           1);
            router.install(0x0a000002,
                           0x0002,
                           0x802200010001,
                           2);
        }

        {
            ipsg.activate(false, false);
            ipsg.install(0x0a000001, 1);
            ipsg.install(0x0a000002, 2);
        }

        {
            firewall.activate(false, false);
            firewall.install_white_list(0x0a000001,
                                        0x0a0000002,
                                        10);
            firewall.install_white_list(0x0a000002,
                                        0x0a000001,
                                        10);
        }
    }

    void policy7(hyperv_controller * controller) {
        uint8_t stage_id = CONST_STAGE_1;
        controller->assign_device((char *) "9090");
        controller->set_default_entries();

        hyperv_learning learning(0,
                                 1,
                                 0,
                                 stage_id,
                                 controller);

        stage_id += learning.get_stage_num();

        hyperv_router router(0,
                             1,
                             0,
                             stage_id,
                             controller);

        stage_id += router.get_stage_num();

        hyperv_ipsg ipsg(0,
                         1,
                         0,
                         stage_id,
                         controller);

        stage_id += ipsg.get_stage_num();

        hyperv_firewall firewall(0,
                                 1,
                                 0,
                                 stage_id,
                                 controller);

        stage_id += firewall.get_stage_num();

        hyperv_qos qos(0,
                       1,
                       0,
                       stage_id,
                       controller);

        learning>>router>>ipsg>>firewall>>qos;

        {
            learning.activate(true, false);
            learning.install(0x802200010001);
            learning.install(0x802200010002);
        }
        {
            router.activate(false, false);
            router.install(0x0a000001,
                           0x0001,
                           0x802200010002,
                           1);
            router.install(0x0a000002,
                           0x0002,
                           0x802200010001,
                           2);
        }
        {
            ipsg.activate(false, false);
            ipsg.install(0x0a000001, 1);
            ipsg.install(0x0a000002, 2);
        }
        {
            firewall.activate(false, false);
            firewall.install_white_list(0x0a000001,
                                        0x0a000002,
                                        10);
            firewall.install_white_list(0x0a000002,
                                        0x0a000001,
                                        10);
        }
        {
            qos.activate(false, false);
            qos.install(0x0a000001, 0x0a000002, 1);
            qos.install(0x0a000002, 0x0a000001, 1);
        }
    }

    uint16_t get_eth_dst(int dst) {
        return uint16_t(dst);
    }

    uint64_t get_eth_src(int src) {
        auto x  = 0x802200000000ull;
        x |= src;
        x |= (src<<16);
        return x;
    }

    uint32_t get_ip(int src) {
        return uint32_t (src|0x0a000000);
    }



    void intra_pod_edge(int src,
                        int dst,
                        hyperv_controller * controller) {
        auto vdp_id = (uint8_t) dst;

        { // SRC_EDGE
            auto dev_id = (pi_dev_id_t)(src - 1);
            auto stage_id = (uint8_t) CONST_STAGE_1;

            hyperv_learning learning(dev_id,
                                     vdp_id,
                                     0,
                                     stage_id,
                                     controller);

            stage_id += learning.get_stage_num();

            hyperv_switch l2sw(dev_id,
                               vdp_id,
                               0,
                               stage_id,
                               controller);

            stage_id += l2sw.get_stage_num();

            hyperv_ipsg ipsg(dev_id,
                             vdp_id,
                             0,
                             stage_id,
                             controller);

            stage_id += ipsg.get_stage_num();

            hyperv_qos qos(dev_id,
                           vdp_id,
                           0,
                           stage_id,
                           controller);

            learning>>l2sw>>ipsg>>qos;

            // Learning
            {
                learning.activate(true, false);
                learning.install(get_eth_src(src));
            }
            {
                l2sw.activate(false, false);
                l2sw.install(get_eth_dst(dst),
                             1);
            }
            {
                ipsg.activate(false, false);
                ipsg.install(get_ip(src),
                             3);
            }
            {
                qos.activate(false, false);
                qos.install(get_ip(src),
                            get_ip(dst), 1);
            }
        }
        { // DST_EDGE
            auto dev_id = (pi_dev_id_t)(dst - 1);
            auto stage_id = (uint8_t) CONST_STAGE_1;

            hyperv_learning learning(dev_id,
                                     vdp_id,
                                     0,
                                     stage_id,
                                     controller);

            stage_id += learning.get_stage_num();

            hyperv_switch l2sw(dev_id,
                               vdp_id,
                               0,
                               stage_id,
                               controller);

            stage_id += l2sw.get_stage_num();

            hyperv_qos qos(dev_id,
                           vdp_id,
                           0,
                           stage_id,
                           controller);

            learning>>l2sw>>qos;

            // Learning
            {
                learning.activate(true, false);
                learning.install(get_eth_src(src));
            }
            {
                l2sw.activate(false, false);
                l2sw.install(get_eth_dst(dst), 3);
            }
            {
                qos.activate(false, false);
                qos.install(get_ip(src),
                            get_ip(dst), 1);
            }
        }
    }

    void intra_pod_aggregate(int src, int dst, hyperv_controller * controller) {

        auto dev_id = controller->get_dev_id(src + 3);
        auto vdp_id = (uint8_t) dst;
        auto stage_id = (uint8_t) CONST_STAGE_1;

        hyperv_learning learning(dev_id,
                                 vdp_id,
                                 0,
                                 stage_id,
                                 controller);

        stage_id += learning.get_stage_num();

        hyperv_switch l2sw(dev_id,
                           vdp_id,
                           0,
                           stage_id,
                           controller);

        stage_id += l2sw.get_stage_num();

        hyperv_qos qos(dev_id,
                       vdp_id,
                       0,
                       stage_id,
                       controller);

        learning>>l2sw>>qos;

        // Learning
        {
            learning.activate(true, false);
            learning.install(get_eth_src(src));
        }
        {
            l2sw.activate(false, false);
            l2sw.install(get_eth_dst(dst),
                         2);
        }
        {
            qos.activate(false, false);
            qos.install(get_ip(src),
                        get_ip(dst),
                        1);
        }
    }

    void intra_pod(int src, int dst, hyperv_controller * controller) {
        intra_pod_edge(src, dst, controller);
        intra_pod_aggregate(src, dst, controller);
    }

    void inter_pod_edge(int src,
                        int dst,
                        hyperv_controller * controller) {
        auto vdp_id = (uint8_t) (dst + 16);
        { // SRC-EDGE
            auto dev_id = (pi_dev_id_t)(src - 1);
            auto stage_id = (uint8_t) CONST_STAGE_1;

            hyperv_switch l2sw(dev_id,
                               vdp_id,
                               0,
                               stage_id,
                               controller);

            stage_id += l2sw.get_stage_num();

            hyperv_ipsg ipsg(dev_id,
                             vdp_id,
                             0,
                             stage_id,
                             controller);


            l2sw>>ipsg;

            {
                l2sw.activate(true, false);
                l2sw.install(get_eth_dst(dst),
                             1);
            }
            {
                ipsg.activate(false, false);
                ipsg.install(get_ip(src),
                             3);
            }

        }

        { //DST-EDGE
            auto dev_id = (pi_dev_id_t)(dst - 1);
            auto stage_id = (uint8_t) CONST_STAGE_1;

            hyperv_switch l2sw(dev_id,
                               vdp_id,
                               0,
                               stage_id,
                               controller);
            // Learning
            {
                l2sw.activate(true, true);
                l2sw.install(get_eth_dst(dst),
                             3);
            }
        }

    }
    void inter_pod_aggregate(int src,
                             int dst,
                             hyperv_controller * controller) {
        auto vdp_id = (uint8_t) (dst + 16);
        { // SRC-AGG
            auto dev_id = (pi_dev_id_t)(src + 3);
            auto stage_id = (uint8_t) CONST_STAGE_1;

            hyperv_switch l2sw(dev_id,
                               vdp_id,
                               0,
                               stage_id,
                               controller);
            // Learning
            {
                l2sw.activate(true, false);
                l2sw.install(get_eth_dst(dst),
                             3);
            }
        }
        { // DST_AGG
            {
                auto dev_id = (pi_dev_id_t) (dst + 3);
                auto stage_id = (uint8_t) CONST_STAGE_1;

                hyperv_switch l2sw(dev_id,
                                   vdp_id,
                                   0,
                                   stage_id,
                                   controller);
                // Learning
                {
                    l2sw.activate(true, false);
                    l2sw.install(get_eth_dst(dst), 1);
                }
            }

            {
                if (dst % 2 == 1) {
                    auto dev_id = (pi_dev_id_t) (dst + 4);
                    auto stage_id = (uint8_t) CONST_STAGE_1;

                    hyperv_switch l2sw(dev_id,
                                       vdp_id,
                                       0,
                                       stage_id,
                                       controller);
                    // Learning
                    {
                        l2sw.activate(true, false);
                        l2sw.install(get_eth_dst(dst), 2);
                    }
                } else {
                    auto dev_id = (pi_dev_id_t) (dst + 2);
                    auto stage_id = (uint8_t) CONST_STAGE_1;

                    hyperv_switch l2sw(dev_id,
                                       vdp_id,
                                       0,
                                       stage_id,
                                       controller);
                    // Learning
                    {
                        l2sw.activate(true, false);
                        l2sw.install(get_eth_dst(dst), 2);
                    }
                }

            }


        }
    }
    void inter_pod_core(int src,
                        int dst,
                        hyperv_controller * controller) {
        auto dev_id = pi_dev_id_t(8);
        auto port = uint16_t(dst > src ? 2 : 1);
        for (; dev_id < 10; dev_id ++) {

            auto vdp_id = (uint8_t) (dst + 16);

            uint8_t stage_id = CONST_STAGE_1;

            hyperv_router router(dev_id,
                                 vdp_id,
                                 0,
                                 stage_id,
                                 controller);

            stage_id += router.get_stage_num();

            hyperv_firewall firewall(dev_id,
                                     vdp_id,
                                     0,
                                     stage_id,
                                     controller);

            stage_id += firewall.get_stage_num();


            hyperv_hitter hitter(dev_id,
                                 vdp_id,
                                 0,
                                 stage_id,
                                 controller);
            stage_id += hitter.get_stage_num();

            hyperv_qos qos(dev_id,
                           vdp_id,
                           0,
                           stage_id,
                           controller);

            router >> firewall >> hitter >> qos;

            {
                router.activate(true, false);
                router.install(get_ip(dst),
                               get_eth_dst(dst),
                               get_eth_src(src),
                               port);
            }
            {
                firewall.activate(false, false);
                firewall.install_white_list(get_ip(src),
                                            get_ip(dst),
                                            10);
            }
            {
                hitter.activate(false, false);
                hitter.install(get_ip(src),
                               get_ip(dst));
            }

            {
                qos.activate(false, false);
                qos.install(get_ip(src),
                            get_ip(dst), 1);
            }
        }
    }

    void inter_pod(int src,
                   int dst,
                   hyperv_controller * controller) {
        inter_pod_edge(src, dst, controller);
        inter_pod_aggregate(src, dst, controller);
        inter_pod_core(src, dst, controller);
    }


    void fat_tree(hyperv_controller * controller) {
        controller->assign_device((char *) "9090");
        controller->assign_device((char *) "9091");
        controller->assign_device((char *) "9092");
        controller->assign_device((char *) "9093");
        controller->assign_device((char *) "9094");
        controller->assign_device((char *) "9095");
        controller->assign_device((char *) "9096");
        controller->assign_device((char *) "9097");
        controller->assign_device((char *) "9098");
        controller->assign_device((char *) "9099");
        controller->set_default_entries();
        { // INTRA-POD
            intra_pod(1, 2, controller);
            intra_pod(2, 1, controller);
            intra_pod(3, 4, controller);
            intra_pod(4, 3, controller);
        }
        {
            inter_pod(1, 3, controller);
            inter_pod(1, 4, controller);
            inter_pod(2, 3, controller);
            inter_pod(2, 4, controller);
            inter_pod(3, 1, controller);
            inter_pod(3, 2, controller);
            inter_pod(4, 1, controller);
            inter_pod(4, 2, controller);
        }
    }

}  // namespace

int main(int argc, char *argv[]) {

    pi_init(256, nullptr);  // 256 max devices
    pi_p4info_t *p4info;
    pi_add_config_from_file("../build/hyperv.json",
                            PI_CONFIG_TYPE_BMV2_JSON,
                            &p4info);


    hyperv_controller controller(p4info);
    hyperv::fat_tree(&controller);

}
