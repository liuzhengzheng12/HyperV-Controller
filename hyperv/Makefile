SOURCE_FILES= main.cpp \
              hyperv_utils.cpp \
              hyperv_control.cpp \
              hyperv_conditional.cpp \
              hyperv_controller.cpp \
              hyperv_match.cpp \
              hyperv_action.cpp \
              hyperv_helper.cpp \
              hyperv.cpp \
              apps/router/hyperv_router.cpp \
              apps/firewall/hyperv_firewall.cpp \
              apps/switch/hyperv_switch.cpp \
              apps/hitter/hyperv_hitter.cpp \
              apps/ipsg/hyperv_ipsg.cpp \
              apps/learning/hyperv_learning.cpp \
              apps/multicast/hyperv_multicast.cpp \
              apps/qos/hyperv_qos.cpp \


LIBS=    -lpi \
         -lpip4info \
         -lpifecpp \
         -lpi_bmv2 -lthrift -lruntimestubs -lsimpleswitch_thrift \
         -lboost_system

compile:
	@g++ -o controller $(SOURCE_FILES) $(LIBS) -std=c++11 -static-libstdc++