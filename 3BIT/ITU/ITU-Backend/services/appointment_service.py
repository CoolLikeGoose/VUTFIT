from db import db
from db.appointment_model import Appointment
from datetime import datetime
from dateutil.parser import isoparse

def get_appointments():
    appointments = Appointment.query.all()
    return [appointment.serialize() for appointment in appointments]


def get_appointments_by_criteria(start, end, provider_ids, calendar_ids=None):
    start_date = isoparse(start)
    end_date = isoparse(end)

    # Build the initial query
    query = Appointment.query.filter(
        Appointment.appointment_datetime >= start_date,
        Appointment.appointment_datetime <= end_date,
        Appointment.provider_id.in_(provider_ids),
        Appointment.status == "booked"  # Filter only booked appointments
    )

    # Add an additional filter for calendar IDs if provided
    if calendar_ids:
        query = query.filter(Appointment.calendar_id.in_(calendar_ids))

    appointments = query.all()
    return [appointment.serialize() for appointment in appointments]


def get_appointment_by_id(appointment_id):
    appointment = Appointment.query.get(appointment_id)
    if appointment:
        return appointment.serialize()
    return None

def create_appointment(data):
    new_appointment = Appointment(
        client_id = data["client_id"],
        provider_id = data["provider_id"],
        calendar_id = data["calendar_id"],
        appointment_datetime = data["appointment_datetime"],
        status = data["status"],
        duration=data["duration"],
    )
    db.session.add(new_appointment)
    db.session.commit()
    return new_appointment.serialize()

def update_appointment(appointment_id, data):
    appointment = Appointment.query.get(appointment_id)
    if (appointment is None):
        return {'error': 'Appointment not found'}, 404
    
    appointment.client_id = data.get('client_id', appointment.client_id)
    appointment.provider_id = data.get('provider_id', appointment.provider_id)
    appointment.calendar_id = data.get('calendar_id', appointment.calendar_id)
    appointment.appointment_datetime = data.get('appointment_datetime', appointment.appointment_datetime)
    appointment.status = data.get('status', appointment.status)
    appointment.duration = data.get('duration', appointment.duration)
    
    db.session.commit()
    return appointment.serialize(), 200

def remove_appointment(appointment_id):
    appointment = Appointment.query.get(appointment_id)
    if (appointment is None):
        return False
    
    db.session.delete(appointment)
    db.session.commit()
    return True


def update_appointment_status_service(appointment_id, status):
    appointment = Appointment.query.get(appointment_id)
    if not appointment:
        return {'error': 'Appointment not found'}, 404

    appointment.status = status
    db.session.commit()
    return appointment.serialize(), 200

def get_appointments_by_client(client):
    appointments = Appointment.query.filter_by(client_id=client).all()
    return [appointment.serialize() for appointment in appointments]