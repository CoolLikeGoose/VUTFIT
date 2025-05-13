import os
from dotenv import load_dotenv

load_dotenv()
class Config:
    SQLALCHEMY_DATABASE_URI = 'postgresql://myuser:mypassword@localhost:5432/mydatabase'  # Fetch database URL from .env
    SQLALCHEMY_TRACK_MODIFICATIONS = False  # Disable modification tracking for performance