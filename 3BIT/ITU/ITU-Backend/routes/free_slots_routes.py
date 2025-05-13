from flask import Blueprint, request, jsonify
from services.free_slots_service import get_free_slots

free_slots_bp = Blueprint('free-slots', __name__)

@free_slots_bp.route('/free-slots', methods=['GET'])
def get_free_slots_route():
    start = request.args.get('start')
    end = request.args.get('end')
    provider_ids = request.args.get('providers')
    appointment_duration = request.args.get('appointment_duration')
    appointment_id = request.args.get('appointment_id')  # Optional parameter

    # Validate input
    if not (start and end and provider_ids and appointment_duration):
        return jsonify({"error": "Missing required parameters"}), 400

    try:
        # Parse provider_ids as a list of integers
        provider_ids = [int(id) for id in provider_ids.split(',')]
        appointment_duration = int(appointment_duration)
        appointment_id = int(appointment_id) if appointment_id else None  # Convert to int if provided

        # Calculate free slots for each provider, excluding the specified appointment if provided
        free_slots = {
            provider_id: get_free_slots(start, end, provider_id, appointment_duration, appointment_id)
            for provider_id in provider_ids
        }

        return jsonify(free_slots), 200
    except Exception as e:
        return jsonify({"error": str(e)}), 500
