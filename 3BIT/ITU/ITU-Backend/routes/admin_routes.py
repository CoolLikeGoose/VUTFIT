from flask import Blueprint, request, jsonify
from services.admin_service import *

admin_bp = Blueprint('admin', __name__)

@admin_bp.route('/admins', methods=['GET'])
def get_all_admins():
    admins = get_admins()

    return jsonify(admins), 200

@admin_bp.route('/admins/<int:admin_id>', methods=['GET'])
def get_single_admin(admin_id):
    admin = get_admin_by_id(admin_id)

    if (admin):
        return jsonify(admin), 200
    return jsonify({'message': 'Admin not found'}), 404

@admin_bp.route('/admins', methods=['POST'])
def add_admin():
    data = request.get_json()
    admin = create_admin(data)

    return jsonify(admin), 201

@admin_bp.route('/admins/<int:admin_id>', methods=['PUT'])
def modify_admin(admin_id):
    data = request.get_json()
    admin, status_code = update_admin(admin_id, data)

    return jsonify(admin), status_code

@admin_bp.route('/admins/<int:admin_id>', methods=['DELETE'])
def delete_admin(admin_id):
    success = remove_admin(admin_id)
    if (success):
        return jsonify({'message': 'Admin deleted successfully'}), 200
    return jsonify({'message': 'Admin not found'}), 404
