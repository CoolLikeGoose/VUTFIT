/**
 * @file calendarService.js
 * @description This service provides functions for interacting with the calendar-related API endpoints.
 *              It allows fetching, creating, updating, and managing calendars and their associated providers.
 *              The functions are asynchronous and designed to work with the backend RESTful API.
 * @author Daniil Zvereve (xzvere00) and Vladyslav Yeroma (xyerom00)
 */

import {BASE_URL} from './config'; // Import the base URL for API requests

export const calendarService = {
    /**
     * Fetches all calendars from the backend.
     * @returns {Promise<Array>} A promise resolving to a list of calendars.
     * @throws Will throw an error if the API call fails.
     */
    getAllCalendars: async () => {
        const response = await fetch(`${BASE_URL}/calendars`);
        if (!response.ok) {
            throw new Error('Failed to fetch calendars');
        }
        return await response.json();
    },

    /**
     * Fetches details of a specific calendar by its ID.
     * @param {number} calendarId - The ID of the calendar to retrieve.
     * @returns {Promise<Object>} A promise resolving to the calendar details.
     * @throws Will throw an error if the API call fails.
     */
    getCalendarById: async (calendarId) => {
        const response = await fetch(`${BASE_URL}/calendars/${calendarId}`);
        if (!response.ok) {
            throw new Error('Failed to fetch calendar');
        }
        return await response.json();
    },

    /**
     * Fetches providers associated with a specific calendar.
     * @param {number} calendarId - The ID of the calendar.
     * @returns {Promise<Array>} A promise resolving to a list of providers.
     * @throws Will throw an error if the API call fails.
     */
    getProvidersForCalendar: async (calendarId) => {
        const response = await fetch(`${BASE_URL}/calendars/${calendarId}/providers`);
        if (!response.ok) {
            throw new Error('Failed to fetch providers for calendar');
        }
        return await response.json();
    },

    /**
     * Creates a new calendar in the backend.
     * @param {Object} data - The details of the calendar to be created.
     * @returns {Promise<Object>} A promise resolving to the created calendar details.
     * @throws Will throw an error if the API call fails.
     */
    createCalendar: async (data) => {
        const response = await fetch(`${BASE_URL}/calendars`, {
            method: 'POST', // Use POST to create a new resource
            headers: { 'Content-Type': 'application/json' }, // Set content type to JSON
            body: JSON.stringify(data), // Send calendar data as JSON
        });
        if (!response.ok) {
            throw new Error('Failed to create calendar');
        }
        return await response.json();
    },

    /**
     * Updates details of an existing calendar by its ID.
     * @param {number} calendarId - The ID of the calendar to update.
     * @param {Object} data - The updated details of the calendar.
     * @returns {Promise<Object>} A promise resolving to the updated calendar details.
     * @throws Will throw an error if the API call fails.
     */
    updateCalendar: async (calendarId, data) => {
        const response = await fetch(`${BASE_URL}/calendars/${calendarId}`, {
            method: 'PUT', // Use PUT to update an existing resource
            headers: { 'Content-Type': 'application/json' }, // Set content type to JSON
            body: JSON.stringify(data), // Send updated calendar data as JSON
        });
        if (!response.ok) {
            throw new Error('Failed to update calendar');
        }
        return await response.json();
    },

    /**
     * Adds a provider to a specific calendar.
     * @param {number} calendarId - The ID of the calendar.
     * @param {number} providerId - The ID of the provider to add.
     * @returns {Promise<Object>} A promise resolving to the updated calendar-provider association.
     * @throws Will throw an error if the API call fails.
     */
    SetProviderForCalendar: async (calendarId, providerId) => {
        const response = await fetch(`${BASE_URL}/calendars/${calendarId}/providers`, {
            method: 'POST', // Use POST to create a new resource
            headers: { 'Content-Type': 'application/json' }, // Set content type to JSON
            body: JSON.stringify({ provider_id: providerId }), // Send provider data as JSON
        });
        if (!response.ok) {
            throw new Error('Failed to add provider to calendar');
        }
        return response.json();
    },

    /**
     * Removes a provider from a specific calendar.
     * @param {number} calendarId - The ID of the calendar.
     * @param {number} providerId - The ID of the provider to remove.
     * @returns {Promise<Object>} A promise resolving to the updated calendar-provider association.
     * @throws Will throw an error if the API call fails.
     */
    RemoveProviderFromCalendar: async (calendarId, providerId) => {
        const response = await fetch(`${BASE_URL}/calendars/${calendarId}/providers/${providerId}`, {
            method: 'DELETE', // Use DELETE to remove a resource
            headers: { 'Content-Type': 'application/json' }, // Set content type to JSON
        });
        if (!response.ok) {
            throw new Error('Failed to remove provider from calendar');
        }
        return response.json();
    },

    /**
     * Deletes a calendar by its ID.
     * @param {number} calendarId - The ID of the calendar to delete.
     * @returns {Promise<boolean>} A promise resolving to a boolean indicating successful deletion.
     * @throws Will throw an error if the API call fails.
     */
    deleteCalendar: async (calendarId) => {
        const response = await fetch(`${BASE_URL}/calendars/${calendarId}`, {
            method: 'DELETE', // Use DELETE to remove a resource
        });
        if (!response.ok) {
            throw new Error('Failed to delete calendar');
        }
        return response.status === 204; // Return true if status is 204 (No Content)
    },
};
