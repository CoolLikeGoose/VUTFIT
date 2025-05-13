from db import db
from db.admin_model import Admin

def get_admins():
    admins = Admin.query.all()
    return [admin.serialize() for admin in admins]

def get_admin_by_id(admin_id):
    admin = Admin.query.get(admin_id)
    if (admin): 
        return admin.serialize()
    return None

def create_admin(data):
    new_admin = Admin(
        name = data["name"],
        email = data["email"],
        password_hash = data["password_hash"]
    )
    db.session.add(new_admin)
    db.session.commit()
    return new_admin.serialize()

def update_admin(admin_id, data):
    admin = Admin.query.get(admin_id)
    if (admin is None):
        return {'error': 'Admin not found'}, 404
    
    admin.name = data.get('name', admin.name)
    admin.email = data.get('email', admin.email)
    admin.password_hash = data.get('password_hash', admin.password_hash)
    
    db.session.commit()
    return admin.serialize(), 200

def remove_admin(admin_id):
    admin = Admin.query.get(admin_id)
    if (admin is None):
        return False
    
    db.session.delete(admin)
    db.session.commit()
    return True