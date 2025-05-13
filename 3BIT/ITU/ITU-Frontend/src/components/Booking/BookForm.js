/**
 * @file BookForm.js
 * @description Form component for booking an appointment.
 *              Handles user inputs for name and email(and additional), validates the form, and submits the data to create an appointment.
 * @author Nikita Smirnov (xsmirn02)
 */


import React, { useState } from 'react';
import '../../styles/booking_page.css'
import { clientsService } from '../../services/clientsService';
import { appointmentsService} from '../../services/appointmentsService';

export const BookForm = ({setIsConfirmed, setAppointmentProviderID, appointmentProviderID,
    calendarId, calendarDuration, selectedTime, selectedDate, formatedDate}) => {
    const handleReturn = () => {
        setIsConfirmed(false);
        setAppointmentProviderID(null);
    };

    const transformDateTime = (date, time) => {
        const [hours, minutes] = time.split(':');
        date.setHours(hours);
        date.setMinutes(minutes);
        date.setSeconds(0);

        return date.toISOString();
    };

    const handleSubmit = async (e) => {
        e.preventDefault();

        //TODO: add email validation
        const name = e.target[0].value;
        const email = e.target[1].value;
        //TODO: add custom field support
        
        try {
            const user = await clientsService.createClient({'name': `${name}`, 'email':`${email}`, 'phone':'10000000'});

            const appointment = await appointmentsService.createAppointment({
                'client_id': `${user.id}`,
                'provider_id': `${appointmentProviderID}`,
                'calendar_id': `${calendarId}`,
                'appointment_datetime': `${transformDateTime(selectedDate, selectedTime)}`,
                'status': "booked",
                'duration': `${calendarDuration}`
            })

            // setShowPopup({ type: 'success', message: 'Appointment successfully created!' });
            alert('completed');
            window.location.reload();
        } catch (error) {
            alert('error');
            console.log(error);
            // setShowPopup({ type: 'error', message: error.message });  
        }
    };

    return (
        <div className='book-form'>
            <div className='form-header'>
                <button onClick={() => handleReturn()}>Select another time</button>
                <div>{selectedTime}, {formatedDate}</div>
            </div>

            <form onSubmit={handleSubmit}>
                <div className='form-fields'>
                    {/* necessary email and name information */}
                    <label>Full Name</label>
                    <input type='text' placeholder='Your Name' required></input>
                    <label>Email</label>
                    <input type='text' placeholder='youremail@gmail.com' required></input>

                    {/*TODO: others form fields */}
                </div>

                <button type='submit' className='form-submit'>Schedule</button>
            </form>
        </div>
    );
};