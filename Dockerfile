FROM ubuntu:latest

#this line is for update and to install the latest packages
RUN apt-get update && apt-get install -y \
Cmake\
g++ \
libzip.dev


