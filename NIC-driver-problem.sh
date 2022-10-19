# check if nic is shown in lspci and  lshw
lspci |grep Eth
lshw -C network 

# install MLNX OFED driver  
download driver from https://network.nvidia.com/products/infiniband-drivers/linux/mlnx_ofed/  
安装 MLNX_OFED 4.9-x LTS 版即可，这一版本可支持connectX3 - connectX6的很多网卡。最新版本OFED容易出错，不建议安装。
tar and install
