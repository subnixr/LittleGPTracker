# Save docker instance with 
## docker ps
## docker commit CONTAINER_ID docker_deb686
#docker run -it -v $(PWD)../../:/home/LittleGPTracker -w /home/LittleGPTracker/projects docker_deb686 /bin/bash -c "make && make PLATFORM=DEB"
#docker run -it -v $(PWD)../../:/home/LittleGPTracker \
#-w /home/LittleGPTracker/projects --user "$(id -u):$(id -g)" \
#docker_deb686 /bin/bash -c "make PLATFORM=DEB"
docker run -it -v $(PWD)../../:/home/LittleGPTracker \
-w /home/LittleGPTracker/projects --user "$(id -u):$(id -g)" \
i386/debian /bin/bash -c "make PLATFORM=DEB"