cd build
make
cd ..
docker build -t db_service_image -f Dockerfile .
docker run -it --name db_service_container -v /tmp:/tmp db_service_image

cd build
make
cd ..
docker build -t flights_service_image -f Dockerfile .
docker run -it --name flights_service_container -v /tmp:/tmp flights_service_image

