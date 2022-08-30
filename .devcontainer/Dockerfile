FROM ubuntu:22.04

ARG UTLN
ARG LOCAL_FOLDER
ARG COURSE_NUM

RUN apt-get update -y && \
    apt-get upgrade -y && \
    apt-get install -y \
            lsb-release build-essential software-properties-common \
            wget git ssh sudo time \                                        
            clang-12 clang-format valgrind \
            python3-pip jq

# install opengl
RUN apt-get install -y libglu1-mesa-dev freeglut3-dev mesa-common-dev libglew-dev

# install python packages
RUN python3 -m pip install --upgrade pip
RUN python3 -m pip install toml dataclasses tqdm filelock python_dateutil

# use clang++
RUN ln -s /usr/bin/clang++-12 /usr/bin/clang++

RUN adduser --disabled-password --gecos "" ${UTLN} 
RUN usermod -aG sudo ${UTLN}

RUN printf "${utln}  ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers
RUN printf "export PS1=\"\\u@${COURSE_NUM}-local:\\W\\$ \"\n" >> /home/${UTLN}/.bashrc
RUN printf "export UTLN=${UTLN}\n" >> /home/${UTLN}/.bashrc
RUN printf "export PATH=\${PATH}:/workspaces/${LOCAL_FOLDER}/.devcontainer/scripts\n" >> /home/${UTLN}/.bashrc
