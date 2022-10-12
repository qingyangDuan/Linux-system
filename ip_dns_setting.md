命令行配置网卡连接信息。

# 方案1（旧方案）：
sudo ifconfig [ifname] up

sudo ifconfig [ifname] ip.ip.ip.ip netmask 255.255.255.0 

# 方案2：
- ubuntu18的网卡配置被NetworkManager服务管理。配置文件是在 /etc/NetworkManger/xxx中。    
- 如遇某个网卡状态为 unmanaged 问题，使用 sudo systemctl reload NetworkManager 来重置 NetworkManager 服务的配置信息。  
- 可用 NetworkManager服务提供的命令 nmcli device show / nmcli device / nmcli connection show 查看各个网卡的配置和状态。   
- 可用 sudo nmcli connection modify [ifname] ipv4.address/ipv4.gateway/ipv4.dns xx.xx.xx.xx 来配置网卡信息，  
  然后  sudo systemctl restart NetworkManager，再用 nmcli device show 查看各个网卡的配置是否更改。
 
DNS生效顺序是：   
- 1）/etc/hosts文件:  一般不在这里配置   
- 2）NetworkManager 服务为各个网卡配置的DNS：
- 3）/etc/resolv.conf : ubuntu18中这个文件不起作用，被 systemd-resolved 服务托管。  
  此服务的配置文件在 /etc/systemd/resolved.conf 中。  
  可用 sudo systemd-resolve --status 查看各个网卡的 dns 配置（上面第2项）和全局dns配置。  

 
