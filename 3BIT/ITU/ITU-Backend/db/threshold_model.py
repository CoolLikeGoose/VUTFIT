from db import db
from .models import BaseModel

class Threshold(BaseModel):
    __tablename__ = 'thresholds'

    admin_id  = db.Column(db.Integer, db.ForeignKey('admins.id'), nullable = False)
    metric = db.Column(db.String(20), nullable=False)
    entity_type = db.Column(db.String(20), nullable=True)
    start_date = db.Column(db.DateTime, nullable = False)
    end_date = db.Column(db.DateTime, nullable=False)
    value = db.Column(db.Integer, nullable=False)
    comparison_operator = db.Column(db.String(1), nullable=False)

    def serialize(self):
        """Return object data in easily serializable format"""
        return {
            'id': self.id,
            'admin_id': self.admin_id,
            'metric': self.metric,
            'entity_type': self.entity_type,
            'start_date': self.start_date,
            'end_date': self.end_date,
            'value': self.value,
            'comparison_operator': self.comparison_operator,
            'created_at': self.created_at,
            'updated_at': self.updated_at
        }
