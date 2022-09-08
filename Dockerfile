# FROM ubuntu:20.04
FROM ubuntu:latest

# Ubuntu spesific timezone shenaningans
ENV TZ=Europe/Amsterdam
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone


RUN apt-get update -y -qq
RUN apt-get install -y asterisk

# print asterisk version
# Or run: `core show version` in the asterisk cli
RUN asterisk -V

CMD asterisk -vvvv -c


# docker build -t asterisk . && \
# docker run -it --rm --net=host --name asterisk \
# -v $PWD/sip_server/etc/asterisk/sip.conf:/etc/asterisk/sip.conf \
# -v $PWD/sip_server/etc/asterisk/extensions.conf:/etc/asterisk/extensions.conf \
# -v $PWD/sip_server/etc/asterisk/voicemail.conf:/etc/asterisk/voicemail.conf \
# asterisk
