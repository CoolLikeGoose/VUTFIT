from flask import Flask
from db import db
from flask_migrate import Migrate
from routes import init_routes
from config import Config
from populate_data import populate_data
from flask_cors import CORS

def create_app():
    app = Flask(__name__)
    app.config.from_object(Config)

    # Initialize CORS with default settings (allow all origins)
    CORS(app)

    # Initialize the database
    db.init_app(app)

    # Initialize migrations
    migrate = Migrate(app, db)

    # Initialize routes
    init_routes(app)

    # Custom CLI command to populate data
    @app.cli.command("populate_data")
    def populate_data_command():
        populate_data()

    return app

if __name__ == "__main__":
    app = create_app()
    app.run(debug=True)
