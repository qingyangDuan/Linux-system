# check if nic is shown in lspci and  lshw
lspci |grep Eth
lshw -C network 

# install MLNX OFED driver  
- download driver from https://network.nvidia.com/products/infiniband-drivers/linux/mlnx_ofed/  
- tar and install   
安装 MLNX_OFED 4.9-x LTS 版即可，这一版本可支持connectX3 - connectX6的很多网卡。最新版本OFED容易出错，不建议安装。


# install mft tool
- download driver from https://network.nvidia.com/products/adapter-software/firmware-tools/   
这个管理工具可修改connectX5 网卡的模式，即 IB模式 或 Ethernet 模式。    
修改方法： https://mymellanox.force.com/mellanoxcommunity/s/article/getting-started-with-connectx-5-100gb-s-adapter-for-windows
