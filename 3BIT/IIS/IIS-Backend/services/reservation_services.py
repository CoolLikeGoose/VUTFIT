from db import db
from db.conference_model import Conference
from db.reservation_model import Reservation
from datetime import datetime

from services.conference_services import update_current_occupancy, get_current_occupancy, get_max_occupancy


def get_reservations(start, limit, search_query=None, filter_time='all', user_id=None):
    # Join with Conference to apply filters on conference details
    query = Reservation.query.join(Conference)

    # Apply user filter if provided (e.g., to get reservations by a specific user)
    if user_id:
        query = query.filter(Reservation.user_id == user_id)

    # Apply search filter if provided
    if search_query:
        query = query.filter(
            (Reservation.status.ilike(f"%{search_query}%")) |
            (Conference.name.ilike(f"%{search_query}%")) |
            (Conference.description.ilike(f"%{search_query}%"))
        )

    # Apply time filter if provided
    now = datetime.now()
    if filter_time == 'past':
        query = query.filter(Conference.end_time < now)
    elif filter_time == 'upcoming':
        query = query.filter(Conference.start_time >= now)

    # Apply pagination
    if limit != -1:
        query = query.offset(start).limit(limit)

    return [reservation.serialize() for reservation in query.all()]


def get_reservation_by_id(reservation_id):
    reservation = Reservation.query.get(reservation_id)
    if (reservation): 
        return reservation.serialize()
    return None

def create_reservation(data):
    old_reservation = Reservation.query.filter_by(conference_id=data['conference_id'], user_id=data['user_id']).filter(Reservation.status != "canceled").all()
    if (old_reservation):
        return {'error': 'User already has reservation'}, 403
    
    available_occupancy = get_max_occupancy(data['conference_id']) - get_current_occupancy(data['conference_id'])
    if (available_occupancy < data['visitors_count']):
        return {'error': 'Occupancy already full'}, 410

    new_reservation = Reservation(
        conference_id = data['conference_id'],
        user_id = data['user_id'],
        visitors_count = data['visitors_count'],
        status = data['status']
    )

    # update Conference current_occupancy
    update_current_occupancy(new_reservation.conference_id, new_reservation.visitors_count)

    db.session.add(new_reservation)
    db.session.commit()
    return new_reservation.serialize(), 201

def update_reservation(reservation_id, data):
    reservation = Reservation.query.get(reservation_id)
    if (reservation is None):
        return {'error': 'Reservation not found'}, 404
    
    changed_visitors_count = reservation.visitors_count
    
    reservation.conference_id = data.get('conference_id', reservation.conference_id)
    reservation.user_id = data.get('user_id', reservation.user_id)
    reservation.visitors_count = data.get('visitors_count', reservation.visitors_count)
    reservation.status = data.get('status', reservation.status)

    changed_visitors_count -= reservation.visitors_count

    if (reservation.status == 'canceled'):
        update_current_occupancy(reservation.conference_id, -reservation.visitors_count)
    else:
        update_current_occupancy(reservation.conference_id, -changed_visitors_count)

    db.session.commit()
    return reservation.serialize(), 200

def remove_reservation(reservation_id):
    reservation = Reservation.query.get(reservation_id)
    if (reservation is None):
        return False
    
    db.session.delete(reservation)
    db.session.commit()
    return True

def get_reservations_by_conference(conference_id):
    reservations = Reservation.query.filter_by(conference_id=conference_id).all()
    return [reservation.serialize() for reservation in reservations]

def get_reservations_by_conference_user(conference_id, user_id):
    presentations = Reservation.query.filter_by(conference_id=conference_id, user_id=user_id).all()
    return [presentation.serialize() for presentation in presentations]