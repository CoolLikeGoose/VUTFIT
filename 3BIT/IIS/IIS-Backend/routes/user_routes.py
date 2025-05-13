from flask import Blueprint, request, jsonify
from services.user_service import *
from services.conference_services import get_conferences
from services.presentation_services import get_presentations
from services.reservation_services import get_reservations

user_bp = Blueprint('user', __name__)


@user_bp.route('/api/users', methods=['GET'])
def get_all_users():
    start = request.args.get('start', default=0, type=int)
    limit = request.args.get('limit', default=-1, type=int)
    search = request.args.get('search', default=None, type=str)
    role = request.args.get('role', default=None, type=str)

    users = get_users(start, limit, search, role)

    return jsonify(users), 200

@user_bp.route('/api/users/<int:user_id>', methods=['GET'])
def get_single_user(user_id):
    user = get_user_by_id(user_id)

    if user:
        return jsonify(user), 200
    return jsonify({'message': 'User not found'}), 404

@user_bp.route('/api/users', methods=['POST'])
def add_user():
    data = request.get_json()
    user, status_code = create_user(data)

    # If the returned user is already serialized (in case of an existing user), return it directly
    if isinstance(user, dict):
        user_data = user
    else:
        # If the returned user is an object, serialize it
        user_data = user.serialize()
        user_data.pop('password_hash', None)  # Remove sensitive data like the password hash

    return jsonify(user_data), status_code

@user_bp.route('/api/users/<int:user_id>', methods=['PUT'])
def modify_user(user_id):
    data = request.get_json()
    user, status_code = update_user(user_id, data)

    return jsonify(user), status_code

@user_bp.route('/api/users/<int:user_id>', methods=['DELETE'])
def delete_user(user_id):
    success = remove_user(user_id)
    if success:
        return jsonify({'message': 'User deleted successfully'}), 200
    return jsonify({'message': 'User not found'}), 404

@user_bp.route('/api/users/<int:user_id>/events', methods=['GET'])
def get_users_events(user_id):
    start = request.args.get('start', default=0, type=int)
    limit = request.args.get('limit', default=-1, type=int)
    search_query = request.args.get('search', default=None, type=str)
    filter_time = request.args.get('filter_time', default='all', type=str)
    event_type = request.args.get('event_type', default='all', type=str)

    # Initialize an empty list to collect all events
    all_events = []

    # Fetch only the requested event types based on the event_type parameter
    if event_type in ['all', 'conference']:
        conferences = get_conferences(0, -1, search_query, filter_time, user_id=user_id)  # Fetch all first
        all_events += [{'type': 'conference', **conference} for conference in conferences]

    if event_type in ['all', 'presentation']:
        presentations = get_presentations(0, -1, search_query, filter_time, user_id=user_id)  # Fetch all first
        all_events += [{'type': 'presentation', **presentation} for presentation in presentations]

    if event_type in ['all', 'reservation']:
        reservations = get_reservations(0, -1, search_query, filter_time, user_id=user_id)  # Fetch all first
        all_events += [{'type': 'reservation', **reservation} for reservation in reservations]

    # Sort all events by their start time
    all_events.sort(key=lambda x: x.get('start_time') or x.get('conference', {}).get('start_time'))

    # Apply pagination after sorting
    total_events = len(all_events)

    # Apply pagination only if the limit is not -1 (which means get all)
    if limit != -1:
        all_events = all_events[start:start + limit]

    # Prepare metadata for the response
    metadata = {
        "total": total_events,
        "start": start,
        "limit": limit,
        "returned": len(all_events)
    }

    return jsonify({"metadata": metadata, "events": all_events}), 200

@user_bp.route('/api/users/<int:user_id>/password', methods=['PUT'])
def change_password(user_id):
    data = request.get_json()
    current_password = data.get('currentPassword')
    new_password = data.get('newPassword')

    # Retrieve the actual user model instance
    user_model = User.query.get(user_id)
    if not user_model:
        return jsonify({'message': 'User not found'}), 404

    # Verify the current password
    if not verify_password(user_model.password_hash, current_password):
        return jsonify({'message': 'Current password is incorrect'}), 400

    # Update the password hash
    user_model.password_hash = generate_password_hash(new_password)
    db.session.commit()

    return jsonify({'message': 'Password updated successfully'}), 200

