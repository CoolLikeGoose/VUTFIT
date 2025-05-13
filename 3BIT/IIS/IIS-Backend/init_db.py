# Import necessary models and datetime
from db import db
from datetime import datetime
from db.auditorium_model import Auditorium
from db.ConferenceAuditorium import ConferenceAuditorium
from db.user_model import User
from db.conference_model import Conference
from db.presentation_model import Presentation
from db.reservation_model import Reservation
from werkzeug.security import generate_password_hash


def populate_db():
    """Populate the database with initial data."""

    print("Deleting existing data...")

    # Delete all records from dependent tables first to avoid foreign key constraint issues
    ConferenceAuditorium.query.delete()
    Reservation.query.delete()
    Presentation.query.delete()
    Conference.query.delete()
    Auditorium.query.delete()
    User.query.delete()
    db.session.commit()

    print("Populating new data...")

    # Populate Auditoriums
    auditoriums_data = [
        {"name": "D105", "capacity": 300},
        {"name": "D206", "capacity": 40},
        {"name": "D207", "capacity": 100},
        {"name": "B101", "capacity": 50},
        {"name": "A304", "capacity": 150},
    ]
    auditoriums = [Auditorium(**data) for data in auditoriums_data]
    db.session.add_all(auditoriums)
    db.session.commit()

    # Populate Users with password hashing
    users_data = [
        {"name": "Alice Johnson", "email": "alice.johnson@example.com", "role": "user", "picture": "https://static.vecteezy.com/system/resources/thumbnails/038/974/578/small_2x/ai-generated-professional-portrait-of-a-competent-woman-free-photo.jpg", "password": "alice123"},
        {"name": "Bob Smith", "email": "bob.smith@example.com", "role": "user", "picture": "https://htmlstream.com/preview/unify-v2.6.2/assets/img-temp/400x450/img5.jpg", "password": "bob123"},
        {"name": "Charlie Brown", "email": "charlie.brown@example.com", "role": "user", "picture": "https://preview.keenthemes.com/metronic-v4/theme/assets/pages/media/profile/people19.png", "password": "charlie123"},
        {"name": "Dana White", "email": "dana.white@example.com", "role": "admin", "picture": "https://variety.com/wp-content/uploads/2018/10/dana_white.png", "password": "dana123"},
        {"name": "Evan Green", "email": "evan.green@example.com", "role": "user", "picture": "https://t3.ftcdn.net/jpg/04/60/91/88/360_F_460918802_XVCymFr7MoziFpnInbTDvrlblYhvAOi2.jpg", "password": "evan123"},
    ]

    for i in range(6, 21):
        users_data.append({
            "name": f"User{i}",
            "email": f"user{i}@example.com",
            "role": "user",
            "picture": f"https://example.com/pictures/user{i}.jpg",
            "password": f"user{i}123"
        })

    users = []
    for data in users_data:
        password_hash = generate_password_hash(data["password"])
        user = User(
            name=data["name"],
            email=data["email"],
            role=data["role"],
            picture=data["picture"],
            password_hash=password_hash
        )
        users.append(user)

    db.session.add_all(users)
    db.session.commit()

    # Now we can safely refer to users when adding conferences
    alice = User.query.filter_by(email="alice.johnson@example.com").first()
    bob = User.query.filter_by(email="bob.smith@example.com").first()
    charlie = User.query.filter_by(email="charlie.brown@example.com").first()

    # Populate Conferences
    conferences_data = [
        {
            "organizer_id": alice.id,
            "name": "Tech Innovations 2024",
            "description": "Annual conference on the latest in technology and innovation.",
            "capacity": 10,
            "cost": 150,
            "max_reservations_per_user": 3,
            "current_occupancy": 0,
            "picture": "https://weareprimegroup.com/_nuxt/image/f69e30.jpeg",
            "tags": ["technology", "innovation", "2024"],
            "start_time": datetime(2025, 1, 10, 9, 0),
            "end_time": datetime(2025, 1, 10, 17, 0),
        },
        {
            "organizer_id": alice.id,
            "name": "Old Conference",
            "description": "Some old conference. example",
            "capacity": 10,
            "cost": 150,
            "max_reservations_per_user": 3,
            "current_occupancy": 0,
            "picture": "https://images.theconversation.com/files/393210/original/file-20210401-13-z6rl6z.jpg?ixlib=rb-4.1.0&rect=0%2C292%2C3008%2C1504&q=45&auto=format&w=1356&h=668&fit=crop",
            "tags": ['innovation'],
            "start_time": datetime(2024, 1, 10, 9, 0),
            "end_time": datetime(2024, 1, 10, 17, 0),
        },
        {
            "organizer_id": alice.id,
            "name": "Healthcare Advances 2024",
            "description": "Exploring the latest trends in healthcare technology.",
            "capacity": 10,
            "cost": 100,
            "max_reservations_per_user": 2,
            "current_occupancy": 0,
            "picture": "https://assets.contenthub.wolterskluwer.com/api/public/content/2336569-female-doctor-using-a-digital-tablet-in-a-modern-hospital-16a7c0e8ea?v=dc468bea",
            "tags": ["healthcare", "innovation"],
            "start_time": datetime(2025, 1, 15, 9, 0),
            "end_time": datetime(2025, 1, 15, 17, 0),
        },
        {
            "organizer_id": alice.id,
            "name": "Green Energy Summit",
            "description": "Conference on renewable energy sources and sustainability.",
            "capacity": 30,
            "cost": 120,
            "max_reservations_per_user": 2,
            "current_occupancy": 0,
            "picture": "https://www.twi-global.com/CachedImage.axd?ImageName=Wind-Turbines.jpg&ImageWidth=784&ImageHeight=379&ImageVersionID=98786&ImageModified=20200624095115",
            "tags": ["energy", "sustainability"],
            "start_time": datetime(2025, 2, 5, 9, 0),
            "end_time": datetime(2025, 2, 5, 17, 0),
        },
        {
            "organizer_id": alice.id,
            "name": "AI and Ethics",
            "description": "Discussion on the ethical implications of artificial intelligence.",
            "capacity": 20,
            "cost": 80,
            "max_reservations_per_user": 1,
            "current_occupancy": 0,
            "picture": "https://eu-images.contentstack.com/v3/assets/blt6b0f74e5591baa03/blt7ff31ef3cc9e7065/65ae97402bb7f9040a716275/News_Image_-_2024-01-22T102627.569.png?disable=upscale&width=1200&height=630&fit=crop",
            "tags": ["ai", "ethics"],
            "start_time": datetime(2025, 3, 10, 9, 0),
            "end_time": datetime(2025, 3, 10, 17, 0),
        },
        {
            "organizer_id": alice.id,
            "name": "Blockchain for Business",
            "description": "How blockchain technology can be applied to business scenarios.",
            "capacity": 25,
            "cost": 200,
            "max_reservations_per_user": 2,
            "current_occupancy": 0,
            "picture": "https://cdn.media.amplience.net/i/epammarketplace/blockchain-uses-cryptocurrency?maxW=1200&qlt=80&fmt=jpg&bg=rgb(255,255,255)&v=126375874593",
            "tags": ["blockchain", "business"],
            "start_time": datetime(2025, 4, 20, 9, 0),
            "end_time": datetime(2025, 4, 20, 17, 0),
        },
        {
            "organizer_id": alice.id,
            "name": "Virtual Reality in Education",
            "description": "The impact of VR technologies in the education sector.",
            "capacity": 15,
            "cost": 50,
            "max_reservations_per_user": 1,
            "current_occupancy": 0,
            "picture": "https://www.computeam.co.uk/img/dynamic/1000/w/90/what-is-the-future-of-virtual-reality-in-education-34363342.jpg",
            "tags": ["vr", "education"],
            "start_time": datetime(2025, 5, 25, 9, 0),
            "end_time": datetime(2025, 5, 25, 17, 0),
        },
        {
            "organizer_id": alice.id,
            "name": "Space Exploration Forum",
            "description": "A deep dive into recent advancements in space exploration.",
            "capacity": 40,
            "cost": 180,
            "max_reservations_per_user": 2,
            "current_occupancy": 0,
            "picture": "https://res.cloudinary.com/momentum-media-group-pty-ltd/image/upload/v1686795211/Space%20Connect/space-exploration-sc_fm1ysf.jpg",
            "tags": ["space", "exploration"],
            "start_time": datetime(2025, 6, 5, 9, 0),
            "end_time": datetime(2025, 6, 5, 17, 0),
        },
    ]
    conferences = [Conference(**data) for data in conferences_data]
    db.session.add_all(conferences)
    db.session.commit()

    # Populate Reservations
    users = User.query.all()
    tech_conference = Conference.query.filter_by(name="Tech Innovations 2024").first()
    healthcare_conference = Conference.query.filter_by(name="Healthcare Advances 2024").first()

    # Create reservations for "Tech Innovations 2024" to make it sold out
    reservations_data = []
    for i in range(10):
        reservations_data.append({
            "conference_id": healthcare_conference.id,
            "user_id": users[i].id,
            "visitors_count": 1,
            "status": "approved",
        })

    reservations_data.append({
        "conference_id": tech_conference.id,
        "user_id": charlie.id,
        "visitors_count": 2,
        "status": "approved",
    })

    reservations = [Reservation(**data) for data in reservations_data]
    db.session.add_all(reservations)
    db.session.commit()

    # Update conference occupancy based on actual reservations
    tech_conference.current_occupancy = len([r for r in reservations if r.conference_id == tech_conference.id])
    healthcare_conference.current_occupancy = len([r for r in reservations if r.conference_id == healthcare_conference.id])
    db.session.commit()

    # Populate Presentations
    presentations_data = [
        {
            "speaker_id": bob.id,
            "conference_id": tech_conference.id,
            "name": "The Future of AI",
            "description": "Exploring advancements and ethical considerations in artificial intelligence.",
            "picture": "https://imageio.forbes.com/specials-images/imageserve/66bee357cf48b97789cbc270/0x0.jpg?format=jpg&height=900&width=1600&fit=bounds",
            "presentation_text": "In this talk, we will dive into the latest breakthroughs in AI...",
            "auditorium_id": auditoriums[0].id,
            "status": "confirmed",
            "start_time": datetime(2025, 1, 10, 10, 0),
            "end_time": datetime(2025, 1, 10, 11, 0),
        }
    ]
    presentations = [Presentation(**data) for data in presentations_data]
    db.session.add_all(presentations)
    db.session.commit()

    # Link Conferences to Auditoriums
    conference_auditorium_data = [
        {
            "conference_id": tech_conference.id,
            "auditorium_id": auditoriums[0].id
        },
        {
            "conference_id": healthcare_conference.id,
            "auditorium_id": auditoriums[1].id
        }
    ]
    conference_auditoriums = [ConferenceAuditorium(**data) for data in conference_auditorium_data]
    db.session.add_all(conference_auditoriums)
    db.session.commit()

    print("Database initialized successfully.")
