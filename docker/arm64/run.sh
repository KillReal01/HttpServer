# Arm64

sudo docker build --platform linux/arm64 -t webserver_arm64 -f docker/arm64/Dockerfile .

sudo docker run -it --rm --platform linux/arm64 --user root --name WebServer -p 1616:1616 webserver_arm64
# sudo docker run -it --rm --platform linux/arm64 --user myuser --name WebServer -p 1616:1616 webserver_arm64

