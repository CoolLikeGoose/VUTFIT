# Use an official Python image as the base
FROM python:3.12-slim

# Set the working directory in the container
WORKDIR /app

# Copy the requirements file and install dependencies
COPY requirements.txt ./
RUN pip install --no-cache-dir -r requirements.txt

# Copy the rest of the application code to the container
COPY . .

# Ensure the start script is executable
RUN chmod +x ./start.sh

# Expose the port that the app runs on
EXPOSE 8080

# Run the application
CMD ["sh", "./start.sh"]
