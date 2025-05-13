from db import db
from db.appointment_model import Appointment


def get_total_bookings_by_provider_criteria(start,end,provider_id) :
    total_bookings = Appointment.query.filter(
        Appointment.appointment_datetime >= start,
        Appointment.appointment_datetime <= end,
        Appointment.provider_id == (provider_id),
        ).count()
    return total_bookings

def get_total_bookings_by_provider(provider_id) :
    total_bookings = Appointment.query.filter_by(provider_id=provider_id).count()
    return total_bookings