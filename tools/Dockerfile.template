FROM ubuntu:16.04
COPY ./install_realsense_pkgs.sh /root
ENV docker_user="<your_username>"
ENV git_user_name="<your_git_name>"
ENV git_user_email="<your_git_email>"

RUN apt-get update \
 && apt-get install -y udev sudo \
 && apt-get install -y nodejs npm git \
 && apt-get install -y netcat-openbsd expect \
 && ln -s /usr/bin/nodejs /usr/bin/node \
 && adduser --disabled-password --gecos "" ${docker_user} \
 && adduser ${docker_user} sudo \
 && echo "%sudo ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers.d/nopass \
 && echo "Defaults  env_keep += \"http_proxy https_proxy no_proxy\"" > /etc/sudoers.d/env_keep \
 && sudo -H -u ${docker_user} echo "[user]" > /home/${docker_user}/.gitconfig \
 && sudo -H -u ${docker_user} echo "name=${git_user_name}" >> /home/${docker_user}/.gitconfig \
 && sudo -H -u ${docker_user} echo "email=${git_user_email}" >> /home/${docker_user}/.gitconfig \
 && bash /root/install_realsense_pkgs.sh
