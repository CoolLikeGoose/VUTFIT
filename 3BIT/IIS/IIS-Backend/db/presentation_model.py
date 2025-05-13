from db import db
from .models import BaseModel

class Presentation(BaseModel):
    __tablename__ = 'presentations'

    speaker_id = db.Column(db.Integer, db.ForeignKey('users.id', ondelete='CASCADE'), nullable=False)
    conference_id = db.Column(db.Integer, db.ForeignKey('conferences.id', ondelete='CASCADE'), nullable=False)
    auditorium_id = db.Column(db.Integer, db.ForeignKey('auditoriums.id', ondelete='SET NULL'), nullable=True)

    name = db.Column(db.String(100), nullable=False)
    description = db.Column(db.String(300), nullable=True)
    picture = db.Column(db.String(256), nullable=True)
    presentation_text = db.Column(db.String(500), nullable=False)
    status = db.Column(db.String(11), nullable=False)
    start_time = db.Column(db.DateTime, nullable=True)
    end_time = db.Column(db.DateTime, nullable=True)

    # Define relationships to related models
    speaker = db.relationship('User', backref=db.backref('presentations', cascade='all, delete-orphan', lazy=True))
    conference = db.relationship('Conference', backref=db.backref('presentations', cascade='all, delete-orphan', lazy=True))
    auditorium = db.relationship('Auditorium', backref=db.backref('presentations', lazy=True))

    def serialize(self):
        """Return object data in easily serializable format"""
        return {
            'id': self.id,
            'speaker_id': self.speaker_id,
            'conference_id': self.conference_id,
            'auditorium_id': self.auditorium_id,
            'name': self.name,
            'description': self.description,
            'picture': self.picture,
            'presentation_text': self.presentation_text,
            'status': self.status,
            'start_time': self.start_time,
            'end_time': self.end_time,
            'created_at': self.created_at,
            'updated_at': self.updated_at
        }