/**
 * @file freeSlotsService.js
 * @description Provides API methods for fetching free slots available for appointments.
 *              This service handles the logic to communicate with the backend to retrieve
 *              available time slots for a given date range, providers, and appointment constraints.
 * @author Vladyslav Yeroma (xyerom00)
 */

import { BASE_URL } from './config'; // Import the base URL for API requests

export const freeSlotsService = {
    /**
     * Fetch available free slots for a specific date range, providers, and appointment constraints.
     *
     * @param {string} start - ISO-formatted start date and time for the search range.
     * @param {string} end - ISO-formatted end date and time for the search range.
     * @param {Array<number>} providerIds - List of provider IDs for which to fetch slots.
     * @param {number} appointmentDuration - Duration of the appointment in minutes.
     * @param {number} [appointmentId] - Optional ID of the appointment to ignore for conflicts.
     * @returns {Promise<Object>} A promise that resolves to an object containing the available slots.
     * @throws {Error} Throws an error if the request fails or the response is invalid.
     */
    getFreeSlots: async (start, end, providerIds, appointmentDuration, appointmentId) => {
        // Convert the list of provider IDs into a comma-separated string for the query parameter
        const providerIdsParam = providerIds.join(',');

        // Build the request URL with mandatory parameters
        let request_string = `${BASE_URL}/free-slots?start=${start}&end=${end}&providers=${providerIdsParam}&appointment_duration=${appointmentDuration}`;

        // Append the optional `appointment_id` parameter if provided
        if (appointmentId) {
            request_string += `&appointment_id=${appointmentId}`;
        }

        try {
            // Make the API request to fetch free slots
            const response = await fetch(request_string);

            // Check if the response status is not OK (200-299)
            if (!response.ok) {
                let errorMessage = 'Failed to load free slots'; // Default error message

                // Attempt to parse the error message from the server's response
                try {
                    const errorData = await response.json();
                    errorMessage = errorData.message || errorMessage;
                } catch (e) {
                    console.error('Error parsing response:', e);
                }

                // Throw an error with the parsed or default message
                throw new Error(errorMessage);
            }

            // Return the parsed JSON response containing the free slots
            return await response.json();
        } catch (error) {
            console.error('Error fetching free slots:', error);
            throw error; // Re-throw the error for the caller to handle
        }
    },
};
