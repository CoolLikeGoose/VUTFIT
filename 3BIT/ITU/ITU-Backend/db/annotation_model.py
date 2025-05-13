from db import db
from .models import BaseModel

class Annotation(BaseModel):
    __tablename__ = 'annotations'

    admin_id = db.Column(db.Integer, db.ForeignKey('admins.id'), nullable = False)
    content = db.Column(db.String(300), nullable=False)
    start_date = db.Column(db.DateTime, nullable = False)
    end_date = db.Column(db.DateTime, nullable=False)

    def serialize(self):
        """Return object data in easily serializable format"""
        return {
            'id': self.id,
            'admin_id': self.admin_id,
            'content': self.content,
            'start_date': self.start_date,
            'end_date': self.end_date,
            'created_at': self.created_at,
            'updated_at': self.updated_at
        }
