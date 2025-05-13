from flask import Blueprint, request, jsonify
from services.presentation_services import *

presentation_bp = Blueprint('presentation', __name__)

@presentation_bp.route('/api/presentations', methods=['GET'])
def get_all_presentations():
    search_query = request.args.get('search', default='', type=str).lower()
    filter_time = request.args.get('filter_time', default='all', type=str)
    start = request.args.get('start', default=0, type=int)
    limit = request.args.get('limit', default=10, type=int)

    presentations = get_presentations(start, limit, search_query, filter_time)

    return jsonify(presentations), 200

@presentation_bp.route('/api/presentations/<int:presentation_id>', methods=['GET'])
def get_single_presentation(presentation_id):
    presentation = get_presentation_by_id(presentation_id)

    if (presentation):
        return jsonify(presentation), 200
    return jsonify({'message': 'Presentation not found'}), 404

@presentation_bp.route('/api/presentations', methods=['POST'])
def add_presentation():
    data = request.get_json()
    presentation = create_presentation(data)

    return jsonify(presentation), 201

@presentation_bp.route('/api/presentations/<int:presentation_id>', methods=['PUT'])
def modify_presentation(presentation_id):
    data = request.get_json()
    presentation, status_code = update_presentation(presentation_id, data)

    return jsonify(presentation), status_code

@presentation_bp.route('/api/presentations/<int:presentation_id>', methods=['DELETE'])
def delete_presentation(presentation_id):
    success = remove_presentation(presentation_id)
    if (success):
        return jsonify({'message': 'Presentation deleted successfully'}), 200
    return jsonify({'message': 'Presentation not found'}), 404

#TODO: link presentation to auditorium