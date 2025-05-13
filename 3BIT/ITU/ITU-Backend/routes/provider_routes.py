from flask import Blueprint, request, jsonify
from services.provider_service import *

provider_bp = Blueprint('provider', __name__)

@provider_bp.route('/providers', methods=['GET'])
def get_all_providers():
    providers = get_providers()
    return jsonify(providers), 200

@provider_bp.route('/providers/<int:provider_id>', methods=['GET'])
def get_single_provider(provider_id):
    provider = get_provider_by_id(provider_id)

    if (provider):
        return jsonify(provider), 200
    return jsonify({'message': 'Provider not found'}), 404

@provider_bp.route('/providers', methods=['POST'])
def add_provider():
    data = request.get_json()
    provider = create_provider(data)
    
    return jsonify(provider), 201

@provider_bp.route('/providers/<int:provider_id>', methods=['PUT'])
def modify_provider(provider_id):
    data = request.get_json()
    provider, status_code = update_provider(provider_id, data)
    
    return jsonify(provider), status_code

@provider_bp.route('/providers/<int:provider_id>', methods=['DELETE'])
def delete_provider(provider_id):
    success = remove_provider(provider_id)
    if (success):
        return jsonify({'message': 'Provider deleted successfully'}), 200
    return jsonify({'message': 'Provider not found'}), 404

# Extend provider routes to manage availability
@provider_bp.route('/providers/<int:provider_id>/availability', methods=['GET'])
def get_provider_availability(provider_id):
    availability = get_availability_for_provider(provider_id)
    return jsonify(availability), 200

@provider_bp.route('/providers/<int:provider_id>/availability', methods=['POST'])
def add_provider_availability(provider_id):
    data = request.get_json()
    availability = create_provider_availability(provider_id, data)
    return jsonify(availability), 201

@provider_bp.route('/providers/<int:provider_id>/availability/<int:availability_id>', methods=['DELETE'])
def delete_provider_availability(provider_id, availability_id):
    success = remove_provider_availability(provider_id, availability_id)
    if success:
        return jsonify({'message': 'Provider availability deleted successfully'}), 200
    return jsonify({'message': 'Availability entry not found'}), 404



#TODO: api/providers
# GET /api/providers/:provider_id/availability
# POST /api/providers/:provider_id/availability
# PUT /api/providers/:provider_id/availability/:availability_id
# DELETE /api/providers/:provider_id/availability/:availability_id