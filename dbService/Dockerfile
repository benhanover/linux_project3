# Use an Ubuntu base image
FROM ubuntu:latest

# Install Python 3 and cURL
RUN apt-get update && apt-get install -y python3 python3-pip curl

# Set the working directory inside the container
WORKDIR /app

# Copy the necessary files to the container
COPY build/dbService /app/dbService
COPY flightScanner.sh /app/flightScanner.sh
COPY clean.sh /app/clean.sh
COPY build/utilities /app/utilities

# Create a new directory for storing shared library dependencies
RUN mkdir /app/dependencies

# Copy the shared library dependencies from the dependencies directory
COPY build/dependencies/* /app/dependencies/

# Set permissions for the scripts and dbService
RUN chmod +x /app/dbService /app/flightScanner.sh /app/clean.sh

# Set the environment variable LD_LIBRARY_PATH to include both utilities and dependencies directories
ENV LD_LIBRARY_PATH="/app/utilities:/app/dependencies"

# Run the dbService program by default
CMD ["./dbService"]
