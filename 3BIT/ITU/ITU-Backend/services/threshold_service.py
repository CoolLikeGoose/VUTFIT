from db import db
from db.threshold_model import Threshold

def get_thresholds():
    thresholds = Threshold.query.all()
    return [threshold.serialize() for threshold in thresholds]

def get_threshold_by_id(threshold_id):
    threshold = Threshold.query.get(threshold_id)
    if (threshold): 
        return threshold.serialize()
    return None

def create_threshold(data):
    new_threshold = Threshold(
        admin_id = data["admin_id"],
        metric = data["metric"],
        entity_type = data["entity_type"],
        start_date = data["start_date"],
        end_date = data["end_date"],
        value = data["value"],
        comparison_operator = data["comparison_operator"]
    )
    db.session.add(new_threshold)
    db.session.commit()
    return new_threshold.serialize()

def update_threshold(threshold_id, data):
    threshold = Threshold.query.get(threshold_id)
    if (threshold is None):
        return {'error': 'Threshold not found'}, 404
    
    threshold.admin_id = data.get('admin_id', threshold.admin_id)
    threshold.metric = data.get('metric', threshold.metric)
    threshold.entity_type = data.get('entity_type', threshold.entity_type)
    threshold.start_date = data.get('start_date', threshold.start_date)
    threshold.end_date = data.get('end_date', threshold.end_date)
    threshold.value = data.get('value', threshold.value)
    threshold.comparison_operator = data.get('comparison_operator', threshold.comparison_operator)
    
    db.session.commit()
    return threshold.serialize(), 200

def remove_threshold(threshold_id):
    threshold = Threshold.query.get(threshold_id)
    if (threshold is None):
        return False
    
    db.session.delete(threshold)
    db.session.commit()
    return True