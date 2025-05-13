from db import db
from db.conference_model import Conference
from db.presentation_model import Presentation
from datetime import datetime

def get_presentations(start, limit, search_query=None, filter_time='all', user_id=None):
    query = Presentation.query

    # Apply user filter if provided (e.g., to get presentations by a specific speaker)
    if user_id:
        query = query.filter(Presentation.speaker_id == user_id)

    # Apply search filter if provided
    if search_query:
        query = query.filter(
            (Presentation.name.ilike(f"%{search_query}%")) |
            (Presentation.description.ilike(f"%{search_query}%"))
        )

    # Apply time filter if provided
    now = datetime.now()
    if filter_time in ['past', 'upcoming']:
        # Join with the Conference table to get the start and end times if presentation times are None
        query = query.join(Conference, Presentation.conference_id == Conference.id)

        if filter_time == 'past':
            # Filter past presentations (either based on presentation or conference times)
            query = query.filter(
                (Presentation.end_time < now) |
                ((Presentation.end_time == None) & (Conference.end_time < now))  # Use conference end time if presentation end time is None
            )
        elif filter_time == 'upcoming':
            # Filter upcoming presentations (either based on presentation or conference times)
            query = query.filter(
                (Presentation.start_time >= now) |
                ((Presentation.start_time == None) & (Conference.start_time >= now))  # Use conference start time if presentation start time is None
            )

    # Apply pagination
    if limit != -1:
        query = query.offset(start).limit(limit)

    # Serialize presentations and fill in missing start and end times with associated conference times if necessary
    presentations = []
    for presentation in query.all():
        presentation_data = presentation.serialize()

        # If presentation has no start or end time, use conference times
        if presentation_data['start_time'] is None or presentation_data['end_time'] is None:
            conference = Conference.query.get(presentation.conference_id)
            if conference:
                presentation_data['start_time'] = conference.start_time
                presentation_data['end_time'] = conference.end_time

        presentations.append(presentation_data)

    return presentations



def get_presentation_by_id(presentation_id):
    presentation = Presentation.query.get(presentation_id)
    if (presentation): 
        return presentation.serialize()
    return None

def create_presentation(data):
    new_presentation = Presentation(
        speaker_id = data['speaker_id'],
        conference_id = data['conference_id'],
        name = data['name'],
        description = data['description'],
        picture = data.get('picture', ''),
        presentation_text = data['presentation_text'],
        status = data['status']
    )

    db.session.add(new_presentation)
    db.session.commit()
    return new_presentation.serialize()

def update_presentation(presentation_id, data):
    presentation = Presentation.query.get(presentation_id)
    if (presentation is None):
        return {'error': 'Presentation not found'}, 404
    
    presentation.speaker_id = data.get('speaker_id', presentation.speaker_id)
    presentation.conference_id = data.get('conference_id', presentation.conference_id)
    presentation.name = data.get('name', presentation.name)
    presentation.description = data.get('description', presentation.description)
    presentation.picture = data.get('picture', presentation.picture)
    presentation.presentation_text = data.get('presentation_text', presentation.presentation_text)
    presentation.auditorium_id = data.get('auditorium_id', presentation.auditorium_id)
    presentation.status = data.get('status', presentation.status)
    presentation.start_time = data.get('start_time', presentation.start_time)
    presentation.end_time = data.get('end_time', presentation.end_time)

    db.session.commit()
    return presentation.serialize(), 200

def remove_presentation(presentation_id):
    presentation = Presentation.query.get(presentation_id)
    if (presentation is None):
        return False
    
    db.session.delete(presentation)
    db.session.commit()
    return True

def get_presentations_by_conference(conference_id):
    presentations = Presentation.query.filter_by(conference_id=conference_id).all()
    return [presentation.serialize() for presentation in presentations]

def get_unconfirmed_presentations_by_conference(conference_id):
    presentations = Presentation.query.filter_by(conference_id=conference_id).filter_by(status='unconfirmed').all()
    return [presentation.serialize() for presentation in presentations]

def get_confirmed_presentations_by_conference(conference_id):
    presentations = Presentation.query.filter_by(conference_id=conference_id).filter_by(status='confirmed').all()
    return [presentation.serialize() for presentation in presentations]

def get_presentations_by_conferenece_user(conference_id, user_id):
    presentations = Presentation.query.filter_by(conference_id=conference_id, speaker_id=user_id).all()
    return [presentation.serialize() for presentation in presentations]