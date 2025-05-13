from db import db

class ConferenceAuditorium(db.Model):
    __tablename__ = 'conference_auditorium'

    conference_id = db.Column(db.Integer, db.ForeignKey('conferences.id', ondelete='CASCADE'), primary_key=True)
    auditorium_id = db.Column(db.Integer, db.ForeignKey('auditoriums.id', ondelete='CASCADE'), primary_key=True)

    conference = db.relationship('Conference',
                                 backref=db.backref('conference_auditoriums', cascade='all, delete-orphan', lazy=True))
    auditorium = db.relationship('Auditorium',
                                 backref=db.backref('conference_auditoriums', cascade='all, delete-orphan', lazy=False))

    created_at = db.Column(db.DateTime, default=db.func.now())
    updated_at = db.Column(db.DateTime, default=db.func.now(), onupdate=db.func.now())

    def serialize(self):
        """Return object data in easily serializable format"""
        return {
            'conference_id': self.conference_id,
            'auditorium_id': self.auditorium_id,
            'auditorium': self.auditorium.serialize()
        }
