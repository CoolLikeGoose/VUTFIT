/**
 * @file useCalendarController.js
 * @description Custom React hook for managing calendar data, including fetching calendar details from the backend.
 *              Handles API communication, state updates, and error handling for calendar operations.
 * @author Vladyslav Yeroma (xyerom00)
 */

import { useState, useEffect } from 'react';
import { calendarService } from '../services/calendarService';

/**
 * Custom hook for managing calendar data.
 * Provides functionality to fetch calendars and manage loading/error states.
 */
export const useCalendarController = () => {
    // State to store fetched calendar data
    const [calendars, setCalendars] = useState([]);

    // State to indicate loading status
    const [loading, setLoading] = useState(false);

    // State to store any error that occurs during API requests
    const [error, setError] = useState(null);

    /**
     * Fetches all calendars from the backend when the component mounts.
     * Updates the calendars state with the fetched data or logs an error if the request fails.
     */
    useEffect(() => {
        const fetchCalendars = async () => {
            setLoading(true); // Set loading state to true before starting the request
            try {
                // Fetch calendar data from the service
                const calendarData = await calendarService.getAllCalendars();
                setCalendars(calendarData); // Update state with fetched calendar data
            } catch (err) {
                setError(err); // Store error in state
                console.error("Error fetching calendars:", err); // Log error for debugging
            } finally {
                setLoading(false); // Reset loading state regardless of success or failure
            }
        };

        fetchCalendars(); // Trigger the fetch operation when the component mounts
    }, []); // Dependency array ensures this effect runs only once when the component is mounted

    // Return hook API for accessing calendars, loading status, and any errors
    return {
        calendars,
        loading,
        error
    };
};
