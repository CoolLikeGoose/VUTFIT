import os
from dotenv import load_dotenv

load_dotenv()  # Load environment variables from .env file

class Config:
    SQLALCHEMY_DATABASE_URI = os.getenv('DATABASE_URL')  # Fetch database URL from .env
    SQLALCHEMY_TRACK_MODIFICATIONS = False  # Disable modification tracking for performance
    SECRET_KEY = os.getenv('SECRET_KEY')  # Secret key from .env