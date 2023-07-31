# Use an Ubuntu base image
FROM ubuntu:latest

# Set the working directory inside the container
WORKDIR /app

# Copy the compiled "flightsService" binary to the container
COPY build/flightsService /app/flightsService

# Set permissions for the binary
RUN chmod +x /app/flightsService

# Run the "flightsService" program in interactive mode
CMD ["./flightsService"]
