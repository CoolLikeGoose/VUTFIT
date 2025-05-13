from .admin_routes import admin_bp
from .analytics_routes import analytics_bp
from .annotation_routes import annotation_bp
from .appointment_routes import appointment_bp
from .calendar_routes import calendar_bp
from .client_routes import client_bp
from .provider_routes import provider_bp
from .threshold_routes import threshold_bp
from .free_slots_routes import free_slots_bp

def init_routes(app):
    app.register_blueprint(admin_bp)
    app.register_blueprint(analytics_bp)
    app.register_blueprint(annotation_bp)
    app.register_blueprint(appointment_bp)
    app.register_blueprint(calendar_bp)
    app.register_blueprint(client_bp)
    app.register_blueprint(provider_bp)
    app.register_blueprint(threshold_bp)
    app.register_blueprint(free_slots_bp)