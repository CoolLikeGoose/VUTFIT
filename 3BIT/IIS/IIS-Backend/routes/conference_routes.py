from flask import Blueprint, request, jsonify
from services.conference_services import *
from services.presentation_services import get_presentations_by_conference, get_unconfirmed_presentations_by_conference, get_confirmed_presentations_by_conference, get_presentations_by_conferenece_user
from services.reservation_services import get_reservations_by_conference, get_reservations_by_conference_user
from services.conference_auditorium_service import *

conference_bp = Blueprint('conference', __name__)


@conference_bp.route('/api/conferences', methods=['GET'])
def get_all_conferences():
    search_query = request.args.get('search', default='', type=str).lower()
    filter_time = request.args.get('filter_time', default='all', type=str)
    start = request.args.get('start', default=0, type=int)
    limit = request.args.get('limit', default=10, type=int)
    tags = request.args.get('tags', default='', type=str)  # Get the tags as a single string

    # Split tags by commas if provided
    tags_list = tags.split(',') if tags else []

    conferences = get_conferences(start, limit, search_query, filter_time, tags=tags_list)

    return jsonify(conferences), 200

@conference_bp.route('/api/conferences/tags/popular', methods=['GET'])
def get_most_popular_tags():
    filter_time = request.args.get('filter_time', default='all', type=str)
    most_popular_tags = get_popular_tags(filter_time)

    return jsonify(most_popular_tags), 200

@conference_bp.route('/api/conferences/<int:conference_id>', methods=['GET'])
def get_single_conference(conference_id):
    conference = get_conference_by_id(conference_id)

    if (conference):
        return jsonify(conference), 200
    return jsonify({'message': 'Conference not found'}), 404

@conference_bp.route('/api/conferences', methods=['POST'])
def add_conference():
    data = request.get_json()
    conference = create_conference(data)

    return jsonify(conference), 201

@conference_bp.route('/api/conferences/<int:conference_id>', methods=['PUT'])
def modify_conference(conference_id):
    data = request.get_json()
    conference, status_code = update_conference(conference_id, data)

    return jsonify(conference), status_code

@conference_bp.route('/api/conferences/<int:conference_id>', methods=['DELETE'])
def delete_conference(conference_id):
    success = remove_conference(conference_id)
    if (success):
        return jsonify({'message': 'Conference deleted successfully'}), 200
    return jsonify({'message': 'Conference not found'}), 404

@conference_bp.route('/api/conferences/<int:conference_id>/<int:user_id>', methods=['GET'])
def get_conference_user_activities(conference_id, user_id):
    presentations = get_presentations_by_conferenece_user(conference_id, user_id)
    reservation = get_reservations_by_conference_user(conference_id, user_id)
    response = {
        "reservation": reservation,
        "presentations": presentations
    }

    return jsonify(response), 200

# get all presentations
@conference_bp.route('/api/conferences/<int:conference_id>/presentations', methods=['GET'])
def get_conference_presentations(conference_id):
    presentations = get_presentations_by_conference(conference_id)

    return jsonify(presentations), 200

# get unconfirmed presentations only
@conference_bp.route('/api/conferences/<int:conference_id>/presentations/unconfirmed', methods=['GET'])
def get_conference_unconfirmed_presentations(conference_id):
    presentations = get_unconfirmed_presentations_by_conference(conference_id)

    return jsonify(presentations), 200

# get confirmed presentations only
@conference_bp.route('/api/conferences/<int:conference_id>/presentations/confirmed', methods=['GET'])
def get_conference_confirmed_presentations(conference_id):
    presentations = get_confirmed_presentations_by_conference(conference_id)

    return jsonify(presentations), 200

# get reservations
@conference_bp.route('/api/conferences/<int:conference_id>/reservations', methods=['GET'])
def get_conference_reservations(conference_id):
    presentations = get_reservations_by_conference(conference_id)

    return jsonify(presentations), 200

# auditorium links
@conference_bp.route('/api/conferences/<int:conference_id>/auditoriums/available', methods=['GET'])
def get_conference_avaibleAuditoriums(conference_id):
    auditoriums = get_available_auditoriums_by_conference(conference_id)

    return jsonify(auditoriums), 200

@conference_bp.route('/api/conferences/<int:conference_id>/auditoriums', methods=['GET'])
def get_conference_auditoriums(conference_id):
    auditoriums = get_auditoriums_by_conference(conference_id)

    return jsonify(auditoriums), 200

@conference_bp.route('/api/conferences/<int:conference_id>/auditoriums/<int:auditorium_id>', methods=['POST'])
def modify_conference_auditoriums(conference_id, auditorium_id):
    success = link_auditorium_to_confernce(auditorium_id, conference_id)

    if (success):
        return jsonify({'message': 'Auditoium linked successfully'}), 200
    return jsonify({'message': 'Auditoium not linked'}), 404

@conference_bp.route('/api/conferences/<int:conference_id>/auditoriums/<int:auditorium_id>', methods=['DELETE'])
def delete_conference_auditoriums(conference_id, auditorium_id):
    success = unlink_auditorium_to_confernce(auditorium_id, conference_id)

    if (success):
        return jsonify({'message': 'Auditoium unlinked successfully'}), 200
    return jsonify({'message': 'Auditoium didnt unlinked'}), 404