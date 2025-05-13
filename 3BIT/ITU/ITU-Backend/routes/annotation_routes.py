from flask import Blueprint, request, jsonify
from services.annotation_service import *

annotation_bp = Blueprint('annotation', __name__)

@annotation_bp.route('/annotations', methods=['GET'])
def get_all_annotations():
    annotations = get_annotations()

    return jsonify(annotations), 200

@annotation_bp.route('/annotations/<int:annotation_id>', methods=['GET'])
def get_single_annotation(annotation_id):
    annotation = get_annotation_by_id(annotation_id)

    if (annotation):
        return jsonify(annotation), 200
    return jsonify({'message': 'Annotation not found'}), 404

@annotation_bp.route('/annotations', methods=['POST'])
def add_annotation():
    data = request.get_json()
    annotation = create_annotation(data)

    return jsonify(annotation), 201

@annotation_bp.route('/annotations/<int:annotation_id>', methods=['PUT'])
def modify_annotation(annotation_id):
    data = request.get_json()
    annotation, status_code = update_annotation(annotation_id, data)

    return jsonify(annotation), status_code

@annotation_bp.route('/annotations/<int:annotation_id>', methods=['DELETE'])
def delete_annotation(annotation_id):
    success = remove_annotation(annotation_id)
    if (success):
        return jsonify({'message': 'Annotation deleted successfully'}), 200
    return jsonify({'message': 'Annotation not found'}), 404
