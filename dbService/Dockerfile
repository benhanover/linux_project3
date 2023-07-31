# Use an Ubuntu base image
FROM ubuntu:latest

# Install runtime dependencies and build tools
RUN apt-get update && apt-get install -y build-essential libzip-dev

# Set the working directory inside the container
WORKDIR /app

# Copy the necessary files to the container
COPY build/dbService /app/dbService
COPY flightScanner.sh /app/flightScanner.sh
COPY clean.sh /app/clean.sh
COPY build/utilities /app/utilities

# Set permissions for the scripts and the dbService binary
RUN chmod +x /app/dbService /app/flightScanner.sh /app/clean.sh

# Set the environment variable LD_LIBRARY_PATH to include the libraries directory
ENV LD_LIBRARY_PATH="/app"

# Run the dbService program by default
CMD ["./dbService"]
