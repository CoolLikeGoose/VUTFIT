from db import db
from db.provider_model import Provider
from db.provider_availability_model import ProviderAvailability
from datetime import datetime

def get_providers():
    providers = Provider.query.all()
    return [provider.serialize() for provider in providers]

def get_provider_by_id(provider_id):
    provider = Provider.query.get(provider_id)
    if (provider): 
        return provider.serialize()
    return None

def create_provider(data):
    new_provider = Provider(
        name = data["name"],
        contact_info = data["contact_info"]
    )
    db.session.add(new_provider)
    db.session.commit()
    return new_provider.serialize()

def update_provider(provider_id, data):
    provider = Provider.query.get(provider_id)
    if (provider is None):
        return {'error': 'Provider not found'}, 404
    
    provider.name = data.get('name', provider.name)
    provider.contact_info = data.get('contact_info', provider.contact_info)
    
    db.session.commit()
    return provider.serialize(), 200

def remove_provider(provider_id):
    provider = Provider.query.get(provider_id)
    if (provider is None):
        return False
    
    db.session.delete(provider)
    db.session.commit()
    return True


def get_availability_for_provider(provider_id):
    availability = ProviderAvailability.query.filter_by(provider_id=provider_id).all()
    return [avail.serialize() for avail in availability]


def create_provider_availability(provider_id, data):
    """
    Deletes all existing availability entries for the specified provider,
    then adds new availability records from the provided data.
    The data parameter should be a list of dictionaries, each containing
    'start_time', 'end_time' as "HH:MM" strings, and 'week_day' as an integer.
    """

    # Delete existing availability for the provider
    ProviderAvailability.query.filter_by(provider_id=provider_id).delete()
    db.session.commit()

    # Add new availability entries
    new_availability_entries = []
    for availability in data:
        start_time = datetime.strptime(availability['start_time'], "%H:%M").time()
        end_time = datetime.strptime(availability['end_time'], "%H:%M").time()

        new_availability = ProviderAvailability(
            provider_id=provider_id,
            start_time=start_time,
            end_time=end_time,
            week_day=availability['week_day']
        )
        db.session.add(new_availability)
        new_availability_entries.append(new_availability.serialize())

    # Commit all new availability entries at once
    db.session.commit()

    return new_availability_entries


def remove_provider_availability(provider_id, availability_id):
    availability = ProviderAvailability.query.filter_by(id=availability_id, provider_id=provider_id).first()
    if availability:
        db.session.delete(availability)
        db.session.commit()
        return True
    return False