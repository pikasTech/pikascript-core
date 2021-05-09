DOCKER_NAME=mimiLib

docker rmi -f $DOCKER_NAME 
docker rm -f $DOCKER_NAME
docker build -t $DOCKER_NAME . 


docker run -itd \
	--name $DOCKER_NAME \
	-v $PWD/../:/root/dev  \
	-v /tmp/.X11-unix:/tmp/.X11-unix \
	-e DISPLAY \
	--restart=always \
	$DOCKER_NAME \
	bash
