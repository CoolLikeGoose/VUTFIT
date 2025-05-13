/**
 * @file appointmentsService.js
 * @description This service provides a set of functions for managing appointments via API calls.
 *              It interacts with the backend to handle fetching, updating, and creating appointments.
 *              Each function returns a promise, making it compatible with async/await syntax.
 * @author Vladyslav Yeroma (xyerom00) and Nikita Smirnov (xsmirn02)
 */

import { BASE_URL } from './config'; // Import the base URL for API calls

export const appointmentsService = {
    /**
     * Fetches appointments from the backend based on the provided criteria.
     * @param {string} start - The start date/time in ISO format.
     * @param {string} end - The end date/time in ISO format.
     * @param {Array<number>} providers - Array of provider IDs to filter appointments.
     * @param {number|null} calendarId - The calendar ID to filter appointments (optional).
     * @returns {Promise<Array>} A promise resolving to the fetched appointments.
     * @throws Will throw an error if the API call fails.
     */
    getAppointments: async (start, end, providers, calendarId) => {
        const providerIds = providers.join(','); // Convert the provider IDs array to a comma-separated string
        const calendarParam = calendarId ? `&calendars=${calendarId}` : ''; // Append calendar ID if provided
        const response = await fetch(
            `${BASE_URL}/appointments?start=${start}&end=${end}&providers=${providerIds}${calendarParam}`
        );
        if (!response.ok) {
            throw new Error('Failed to fetch appointments');
        }
        return await response.json(); // Parse and return the JSON response
    },

    /**
     * Updates an appointment's details in the backend.
     * @param {number} id - The ID of the appointment to update.
     * @param {Object} data - The data to update the appointment with.
     * @returns {Promise<Object>} A promise resolving to the updated appointment data.
     * @throws Will throw an error if the API call fails.
     */
    updateAppointment: async (id, data) => {
        const response = await fetch(`${BASE_URL}/appointments/${id}`, {
            method: 'PUT', // Use PUT for updating existing resources
            headers: { 'Content-Type': 'application/json' }, // Set the request content type
            body: JSON.stringify(data), // Convert the data object to a JSON string
        });
        if (!response.ok) {
            throw new Error('Failed to update appointment');
        }
        return await response.json(); // Parse and return the JSON response
    },

    /**
     * Updates the status of an appointment in the backend.
     * @param {number} id - The ID of the appointment to update.
     * @param {string} status - The new status for the appointment (e.g., 'booked', 'canceled').
     * @returns {Promise<Object>} A promise resolving to the updated appointment status.
     * @throws Will throw an error if the API call fails.
     */
    updateAppointmentStatus: async (id, status) => {
        const response = await fetch(`${BASE_URL}/appointments/${id}/status`, {
            method: 'PUT', // Use PUT for updating the status
            headers: { 'Content-Type': 'application/json' }, // Set the request content type
            body: JSON.stringify({ status }), // Send the new status in the request body
        });
        if (!response.ok) {
            throw new Error('Failed to update appointment status');
        }
        return await response.json(); // Parse and return the JSON response
    },

    /**
     * Creates a new appointment in the backend.
     * @param {Object} data - The data for the new appointment.
     * @returns {Promise<Object>} A promise resolving to the created appointment data.
     * @throws Will throw an error if the API call fails.
     */
    createAppointment: async (data) => {
        const response = await fetch(`${BASE_URL}/appointments`, {
            method: 'POST', // Use POST for creating new resources
            headers: { 'Content-Type': 'application/json' }, // Set the request content type
            body: JSON.stringify(data), // Convert the data object to a JSON string
        });
        if (!response.ok) {
            throw new Error('Failed to create appointment');
        }
        return await response.json(); // Parse and return the JSON response
    },
};
