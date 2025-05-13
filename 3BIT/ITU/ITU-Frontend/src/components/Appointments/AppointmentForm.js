/**
 * @file AppointmentForm.js
 * @description React component for handling appointment creation and editing, including validation and rescheduling.
 * @author Vladyslav Yeroma (xyerom00)
 *
 * This file defines the AppointmentForm component, which provides a user interface
 * for editing or creating appointments. It includes real-time validation for user inputs,
 * dynamically fetched available slots for rescheduling, and integration with the backend.
 */

import React, { useState, useEffect, useRef } from 'react';
import { useReschedule } from '../../controllers/useReschedule';
import { format } from 'date-fns';
import '../../styles/calendar_popup.css';

/**
 * AppointmentForm Component
 * @param {Object} props - Component properties.
 * @param {Object} props.appointment - Appointment details to edit or reschedule.
 * @param {Function} props.onSave - Callback to save the updated appointment.
 * @param {Function} props.onClose - Callback to close the form without saving.
 */
export const AppointmentForm = ({ appointment, onSave, onClose }) => {
    // State variables for form inputs
    const [name, setName] = useState('');
    const [email, setEmail] = useState('');
    const [phone, setPhone] = useState('');
    const [date, setDate] = useState(''); // Separate date field
    const [time, setTime] = useState(''); // Separate time field
    const [start, setStart] = useState(null); // Combined date and time
    const [duration, setDuration] = useState(60); // Default duration in minutes
    const [emailError, setEmailError] = useState('');
    const [phoneError, setPhoneError] = useState('');
    const formRef = useRef(null); // Reference to the form container

    // Hook for fetching available slots for rescheduling
    const { availableSlots, loadDailyAvailability } = useReschedule(appointment?.provider_id, duration, appointment?.id);

    /**
     * Populate initial form fields when an appointment is provided.
     */
    useEffect(() => {
        if (appointment) {
            setName(appointment.client.name || '');
            setEmail(appointment.client.email || '');
            setPhone(formatPhoneNumber(appointment.client.phone || ''));
            setDate(new Date(appointment.appointment_datetime).toISOString().slice(0, 10));
            setTime(new Date(appointment.appointment_datetime).toTimeString().slice(0, 5));
            setDuration(appointment.duration || 60);
        }
    }, [appointment]);

    /**
     * Fetch available slots whenever the selected date or duration changes.
     */
    useEffect(() => {
        if (date) {
            loadDailyAvailability(new Date(date));
        }
    }, [date, duration]);

    /**
     * Update the combined start date and time when either date or time changes.
     */
    useEffect(() => {
        if (date && time) {
            setStart(new Date(`${date}T${time}`));
        }
    }, [date, time]);

    /**
     * Close the form when clicking outside of it.
     */
    useEffect(() => {
        const handleOutsideClick = (event) => {
            if (formRef.current && !formRef.current.contains(event.target)) {
                onClose(); // Close the form
            }
        };

        document.addEventListener('mousedown', handleOutsideClick);
        return () => {
            document.removeEventListener('mousedown', handleOutsideClick);
        };
    }, [onClose]);

    /**
     * * Handle form submission and save the updated appointment.
     */
    const handleSubmit = () => {
        if (validateInputs() && start) {
            const updatedAppointment = {
                ...appointment,
                client: {
                    ...appointment.client,
                    name,
                    email,
                    phone: unformatPhoneNumber(phone),
                },
                appointment_datetime: start.toISOString(),
                duration,
            };
            onSave(updatedAppointment);
        } else {
            console.error('Start date and time are not fully defined.');
        }
    };

    /**
     * Format a phone number into a standardized format.
     * @param {string} phoneNumber - The phone number to format.
     * @returns {string} Formatted phone number.
     */
    const formatPhoneNumber = (phoneNumber) => {
        const cleaned = phoneNumber.replace(/\D/g, '');
        if (cleaned.length > 10) {
            const localPart = cleaned.slice(-10);
            return `(${localPart.slice(0, 3)}) ${localPart.slice(3, 6)}-${localPart.slice(6)}`;
        }
        if (cleaned.length === 10) {
            return `(${cleaned.slice(0, 3)}) ${cleaned.slice(3, 6)}-${cleaned.slice(6)}`;
        }
        return phoneNumber;
    };

    /**
     * Remove formatting from a phone number.
     * @param {string} formattedPhone - The formatted phone number.
     * @returns {string} Unformatted phone number.
     */
    const unformatPhoneNumber = (formattedPhone) => formattedPhone.replace(/\D/g, '');

    /**
     * Validate all form inputs and return true if valid.
     * @returns {boolean} Whether all inputs are valid.
     */
    const validateInputs = () => {
        const isValidEmail = validateEmail(email);
        const isValidPhone = validatePhoneNumber(phone);
        setEmailError(isValidEmail ? '' : 'Invalid email format');
        setPhoneError(isValidPhone ? '' : 'Invalid phone number format');
        return isValidEmail && isValidPhone;
    };

    /**
     * Validate a phone number format.
     * @param {string} number - The phone number to validate.
     * @returns {boolean} Whether the phone number is valid.
     */
    const validatePhoneNumber = (number) => number.replace(/\D/g, '').length >= 10;

    /**
     * Validate an email format.
     * @param {string} email - The email to validate.
     * @returns {boolean} Whether the email is valid.
     */
    const validateEmail = (email) => /^[^\s@]+@[^\s@]+\.[^\s@]+$/.test(email);

    // Format the selected appointment date and time for display
    const formattedAppointmentInfo = start
        ? `${format(start, 'EEEE')}, ${format(start, 'MMMM d')} at ${format(start, 'h:mm a')}`
        : '';

    return (
        <div
            className="appointment-form-modal"
            ref={formRef} // Attach the ref to the form container
        >
            <div className="appointment-form-content">
                <h2>{name}</h2>
                <p><strong>{formattedAppointmentInfo}</strong></p>
                <div className="form-columns">
                    {/* Left Column: Client Details */}
                    <div className="form-left">
                        <label>Full Name</label>
                        <input
                            type="text"
                            value={name}
                            onChange={(e) => setName(e.target.value)}
                        />
                        <label>Email</label>
                        <input
                            type="email"
                            value={email}
                            onChange={(e) => setEmail(e.target.value)}
                            onBlur={() => setEmailError(validateEmail(email) ? '' : 'Invalid email format')}
                        />
                        {emailError && <span className="error-text">{emailError}</span>}
                        <label>Phone</label>
                        <input
                            type="text"
                            value={phone}
                            onChange={(e) => setPhone(formatPhoneNumber(e.target.value))}
                            onBlur={() => setPhoneError(validatePhoneNumber(phone) ? '' : 'Invalid phone number format')}
                            placeholder="(XXX) XXX-XXXX"
                        />
                        {phoneError && <span className="error-text">{phoneError}</span>}
                    </div>
                    {/* Right Column: Appointment Details */}
                    <div className="form-right">
                        <label>Date</label>
                        <input
                            type="date"
                            value={date}
                            onChange={(e) => setDate(e.target.value)}
                            className="styled-select"
                        />

                        <label>Time</label>
                        <select
                            value={time}
                            onChange={(e) => setTime(e.target.value)}
                            className="styled-select"
                        >
                            <option value="">Select a time slot</option>
                            {availableSlots.map((slot) => {
                                const localSlot = new Date(slot.getTime() - (slot.getTimezoneOffset() * 60000));
                                return (
                                    <option key={slot.toISOString()} value={localSlot.toTimeString().slice(0, 5)}>
                                        {localSlot.toLocaleTimeString([], { hour: '2-digit', minute: '2-digit' })}
                                    </option>
                                );
                            })}
                        </select>

                        <label>Duration</label>
                        <select
                            value={duration}
                            onChange={(e) => setDuration(Number(e.target.value))}
                            className="styled-select"
                        >
                            {Array.from({ length: 12 }, (_, i) => (i + 1) * 15).map((mins) => (
                                <option key={mins} value={mins}>
                                    {`${Math.floor(mins / 60)}h ${mins % 60}m`.replace('0h ', '')}
                                </option>
                            ))}
                        </select>
                    </div>
                </div>
                <div className="button-group">
                    <button className="form-button cancel-button" onClick={onClose}>Back</button>
                    <button className="form-button save-button" onClick={handleSubmit}>Save</button>
                </div>
            </div>
        </div>
    );
};
