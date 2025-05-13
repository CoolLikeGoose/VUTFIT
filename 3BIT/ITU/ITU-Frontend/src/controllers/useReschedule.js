/**
 * @file useReschedule.js
 * @description Custom React hook for managing the rescheduling of appointments.
 *              It fetches and manages daily availability of free slots for a given provider.
 * @author Vladyslav Yeroma (xyerom00)
 */

import { useState, useCallback } from 'react';
import { freeSlotsService } from '../services/freeSlotsService';
import { startOfDay, endOfDay } from 'date-fns';

/**
 * Custom hook for fetching and managing free time slots for rescheduling appointments.
 *
 * @param {number} providerId - The ID of the provider for whom availability is being fetched.
 * @param {number} appointmentDuration - The duration of the appointment in minutes.
 * @param {number} appointmentId - The ID of the appointment being rescheduled (optional for conflict exclusion).
 * @returns {Object} - Contains the following properties:
 *  - availableSlots: Array of free time slots (Date objects).
 *  - loadDailyAvailability: Function to fetch availability for a specific day.
 *  - loading: Boolean indicating if availability is being loaded.
 *  - error: Error message, if any occurs during fetching.
 */
export const useReschedule = (providerId, appointmentDuration, appointmentId) => {
    // State to store available free slots for the selected provider
    const [availableSlots, setAvailableSlots] = useState([]);

    // State to indicate loading state for fetching availability
    const [loading, setLoading] = useState(false);

    // State to store errors during the fetch operation
    const [error, setError] = useState(null);

    /**
     * Function to load daily availability of free slots for a given day.
     * This function is memoized with `useCallback` to prevent unnecessary re-creation during renders.
     *
     * @param {Date} day - The day for which availability should be fetched.
     */
    const loadDailyAvailability = useCallback(async (day) => {
        setLoading(true); // Indicate loading state
        setError(null); // Clear any previous errors

        try {
            // Format the start and end of the selected day
            const start = startOfDay(day).toISOString();
            const end = endOfDay(day).toISOString();

            // Fetch free slots from the backend service
            const freeSlotsResponse = await freeSlotsService.getFreeSlots(
                start,
                end,
                [providerId],  // Provide the ID of the provider
                appointmentDuration,
                appointmentId
            );

            // Extract free slots for the specified provider
            const freeSlots = freeSlotsResponse[providerId] || [];
            console.log(freeSlots); // Log the fetched slots for debugging

            // Convert slots to Date objects and update state
            setAvailableSlots(freeSlots.map(slot => new Date(slot)));
        } catch (err) {
            // Set error message and log error
            setError(err.message || "Error loading availability");
            console.error("Error loading daily availability:", err);
        } finally {
            // Reset loading state
            setLoading(false);
        }
    }, [providerId, appointmentDuration, appointmentId]); // Dependencies ensure function updates when inputs change

    // Return the hook's API
    return { availableSlots, loadDailyAvailability, loading, error };
};
