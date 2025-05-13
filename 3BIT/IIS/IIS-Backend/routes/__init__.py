from .auditorium_routes import auditorium_bp
from .auth_routes import auth_bp
from .conference_routes import conference_bp
from .presentation_routes import presentation_bp
from .reservation_routes import reservation_bp
from .user_routes import user_bp

def init_routes(app):
    app.register_blueprint(auditorium_bp)
    app.register_blueprint(conference_bp)
    app.register_blueprint(presentation_bp)
    app.register_blueprint(reservation_bp)
    app.register_blueprint(user_bp)
    app.register_blueprint(auth_bp)