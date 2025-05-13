import json

from db import db
from db.client_model import Client
from flask import jsonify, make_response


def create_client(data):
    """Create a new client in the database or update fields if it already exists"""
    # Check if a client with this email already exists
    existing_client = Client.query.filter_by(email=data['email']).first()
    if existing_client:
        # Update fields that are different from the request
        updated = False
        if existing_client.name != data.get('name'):
            existing_client.name = data['name']
            updated = True
        if existing_client.phone != data.get('phone'):
            existing_client.phone = data['phone']
            updated = True
        if existing_client.custom_fields != data.get('custom_fields', existing_client.custom_fields):
            existing_client.custom_fields = data.get('custom_fields', existing_client.custom_fields)
            updated = True
        if updated:
            db.session.commit()  # Commit only if there are updates
        return existing_client.serialize()  # Return the existing (potentially updated) client

    # If client does not exist, create a new one
    new_client = Client(
        name=data['name'],
        email=data['email'],
        phone=data['phone'],
        custom_fields=data.get('custom_fields', {})
    )
    db.session.add(new_client)
    db.session.commit()
    return new_client.serialize()

def get_clients():
    """Retrieve all clients from the database"""
    clients = Client.query.all()
    return [client.serialize() for client in clients]

def get_client_by_id(client_id):
    """Retrieve a single client by ID"""
    client = Client.query.get(client_id)
    if client:
        return client.serialize()
    return None

def update_client(client_id, data):
    """Update an existing client"""
    client = Client.query.get(client_id)
    if client:
        # Check if the new email already exists for another client
        if 'email' in data:
            existing_client = Client.query.filter_by(email=data['email']).first()
            if existing_client and existing_client.id != client.id:
                return {'error': 'A client with this email already exists.'}, 409

        # Update client information
        client.name = data.get('name', client.name)
        client.email = data.get('email', client.email)
        client.phone = data.get('phone', client.phone)
        client.custom_fields = data.get('custom_fields', client.custom_fields)
        db.session.commit()
        return client.serialize(), 200  # Return the updated client and 200 OK status

    # If the client is not found
    return {'error': 'Client not found'}, 404

def delete_client(client_id):
    """Delete a client by ID"""
    client = Client.query.get(client_id)
    if client:
        db.session.delete(client)
        db.session.commit()
        return True
    return False
