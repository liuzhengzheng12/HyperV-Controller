# HyperV

A high performance hypervisor for fully virtualizing the programmable data plane

## Prerequisites

### install Bmv2

```
git clone https://github.com/p4lang/behavioral-model.git
./install_deps.sh
./autogen.sh
./configure
make 
sudo make install
```

### install PI

```
sudo apt-get install libreadline-dev libjudy-dev
git clone https://github.com/p4lang/PI.git
git submodule update --init --recursive
./autogen.sh 
./configure --with-bmv2 --with-fe-cpp
make
sudo make install
sudo ldconfig
```

## Compile

### compile P4 program

```
cd /root/path/of/project/
make compile
```
P4 configuration of both hyperv and hyper4 will be built.

### compile controller

```
cd controller
make compile
```
Controllers of both hyperv and hyper4 will be built.


## Test Policies

* set up network topology

```
[command] # for policy 1 ~ 7
[command] # for policy fat-tree
```

* start Bmv2 target

```
[command] # for policy 1 ~ 7
[command] # for policy fat-tree
```

* populate match-action tables with controller

```
cd controller
make 4 # for hyper4 controller
make v # for hyperv controller
```
