/**
 * @file useProvidersController.js
 * @description Custom React hook for managing provider data for a specific calendar.
 *              Fetches and updates the list of providers assigned to a given calendar.
 * @author Vladyslav Yeroma (xyerom00)
 */

import { useState, useEffect } from 'react';
import { calendarService } from '../services/calendarService';

/**
 * Custom hook for managing providers assigned to a specific calendar.
 *
 * @param {number} calendarId - The ID of the calendar to fetch providers for.
 * @returns {Object} providers - The list of providers assigned to the specified calendar.
 */
export const useProvidersController = (calendarId) => {
    // State to store the list of providers
    const [providers, setProviders] = useState([]);

    /**
     * Effect to fetch providers whenever the calendar ID changes.
     */
    useEffect(() => {
        const fetchProviders = async () => {
            try {
                // Ensure calendarId is valid before fetching providers
                if (calendarId) {
                    // Fetch providers associated with the calendar from the API
                    const assignedProviders = await calendarService.getProvidersForCalendar(calendarId);
                    setProviders(assignedProviders); // Update state with fetched providers
                }
            } catch (error) {
                // Log error if fetching providers fails
                console.error('Failed to load providers:', error);
            }
        };

        fetchProviders(); // Trigger the fetch operation whenever the effect runs
    }, [calendarId]); // Dependency array ensures the effect runs when calendarId changes

    // Return the list of providers as the hook's API
    return { providers };
};
