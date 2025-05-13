/**
 * @file providerService.js
 * @description Provides API methods for managing providers, including CRUD operations and availability handling.
 *              This service interacts with the backend to fetch, create, update, and delete providers,
 *              as well as retrieve and set their availability.
 * @author Daniil Zvereve (xzvere00) and Vladyslav Yeroma (xyerom00)
 */

import { BASE_URL } from './config'; // Import the base URL for API requests

export const providerService = {
    /**
     * Fetch all providers from the backend.
     *
     * @returns {Promise<Array>} A promise that resolves to an array of providers.
     * @throws {Error} Throws an error if the request fails.
     */
    getProviders: async () => {
        const response = await fetch(`${BASE_URL}/providers`);
        if (!response.ok) {
            let errorMessage = 'Failed to load providers';
            try {
                const errorData = await response.json();
                errorMessage = errorData.message || errorMessage;
            } catch (e) {
                console.error('Error parsing response:', e);
            }
            throw new Error(errorMessage);
        }
        return await response.json();
    },

    /**
     * Fetch a specific provider by its ID.
     *
     * @param {number} providerId - The ID of the provider to fetch.
     * @returns {Promise<Object>} A promise that resolves to the provider's details.
     * @throws {Error} Throws an error if the request fails.
     */
    getProviderById: async (providerId) => {
        const response = await fetch(`${BASE_URL}/providers/${providerId}`);
        if (!response.ok) {
            let errorMessage = 'Failed to load provider';
            try {
                const errorData = await response.json();
                errorMessage = errorData.message || errorMessage;
            } catch (e) {
                console.error('Error parsing response:', e);
            }
            throw new Error(errorMessage);
        }
        return await response.json();
    },

    /**
     * Create a new provider.
     *
     * @param {Object} data - The provider data to create.
     * @returns {Promise<Object>} A promise that resolves to the newly created provider.
     * @throws {Error} Throws an error if the request fails.
     */
    createProvider: async (data) => {
        const response = await fetch(`${BASE_URL}/providers`, {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify(data),
        });
        if (!response.ok) {
            let errorMessage = 'Failed to create provider';
            try {
                const errorData = await response.json();
                errorMessage = errorData.message || errorMessage;
            } catch (e) {
                console.error('Error parsing response:', e);
            }
            throw new Error(errorMessage);
        }
        return await response.json();
    },

    /**
     * Update an existing provider.
     *
     * @param {number} providerId - The ID of the provider to update.
     * @param {Object} data - The updated provider data.
     * @returns {Promise<Object>} A promise that resolves to the updated provider.
     * @throws {Error} Throws an error if the request fails.
     */
    updateProvider: async (providerId, data) => {
        const response = await fetch(`${BASE_URL}/providers/${providerId}`, {
            method: 'PUT',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify(data),
        });
        if (!response.ok) {
            let errorMessage = 'Failed to update provider';
            try {
                const errorData = await response.json();
                errorMessage = errorData.message || errorMessage;
            } catch (e) {
                console.error('Error parsing response:', e);
            }
            throw new Error(errorMessage);
        }
        return await response.json();
    },

    /**
     * Delete a provider by its ID.
     *
     * @param {number} providerId - The ID of the provider to delete.
     * @returns {Promise<boolean>} A promise that resolves to true if the provider is successfully deleted.
     * @throws {Error} Throws an error if the request fails.
     */
    deleteProvider: async (providerId) => {
        const response = await fetch(`${BASE_URL}/providers/${providerId}`, {
            method: 'DELETE',
            headers: { 'Content-Type': 'application/json' },
        });
        if (!response.ok) {
            let errorMessage = 'Failed to delete provider';
            try {
                const errorData = await response.json();
                errorMessage = errorData.message || errorMessage;
            } catch (e) {
                console.error('Error parsing response:', e);
            }
            throw new Error(errorMessage);
        }
        return response.status === 204;
    },

    /**
     * Fetch availability for a specific provider.
     *
     * @param {number} providerId - The ID of the provider.
     * @returns {Promise<Array>} A promise that resolves to the provider's availability data.
     * @throws {Error} Throws an error if the request fails.
     */
    getProviderAvailability: async (providerId) => {
        const response = await fetch(`${BASE_URL}/providers/${providerId}/availability`);
        if (!response.ok) {
            let errorMessage = 'Failed to load provider availability';
            try {
                const errorData = await response.json();
                errorMessage = errorData.message || errorMessage;
            } catch (e) {
                console.error('Error parsing response:', e);
            }
            throw new Error(errorMessage);
        }
        return await response.json();
    },

    /**
     * Set availability for a specific provider.
     *
     * @param {number} providerId - The ID of the provider.
     * @param {Object} availability - The availability data to set.
     * @returns {Promise<Object>} A promise that resolves to the updated availability data.
     * @throws {Error} Throws an error if the request fails.
     */
    setAvailability: async (providerId, availability) => {
        const response = await fetch(`${BASE_URL}/providers/${providerId}/availability`, {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify(availability),
        });
        if (!response.ok) {
            let errorMessage = 'Failed to set provider availability';
            try {
                const errorData = await response.json();
                errorMessage = errorData.message || errorMessage;
            } catch (e) {
                console.error('Error parsing response:', e);
            }
            throw new Error(errorMessage);
        }
        return await response.json();
    },
};