# Appointment Booking Backend: Setup Guide

## Prerequisites

Before starting, ensure you have the following installed:
1. **Docker** – For running PostgreSQL in a container.
2. **Python 3.8+** – For running the Flask backend.
3. **pip** – For managing Python dependencies.
4. **Postman or curl** – To test the API endpoints.

## 1. Set Up PostgreSQL with Docker

First, create and run a PostgreSQL container using Docker.

### 1.1 Run PostgreSQL Container

Use the following Docker command to create and run a PostgreSQL container without authentication:

```bash
docker run --name my_postgres_db -e POSTGRES_USER=myuser -e POSTGRES_PASSWORD=mypassword -e POSTGRES_DB=mydatabase -p 5432:5432 -d postgres
```

- `--name my_postgres_db`: Name of the Docker container.
- `-e POSTGRES_USER=myuser`: The username for the database.
- `-e POSTGRES_PASSWORD=mypassword`: The password for the database.
- `-e POSTGRES_DB=mydatabase`: The name of the database.
- `-p 5432:5432`: Expose port 5432 for external connections.

### 1.2 Verify PostgreSQL is Running

To verify that PostgreSQL is running, execute:

```bash
docker ps
```

This should show the `my_postgres_db` container running.

### 1.3 Connecting to PostgreSQL from Console

You can also connect to the database with the following command:

```bash
docker exec -it my_postgres_db psql -U myuser -d mydatabase
```

## 2. Set Up the Flask Backend

### 2.1 Clone the Repository

```bash
git clone https://github.com/your-repo/appointment-booking-backend.git
cd appointment-booking-backend
```

### 2.2 Set Up Virtual Environment

To create a Python virtual environment and activate it:

```bash
python3 -m venv .venv
source .venv/bin/activate
```

### 2.3 Install Dependencies

```bash
pip install -r requirements.txt
```

### 2.4 Set Up Environment Variables

Create a `.env` file in the root of your project:

```bash
touch .env
```

Add the following content to `.env`:

```
DATABASE_URL=postgresql://myuser:mypassword@localhost:5432/mydatabase
SECRET_KEY=mysecretkey
```

### 2.5 Configure Flask App

In `config.py`, Flask reads the environment variables from `.env` to configure the app:

```python
import os
from dotenv import load_dotenv

load_dotenv()

class Config:
    SQLALCHEMY_DATABASE_URI = os.getenv('DATABASE_URL')
    SQLALCHEMY_TRACK_MODIFICATIONS = False
    SECRET_KEY = os.getenv('SECRET_KEY')
```

## 3. Database Migrations

Flask-Migrate is used for handling database migrations.

### 3.1 Initialize Migrations (If doesn't migrations/ doesn't exist)

```bash
flask db init
```

### 3.2 Create Migration Script

Generate a migration script based on your current models:

```bash
flask db migrate -m "Initial migration"
```

### 3.3 Apply Migrations

Apply the migration to the PostgreSQL database:

```bash
flask db upgrade
```

## 4. Running the Application

Start the Flask application:

```bash
flask run
```

You should see output like this:

```
 * Running on http://127.0.0.1:5000/
```

You can now access the API at `http://127.0.0.1:5000/`.

## 5. Testing the Application

### 5.1 Create a Client

Use `curl` or Postman to create a client:

```bash
curl -X POST http://127.0.0.1:5000/clients \
-H "Content-Type: application/json" \
-d '{
    "name": "John Doe",
    "email": "john.doe@example.com",
    "phone": "123-456-7890"
}'
```

### 5.2 Retrieve Clients

```bash
curl http://127.0.0.1:5000/clients
```

### 5.3 Update a Client

```bash
curl -X PUT http://127.0.0.1:5000/clients/1 \
-H "Content-Type: application/json" \
-d '{
    "name": "John Doe Updated",
    "email": "john.doe@example.com",
    "phone": "987-654-3210"
}'
```

### 5.4 Delete a Client

```bash
curl -X DELETE http://127.0.0.1:5000/clients/1
```

## 6. Additional Docker Commands

### 6.1 Stop PostgreSQL Container

```bash
docker stop my_postgres_db
```

### 6.2 Remove PostgreSQL Container

```bash
docker rm my_postgres_db
```

### 6.3 Restart PostgreSQL Container

```bash
docker start my_postgres_db
```

---