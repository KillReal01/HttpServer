FROM arm64v8/ubuntu

RUN groupadd -r myuser && useradd -r -g myuser myuser

COPY ./build/arm64/WebServer /usr/local/bin/

RUN chown myuser:myuser /usr/local/bin/WebServer

USER myuser

CMD ["/usr/local/bin/WebServer"]
