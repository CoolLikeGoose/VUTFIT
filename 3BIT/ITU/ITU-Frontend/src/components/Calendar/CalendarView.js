import React, { useState, useRef, useEffect } from 'react';
import { Calendar, momentLocalizer, Views } from 'react-big-calendar';
import 'react-big-calendar/lib/css/react-big-calendar.css';
import moment from 'moment';
import { Box, Button } from '@mui/material';
import { Slide, toast, ToastContainer } from 'react-toastify';
import 'react-toastify/dist/ReactToastify.css';
import { AppointmentOptionsOverlay } from '../Appointments/AppointmentOptionsOverlay';
import { AppointmentForm } from '../Appointments/AppointmentForm';

const localizer = momentLocalizer(moment);

/**
 * CalendarView Component
 * @param {Object} props - Component properties.
 * @param {string} props.view - Current view mode of the calendar ("week" or "day").
 * @param {Date} props.selectedDate - The currently selected date for the calendar view.
 * @param {Array} props.appointments - List of appointment objects to display on the calendar.
 * @param {Function} props.onRangeChange - Callback triggered when the visible range changes.
 * @param {Function} props.saveAppointment - Callback to handle saving updated appointments.
 * @param {Function} props.cancelAppointment - Callback to handle canceling an appointment.
 * @param {Function} props.restoreAppointment - Callback to handle restoring a canceled appointment.
 * @param {Array} props.providerColors - Array of colors for providers, used to color-code appointments.
 * @returns {JSX.Element} Rendered calendar view component.
 */
export const CalendarView = ({ view, selectedDate, appointments, onRangeChange, saveAppointment, cancelAppointment, restoreAppointment, providerColors }) => {
    const [selectedAppointment, setSelectedAppointment] = useState(null); // Currently selected appointment
    const [isEditing, setIsEditing] = useState(false); // Editing mode toggle
    const [popupPosition, setPopupPosition] = useState({ top: 0, left: 0 }); // Position of the appointment popup
    const calendarRef = useRef(null); // Reference to the calendar for positioning calculations
    const overlayRef = useRef(null); // Reference to detect outside clicks for the overlay

    /**
     * Close the overlay if clicking outside of it.
     * @param {MouseEvent} event - The click event.
     */
    const handleOutsideClick = (event) => {
        if (
            overlayRef.current &&
            !overlayRef.current.contains(event.target) // Check if click is outside overlay
        ) {
            setSelectedAppointment(null); // Close the overlay
            setIsEditing(false); // Exit editing mode
        }
    };


    /**
     * Attach and detach the outside click listener.
     */
    useEffect(() => {
        if (selectedAppointment || isEditing) {
            document.addEventListener('mousedown', handleOutsideClick);
        } else {
            document.removeEventListener('mousedown', handleOutsideClick);
        }

        return () => {
            document.removeEventListener('mousedown', handleOutsideClick);
        };
    }, [selectedAppointment, isEditing]);

    /**
     * Get custom styles for each event based on the provider ID.
     * @param {Object} event - Event object representing the appointment.
     * @returns {Object} Style object for the event.
     */
    const eventStyleGetter = (event) => {
        const color = providerColors[event.provider_id % providerColors.length];
        return {
            style: {
                backgroundColor: color,
                borderRadius: '5px',
                opacity: 0.9,
                color: 'white',
                border: '0px',
                display: 'block',
            },
        };
    };

    /**
     * Handles selecting an event on the calendar, displaying the options overlay.
     * @param {Object} event - Selected event object.
     * @param {Object} e - Mouse event for calculating popup position.
     */
    const handleSelectEvent = (event, e) => {
        setSelectedAppointment(selectedAppointment && selectedAppointment.id === event.id ? null : event);
        const calendarRect = calendarRef.current.getBoundingClientRect();
        const popupWidth = 200;
        const popupHeight = 150;
        let top = e.clientY - calendarRect.top;
        let left = e.clientX - calendarRect.left;

        // Adjust position to ensure the popup stays within calendar bounds
        if (left + popupWidth > calendarRect.width) left -= popupWidth + 20;
        if (top + popupHeight > calendarRect.height) top -= popupHeight + 20;

        setPopupPosition({ top, left });
    };

    /**
     * Enters editing mode for the selected appointment.
     */
    const handleEdit = () => setIsEditing(true);

    /**
     * Closes the appointment form or overlay.
     */
    const handleCloseForm = () => setIsEditing(false) || setSelectedAppointment(null);

    /**
     * Saves the updated appointment details.
     * @param {Object} updatedAppointment - The updated appointment object.
     */
    const handleSaveAppointment = async (updatedAppointment) => {
        try {
            await saveAppointment(updatedAppointment);
            handleCloseForm();
            toast.success("Appointment saved successfully!", {
                position: "top-right",
                autoClose: 1500,
                hideProgressBar: false,
                closeOnClick: true,
                pauseOnHover: true,
                draggable: true,
                transition: Slide,
            });
        } catch (error) {
            console.error("Error saving appointment:", error);
            toast.error("Failed to save appointment. Please try again.", {
                position: "top-right",
                autoClose: 1500,
                hideProgressBar: false,
                closeOnClick: true,
                pauseOnHover: true,
                draggable: true,
                transition: Slide,
            });
        }
    };


    /**
     * Cancels the selected appointment and shows an undo notification.
     */
    const handleCancelAppointment = async () => {
        if (!selectedAppointment) return;

        const appointmentToCancel = selectedAppointment;

        await cancelAppointment(appointmentToCancel.id);
        setSelectedAppointment(null);
        showToastWithUndo(appointmentToCancel);
    };

    /**
     * Displays a toast notification with an undo option for canceled appointments.
     * @param {Object} appointmentToCancel - The appointment that was canceled.
     */
    const showToastWithUndo = (appointmentToCancel) => {
        toast(
            ({ closeToast }) => (
                <div>
                    <span>Appointment canceled</span>
                    <Button
                        color="secondary"
                        size="small"
                        onClick={() => handleUndo(appointmentToCancel, closeToast)}
                        sx={{ marginLeft: '20px' }}
                    >
                        Undo
                    </Button>
                </div>
            ),
            {
                position: "top-right",
                autoClose: 5000,
                hideProgressBar: false,
                closeOnClick: false,
                pauseOnHover: true,
                draggable: false,
                transition: Slide,
            }
        );
    };

    /**
     * Restores a canceled appointment and closes the toast notification.
     * @param {Object} appointmentToCancel - The appointment to restore.
     * @param {Function} closeToast - Function to close the toast notification.
     */
    const handleUndo = async (appointmentToCancel, closeToast) => {
        console.log("Undo clicked, last canceled appointment:", appointmentToCancel);
        if (appointmentToCancel) {
            await restoreAppointment(appointmentToCancel);
            closeToast();
        }
    };

    return (
        <Box sx={{ height: '100%', overflowY: 'auto', position: 'relative' }} ref={calendarRef}>
            {/* Options overlay for the selected appointment */}
            {selectedAppointment && !isEditing && (
                <div ref={overlayRef}>
                    <AppointmentOptionsOverlay
                        appointment={selectedAppointment}
                        onEdit={handleEdit}
                        onCancel={handleCancelAppointment}
                        onClose={() => setSelectedAppointment(null)}
                        style={{
                            position: 'absolute',
                            top: popupPosition.top,
                            left: popupPosition.left + 20,
                            zIndex: 1000,
                        }}
                    />
                </div>
            )}

            {/* Appointment form for editing */}
            {isEditing && (
                <div className="modal-overlay">
                    <AppointmentForm
                        appointment={selectedAppointment}
                        onSave={handleSaveAppointment}
                        onClose={handleCloseForm}
                    />
                </div>
            )}

            {/* Calendar view */}
            <Calendar
                localizer={localizer}
                events={appointments.map(app => ({
                    ...app,
                    start: new Date(app.appointment_datetime),
                    end: new Date(new Date(app.appointment_datetime).getTime() + app.duration * 60 * 1000),
                    title: `${app.client.name}`,
                }))}
                startAccessor="start"
                endAccessor="end"
                titleAccessor="title"
                style={{ height: '100%' }}
                view={view === 'week' ? Views.WEEK : Views.DAY}
                onRangeChange={onRangeChange}
                toolbar={false}
                views={['week', 'day']}
                date={selectedDate}
                eventPropGetter={eventStyleGetter}
                onSelectEvent={handleSelectEvent}
            />

            <ToastContainer />
        </Box>
    );
};
