/**
 * @file clientsService.js
 * @description This service handles all client-related API interactions, including fetching, updating,
 *              and creating client records in the backend. It abstracts the complexities of HTTP requests
 *              and error handling for client data management.
 * @author Vladyslav Yeroma (xyerom00) and Nikita Smirnov (xsmirn02)
 */

import { BASE_URL } from './config'; // Import the base URL for API requests

export const clientsService = {
    /**
     * Fetches client details by their ID.
     * @param {number} clientId - The ID of the client to fetch.
     * @returns {Promise<Object>} A promise resolving to the client data.
     * @throws Will throw an error if the API call fails.
     */
    getClientById: async (clientId) => {
        try {
            const response = await fetch(`${BASE_URL}/clients/${clientId}`); // Fetch client by ID
            if (!response.ok) {
                throw new Error(`Failed to fetch client with ID ${clientId}`); // Handle HTTP errors
            }
            return await response.json(); // Parse and return client data
        } catch (error) {
            console.error("Error fetching client data:", error); // Log errors for debugging
            throw error; // Re-throw the error to allow higher-level handling
        }
    },

    /**
     * Updates client information.
     * @param {number} id - The ID of the client to update.
     * @param {Object} data - The updated client data to send to the server.
     * @returns {Promise<Object>} A promise resolving to the updated client details.
     * @throws Will throw an error if the API call fails.
     */
    updateClient: async (id, data) => {
        const response = await fetch(`${BASE_URL}/clients/${id}`, {
            method: 'PUT', // Use PUT to update existing client data
            headers: { 'Content-Type': 'application/json' }, // Indicate JSON content
            body: JSON.stringify(data), // Convert data to JSON
        });
        if (!response.ok) {
            throw new Error('Failed to update client'); // Handle HTTP errors
        }
        return await response.json(); // Parse and return updated client data
    },

    /**
     * Creates a new client.
     * @param {Object} data - The client data to create.
     * @returns {Promise<Object>} A promise resolving to the newly created client details.
     * @throws Will throw an error if the API call fails.
     */
    createClient: async (data) => {
        const response = await fetch(`${BASE_URL}/clients`, {
            method: 'POST', // Use POST to create a new client
            headers: { 'Content-Type': 'application/json' }, // Indicate JSON content
            body: JSON.stringify(data), // Convert data to JSON
        });
        if (!response.ok) {
            throw new Error('Failed to create client'); // Handle HTTP errors
        }
        return await response.json(); // Parse and return created client data
    },
};
