如果原ubuntu OS损坏无法启动，用USB启动进入，修改原硬盘中的系统文件或更新等。 
参考 https://help.ubuntu.com/community/LiveCdRecovery#Update_Failure   
简单步骤：
- Boot the Ubuntu using USB with a ubuntu OS in it.  
- sudo mount /dev/xxx /mnt          # /dev/xxx 是你的硬盘位置，需要挂载到一个位置（如/mnt）以便我们访问它。  
- sudo mount --bind /dev /mnt/dev   # 一些必要的操作  
- sudo mount --bind /proc /mnt/proc   
- sudo mount --bind /sys /mnt/sys  
- sudo chroot /mnt   #进入硬盘里的原系统进行操作  
- 可修改/etc/default/grub，执行 update-grub  从而使得启动时可以选择ubuntu内核版本，或执行apt update/apt upgrade 等命令。
