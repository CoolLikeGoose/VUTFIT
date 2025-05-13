/**
 * @file useCalendarSettingsController.js
 * @description Custom React hook for managing calendar settings, including fetching, editing,
 *              creating, and deleting calendars. It handles state management, interactions with
 *              backend services, and user feedback through snackbars.
 * Author: Zverev Daniil (xzvere00)
 */

import { useEffect, useState } from 'react';
import { calendarService } from '../services/calendarService';
import { useNavigate } from 'react-router-dom';

/**
 * Custom Hook: useCalendarSettingsController
 *
 * @returns {Object} An object containing state variables and handler functions for managing calendar settings.
 *
 * @description
 * This hook manages the state and side-effects related to calendar settings, including:
 * - Fetching all calendars and their associated providers.
 * - Handling actions such as editing, deleting, and creating calendars.
 * - Managing UI elements like menus and snackbars for user feedback.
 */
const useCalendarSettingsController = () => {
    // State variables for calendars and UI controls
    const [calendars, setCalendars] = useState([]); // List of all calendars
    const [anchorEl, setAnchorEl] = useState(null); // Anchor element for the actions menu
    const [selectedCalendar, setSelectedCalendar] = useState(null); // Currently selected calendar for actions
    const [error, setError] = useState(null); // Error message, if any
    const [loading, setLoading] = useState(false); // Loading state for data fetching
    const navigate = useNavigate(); // Navigation hook from react-router-dom

    // State variables for snackbar notifications
    const [snackbarOpen, setSnackbarOpen] = useState(false); // Controls snackbar visibility
    const [snackbarMessage, setSnackbarMessage] = useState(''); // Message to display in snackbar
    const [snackbarSeverity, setSnackbarSeverity] = useState('success'); // Severity level of snackbar

    // State variables for the modal window used in editing or creating calendars
    const [isModalOpen, setIsModalOpen] = useState(false); // Controls modal visibility
    const [modalCalendarId, setModalCalendarId] = useState(null); // ID of the calendar being edited; null if creating a new calendar

    /**
     * Fetches all calendars and their associated provider counts from the backend.
     * Updates the calendars state with the fetched data.
     */
    const loadCalendars = async () => {
        setLoading(true); // Start loading
        try {
            const data = await calendarService.getAllCalendars(); // Fetch all calendars

            // For each calendar, fetch the number of associated providers
            const calendarsWithProviderCount = await Promise.all(
                data.map(async (calendar) => {
                    let providerCount = 0;
                    try {
                        const providers = await calendarService.getProvidersForCalendar(calendar.id);
                        providerCount = providers.length;
                    } catch (err) {
                        console.error(`Error fetching providers for calendar ${calendar.id}:`, err);
                        // If fetching providers fails, providerCount remains 0
                    }

                    return {
                        ...calendar,
                        providerCount, // Add provider count to the calendar object
                        updated_at: calendar.updated_at || null, // Ensure updated_at is present
                    };
                })
            );

            setCalendars(calendarsWithProviderCount); // Update calendars state
        } catch (err) {
            console.error('Error fetching calendars:', err);
            setError('Failed to load calendars'); // Set error message
            setSnackbarMessage('Failed to load calendars'); // Set snackbar message
            setSnackbarSeverity('error'); // Set snackbar severity to error
            setSnackbarOpen(true); // Open snackbar
        } finally {
            setLoading(false); // End loading
        }
    };

    /**
     * useEffect Hook: Fetches calendars when the component mounts.
     */
    useEffect(() => {
        loadCalendars(); // Initiate data loading on mount
    }, []);

    /**
     * Handles opening the actions menu for a selected calendar.
     *
     * @param {Event} event - The click event triggering the menu.
     * @param {Object} calendar - The calendar object for which the menu is opened.
     */
    const handleMenuOpen = (event, calendar) => {
        setAnchorEl(event.currentTarget); // Set the anchor element for the menu
        setSelectedCalendar(calendar); // Set the selected calendar
    };

    /**
     * Handles closing the actions menu.
     */
    const handleMenuClose = () => {
        setAnchorEl(null); // Remove the anchor element
        setSelectedCalendar(null); // Clear the selected calendar
    };

    /**
     * Handles initiating the edit action for a selected calendar.
     * Opens the modal window with the calendar's ID for editing.
     */
    const handleEditCalendar = () => {
        setModalCalendarId(selectedCalendar.id); // Set the calendar ID to be edited
        setIsModalOpen(true); // Open the modal window
        handleMenuClose(); // Close the actions menu
    };

    /**
     * Handles deleting a selected calendar.
     * Calls the backend service to delete the calendar and updates the state accordingly.
     */
    const handleDeleteCalendar = async () => {
        try {
            await calendarService.deleteCalendar(selectedCalendar.id); // Delete the calendar from backend
            setCalendars((prev) => prev.filter((cal) => cal.id !== selectedCalendar.id)); // Remove calendar from state
            setSnackbarMessage('Calendar deleted successfully'); // Set success message
            setSnackbarSeverity('success'); // Set snackbar severity to success
            setSnackbarOpen(true); // Open snackbar
        } catch (err) {
            console.error('Failed to delete calendar:', err);
            setError('Failed to delete calendar'); // Set error message
            setSnackbarMessage('Failed to delete calendar'); // Set snackbar message
            setSnackbarSeverity('error'); // Set snackbar severity to error
            setSnackbarOpen(true); // Open snackbar
        } finally {
            handleMenuClose(); // Close the actions menu
        }
    };

    /**
     * Handles initiating the creation of a new calendar.
     * Opens the modal window without setting a calendar ID, indicating a new calendar.
     */
    const handleCreateCalendar = () => {
        setModalCalendarId(null); // No calendar ID indicates creation of a new calendar
        setIsModalOpen(true); // Open the modal window
    };

    /**
     * Handles closing the modal window.
     * Resets the modal state and reloads the calendars to reflect any changes.
     */
    const handleModalClose = () => {
        setIsModalOpen(false); // Close the modal window
        setModalCalendarId(null); // Clear the calendar ID
        loadCalendars(); // Reload calendars to update the list
    };

    /**
     * Handles closing the snackbar notification.
     */
    const handleSnackbarClose = () => {
        setSnackbarOpen(false); // Close the snackbar
    };

    // Return all necessary state variables and handler functions to be used by the component
    return {
        calendars, // List of calendars with provider counts
        loading, // Loading state
        error, // Error message
        anchorEl, // Anchor element for actions menu
        selectedCalendar, // Currently selected calendar for actions
        snackbarOpen, // Controls snackbar visibility
        snackbarMessage, // Message to display in snackbar
        snackbarSeverity, // Severity level of snackbar
        handleMenuOpen, // Function to open actions menu
        handleMenuClose, // Function to close actions menu
        handleEditCalendar, // Function to initiate calendar editing
        handleDeleteCalendar, // Function to delete a calendar
        handleCreateCalendar, // Function to initiate calendar creation
        handleSnackbarClose, // Function to close snackbar

        isModalOpen, // Controls modal visibility
        modalCalendarId, // ID of the calendar being edited; null if creating a new calendar
        handleModalClose, // Function to close the modal window
    };
};

export default useCalendarSettingsController;
