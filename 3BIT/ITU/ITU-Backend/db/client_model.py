# db/client_model.py
from db import db
from .models import BaseModel


class Client(BaseModel):
    __tablename__ = 'clients'

    name = db.Column(db.String(100), nullable=False)
    email = db.Column(db.String(100), nullable=False, unique=True)
    phone = db.Column(db.String(20), nullable=False)
    custom_fields = db.Column(db.JSON, nullable=True)

    def serialize(self):
        """Return object data in easily serializable format"""
        return {
            'id': self.id,
            'name': self.name,
            'email': self.email,
            'phone': self.phone,
            'custom_fields': self.custom_fields,
            'created_at': self.created_at,
            'updated_at': self.updated_at
        }
