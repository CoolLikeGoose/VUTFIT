from db import db
from .models import BaseModel

class Appointment(BaseModel):
    __tablename__ = 'appointments'

    client_id = db.Column(db.Integer, db.ForeignKey('clients.id'), nullable = False)
    provider_id = db.Column(db.Integer, db.ForeignKey('providers.id'), nullable = False)
    calendar_id = db.Column(db.Integer, db.ForeignKey('calendars.id'), nullable = False)
    appointment_datetime  = db.Column(db.DateTime, nullable = False)
    status = db.Column(db.String(20), nullable=False)
    duration = db.Column(db.Integer, nullable = False)

    def serialize(self):
        """Return object data in easily serializable format"""
        return {
            'id': self.id,
            'client_id': self.client_id,
            'provider_id': self.provider_id,
            'calendar_id': self.calendar_id,
            'appointment_datetime': self.appointment_datetime,
            'status': self.status,
            'duration': self.duration,
            'created_at': self.created_at,
            'updated_at': self.updated_at
        }
