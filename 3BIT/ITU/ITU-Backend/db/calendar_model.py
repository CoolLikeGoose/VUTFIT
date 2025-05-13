from db import db
from .models import BaseModel

class Calendar(BaseModel):
    __tablename__ = 'calendars'

    name = db.Column(db.String(100), nullable=False)
    img = db.Column(db.String(300), nullable=True)
    description = db.Column(db.String(300), nullable = False)
    default_duration = db.Column(db.Integer, nullable = False)
    place = db.Column(db.String(100), nullable = True)
    form_fields = db.Column(db.JSON, nullable=True)

    def serialize(self):
        """Return object data in easily serializable format"""
        return {
            'id': self.id,
            'name': self.name,
            'img': self.img,
            'description': self.description,
            'default_duration': self.default_duration,
            'place': self.place,
            'form_fields': self.form_fields,
            'created_at': self.created_at,
            'updated_at': self.updated_at
        }
