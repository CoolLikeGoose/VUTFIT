/**
 * @file useProviderInfoController.js
 * @description Custom React hook for managing provider information, including fetching, searching,
 *              editing, creating, and deleting providers. It handles state management, interactions with
 *              backend services, and user feedback through snackbars.
 * Author: Zverev Daniil (xzvere00)
 */

import { useState, useEffect } from 'react';
import { useNavigate } from 'react-router-dom';
import { providerService } from '../services/providerService';
import { calendarService } from '../services/calendarService'; // Import calendarService
import { format } from 'date-fns';

/**
 * Custom Hook: useProviderInfoController
 *
 * @returns {Object} An object containing state variables and handler functions for managing provider information.
 *
 * @description
 * This hook manages the state and side-effects related to provider information, including:
 * - Fetching all providers and their associated calendar counts.
 * - Handling actions such as editing, deleting, and creating providers.
 * - Managing UI elements like menus and snackbars for user feedback.
 * - Implementing search functionality to filter providers based on user input.
 */
const useProviderInfoController = () => {
    // State variables for providers and search functionality
    const [providers, setProviders] = useState([]); // List of all providers
    const [searchTerm, setSearchTerm] = useState(''); // Current search term for filtering providers

    // State variables for the actions menu
    const [anchorEl, setAnchorEl] = useState(null); // Anchor element for the actions menu
    const [selectedProvider, setSelectedProvider] = useState(null); // Currently selected provider for actions

    // State variables for handling errors and loading states
    const [error, setError] = useState(null); // Error message, if any

    // Navigation hook for redirecting users
    const navigate = useNavigate();

    // State variables for snackbar notifications
    const [snackbarOpen, setSnackbarOpen] = useState(false); // Controls snackbar visibility
    const [snackbarMessage, setSnackbarMessage] = useState(''); // Message to display in snackbar
    const [snackbarSeverity, setSnackbarSeverity] = useState('success'); // Severity level of snackbar

    // State variables for the modal window used in editing or creating providers
    const [isModalOpen, setIsModalOpen] = useState(false); // Controls modal visibility
    const [modalProviderId, setModalProviderId] = useState(null); // ID of the provider being edited; null if creating a new provider

    /**
     * Fetches providers and their associated calendar counts from the backend.
     * Updates the providers state with the fetched and transformed data.
     */
    const loadProviders = async () => {
        try {
            const providersData = await providerService.getProviders(); // Fetch all providers
            const calendarsData = await calendarService.getAllCalendars(); // Fetch all calendars

            const providerCalendarCount = {}; // Object to store the count of calendars per provider

            // Iterate through each calendar to count associated providers
            for (const calendar of calendarsData) {
                try {
                    const calendarProviders = await calendarService.getProvidersForCalendar(calendar.id); // Fetch providers for the current calendar
                    for (const provider of calendarProviders) {
                        if (providerCalendarCount[provider.id]) {
                            providerCalendarCount[provider.id] += 1; // Increment count if provider already exists
                        } else {
                            providerCalendarCount[provider.id] = 1; // Initialize count for new provider
                        }
                    }
                } catch (error) {
                    console.error(`Error fetching providers for calendar ${calendar.id}:`, error);
                    // Continue processing other calendars even if one fails
                }
            }

            // Transform providers data to include firstName, lastName, createdAt, and calendarCount
            const transformedProviders = providersData.map((provider) => {
                const nameParts = provider.name ? provider.name.split(' ') : []; // Split full name into parts
                return {
                    ...provider,
                    firstName: nameParts.slice(0, -1).join(' ') || '', // Extract first name
                    lastName: nameParts.slice(-1).join(' ') || '', // Extract last name
                    createdAt: provider.created_at, // Use created_at for the creation date
                    calendarCount: providerCalendarCount[provider.id] || 0, // Assign calendar count or 0 if none
                };
            });

            setProviders(transformedProviders); // Update providers state with transformed data
        } catch (error) {
            setError('Failed to load providers'); // Set error message
            console.error('Error loading providers:', error);
            setSnackbarMessage('Failed to load providers'); // Set snackbar message
            setSnackbarSeverity('error'); // Set snackbar severity to error
            setSnackbarOpen(true); // Open snackbar
        }
    };

    /**
     * useEffect Hook: Fetches providers and their calendar counts when the component mounts.
     */
    useEffect(() => {
        loadProviders(); // Initiate data loading on mount
    }, []);

    /**
     * Handles opening the actions menu for a selected provider.
     *
     * @param {Event} event - The click event triggering the menu.
     * @param {Object} provider - The provider object for which the menu is opened.
     */
    const handleMenuOpen = (event, provider) => {
        setAnchorEl(event.currentTarget); // Set the anchor element for the menu
        setSelectedProvider(provider); // Set the selected provider
    };

    /**
     * Handles closing the actions menu.
     */
    const handleMenuClose = () => {
        setAnchorEl(null); // Remove the anchor element
        setSelectedProvider(null); // Clear the selected provider
    };

    /**
     * Handles initiating the edit action for a selected provider.
     * Opens the modal window with the provider's ID for editing.
     */
    const handleEditProvider = () => {
        if (selectedProvider && selectedProvider.id) {
            setModalProviderId(selectedProvider.id); // Set the provider ID to be edited
            setIsModalOpen(true); // Open the modal window
            handleMenuClose(); // Close the actions menu
        }
    };

    /**
     * Handles initiating the creation of a new provider.
     * Opens the modal window without setting a provider ID, indicating a new provider.
     */
    const handleCreateProvider = () => {
        setModalProviderId(null); // No provider ID indicates creation of a new provider
        setIsModalOpen(true); // Open the modal window
    };

    /**
     * Handles deleting a selected provider.
     * Calls the backend service to delete the provider and updates the state accordingly.
     */
    const handleDeleteProvider = async () => {
        if (!selectedProvider || !selectedProvider.id) return; // Ensure a provider is selected
        try {
            await providerService.deleteProvider(selectedProvider.id); // Delete the provider from backend
            setProviders((prev) => prev.filter((prov) => prov.id !== selectedProvider.id)); // Remove provider from state
            setSnackbarMessage('Provider successfully deleted'); // Set success message
            setSnackbarSeverity('success'); // Set snackbar severity to success
            setSnackbarOpen(true); // Open snackbar
        } catch (error) {
            setError('Failed to delete provider'); // Set error message
            setSnackbarMessage('Failed to delete provider'); // Set snackbar message
            setSnackbarSeverity('error'); // Set snackbar severity to error
            setSnackbarOpen(true); // Open snackbar
            console.error('Error deleting provider:', error);
        } finally {
            handleMenuClose(); // Close the actions menu
        }
    };

    /**
     * Handles closing the modal window.
     * Resets the modal state and reloads the providers to reflect any changes.
     */
    const handleModalClose = () => {
        setIsModalOpen(false); // Close the modal window
        setModalProviderId(null); // Clear the provider ID
        loadProviders(); // Reload providers to update the list
    };

    /**
     * Handles closing the snackbar notification.
     */
    const handleSnackbarClose = () => {
        setSnackbarOpen(false); // Close the snackbar
    };

    /**
     * Handles updating the search term for filtering providers.
     *
     * @param {string} value - The new search term entered by the user.
     */
    const handleSearchChange = (value) => {
        setSearchTerm(value); // Update the search term state
    };

    // Return all necessary state variables and handler functions to be used by the component
    return {
        providers, // List of providers with associated calendar counts
        searchTerm, // Current search term for filtering
        anchorEl, // Anchor element for actions menu
        selectedProvider, // Currently selected provider for actions
        error, // Error message, if any
        snackbarOpen, // Controls snackbar visibility
        snackbarMessage, // Message to display in snackbar
        snackbarSeverity, // Severity level of snackbar

        setSearchTerm: handleSearchChange, // Function to update the search term
        handleMenuOpen, // Function to open actions menu
        handleMenuClose, // Function to close actions menu
        handleEditProvider, // Function to initiate provider editing
        handleCreateProvider, // Function to initiate provider creation
        handleDeleteProvider, // Function to delete a provider
        handleSnackbarClose, // Function to close snackbar
        isModalOpen, // Controls modal visibility
        modalProviderId, // ID of the provider being edited; null if creating a new provider
        handleModalClose, // Function to close the modal window
    };
};

export default useProviderInfoController;
