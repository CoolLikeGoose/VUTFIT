from flask import Blueprint, request, jsonify
from services.auditorium_service import *

auditorium_bp = Blueprint('auditorium', __name__)


@auditorium_bp.route('/api/auditoriums', methods=['GET'])
def get_all_auditoriums():
    start = request.args.get('start', default=0, type=int)
    limit = request.args.get('limit', default=-1, type=int)
    search = request.args.get('search', default=None, type=str)
    min_capacity = request.args.get('min_capacity', default=None, type=int)

    auditoriums = get_auditoriums(start, limit, search, min_capacity)

    return jsonify(auditoriums), 200

@auditorium_bp.route('/api/auditoriums/<int:auditorium_id>', methods=['GET'])
def get_single_auditorium(auditorium_id):
    auditorium = get_auditorium_by_id(auditorium_id)

    if (auditorium):
        return jsonify(auditorium), 200
    return jsonify({'message': 'Auditorium not found'}), 404

@auditorium_bp.route('/api/auditoriums', methods=['POST'])
def add_auditorium():
    data = request.get_json()
    auditorium, status_code = create_auditorium(data)

    return jsonify(auditorium), status_code

@auditorium_bp.route('/api/auditoriums/<int:auditorium_id>', methods=['PUT'])
def modify_auditorium(auditorium_id):
    data = request.get_json()
    auditorium, status_code = update_auditorium(auditorium_id, data)

    return jsonify(auditorium), status_code

@auditorium_bp.route('/api/auditoriums/<int:auditorium_id>', methods=['DELETE'])
def delete_auditorium(auditorium_id):
    success = remove_auditorium(auditorium_id)
    if (success):
        return jsonify({'message': 'Auditorium deleted successfully'}), 200
    return jsonify({'message': 'Auditorium not found'}), 404
