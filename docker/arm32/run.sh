# Arm32

sudo docker build --platform linux/arm/v7 -t webserver_arm32 -f docker/arm32/Dockerfile .

sudo docker run -it --rm --platform linux/arm/v7 --user root --name WebServer -p 1616:1616 webserver_arm32
# sudo docker run -it --rm --platform linux/arm/v7 --user myuser --name WebServer -p 1616:1616 webserver_arm32