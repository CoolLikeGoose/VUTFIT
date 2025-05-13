from db import db
from db.auditorium_model import Auditorium


def get_auditoriums(start, limit, search=None, min_capacity=None):
    query = Auditorium.query

    # Apply search filter if provided
    if search:
        query = query.filter(Auditorium.name.ilike(f"%{search}%"))

    # Apply capacity filter if provided
    if min_capacity:
        query = query.filter(Auditorium.capacity >= min_capacity)

    # Apply pagination
    if limit != -1:
        query = query.offset(start).limit(limit)

    auditoriums = query.all()

    return [auditorium.serialize() for auditorium in auditoriums]

def get_auditorium_by_id(auditorium_id):
    auditorium = Auditorium.query.get(auditorium_id)
    if (auditorium): 
        return auditorium.serialize()
    return None

def create_auditorium(data):
    existing_auditorium = Auditorium.query.filter_by(name=data['name']).first()
    if existing_auditorium:
        response_data = existing_auditorium.serialize()
        response_data['message'] = 'Auditorium with this name already exists.'
        return response_data, 409

    new_auditorium = Auditorium(
        name=data['name'],
        capacity=data['capacity']
    )
    db.session.add(new_auditorium)
    db.session.commit()

    return new_auditorium.serialize(), 201


def update_auditorium(auditorium_id, data):
    auditorium = Auditorium.query.get(auditorium_id)
    if auditorium is None:
        return {'error': 'Auditorium not found'}, 404

    if 'name' in data and data['name']:
        new_name = data['name']
        existing_auditorium = Auditorium.query.filter_by(name=new_name).first()
        if existing_auditorium and existing_auditorium.id != auditorium_id:
            return {'message': 'Auditorium with this name already exists.'}, 409
        auditorium.name = new_name

    auditorium.capacity = data.get('capacity', auditorium.capacity)

    db.session.commit()
    return auditorium.serialize(), 200

def remove_auditorium(auditorium_id):
    auditorium = Auditorium.query.get(auditorium_id)
    if (auditorium is None):
        return False
    
    db.session.delete(auditorium)
    db.session.commit()
    return True