/**
 * @file useAppointmentsController.js
 * @description Custom React hook for managing appointment data, including loading, saving, canceling, and restoring appointments.
 *              Handles API communication, state updates, and error handling for appointment operations.
 * @author Vladyslav Yeroma (xyerom00)
 */

import { useState, useCallback } from 'react';
import { appointmentsService } from '../services/appointmentsService';
import { clientsService } from '../services/clientsService';
import { startOfWeek, endOfWeek, isDate } from 'date-fns';

/**
 * Custom hook for managing appointments.
 * Provides functionality for loading, saving, canceling, and restoring appointments.
 * Handles state management for appointments and loading/error states.
 */
export const useAppointmentsController = () => {
    // State to store loaded appointments
    const [appointments, setAppointments] = useState([]);

    // State to indicate loading status
    const [loading, setLoading] = useState(false);

    // State to store error messages
    const [error, setError] = useState(null);

    /**
     * Loads appointments for a specified date range, providers, and calendar.
     * @param {string|Date} start - Start date of the range.
     * @param {string|Date} end - End date of the range.
     * @param {Array} providers - List of provider IDs to filter appointments.
     * @param {string} view - Current calendar view ("week" or "day").
     * @param {number} calendarId - ID of the selected calendar.
     */
    const loadAppointments = useCallback(async (start, end, providers, view, calendarId) => {
        // Clear appointments if no providers are selected
        if (!providers || providers.length === 0) {
            setAppointments([]);
            return;
        }

        let adjustedStart = start;
        let adjustedEnd = end;

        // Ensure start and end are Date objects
        if (!isDate(start)) adjustedStart = new Date(start);
        if (!isDate(end)) adjustedEnd = new Date(end);

        // Adjust date range based on the view
        if (view === 'week') {
            adjustedStart = startOfWeek(adjustedStart, { weekStartsOn: 0 });
            adjustedEnd = endOfWeek(adjustedEnd, { weekStartsOn: 0 });
        } else if (view === 'day') {
            // Adjust start to the beginning of the day and end to the same day's end
            adjustedStart = new Date(adjustedStart.setHours(0, 0, 0, 0)); // Start of the day
            adjustedEnd = new Date(adjustedStart.getTime() + 24 * 60 * 60 * 1000 - 1); // End of the day (23:59:59.999)
        }

        setLoading(true);
        setError(null); // Clear previous errors

        try {
            // Fetch appointments from the API
            const appointmentsData = await appointmentsService.getAppointments(
                adjustedStart.toISOString(),
                adjustedEnd.toISOString(),
                providers,
                calendarId
            );

            // Get unique client IDs from the appointments
            const clientIds = [...new Set(appointmentsData.map(app => app.client_id))];

            // Fetch client data for each unique client ID
            const clientDataArray = await Promise.all(clientIds.map(id => clientsService.getClientById(id)));

            // Map client data for quick lookup
            const clientDataMap = clientDataArray.reduce((acc, client) => {
                acc[client.id] = client;
                return acc;
            }, {});

            // Enrich appointments with client data
            const enrichedAppointments = appointmentsData.map(appointment => ({
                ...appointment,
                client: clientDataMap[appointment.client_id]
            }));

            setAppointments(enrichedAppointments); // Update state with enriched appointments
        } catch (err) {
            console.error('Error loading appointments:', err);
            setError(err); // Store error message in state
        } finally {
            setLoading(false); // Reset loading state
        }
    }, []);

    /**
     * Saves updates to an appointment.
     * @param {Object} updatedAppointment - Appointment object with updated details.
     */
    const saveAppointment = async (updatedAppointment) => {
        try {
            // Update appointment details via the API
            await appointmentsService.updateAppointment(updatedAppointment.id, {
                appointment_datetime: updatedAppointment.appointment_datetime,
                status: updatedAppointment.status,
                duration: updatedAppointment.duration,
                provider_id: updatedAppointment.provider_id,
                calendar_id: updatedAppointment.calendar_id,
            });

            // Update client details via the API
            await clientsService.updateClient(updatedAppointment.client.id, {
                name: updatedAppointment.client.name,
                email: updatedAppointment.client.email,
                phone: updatedAppointment.client.phone,
            });

            updateAppointmentInState(updatedAppointment); // Update appointment in local state
        } catch (err) {
            setError(err);
            console.error("Error saving appointment:", err);
        }
    };

    /**
     * Cancels an appointment.
     * @param {number} appointmentId - ID of the appointment to cancel.
     */
    const cancelAppointment = async (appointmentId) => {
        try {
            // Update appointment status to "canceled"
            await appointmentsService.updateAppointmentStatus(appointmentId, "canceled");
            removeAppointmentFromState(appointmentId); // Remove appointment from local state
        } catch (err) {
            setError(err);
            console.error("Error canceling appointment:", err);
        }
    };

    /**
     * Updates an appointment in the local state.
     * @param {Object} updatedAppointment - Updated appointment details.
     */
    const updateAppointmentInState = (updatedAppointment) => {
        setAppointments((prevAppointments) =>
            prevAppointments.map((app) =>
                app.id === updatedAppointment.id ? updatedAppointment : app
            )
        );
    };

    /**
     * Removes an appointment from the local state.
     * @param {number} appointmentId - ID of the appointment to remove.
     */
    const removeAppointmentFromState = (appointmentId) => {
        setAppointments((prevAppointments) =>
            prevAppointments.filter((app) => app.id !== appointmentId)
        );
    };

    /**
     * Restores a previously canceled appointment.
     * @param {Object} appointment - The canceled appointment to restore.
     */
    const restoreAppointment = async (appointment) => {
        try {
            // Update appointment status to "booked"
            await appointmentsService.updateAppointmentStatus(appointment.id, "booked");
            setAppointments((prevAppointments) => [...prevAppointments, appointment]); // Add appointment back to state
        } catch (err) {
            setError(err);
            console.error("Error restoring appointment:", err);
        }
    };

    // Return hook API for managing appointments
    return {
        appointments,
        loadAppointments,
        saveAppointment,
        cancelAppointment,
        restoreAppointment,
        loading,
        error
    };
};
