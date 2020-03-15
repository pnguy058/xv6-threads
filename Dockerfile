FROM ubuntu:latest

RUN apt-get update

RUN apt-get install -y git build-essential qemu-system-i386

ADD https://api.github.com/repos/pnguy058/xv6-threads/git/refs/heads/clone version.json

RUN cd /root && git clone https://github.com/pnguy058/xv6-threads.git -b clone

WORKDIR /root/xv6-threads

COPY ./entrypoint.sh /
RUN chmod 755 ./entrypoint.sh

CMD bash ./entrypoint.sh
