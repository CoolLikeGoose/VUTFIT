from flask import Blueprint, request, jsonify
from services.appointment_service import *

appointment_bp = Blueprint('appointment', __name__)


@appointment_bp.route('/appointments', methods=['GET'])
def get_all_appointments():
    # Extract query parameters
    start = request.args.get('start')
    end = request.args.get('end')
    providers = request.args.get('providers')
    calendars = request.args.get('calendars')

    if start and end and providers:
        # Parse providers to a list of integers
        provider_ids = [int(id) for id in providers.split(',')]

        # Parse calendars to a list of integers if provided
        calendar_ids = [int(id) for id in calendars.split(',')] if calendars else None

        appointments = get_appointments_by_criteria(start, end, provider_ids, calendar_ids)
    else:
        # Return all appointments if no filters are provided
        appointments = get_appointments()

    return jsonify(appointments), 200

@appointment_bp.route('/appointments/<int:appointment_id>', methods=['GET'])
def get_single_appointment(appointment_id):
    appointment = get_appointment_by_id(appointment_id)

    if (appointment):
        return jsonify(appointment), 200
    return jsonify({'message': 'Appointment not found'}), 404

@appointment_bp.route('/appointments', methods=['POST'])
def add_appointment():
    data = request.get_json()
    appointment = create_appointment(data)

    return jsonify(appointment), 201

@appointment_bp.route('/appointments/<int:appointment_id>', methods=['PUT'])
def modify_appointment(appointment_id):
    data = request.get_json()
    appointment, status_code = update_appointment(appointment_id, data)

    return jsonify(appointment), status_code

@appointment_bp.route('/appointments/<int:appointment_id>', methods=['DELETE'])
def delete_appointment(appointment_id):
    success = remove_appointment(appointment_id)
    if (success):
        return jsonify({'message': 'Appointment deleted successfully'}), 200
    return jsonify({'message': 'Appointment not found'}), 404


@appointment_bp.route('/appointments/<int:appointment_id>/status', methods=['PUT'])
def update_appointment_status(appointment_id):
    data = request.get_json()
    status = data.get("status")

    if not status:
        return jsonify({"error": "Status is required"}), 400

    appointment, status_code = update_appointment_status_service(appointment_id, status)
    return jsonify(appointment), status_code

#TODO:api/appointments