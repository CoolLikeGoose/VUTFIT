import json

from db import db
from db.calendar_model import Calendar
from db.provider_calendar_model import ProviderCalendar


def create_calendar(data):
    new_calendar = Calendar(
        name = data["name"],
        img = data.get('img', ""),
        description = data["description"],
        default_duration = data["default_duration"],
        place = data.get('place', ""),
        form_fields=data.get('form_fields', {})
    )
    db.session.add(new_calendar)
    db.session.commit()
    return new_calendar.serialize()

def get_calendars():
    calendars = Calendar.query.all()
    return [calendar.serialize() for calendar in calendars]

def get_calendar_by_id(calendar_id):
    calendar = Calendar.query.get(calendar_id)
    if (calendar): 
        return calendar.serialize()
    return None

def update_calendar(calendar_id, data):
    calendar = Calendar.query.get(calendar_id)
    if (calendar is None):
        return {'error': 'Calendar not found'}, 404
    
    calendar.name = data.get('name', calendar.name)
    calendar.img = data.get('img', calendar.img)
    calendar.description = data.get('description', calendar.description)
    calendar.default_duration = data.get('default_duration', calendar.default_duration)
    calendar.place = data.get('place', calendar.place)
    calendar.form_fields = data.get('form_fields', calendar.form_fields)
    
    db.session.commit()
    return calendar.serialize(), 200

def remove_calendar(calendar_id):
    calendar = Calendar.query.get(calendar_id)
    if (calendar is None):
        return False
    
    db.session.delete(calendar)
    db.session.commit()
    return True

def get_providers_for_calendar(calendar_id):
    """Retrieve and serialize providers associated with a specific calendar."""
    provider_calendar_entries = ProviderCalendar.query.filter_by(calendar_id=calendar_id).all()
    providers = [entry.provider.serialize() for entry in provider_calendar_entries]
    return providers

def assign_provider_to_calendar(calendar_id, provider_id):
    """Assign a provider to a calendar and return the assignment information."""
    new_assignment = ProviderCalendar(provider_id=provider_id, calendar_id=calendar_id)
    db.session.add(new_assignment)
    db.session.commit()
    return new_assignment.serialize()

def unassign_provider_from_calendar(calendar_id, provider_id):
    """Remove a provider's association with a calendar."""
    assignment = ProviderCalendar.query.filter_by(calendar_id=calendar_id, provider_id=provider_id).first()
    if assignment:
        db.session.delete(assignment)
        db.session.commit()
        return True
    return False