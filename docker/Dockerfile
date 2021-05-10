FROM ubuntu:20.04

USER root
ADD sources.list /etc/apt/



#----------------------->START system tools
RUN apt-get update && apt-get install -y curl
RUN apt-get install -y sudo
RUN DEBIAN_FRONTEND=noninteractive apt install -y tzdata
ENV LANG C.UTF-8
RUN sudo apt-get update && sudo apt-get install -y vim wget
RUN sudo apt-get install -y git
RUN sudo apt install -y openssh-server
RUN sudo apt install -y net-tools
RUN sudo apt install -y build-essential gdb
RUN sudo apt install -y cmake
RUN sudo apt install -y ranger
RUN cp /etc/skel/.bashrc ~/ # enable the colors for root
#----------------------->END system tools






# ---------------------> START support chinese
USER root
RUN sudo apt-get -y install language-pack-zh-hans
RUN sudo echo "LANG=\"zh_CN.UTF-8\"">>/etc/environment
RUN sudo echo "LANGUAGE="zh_CN:zh:en_US:en"">>/etc/environment
RUN sudo echo "en_US.UTF-8 UTF-8">>/var/lib/locales/supported.d/local
RUN sudo echo "zh_CN.UTF-8 UTF-8">>/var/lib/locales/supported.d/local
RUN sudo echo "zh_CN.GBK GBK">>/var/lib/locales/supported.d/local
RUN sudo echo "zh_CN GB2312">>/var/lib/locales/supported.d/local
RUN sudo locale-gen
RUN sudo apt-get -y install fonts-droid-fallback ttf-wqy-zenhei ttf-wqy-microhei fonts-arphic-ukai fonts-arphic-uming
# -------->END support chinede




#----------------------->START git config
RUN git config --global user.email "public518@public518.com"
RUN git config --global user.name "public518"
#------------------------->END git config




# ------------------------------------->START user setup
WORKDIR /root/dev
# --------------------------------------->END user setup
