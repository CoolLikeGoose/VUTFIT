from db import db
from .models import BaseModel

class Reservation(BaseModel):
    __tablename__ = 'reservations'

    conference_id = db.Column(db.Integer, db.ForeignKey('conferences.id', ondelete='CASCADE'), nullable=False)
    user_id = db.Column(db.Integer, db.ForeignKey('users.id', ondelete='CASCADE'), nullable=False)
    visitors_count = db.Column(db.Integer, nullable=False)
    status = db.Column(db.String(10), nullable=False)

    user = db.relationship('User', backref=db.backref('reservations', cascade='all, delete-orphan', lazy=False))
    conference = db.relationship('Conference', backref=db.backref('reservations', cascade='all, delete-orphan', lazy=False))

    def serialize(self):
        """Return object data in easily serializable format"""
        return {
            'id': self.id,
            'conference_id': self.conference_id,
            'user_id': self.user_id,
            'visitors_count': self.visitors_count,
            'status': self.status,
            'user': self.user.serialize(),
            'conference': self.conference.serialize(),
            'created_at': self.created_at,
            'updated_at': self.updated_at
        }