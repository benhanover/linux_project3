#!/bin/bash

# Pull Docker images
sudo docker pull meirzoref/flight_scanner:db_service_image
sudo docker pull meirzoref/flight_scanner:flights_service_image

# Run the db container in the background
sudo docker run -d --name db_service_container -v /tmp:/tmp meirzoref/flight_scanner:db_service_image

# Run the flightsService container in the terminal
sudo docker run -it --name flights_service_container -v /tmp:/tmp meirzoref/flight_scanner:flights_service_image
