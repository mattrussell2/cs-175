FROM ubuntu:22.04

ARG utln

RUN apt-get update -y && \
    apt-get upgrade -y && \
    apt-get install -y \
            lsb-release build-essential software-properties-common \
            wget git ssh sudo time \                                        
            clang-12 clang-format valgrind \
            python3-pip jq

# install python packages
RUN python3 -m pip install --upgrade pip
RUN python3 -m pip install toml dataclasses tqdm filelock python_dateutil

# use clang++
RUN ln -s /usr/bin/clang++-12 /usr/bin/clang++

RUN adduser --disabled-password --gecos "" ${utln} 
RUN usermod -aG sudo ${utln}

RUN printf "${utln}  ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers
RUN printf "export PS1=\"\\u@cs-15-local:\\W\\$ \"\n" >> /home/${utln}/.bashrc
RUN printf "export utln=${utln}\n" >> /home/${utln}/.bashrc
RUN printf "export PATH=\${PATH}:/workspaces/cs-15/.devcontainer/scripts\n" >> /home/$utln/.bashrc
