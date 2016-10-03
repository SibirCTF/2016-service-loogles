# Loogles

Emulation similar search system

## Requirements

* Docker
* Qt5 (c++11)

## Run in Docker

### Build Docker Image With Juri

	$ cd service
	$ sudo docker build --rm=true -t "sibirctf2016:loogles" ./

### Check created images? Are you see it?

	$ sudo docker images

### Check create images? Are you see it?

	$ sudo docker rmi "sibirctf2016:loogles"

### Docker run

	$ sudo docker run -t --name=loogles "sibirctf2016:loogles"
