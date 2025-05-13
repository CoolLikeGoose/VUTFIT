from flask import Blueprint, request, jsonify
from services.user_service import get_user_by_email, verify_password, create_user, get_user_by_id
from werkzeug.security import generate_password_hash, check_password_hash

auth_bp = Blueprint('auth', __name__)

@auth_bp.route('/api/login', methods=['POST'])
def login():
    data = request.get_json()
    email = data.get('email')
    password = data.get('password')
    user = get_user_by_email(email)  # Fetch the user model instead of the serialized version

    if user:
        print(f"Stored password hash: {user.password_hash}")  # Debug statement
        print(f"Provided password: {password}")  # Debug statement

    if user and verify_password(user.password_hash, password):
        user_data = user.serialize()  # Only serialize after password check
        user_data.pop('password_hash', None)  # Remove the password hash before returning user data
        return jsonify(user_data), 200
    return jsonify({'message': 'Invalid email or password'}), 401

@auth_bp.route('/api/signup', methods=['POST'])
def signup():
    data = request.get_json()
    existing_user = get_user_by_email(data.get('email'))
    if existing_user and existing_user.password_hash != "":
        return jsonify({'message': 'User already exists'}), 409

    # Create the user and get both user data and status code
    user, status_code = create_user(data)

    # Serialize user data
    user_data = user.serialize()  # Serialize the user data to match the response from login
    user_data.pop('password_hash', None)  # Remove sensitive data like the password hash

    return jsonify(user_data), status_code


@auth_bp.route('/api/auth/validate', methods=['GET'])
def validate_user():
    user_id = request.headers.get('User-ID')
    if not user_id:
        return jsonify({'message': 'Invalid request'}), 401

    user = get_user_by_id(user_id)  # Fetch the user by ID
    if user:
        user_data = user
        user_data.pop('password_hash', None)  # Remove sensitive data
        return jsonify(user_data), 200
    return jsonify({'message': 'User not found or deleted'}), 401
