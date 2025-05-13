from db import db
from db.ConferenceAuditorium import ConferenceAuditorium
from db.auditorium_model import Auditorium
from db.conference_model import Conference
from db.presentation_model import Presentation

def get_available_auditoriums_by_conference(conference_id):
    # Fetch the current conference details
    conference = Conference.query.get(conference_id)
    if not conference:
        return []

    # Get all auditoriums
    available_auditoriums = Auditorium.query.all()

    # Filter auditoriums based on availability
    filtered_auditoriums = []
    for auditorium in available_auditoriums:
        # Check if auditorium is already linked to this conference
        is_linked_to_current = ConferenceAuditorium.query.filter_by(conference_id=conference_id, auditorium_id=auditorium.id).first()
        if is_linked_to_current:
            continue

        # Check if the auditorium is linked to another conference with overlapping time
        overlapping_conferences = db.session.query(Conference).join(ConferenceAuditorium).filter(
            ConferenceAuditorium.auditorium_id == auditorium.id,
            Conference.id != conference_id,
            Conference.start_time < conference.end_time,
            Conference.end_time > conference.start_time
        ).first()

        if overlapping_conferences:
            continue

        filtered_auditoriums.append(auditorium.serialize())

    return filtered_auditoriums

def get_auditoriums_by_conference(conference_id):
    auditoriums = ConferenceAuditorium.query.filter_by(conference_id=conference_id)
    return [auditorium.serialize() for auditorium in auditoriums]

def link_auditorium_to_confernce(auditorium_id, conference_id):
    new_auditorium_conference = ConferenceAuditorium(
        conference_id = conference_id,
        auditorium_id = auditorium_id
    )
    db.session.add(new_auditorium_conference)
    db.session.commit()
    return new_auditorium_conference.serialize()

def unlink_auditorium_to_confernce(auditorium_id, conference_id):
    auditorium_conference = ConferenceAuditorium.query.filter_by(conference_id=conference_id, auditorium_id=auditorium_id).first()
    if (auditorium_conference is None):
        return False
    
    presentations = Presentation.query.filter_by(auditorium_id=auditorium_id, conference_id=conference_id).all()
    for presentation in presentations:
        presentation.status = "unconfirmed"

    
    db.session.delete(auditorium_conference)
    db.session.commit()
    return True