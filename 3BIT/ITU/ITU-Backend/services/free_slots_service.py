from db import db
from db.appointment_model import Appointment
from db.provider_availability_model import ProviderAvailability
from datetime import timedelta
from sqlalchemy import and_
from dateutil.parser import parse

def get_free_slots(start, end, provider_id, duration, appointment_id=None):
    # Parse start and end as naive datetime objects (assumed to be in UTC)
    start_date = parse(start).replace(tzinfo=None)
    end_date = parse(end).replace(tzinfo=None)

    # Adjust start_date down to the nearest hour
    start_date = start_date.replace(minute=0, second=0, microsecond=0)

    # Adjust end_date up to the nearest hour if it is not already on the hour
    if end_date.minute != 0 or end_date.second != 0 or end_date.microsecond != 0:
        end_date = (end_date + timedelta(hours=1)).replace(minute=0, second=0, microsecond=0)

    print(f"Adjusted range start: {start_date}, end: {end_date}")

    # Fetch provider availability for the provider
    provider_availabilities = ProviderAvailability.query.filter(
        ProviderAvailability.provider_id == provider_id
    ).all()

    # Fetch booked appointments within the specified range, excluding the specified appointment_id if provided
    appointments_query = Appointment.query.filter(
        Appointment.provider_id == provider_id,
        and_(Appointment.appointment_datetime >= start_date,
             Appointment.appointment_datetime <= end_date),
        Appointment.status == 'booked'
    )
    if appointment_id:
        appointments_query = appointments_query.filter(Appointment.id != appointment_id)

    appointments = appointments_query.all()

    # Convert appointments to a list of occupied time ranges as naive datetime objects
    appointment_times = [
        (appt.appointment_datetime.replace(tzinfo=None),
         (appt.appointment_datetime + timedelta(minutes=appt.duration)).replace(tzinfo=None))
        for appt in appointments
    ]

    free_slots = []

    # Iterate over each day in the range to calculate slots
    day = start_date
    while day <= end_date:
        day_of_week = day.weekday()
        daily_availabilities = [avail for avail in provider_availabilities if avail.week_day == day_of_week]

        day_slots = []  # Temporary list to hold slots for the current day

        for availability in daily_availabilities:
            slot_start = day.replace(
                hour=availability.start_time.hour,
                minute=availability.start_time.minute,
                second=0,
                microsecond=0
            )
            slot_end = day.replace(
                hour=availability.end_time.hour,
                minute=availability.end_time.minute,
                second=0,
                microsecond=0
            )

            while slot_start + timedelta(minutes=duration) <= slot_end:
                slot_end_time = slot_start + timedelta(minutes=duration)

                # Check if this slot is free by comparing with appointment times
                is_free = all(
                    not (
                        (appt_start <= slot_start < appt_end) or
                        (appt_start < slot_end_time <= appt_end) or
                        (slot_start <= appt_start and slot_end_time >= appt_end)
                    )
                    for appt_start, appt_end in appointment_times
                )

                if is_free:
                    day_slots.append(slot_start.isoformat())

                slot_start += timedelta(minutes=15)

        # Filter slots to only include those within the requested start and end range
        filtered_day_slots = [
            slot for slot in day_slots if start_date <= parse(slot).replace(tzinfo=None) <= end_date
        ]
        print(f"Filtered slots for {day.date()} within range: {filtered_day_slots}")

        # Add the filtered slots to free_slots
        free_slots.extend(filtered_day_slots)

        day += timedelta(days=1)

    return free_slots
