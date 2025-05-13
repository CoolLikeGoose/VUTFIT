from flask import Flask
from flask_migrate import Migrate
from config import Config
from db import db
from init_db import populate_db
from routes import init_routes
from flask_cors import CORS

def create_app():
    app = Flask(__name__)
    app.config.from_object(Config)

    CORS(app)

    db.init_app(app)

    migrate = Migrate(app, db)

    # Register routes
    init_routes(app)

    # Custom CLI command to populate data
    @app.cli.command("populate_data")
    def populate_data_command():
        populate_db()

    return app

if __name__ == "__main__":
    app = create_app()
    app.run(debug=True, port=7070)  # Run the app on port 7070