# routes/client_routes.py
from flask import Blueprint, request, jsonify
from services.client_service import create_client, get_clients, get_client_by_id, update_client, delete_client
from services.appointment_service import get_appointments_by_client

client_bp = Blueprint('client', __name__)

@client_bp.route('/clients', methods=['GET'])
def get_all_clients():
    """API endpoint to retrieve all clients"""
    clients = get_clients()
    return jsonify(clients), 200

@client_bp.route('/clients/<int:client_id>', methods=['GET'])
def get_single_client(client_id):
    """API endpoint to retrieve a single client by ID"""
    client = get_client_by_id(client_id)
    if client:
        return jsonify(client), 200
    return jsonify({'message': 'Client not found'}), 404

@client_bp.route('/clients', methods=['POST'])
def add_client():
    """API endpoint to create a new client"""
    data = request.get_json()
    client = create_client(data)
    return jsonify(client), 201

@client_bp.route('/clients/<int:client_id>', methods=['PUT'])
def modify_client(client_id):
    """API endpoint to update an existing client"""
    data = request.get_json()
    client, status_code = update_client(client_id, data)  # Get both data and status

    return jsonify(client), status_code  # Return the response with the correct status

@client_bp.route('/clients/<int:client_id>', methods=['DELETE'])
def remove_client(client_id):
    """API endpoint to delete a client"""
    success = delete_client(client_id)
    if success:
        return jsonify({'message': 'Client deleted successfully'}), 200
    return jsonify({'message': 'Client not found'}), 404

@client_bp.route('/clients/<int:client_id>/appointments', methods=['GET'])
def client_appointments(client_id):
    appointments = get_appointments_by_client(client_id)
    return jsonify(appointments), 200