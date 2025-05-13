from db import db
from db.user_model import User
from werkzeug.security import check_password_hash, generate_password_hash
import random
import string


def get_users(start, limit, search=None, role=None):
    query = User.query

    # Apply search filter if provided
    if search:
        query = query.filter((User.name.ilike(f"%{search}%")) | (User.email.ilike(f"%{search}%")))

    # Apply role filter if provided
    if role and role != "all":
        query = query.filter_by(role=role)

    # Apply pagination
    if limit != -1:
        query = query.offset(start).limit(limit)

    users = query.all()

    return [user.serialize() for user in users]

def get_user_by_id(user_id):
    user = User.query.get(user_id)
    if user:
        return user.serialize()
    return None


def get_user_by_email(email):
    user = User.query.filter_by(email=email).first()
    return user


def create_user(data):
    existing_user = User.query.filter_by(email=data['email']).first()

    if existing_user and existing_user.password_hash != "":
        # Return user data with a message indicating the user already exists
        response_data = existing_user.serialize()
        response_data['note'] = 'User with this email already exists.'
        return response_data, 409

    # Generate a random password if none is provided
    password = data.get('password')
    if password:
        pass
    else:
        password = ''.join(random.choices(string.ascii_letters + string.digits, k=10))
        print(f"Generated password for new user: {password}")

    password_hash = ""
    if password != " ":
        password_hash = generate_password_hash(password)

    new_user = None
    if existing_user:
        existing_user.name = data.get('name', existing_user.name)
        existing_user.password_hash = password_hash if password_hash != "" else existing_user.password_hash
    else:
        new_user = User(
            name=data['name'],
            email=data['email'],
            role=data['role'],
            password_hash=password_hash
        )
        db.session.add(new_user)

    db.session.commit()

    # Return newly created user data
    return new_user if new_user else existing_user , 201


def update_user(user_id, data):
    user = User.query.get(user_id)
    if user is None:
        return {'error': 'User not found'}, 404

    # Check if email is provided and needs to be updated
    if 'email' in data and data['email']:
        new_email = data['email']
        if new_email != user.email:  # Proceed only if the new email is different
            existing_user = User.query.filter_by(email=new_email).first()
            if existing_user and existing_user.id != user.id:
                return {'message': 'User with this email already exists.'}, 409
            user.email = new_email  # Update user's email

    user.name = data.get('name', user.name)
    user.role = data.get('role', user.role)

    # Update picture if provided
    if 'picture' in data:
        user.picture = data['picture']

    # Only hash the password if a new one is provided
    if 'password' in data and data['password']:
        user.password_hash = generate_password_hash(data['password'])

    db.session.commit()
    return user.serialize(), 200



def remove_user(user_id):
    user = User.query.get(user_id)
    if user is None:
        return False
    db.session.delete(user)
    db.session.commit()
    return True


def verify_password(stored_password_hash, provided_password):
    # Remove leading/trailing spaces from provided password
    provided_password = provided_password.strip()

    result = check_password_hash(stored_password_hash, provided_password)
    return result
