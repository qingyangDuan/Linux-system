#!/bin/bash

URL="https://10.108.255.249/include/auth_action.php"
username=16307130154
echo "password: "
read -s password
echo "this computer ip: "
read ip

curl $URL --insecure --data "action=login&username=$username&password=$password&ac_id=1&user_ip=$ip&nas_ip=&user_mac=&save_me=1&ajax=1" 
