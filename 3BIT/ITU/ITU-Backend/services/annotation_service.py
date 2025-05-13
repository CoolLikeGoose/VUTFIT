from db import db
from db.annotation_model import Annotation

def get_annotations():
    annotations = Annotation.query.all()
    return [annotation.serialize() for annotation in annotations]

def get_annotation_by_id(annotation_id):
    annotation = Annotation.query.get(annotation_id)
    if (annotation): 
        return annotation.serialize()
    return None

def create_annotation(data):
    new_annotation = Annotation(
        admin_id = data["admin_id"],
        content = data["content"],
        start_date = data["start_date"],
        end_date = data["end_date"],
    )
    db.session.add(new_annotation)
    db.session.commit()
    return new_annotation.serialize()

def update_annotation(annotation_id, data):
    annotation = Annotation.query.get(annotation_id)
    if (annotation is None):
        return {'error': 'Annotation not found'}, 404
    
    annotation.admin_id = data.get('admin_id', annotation.admin_id)
    annotation.content = data.get('content', annotation.content)
    annotation.start_date = data.get('start_date', annotation.start_date)
    annotation.end_date = data.get('end_date', annotation.end_date)
    
    db.session.commit()
    return annotation.serialize(), 200

def remove_annotation(annotation_id):
    annotation = Annotation.query.get(annotation_id)
    if (annotation is None):
        return False
    
    db.session.delete(annotation)
    db.session.commit()
    return True