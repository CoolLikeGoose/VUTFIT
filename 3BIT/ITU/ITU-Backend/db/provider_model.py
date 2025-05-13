from db import db
from .models import BaseModel


class Provider(BaseModel):
    __tablename__ = 'providers'

    name = db.Column(db.String(100), nullable=False)
    contact_info = db.Column(db.String(100), nullable=False)

    def serialize(self):
        """Return object data in easily serializable format"""
        return {
            'id': self.id,
            'name': self.name,
            'contact_info': self.contact_info,
            'created_at': self.created_at,
            'updated_at': self.updated_at
        }
