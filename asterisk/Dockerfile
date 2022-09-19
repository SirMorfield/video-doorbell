FROM ubuntu:latest

# Ubuntu spesific timezone shenaningans
ENV TZ=Europe/Amsterdam
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone


RUN apt-get update -y -qq
RUN apt-get install -y asterisk

# print asterisk version
# Or run: `core show version` in the asterisk cli
# should be 18.10
RUN asterisk -V

CMD asterisk -vvvv -c


# docker build -t asterisk . && \
# docker run -it --net=host --restart unless-stopped --name asterisk \
# -v $PWD/sip.conf:/etc/asterisk/sip.conf \
# -v $PWD/extensions.conf:/etc/asterisk/extensions.conf \
# asterisk
