from flask import Blueprint, request, jsonify
from services.reservation_services import *

reservation_bp = Blueprint('reservation', __name__)

@reservation_bp.route('/api/reservations', methods=['GET'])
def get_all_reservations():
    search_query = request.args.get('search', default='', type=str).lower()
    filter_time = request.args.get('filter_time', default='all', type=str)
    start = request.args.get('start', default=0, type=int)
    limit = request.args.get('limit', default=10, type=int)

    reservations = get_reservations(start, limit, search_query, filter_time)

    return jsonify(reservations), 200

@reservation_bp.route('/api/reservations/<int:reservation_id>', methods=['GET'])
def get_single_reservation(reservation_id):
    reservation = get_reservation_by_id(reservation_id)

    if (reservation):
        return jsonify(reservation), 200
    return jsonify({'message': 'Reservation not found'}), 404

@reservation_bp.route('/api/reservations', methods=['POST'])
def add_reservation():
    data = request.get_json()
    reservation, status_code = create_reservation(data)

    return jsonify(reservation), status_code

@reservation_bp.route('/api/reservations/<int:reservation_id>', methods=['PUT'])
def modify_reservation(reservation_id):
    data = request.get_json()
    reservation, status_code = update_reservation(reservation_id, data)

    return jsonify(reservation), status_code

@reservation_bp.route('/api/reservations/<int:reservation_id>', methods=['DELETE'])
def delete_reservation(reservation_id):
    success = remove_reservation(reservation_id)
    if (success):
        return jsonify({'message': 'Reservation deleted successfully'}), 200
    return jsonify({'message': 'Reservation not found'}), 404