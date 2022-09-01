FROM ubuntu:20.04

# Ubuntu spesific timezone shenaningans
ENV TZ=Europe/Amsterdam
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone


RUN apt-get update -y -qq
RUN apt-get install -y asterisk

# print asterisk version
RUN asterisk -V

CMD asterisk -vvvv -c


# docker build -t asterisk . && \
# docker run -it --net=host \
# -v $PWD/sip_server/etc/asterisk/sip.conf:/etc/asterisk/sip.conf \
# -v $PWD/sip_server/etc/asterisk/extentions.conf:/etc/asterisk/extentions.conf \
# -v $PWD/sip_server/etc/asterisk/voicemail.conf:/etc/asterisk/voicemail.conf \
# asterisk
