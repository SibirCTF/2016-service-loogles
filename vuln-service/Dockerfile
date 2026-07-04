FROM debian:jessie
MAINTAINER Evgenii Sopov <mrseakg@gmail.com>

RUN apt-get update
RUN apt-get upgrade

RUN echo "" >> /etc/apt/sources.list
RUN echo "deb ftp://ftp.ru.debian.org/debian/ testing main contrib non-free" >> /etc/apt/sources.list
RUN echo "deb-src ftp://ftp.ru.debian.org/debian/ testing main contrib non-free" >> /etc/apt/sources.list

RUN apt-get update
RUN apt-get install -y nano make
RUN apt-get install -y -t testing gcc g++ qt5-default

# debconf: delaying package configuration, since apt-utils is not installed

RUN mkdir /root/loogles/
RUN mkdir /root/loogles/src
RUN mkdir /root/loogles/www

COPY loogles.pro /root/loogles/
COPY src /root/loogles/src
COPY www /root/loogles/www

RUN cd /root/loogles/ && qmake
RUN cd /root/loogles/ && make

# web

EXPOSE 8087

# ssh
# EXPOSE 8088

# websocket
# EXPOSE 9087

CMD cd /root/loogles/ && ./loogles

