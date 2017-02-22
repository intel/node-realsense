## To make a fresh new image (Long wait, Coffee Time)
```
mkdir ./tools/.docker
cp ./tools/Dockerfile.template ./tools/.docker/Dockerfile
<Customize ./tools/.docker/Dockerfile with the right ENV values>
cp ./tools/install_realsense_pkgs.sh ./tools/.docker/
docker build -t realsense ./tools/.docker
```

* If you're behind firewall, please append proxy environment after `build`
```
 --build-arg http_proxy=<your_proxy> \
 --build-arg https_proxy=<your_proxy> \
 --build-arg no_proxy=<no_proxy_list>
```

## To run container

* Launch image with interactive mode
```
docker run -ti realsense bash
```

* If you need run realsense tests, you need add host video devices by appending
```
  --device=/dev/video0:/dev/video0 \
  --device=/dev/video1:/dev/video1 \
  --device=/dev/video2:/dev/video2 \
  --device=/dev/video3:/dev/video3
```

** R200 only have three devices (video[0,1,2]), ZR300 have four devices (video[0,1,2,3])

* If you're behind firewall, please append proxy environment after `run`
```
  -e "http_proxy=<your_proxy>" \
  -e "https_proxy=<your_proxy>" \
  -e "no_proxy=<no_proxy_list>"
```

## To customize&save the image

* Do anything you want to customize the image, then
```
docker ps (to get id of running container)
docker pause <container_id>
docker commit <container_id> <your_docker_id>/realsense
docker unpause <container_id>
```

* (optional) Push repo remotely.
```
docker push <your_docker_id>/realsense
```
