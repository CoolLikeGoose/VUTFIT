from db import db
from db.conference_model import Conference
from datetime import datetime
from sqlalchemy import or_, func, cast
from sqlalchemy.dialects.postgresql import JSONB

def get_conferences(start, limit, search_query=None, filter_time='all', user_id=None, tags=None):
    query = Conference.query

    # Apply tags filter if provided
    if tags:
        # Use or_ to match any tag in the list, ensuring at least one tag is present
        tag_conditions = [cast(Conference.tags, JSONB).contains([tag]) for tag in tags]
        query = query.filter(or_(*tag_conditions))

    # Apply user filter if provided (e.g., to get conferences by a specific organizer)
    if user_id:
        query = query.filter(Conference.organizer_id == user_id)

    # Apply search filter if provided (name, description, and tags)
    if search_query:
        query = query.filter(
            or_(
                Conference.name.ilike(f"%{search_query}%"),
                Conference.description.ilike(f"%{search_query}%"),
                func.cast(Conference.tags, db.String).ilike(f"%{search_query}%")
            )
        )

    # Apply time filter if provided
    now = datetime.now()
    if filter_time == 'past':
        query = query.filter(Conference.end_time < now)
    elif filter_time == 'upcoming':
        query = query.filter(Conference.start_time >= now)

    # Order by start time (earliest first)
    query = query.order_by(Conference.start_time.asc())

    # Apply pagination
    if limit != -1:
        query = query.offset(start).limit(limit)

    return [conference.serialize() for conference in query.all()]

def get_popular_tags(filter_time='all'):
    query = Conference.query

    # Apply time filter if provided
    now = datetime.now()
    if filter_time == 'past':
        query = query.filter(Conference.end_time < now)
    elif filter_time == 'upcoming':
        query = query.filter(Conference.start_time >= now)

    # Count the occurrences of each tag
    tag_counts = {}
    for conference in query.all():
        if conference.tags:
            for tag in conference.tags:
                if tag in tag_counts:
                    tag_counts[tag] += 1
                else:
                    tag_counts[tag] = 1

    # Sort tags by occurrences and get the top 5
    sorted_tags = sorted(tag_counts.items(), key=lambda item: item[1], reverse=True)
    most_popular_tags = [tag for tag, count in sorted_tags[:5]]

    return most_popular_tags

def get_conference_by_id(conference_id):
    conference = Conference.query.get(conference_id)
    if (conference): 
        return conference.serialize()
    return None

def create_conference(data):
    new_conference = Conference(
        organizer_id = data['organizer_id'],
        name = data['name'],
        description = data['description'],
        capacity = data['capacity'],
        cost = data['cost'],
        max_reservations_per_user = data['max_reservations_per_user'],
        current_occupancy = data['current_occupancy'],
        picture = data['picture'],
        tags = data['tags'],
        start_time = data['start_time'],
        end_time = data['end_time'],
    )

    db.session.add(new_conference)
    db.session.commit()
    return new_conference.serialize()

def update_conference(conference_id, data):
    conference = Conference.query.get(conference_id)
    if (conference is None):
        return {'error': 'Conference not found'}, 404
    
    conference.organizer_id = data.get('organizer_id', conference.organizer_id)
    conference.name = data.get('name', conference.name)
    conference.description = data.get('description', conference.description)
    conference.capacity = data.get('capacity', conference.capacity)
    conference.cost = data.get('cost', conference.cost)
    conference.max_reservations_per_user = data.get('max_reservations_per_user', conference.max_reservations_per_user)
    conference.current_occupancy = data.get('current_occupancy', conference.current_occupancy)
    conference.picture = data.get('picture', conference.picture)
    conference.tags = data.get('tags', conference.tags)
    conference.start_time = data.get('start_time', conference.start_time)
    conference.end_time = data.get('end_time', conference.end_time)

    db.session.commit()
    return conference.serialize(), 200

def remove_conference(conference_id):
    conference = Conference.query.get(conference_id)
    if (conference is None):
        return False
    
    db.session.delete(conference)
    db.session.commit()
    return True

def update_current_occupancy(conference_id, change):
    conference = Conference.query.get(conference_id)
    new_occupancy = conference.current_occupancy + change

    conference.current_occupancy = new_occupancy

    db.session.commit() 

def get_current_occupancy(conference_id):
    conference = Conference.query.get(conference_id)
    return conference.current_occupancy

def get_max_occupancy(conference_id):
    conference = Conference.query.get(conference_id)
    return conference.capacity