# Use the official Python image as a base image
FROM python:3.12-slim

# Set the working directory in the container
WORKDIR /app

# Copy the requirements file first to leverage Docker cache
COPY requirements.txt .

# Install system dependencies and Python dependencies
RUN apt-get update && \
    apt-get install -y --no-install-recommends build-essential libpq-dev && \
    pip install --no-cache-dir -r requirements.txt && \
    apt-get clean && rm -rf /var/lib/apt/lists/*

# Copy the rest of the application files into the container
COPY . .

# Set environment variables to run Flask properly
ENV FLASK_APP=app.py
ENV FLASK_RUN_HOST=0.0.0.0
ENV FLASK_RUN_PORT=7070
ENV FLASK_ENV=production

# Expose the port that Flask will run on
EXPOSE 7070

# Run database migrations and populate data
RUN flask db upgrade && flask populate_data

# Command to run the Flask app
CMD ["flask", "run", "--port=7070"]