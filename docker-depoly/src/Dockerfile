FROM gcc:9

RUN mkdir -p /code
RUN mkdir -p /var/log/erss
WORKDIR /code
add . /code/

ENTRYPOINT ["./run.sh"]