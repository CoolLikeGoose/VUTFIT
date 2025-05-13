from db import db
from .models import BaseModel

class Auditorium(BaseModel):
    __tablename__ = 'auditoriums'

    name = db.Column(db.String(100), nullable=False)
    capacity = db.Column(db.Integer, nullable=False)

    def serialize(self):
        """Return object data in easily serializable format"""
        return {
            'id': self.id,
            'name': self.name,
            'capacity': self.capacity,
            'created_at': self.created_at,
            'updated_at': self.updated_at
        }