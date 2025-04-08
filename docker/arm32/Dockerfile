
FROM arm32v7/ubuntu

RUN groupadd -r myuser && useradd -r -g myuser myuser

COPY ./build/arm32/WebServer /usr/local/bin/

RUN chown myuser:myuser /usr/local/bin/WebServer

USER myuser

CMD ["/usr/local/bin/WebServer"]
