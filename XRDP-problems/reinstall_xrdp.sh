   sudo apt autoremove xrdp xorg
   
   #switch to python3.6
   sudo update-alternatives --config python3
   sudo apt install python3-gi
   
   # Add the repository and update apt
   sudo add-apt-repository ppa:martinx/xrdp-hwe-18.04
   sudo apt-get update
    
   # Install new packages
   sudo apt-get install xrdp xorg
    
   # Add xrdp user to ssl-cert group and reboot
   sudo adduser xrdp ssl-cert
   sudo reboot
