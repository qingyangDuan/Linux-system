



# Ubuntu 保姆级部署 k8s

本文主要对ubuntu 18.04 版本进行kubernetes 19.02 和docker 19.04 进行部署，其中遇到了很多坑，参考此文的同学可以避免一些坑但是不能保证完全避免

## 系统安装

在vmware 16 上直接安装Ubuntu 18.04，过程略

注意在vmware软件安装ubuntu 18.04之后，需要安装vmware的tool，方便将其他教程的代码直接复制到虚拟机中进行配置

### VMware tool 安装教程

首先在虚拟机上方菜单栏点击虚拟机 ，安装tool![tool安装1](https://img-blog.csdn.net/20181011113334928?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VuZ2VybGE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

在磁盘中会显示vmvare的压缩包

![在这里插入图片描述](https://img-blog.csdn.net/20181011113715152?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VuZ2VybGE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

*注意不要在磁盘里面解压，先将压缩包发送到另外一个文件夹再解压*

可以用命令解压

```bash
tar -zvxf VMwareToolsxxxx.gz 
```

也可以直接右键解压

cd 到解压指定的文件夹，运行`.pl`文件

可以使用代码运行

```bash
./xxxxtool.pl
```

## 系统初始化

设置root密码

```bash
xxx@xxx-machine:~$ sudo passwd root
Enter new UNIX password:  // 输入root密码
Retype new UNIX password: // 重复输入
passwd: password updated successfully
xxx@xxx-machine:~$ su - root
Password: // 输入root密码
```

### 安装curl

```bash
sudo apt-get install curl
```

### 安装vim

```bas
sudo apt-get install vim
```

### 更改net.bridge.bridge-nf-call-iptables的值为1.（Ubuntu 20.04默认为1

配置内核调优

```
cat > /etc/sysctl.conf <<EOF
vm.max_map_count=262144
net.ipv4.ip_forward = 1
net.bridge.bridge-nf-call-ip6tables = 1
net.bridge.bridge-nf-call-iptables = 1
EOF
```

配置文件生效

```bash
sysctl -p 
1  #修改Linux 资源配置文件，调高ulimit最大打开数和systemctl管理的服务文件最大打开数
```

报错：

```bash
sysctl: 无法获取/proc/sys/net/bridge/bridge-nf-call-ip6tables 的文件状态(stat): 没有那个文件或目录
```

解决：

```bash
[root@localhost ~]# modprobe br_netfilter
[root@localhost ~]# ls /proc/sys/net/bridge/
bridge-nf-call-arptables  bridge-nf-call-ip6tables  bridge-nf-call-iptables  bridge-nf-filter-pppoe-tagged  bridge-nf-filter-vlan-tagged  bridge-nf-pass-vlan-input-dev
[root@localhost ~]# sysctl -p

```

更改镜像源

*注意的是更镜像源时，重复修改镜像源可能会导致 source文件里 deb开头的代码重复出现，因此要 vim看 source文件有没有源的重复*

### 两种方式更换deb源

#### 方式一 通过桌面端直接设置

1.  打开软件更新
    
    ![image-20210517202439843](C:%5CUsers%5CFGY-yoga%5CAppData%5CRoaming%5CTypora%5Ctypora-user-images%5Cimage-20210517202439843.png)
    
2.  设置阿里源
    
    ![Ubuntu18更换国内源_03.png](https://img-blog.csdnimg.cn/20181207141357701.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2RhZXJ6ZWk=,size_16,color_FFFFFF,t_70)
    
3.  选择其他站点，选择阿里源
    
    ![Ubuntu18更换国内源_05.png](https://img-blog.csdnimg.cn/20181207141508121.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2RhZXJ6ZWk=,size_16,color_FFFFFF,t_70) ![Ubuntu18更换国内源_06.png](https://img-blog.csdnimg.cn/20181207141526433.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2RhZXJ6ZWk=,size_16,color_FFFFFF,t_70)
    
4. 更新缓存

#### 方式二 命令行设置

1.  打开源文件
    
    ```bash
    sudo vim /etc/apt/sources.list
    ```
2.  在文件最前面添加以下条目，保险起见，的操作之前做好备份
    
    ```bash
    #清华源
    deb http://mirrors.aliyun.com/ubuntu/ bionic main restricted universe multiverse
    deb http://mirrors.aliyun.com/ubuntu/ bionic-security main restricted universe multiverse
    deb http://mirrors.aliyun.com/ubuntu/ bionic-updates main restricted universe multiverse
    deb http://mirrors.aliyun.com/ubuntu/ bionic-proposed main restricted universe multiverse
    deb http://mirrors.aliyun.com/ubuntu/ bionic-backports main restricted universe multiverse
    deb-src http://mirrors.aliyun.com/ubuntu/ bionic main restricted universe multiverse
    deb-src http://mirrors.aliyun.com/ubuntu/ bionic-security main restricted universe multiverse
    deb-src http://mirrors.aliyun.com/ubuntu/ bionic-updates main restricted universe multiverse
    deb-src http://mirrors.aliyun.com/ubuntu/ bionic-proposed main restricted universe multiverse
    deb-src http://mirrors.aliyun.com/ubuntu/ bionic-backports main restricted universe multiverse
    ```
    
    *注意 bionic 代表的是 ubuntu18 的版本，其他版本需要重新改变版本编号*
    
    ```bash
    deb http://mirrors.aliyun.com/ubuntu/ bionic main restricted universe multiverse
    deb http://mirrors.aliyun.com/ubuntu/ bionic-security main restricted universe multiverse
    deb http://mirrors.aliyun.com/ubuntu/ bionic-updates main restricted universe multiverse
    deb http://mirrors.aliyun.com/ubuntu/ bionic-proposed main restricted universe multiverse
    deb http://mirrors.aliyun.com/ubuntu/ bionic-backports main restricted universe multiverse
    deb-src http://mirrors.aliyun.com/ubuntu/ bionic main restricted universe multiverse
    deb-src http://mirrors.aliyun.com/ubuntu/ bionic-security main restricted universe multiverse
    deb-src http://mirrors.aliyun.com/ubuntu/ bionic-updates main restricted universe multiverse
    deb-src http://mirrors.aliyun.com/ubuntu/ bionic-proposed main restricted universe multiverse
    deb-src http://mirrors.aliyun.com/ubuntu/ bionic-backports main restricted universe multiverse
    ```

### 禁用swap

```bash
sudo swapoff -a
sudo sed -i 's/.*swap.*/#&/' /etc/fstab
free -h
```

### 关闭防火墙

```b
systemctl stop firewalld
systemctl disable firewalld
```

**如果这里报错，说明firewalld的安装有问题，需要重新安装firewalld，然后重新运行firewalld重新关闭firewalld**

```bash
sudo apt-get install firewalld
systemctl enable firewalld
systemctl stop firewalld
systemctl disable firewalld
```

### 关闭iptables/ufw

```bash
sudo ufw disable 
sudo ufw status
service ufw disable
```

### 禁用Selinux

```bahs
sudo apt install selinux-utils
sudo setenforce 0
```

### 重启

```bash
sudo reboot
```

## Docker安装

### 卸载旧版本docker

```
sudo apt-get remove docker docker-engine docker.io containerd runc
```

### 安装 apt 依赖包，用于通过HTTPS来获取仓库

```bash
sudo apt-get install \
apt-transport-https \
ca-certificates \
curl \
gnupg-agent \
software-properties-common
```

### 添加 Docker 的官方 GPG 密钥：

```bash
curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo apt-key add -

OK
```

### 设置稳定版仓库

```bash
sudo add-apt-repository \   "deb [arch=amd64] https://download.docker.com/linux/ubuntu \  $(lsb_release -cs) \  stable"

报错

​``````bash
sudo add-apt-repository \
   "deb [arch=amd64] https://download.docker.com/linux/ubuntu \
  $(lsb_release -cs) \
  stable"
 成功
```

在安装k8s之前，对镜像进行加速

### Docker镜像加速

```bash
sudo mkdir -p /etc/docker
sudo tee /etc/docker/daemon.json <<-'EOF'
{
  "registry-mirrors": ["https://68kw1jbc.mirror.aliyuncs.com"]
}
EOF
sudo systemctl daemon-reload
sudo systemctl restart docker
```

### 安装指定版本的docker

```bash
apt-cache madison docker-ce #查看有哪些版本的docker

sudo apt-get -y install docker-ce=5:19.03.13~3-0~ubuntu-bionic
```

**注意这里安装的是19.03.13版本的dockers，该版本较为稳定，可以和19版k8s很好适配**

### Docker检测

```bash
docker run hello-world
```

### 非root用户运行docker

```bash
sudo usermod -aG docker $USER
sudo systemctl restart docker
```

### 统一docker和k8s的cgroup机制

修改docker的cgroup机制为systemd，保持docker和k8s的cgroup机制一致。

```bash
vim /etc/docker/daemon.json

写入以下内容：

​```bahs
{
"exec-opts": ["native.cgroupdriver=systemd"]
}
```

**注意json文件要修改内容，需要在中括号\[\]后面加逗号,再加需要添加的内容**

1.  systemctl restart docker
    
2.  systemctl enable docker

## Kuburnetes 安装

### 更改k8s的源

```bash
#打开apt源文件：
sudo vim /etc/apt/sources.list
#添加如下：
deb https://mirrors.aliyun.com/kubernetes/apt kubernetes-xenial main
#运行
curl https://mirrors.aliyun.com/kubernetes/apt/doc/apt-key.gpg | sudo apt-key add - 
#然后更新apt源：
sudo apt-get update
```

### 安装相关组件

```bas
apt-get update && apt-get install -y apt-transport-https
curl https://mirrors.aliyun.com/kubernetes/apt/doc/apt-key.gpg | apt-key add - 
```

### 安装k8s

```bash
sudo apt-get install -y kubelet=1.19.4-00 kubeadm=1.19.4-00 kubectl=1.19.4-00
sudo apt-mark hold kubelet kubeadm kubectl ##固定版本
```

**这里将k8s的版本进行固定，防止自动更新导致的版本错误**

### 查看版本

```ba
kubelet --version
```

### 查询需要的镜像

```bash
kubeadm config images list --kubernetes-version v1.19.4
```

### 拉取镜像

```bas
docker pull registry.cn-hangzhou.aliyuncs.com/google_containers/kube-apiserver:v1.19.4 
docker pull registry.cn-hangzhou.aliyuncs.com/google_containers/kube-controller-manager:v1.19.4 
docker pull registry.cn-hangzhou.aliyuncs.com/google_containers/kube-scheduler:v1.19.4 
docker pull registry.cn-hangzhou.aliyuncs.com/google_containers/kube-proxy:v1.19.4 
docker pull registry.cn-hangzhou.aliyuncs.com/google_containers/pause:3.2 
docker pull registry.cn-hangzhou.aliyuncs.com/google_containers/etcd:3.4.13-0 
docker pull registry.cn-hangzhou.aliyuncs.com/google_containers/coredns:1.7.0
```

**注意这里版本号要改变**

### 镜像改名

```bash
docker tag registry.cn-hangzhou.aliyuncs.com/google_containers/kube-apiserver:v1.19.4 k8s.gcr.io/kube-apiserver:v1.19.4
docker tag registry.cn-hangzhou.aliyuncs.com/google_containers/kube-controller-manager:v1.19.4 k8s.gcr.io/kube-controller-manager:v1.19.4
docker tag registry.cn-hangzhou.aliyuncs.com/google_containers/kube-scheduler:v1.19.4 k8s.gcr.io/kube-scheduler:v1.19.4
docker tag registry.cn-hangzhou.aliyuncs.com/google_containers/kube-proxy:v1.19.4 k8s.gcr.io/kube-proxy:v1.19.4
docker tag registry.cn-hangzhou.aliyuncs.com/google_containers/pause:3.2 k8s.gcr.io/pause:3.2
docker tag registry.cn-hangzhou.aliyuncs.com/google_containers/etcd:3.4.13-0 k8s.gcr.io/etcd:3.4.13-0
docker tag registry.cn-hangzhou.aliyuncs.com/google_containers/coredns:1.7.0 k8s.gcr.io/coredns:1.7.0
```

### kubeadm 初始化

镜像拉取之后，执行kubeadm init，需要指明pod网络可以使用的IP地址段，即`--pod-network-cidr`，如果安装flannel，参数为`--pod-network-cidr=10.244.0.0/16`，安装calico，参数为`--pod-network-cidr=10.10.0.0/16`

如果想直接从国内镜像仓库拉取镜像，可以增加参数`--image-repository registry.aliyuncs.com/google_containers`

`--apiserver-advertise-address` 输入的是**虚拟机自身的IP地址**

```bash
kubeadm init --image-repository=registry.aliyuncs.com/google_containers \
  --kubernetes-version=v1.19.4 \
  --pod-network-cidr=10.244.0.0/16 \
  --apiserver-advertise-address=192.168.2.133
```

### 保存最后一行有join和token相关的代码

新的节点利用这行代码加入

```bash
kubeadm join 192.168.2.133:6443 --token nfqyvg.3lub98a6ojcsilny \
    --discovery-token-ca-cert-hash sha256:50c3cfd5706220ebe097a0536399f34dfb48e4c08b53173213a0f3e23e847b2f



kubeadm token list
```
### 初始化成功之后 
```bash
Your Kubernetes control-plane has initialized successfully!

To start using your cluster, you need to run the following as a regular user:

  mkdir -p $HOME/.kube
  sudo cp -i /etc/kubernetes/admin.conf $HOME/.kube/config
  sudo chown $(id -u):$(id -g) $HOME/.kube/config

You should now deploy a pod network to the cluster.
Run "kubectl apply -f [podnetwork].yaml" with one of the options listed at:
  https://kubernetes.io/docs/concepts/cluster-administration/addons/

Then you can join any number of worker nodes by running the following on each as root:

kubeadm join 192.168.2.133:6443 --token nfqyvg.3lub98a6ojcsilny \
    --discovery-token-ca-cert-hash sha256:50c3cfd5706220ebe097a0536399f34dfb48e4c08b53173213a0f3e23e847b2f 

```
### 初始化成功后配置

```bash
mkdir -p $HOME/.kube
sudo cp -i /etc/kubernetes/admin.conf $HOME/.kube/config
sudo chown $(id -u):$(id -g) $HOME/.kube/config
```

### 检验安装成果

```bash
kubectl get nodes
```

### 检验的显示

```bash
NAME                       STATUS     ROLES    AGE     VERSION
master01-virtual-machine   NotReady   master   2m49s   v1.19.4
```

状态为notready，是因为没有对CNI进行配置，此时需要安装flannel，

```bash
kubectl apply -f https://raw.githubusercontent.com/coreos/flannel/master/Documentation/kube-flannel.yml
```

此时报错

```ba
The connection to the server raw.githubusercontent.com was refused - did you
```

### 解决方式

* 首先在网站查询ip地址

  ```bash
  http://ip.tool.chinaz.com/raw.githubusercontent.com
  ```

* 在host中修改地址

  ```bash
  vim /etc/hosts
  ```

  ![image-20210524192036242](D:\文档\我的坚果云\k8s\image-20210524192036242.png)

* 重新获取

  ```bahs
  kubectl apply -f https://raw.githubusercontent.com/coreos/flannel/master/Documentation/kube-flannel.yml
  ```

### 查看安装情况

```ba
kubectl get pods --all-namespaces
```

### 设置master也可以运行pod

```bash
kubectl taint nodes --all node-role.kubernetes.io/master-
node/zml untainted
kubectl get nodes -o wide
NAME   STATUS   ROLES    AGE   VERSION   INTERNAL-IP   EXTERNAL-IP   OS-IMAGE             KERNEL-VERSION      CONTAINER-RUNTIME
zml    Ready    master   32m   v1.16.2   192.0.4.138   <none>        Ubuntu 18.04.3 LTS   4.15.0-65-generic   docker://19.3.3
```

### kubernetes安装成功！！！！



## 部署Dashboard

```bahs
wget https://raw.githubusercontent.com/kubernetes/dashboard/v2.1.0/aio/deploy/recommended.yaml
kubectl apply -f recommended.yaml
```

### 创建service account

```bash
cat <<EOF | kubectl apply -f -
apiVersion: v1
kind: ServiceAccount
metadata:
  name: admin-user
  namespace: kubernetes-dashboard
EOF
```

### 创建ClusterRoleBinding

```bash
cat <<EOF | kubectl apply -f -
apiVersion: rbac.authorization.k8s.io/v1
kind: ClusterRoleBinding
metadata:
  name: admin-user
roleRef:
  apiGroup: rbac.authorization.k8s.io
  kind: ClusterRole
  name: cluster-admin
subjects:
- kind: ServiceAccount
  name: admin-user
  namespace: kubernetes-dashboard
EOF
```

### 获得token

```bash
kubectl -n kubernetes-dashboard get secret $(kubectl -n kubernetes-dashboard get sa/admin-user -o jsonpath="{.secrets[0].name}") -o go-template="{{.data.token | base64decode}}"
```

```bahs
kubectl -n kubernetes-dashboard describe secret $(kubectl -n kubernetes-dashboard get secret | grep admin-user | awk '{print $1}')
```

### 首先激活proxy

```bahs
kubectl proxy
```

### 在本机的浏览器上登陆

```bahs
http://localhost:8001/api/v1/namespaces/kubernetes-dashboard/services/https:kubernetes-dashboard:/proxy/
```

### token如下

```bash
eyJhbGciOiJSUzI1NiIsImtpZCI6ImhVd3RXMFB5N2ZfSF9HM0otU2ZCSlZrZTJXRTV0RDdMWEU0eVFTX2NlNTAifQ.eyJpc3MiOiJrdWJlcm5ldGVzL3NlcnZpY2VhY2NvdW50Iiwia3ViZXJuZXRlcy5pby9zZXJ2aWNlYWNjb3VudC9uYW1lc3BhY2UiOiJrdWJlcm5ldGVzLWRhc2hib2FyZCIsImt1YmVybmV0ZXMuaW8vc2VydmljZWFjY291bnQvc2VjcmV0Lm5hbWUiOiJhZG1pbi11c2VyLXRva2VuLWM5dmhjIiwia3ViZXJuZXRlcy5pby9zZXJ2aWNlYWNjb3VudC9zZXJ2aWNlLWFjY291bnQubmFtZSI6ImFkbWluLXVzZXIiLCJrdWJlcm5ldGVzLmlvL3NlcnZpY2VhY2NvdW50L3NlcnZpY2UtYWNjb3VudC51aWQiOiJkZGI4ZTlhYi00Nzc4LTQxNDMtOTUwNC1jYTUwNDJiY2YwMjYiLCJzdWIiOiJzeXN0ZW06c2VydmljZWFjY291bnQ6a3ViZXJuZXRlcy1kYXNoYm9hcmQ6YWRtaW4tdXNlciJ9.r3I-5FIPdf0ytV1bHjZAoNJBJrd2F3AdSInpVP9SZFpkTSqmqv4hVKE2jwjmnQHpJmfXTpnQ1JUVSnPErC0rltEmpPRXU80idL30x7evCMfI9qoJPuvV7_Dqy0O0dveqkYrXnyxa57bIPyUjuiSQACFvG8VPpdCYNDAWrDqjcALIGCV5wa-pThjCjfHXyHdz8BevUssyxevym8vn5gCak4triXwpti5LQncv_u-2ywDZw4s4uFuBVRLiRNlIhmD6lIQAHEFpPOtkkP9q9Ijk0HBYEVKHmm1Itk65pWfb
```



### 部署成功





- --pod-network-cidr指定pod使用的网段，设置值根据不同的网络plugin选择，本文以flannel为例设置值为10.244.0.0/16

apt-get install -y kubelet=1.18.0-00 kubeadm=1.18.0-00 kubectl=1.18.0-00

```kotlin
kubeadm init --kubernetes-version=v1.18.0 --pod-network-cidr=10.10.0.0/16 --apiserver-advertise-address=192.168.202.139--ignore-preflight-errors=Swap 
```

查看k8s可以安装的版本

```
kubeadm config images list
```