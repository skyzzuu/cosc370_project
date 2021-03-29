FROM skyzzuu/cosc370_hpcl_clone:latest


RUN mkdir -p /data/project/
COPY * /data/project/
RUN make -C /data/project clean
RUN make -C /data/project

RUN /data/project/project.out

