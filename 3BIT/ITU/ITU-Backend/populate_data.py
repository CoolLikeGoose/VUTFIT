from datetime import datetime, timedelta
from db import db
from db.provider_model import Provider
from db.appointment_model import Appointment
from db.client_model import Client
from db.calendar_model import Calendar
from db.provider_calendar_model import ProviderCalendar
from db.provider_availability_model import ProviderAvailability
import random
import json

def populate_data():
    print("Deleting existing data...")

    # Delete all records from tables in the correct order to avoid foreign key constraint issues
    Appointment.query.delete()
    ProviderAvailability.query.delete()
    ProviderCalendar.query.delete()
    Calendar.query.delete()
    Client.query.delete()
    Provider.query.delete()
    db.session.commit()

    print("Populating new data...")

    # Populate Providers
    providers = [
        Provider(name="Dr. John Doe", contact_info="john.doe@example.com", created_at=datetime(2023, 1, 1),
                 updated_at=datetime(2023, 1, 1)),
        Provider(name="Dr. Jane Smith", contact_info="jane.smith@example.com", created_at=datetime(2023, 1, 2),
                 updated_at=datetime(2023, 1, 2)),
        Provider(name="Dr. Emily Johnson", contact_info="emily.johnson@example.com", created_at=datetime(2023, 1, 3),
                 updated_at=datetime(2023, 1, 3)),
        Provider(name="Dr. Michael Brown", contact_info="michael.brown@example.com", created_at=datetime(2023, 1, 4),
                 updated_at=datetime(2023, 1, 4)),
        Provider(name="Dr. Sarah Lee", contact_info="sarah.lee@example.com", created_at=datetime(2023, 1, 5),
                 updated_at=datetime(2023, 1, 5)),
    ]
    db.session.add_all(providers)
    db.session.commit()

    # Populate Clients with custom fields
    clients = [
        Client(name="James Robinson", email="james.robinson@example.com", phone="4208529637412",
               custom_fields=json.dumps({"age": 35, "occupation": "Engineer"}), created_at=datetime(2023, 1, 9),
               updated_at=datetime(2023, 1, 9)),
        Client(name="Anna Brown", email="anna.brown@example.com", phone="4208529637413",
               custom_fields=json.dumps({"age": 29, "occupation": "Doctor"}), created_at=datetime(2023, 1, 10),
               updated_at=datetime(2023, 1, 10)),
        Client(name="Tom Smith", email="tom.smith@example.com", phone="4208529637414",
               custom_fields=json.dumps({"age": 42, "occupation": "Teacher"}), created_at=datetime(2023, 1, 11),
               updated_at=datetime(2023, 1, 11)),
        Client(name="Alice Johnson", email="alice.johnson@example.com", phone="4208529637415",
               custom_fields=json.dumps({"age": 31, "occupation": "Nurse"}), created_at=datetime(2023, 1, 12),
               updated_at=datetime(2023, 1, 12)),
    ]
    db.session.add_all(clients)
    db.session.commit()

    # Populate a single Calendar
    calendar = Calendar(name="General Consultation", description="General consultation for all services",
                        img="https://cdn.intuji.com/2022/08/Business-Technology-scaled-1.jpeg",
                        default_duration=30, place="Zoom meeting",
                        form_fields=json.dumps({"required_fields": ["name", "email", "phone"]}),
                        created_at=datetime(2023, 1, 1), updated_at=datetime(2023, 1, 1))
    calendar2 = Calendar(name="General Consultation 2", description="General consultation for all services 2",
                         img="https://online.hbs.edu/Style%20Library/api/resize.aspx?imgpath=/PublishingImages/overhead-view-of-business-strategy-meeting.jpg&w=1200&h=630",
                         default_duration=15, place="106 Arbutus Drive, Miami",
                        form_fields=json.dumps({"required_fields": ["name", "email", "phone"]}),
                        created_at=datetime(2023, 1, 1), updated_at=datetime(2023, 1, 1))
    db.session.add(calendar)
    db.session.add(calendar2)
    db.session.commit()

    # Assign all providers to this single calendar
    provider_calendars = [
        ProviderCalendar(provider_id=provider.id, calendar_id=calendar.id)
        for provider in providers
    ]
    provider_calendar2 = ProviderCalendar(provider_id=providers[0].id, calendar_id=calendar2.id)
    db.session.add_all(provider_calendars)
    db.session.add(provider_calendar2)
    db.session.commit()

    # Populate Provider Availability (10:00 to 18:00 for each day of the week)
    provider_availability = []
    for provider in providers:
        for week_day in range(3, 7):  # 0 = Monday, 6 = Sunday
            availability = ProviderAvailability(
                provider_id=provider.id,
                start_time=datetime.strptime("09:00", "%H:%M").time(),
                end_time=datetime.strptime("18:00", "%H:%M").time(),
                week_day=week_day
            )
            provider_availability.append(availability)

    db.session.add_all(provider_availability)
    db.session.commit()

    # Populate Appointments
    appointments = [
        Appointment(client_id=clients[0].id, provider_id=providers[0].id, calendar_id=calendar.id,
                    appointment_datetime=datetime(2024, 12, 16, 10, 00), status="booked", duration=60,
                    created_at=datetime(2023, 1, 9), updated_at=datetime(2023, 1, 9)),
        Appointment(client_id=clients[1].id, provider_id=providers[0].id, calendar_id=calendar.id,
                    appointment_datetime=datetime(2024, 12, 16, 11, 00), status="booked", duration=60,
                    created_at=datetime(2023, 1, 9), updated_at=datetime(2023, 1, 9)),
        Appointment(client_id=clients[2].id, provider_id=providers[0].id, calendar_id=calendar2.id,
                    appointment_datetime=datetime(2024, 12, 16, 12, 00), status="booked", duration=60,
                    created_at=datetime(2023, 1, 9), updated_at=datetime(2023, 1, 9)),
        Appointment(client_id=clients[3].id, provider_id=providers[1].id, calendar_id=calendar.id,
                    appointment_datetime=datetime(2024, 12, 16, 15, 30), status="booked", duration=45,
                    created_at=datetime(2023, 1, 10), updated_at=datetime(2023, 1, 10)),
        Appointment(client_id=clients[1].id, provider_id=providers[2].id, calendar_id=calendar.id,
                    appointment_datetime=datetime(2024, 12, 17, 10, 30), status="booked", duration=30,
                    created_at=datetime(2023, 1, 11), updated_at=datetime(2023, 1, 11)),
    ]

    # Generate random appointments for the remaining clients and providers
    for i in range(len(clients) - len(appointments)):
        client = clients[i + len(appointments)]
        provider = random.choice(providers)
        appointment_datetime = datetime(2024, 12, 20, 8, 30) + timedelta(days=i, hours=random.randint(1, 8))
        duration = random.choice([15, 30, 45, 60, 90, 120])  # Duration choices in minutes

        appointment = Appointment(
            client_id=client.id,
            provider_id=provider.id,
            calendar_id=calendar.id,
            appointment_datetime=appointment_datetime,
            status="booked",
            duration=duration,
            created_at=datetime(2023, 1, 9) + timedelta(days=i),
            updated_at=datetime(2023, 1, 9) + timedelta(days=i)
        )
        appointments.append(appointment)

    db.session.add_all(appointments)
    db.session.commit()

    print("Dummy data populated successfully.")
