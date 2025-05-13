from db import db


class ProviderCalendar(db.Model):
    __tablename__ = 'provider_calendars'

    provider_id = db.Column(db.Integer, db.ForeignKey('providers.id'), primary_key=True)
    calendar_id = db.Column(db.Integer, db.ForeignKey('calendars.id'), primary_key=True)

    provider = db.relationship('Provider', backref=db.backref('provider_calendars', lazy=True))
    calendar = db.relationship('Calendar', backref=db.backref('provider_calendars', lazy=True))

    created_at = db.Column(db.DateTime, default=db.func.now())
    updated_at = db.Column(db.DateTime, default=db.func.now(), onupdate=db.func.now())

    def serialize(self):
        """Return serialized representation of the provider-calendar association."""
        return {
            'provider_id': self.provider_id,
            'calendar_id': self.calendar_id,
            'created_at': self.created_at,
            'updated_at': self.updated_at
        }
