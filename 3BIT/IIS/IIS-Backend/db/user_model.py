from db import db
from .models import BaseModel


class User(BaseModel):
    __tablename__ = 'users'

    name = db.Column(db.String(100), nullable=False)
    email = db.Column(db.String(100), nullable=False, unique=True)
    role = db.Column(db.String(100), nullable=False)
    password_hash = db.Column(db.String(512), nullable=True)
    picture = db.Column(db.String(256), nullable=True)

    def serialize(self):
        """Return object data in easily serializable format"""
        return {
            'id': self.id,
            'name': self.name,
            'email': self.email,
            'role': self.role,
            'picture': self.picture,
            'created_at': self.created_at,
            'updated_at': self.updated_at
        }
