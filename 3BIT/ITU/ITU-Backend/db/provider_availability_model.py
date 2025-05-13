from db import db
from .models import BaseModel
from sqlalchemy import Time

class ProviderAvailability(BaseModel):
    __tablename__ = 'provider_availability'

    provider_id = db.Column(db.Integer, db.ForeignKey('providers.id'), nullable=False)
    start_time = db.Column(Time, nullable=False)
    end_time = db.Column(Time, nullable=False)
    week_day = db.Column(db.Integer, nullable=False)  # Index [0-6] for days of the week

    def serialize(self):
        """Return object data in easily serializable format"""
        return {
            'id': self.id,
            'provider_id': self.provider_id,
            'start_time': self.start_time.strftime("%H:%M"),
            'end_time': self.end_time.strftime("%H:%M"),
            'week_day': self.week_day,
            'created_at': self.created_at,
            'updated_at': self.updated_at
        }