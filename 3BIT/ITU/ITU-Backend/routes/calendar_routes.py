from flask import Blueprint, request, jsonify
from services.calendar_service import *

calendar_bp = Blueprint('calendar', __name__)

@calendar_bp.route('/calendars', methods=['GET'])
def get_all_calendars():
    calendars = get_calendars()

    return jsonify(calendars), 200

@calendar_bp.route('/calendars/<int:calendar_id>', methods=['GET'])
def get_single_calendar(calendar_id):
    calendar = get_calendar_by_id(calendar_id)

    if (calendar):
        return jsonify(calendar), 200
    return jsonify({'message': 'Calendar not found'}), 404

@calendar_bp.route('/calendars', methods=['POST'])
def add_calendar():
    data = request.get_json()
    calendar = create_calendar(data)

    return jsonify(calendar), 201

@calendar_bp.route('/calendars/<int:calendar_id>', methods=['PUT'])
def modify_calendar(calendar_id):
    data = request.get_json()
    calendar, status_code = update_calendar(calendar_id, data)

    return jsonify(calendar), status_code

@calendar_bp.route('/calendars/<int:calendar_id>', methods=['DELETE'])
def delete_calendar(calendar_id):
    success = remove_calendar(calendar_id)
    if (success):
        return jsonify({'message': 'Calendar deleted successfully'}), 200
    return jsonify({'message': 'Calendar not found'}), 404

# Extend calendar routes to manage provider assignments to calendars
@calendar_bp.route('/calendars/<int:calendar_id>/providers', methods=['GET'])
def get_providers_by_calendar(calendar_id):
    providers = get_providers_for_calendar(calendar_id)
    return jsonify(providers), 200

@calendar_bp.route('/calendars/<int:calendar_id>/providers', methods=['POST'])
def add_provider_to_calendar(calendar_id):
    data = request.get_json()
    provider_id = data.get('provider_id')
    provider_calendar = assign_provider_to_calendar(calendar_id, provider_id)
    return jsonify(provider_calendar), 201

@calendar_bp.route('/calendars/<int:calendar_id>/providers/<int:provider_id>', methods=['DELETE'])
def remove_provider_from_calendar(calendar_id, provider_id):
    success = unassign_provider_from_calendar(calendar_id, provider_id)
    if success:
        return jsonify({'message': 'Provider removed from calendar successfully'}), 200
    return jsonify({'message': 'Provider or calendar not found'}), 404


#TODO: api/calendars
#GET /api/calendars/:calendar_id/providers
#GET /api/calendars/:calendar_id/available-slots?start_date=&end_date=

# POST /api/calendars/:id/providers
# DELETE /api/calendars/:id/providers/:provider_id