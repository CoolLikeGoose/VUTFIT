#!/bin/sh

# Run database migrations to create the necessary tables
flask db upgrade

# Run data population if tables are available
flask populate_data

# Start the Flask application
flask run --host=0.0.0.0 --port=8080
