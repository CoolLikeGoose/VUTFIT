from db import db
from .models import BaseModel

class Conference(BaseModel):
    __tablename__ = 'conferences'

    organizer_id = db.Column(db.Integer, db.ForeignKey('users.id', ondelete='CASCADE'), nullable=False)
    name = db.Column(db.String(100), nullable=False)
    description = db.Column(db.String(300), nullable=True)
    capacity = db.Column(db.Integer, nullable=False)
    cost = db.Column(db.Integer, nullable=False)
    max_reservations_per_user = db.Column(db.Integer, nullable=False)
    current_occupancy = db.Column(db.Integer, nullable=False)
    picture = db.Column(db.String(256), nullable=False)
    tags = db.Column(db.JSON, nullable=True)
    start_time = db.Column(db.DateTime, nullable=False)
    end_time = db.Column(db.DateTime, nullable=False)

    # Only keep the organizer relationship with a backref to User
    organizer = db.relationship('User', backref=db.backref('conferences', cascade='all, delete-orphan', lazy=True))

    def serialize(self):
        """Return object data in easily serializable format"""
        return {
            'id': self.id,
            'organizer_id': self.organizer_id,
            'name': self.name,
            'description': self.description,
            'capacity': self.capacity,
            'cost': self.cost,
            'max_reservations_per_user': self.max_reservations_per_user,
            'current_occupancy': self.current_occupancy,
            'picture': self.picture,
            'tags': self.tags,
            'start_time': self.start_time,
            'end_time': self.end_time,
            'created_at': self.created_at,
            'updated_at': self.updated_at
        }