from flask import Blueprint, request, jsonify
from services.threshold_service import *

threshold_bp = Blueprint('threshold', __name__)

@threshold_bp.route('/thresholds', methods=['GET'])
def get_all_thresholds():
    thresholds = get_thresholds()

    return jsonify(thresholds), 200

@threshold_bp.route('/thresholds/<int:threshold_id>', methods=['GET'])
def get_single_threshold(threshold_id):
    threshold = get_threshold_by_id(threshold_id)

    if (threshold):
        return jsonify(threshold), 200
    return jsonify({'message': 'Threshold not found'}), 404

@threshold_bp.route('/thresholds', methods=['POST'])
def add_threshold():
    data = request.get_json()
    threshold = create_threshold(data)

    return jsonify(threshold), 201

@threshold_bp.route('/thresholds/<int:threshold_id>', methods=['PUT'])
def modify_threshold(threshold_id):
    data = request.get_json()
    threshold, status_code = update_threshold(threshold_id, data)

    return jsonify(threshold), status_code

@threshold_bp.route('/thresholds/<int:threshold_id>', methods=['DELETE'])
def delete_threshold(threshold_id):
    success = remove_threshold(threshold_id)
    if (success):
        return jsonify({'message': 'Threshold deleted successfully'}), 200
    return jsonify({'message': 'Threshold not found'}), 404
