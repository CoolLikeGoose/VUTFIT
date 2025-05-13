/**
 * @file useCalendarManagementController.js
 * @description Custom React hook for managing the state and logic of creating or editing calendars.
 *              It handles data fetching, form state management, localStorage persistence, and interactions
 *              with the backend services for calendar and provider data.
 * Author: Zverev Daniil (xzvere00)
 */

import { useState, useEffect } from 'react';
import { useNavigate } from 'react-router-dom';
import { calendarService } from '../services/calendarService';
import { providerService } from '../services/providerService';

/**
 * Custom Hook: useCalendarManagementController
 *
 * @param {string|null} calendarId - The ID of the calendar to edit. If null, a new calendar is being created.
 * @param {Function|null} onClose - Optional callback function to close the modal or current view after saving.
 *
 * @returns {Object} An object containing state variables and handler functions for managing calendar data.
 * @property {string} calendarName - The name of the calendar.
 * @property {string} description - The description of the calendar.
 * @property {string} image - The image URL associated with the calendar.
 * @property {Array} providers - List of all available providers.
 * @property {Array} selectedProviderIds - IDs of providers selected for the calendar.
 * @property {number} duration - Default duration for calendar events in minutes.
 * @property {Array} formFields - Custom form fields associated with the calendar.
 * @property {Date|null} lastUpdated - Timestamp of the last update to the calendar.
 * @property {boolean} loading - Indicates if the calendar data is currently loading.
 * @property {boolean} snackbarOpen - Controls the visibility of the snackbar notification.
 * @property {string} snackbarMessage - Message to display in the snackbar.
 * @property {string} snackbarSeverity - Severity level of the snackbar ('success', 'error', etc.).
 * @property {string|null} error - Error message to display, if any.
 * @property {Function} handleSaveCalendar - Function to handle saving the calendar data.
 * @property {Function} handleSnackbarClose - Function to handle closing the snackbar.
 * @property {Function} setCalendarName - Function to update the calendar name.
 * @property {Function} setDescription - Function to update the calendar description.
 * @property {Function} setImage - Function to update the calendar image.
 * @property {Function} setSelectedProviderIds - Function to update selected provider IDs.
 * @property {Function} setDuration - Function to update the default duration.
 * @property {Function} setFormFields - Function to update custom form fields.
 * @property {Function} handleCancel - Function to handle form cancellation.
 *
 * @description
 * This hook manages the state and side-effects related to creating or editing a calendar. It initializes
 * form fields, fetches necessary data (such as providers and existing calendar details if editing),
 * handles form submission, and manages localStorage for form persistence. It also provides feedback
 * to the user through snackbars.
 */
const useCalendarManagementController = (calendarId = null, onClose = null) => {
    // State variables for form inputs
    const [calendarName, setCalendarName] = useState(''); // Name of the calendar
    const [description, setDescription] = useState('');   // Description of the calendar
    const [image, setImage] = useState('');               // Image URL for the calendar
    const [providers, setProviders] = useState([]);       // List of available providers
    const [selectedProviderIds, setSelectedProviderIds] = useState([]); // Selected providers for the calendar
    const [duration, setDuration] = useState(60);          // Default duration for calendar events (in minutes)
    const [formFields, setFormFields] = useState([]);      // Custom form fields for the calendar
    const [lastUpdated, setLastUpdated] = useState(null);  // Timestamp of the last update

    // State variables for handling errors and loading states
    const [error, setError] = useState(null);              // Error message, if any
    const [loading, setLoading] = useState(false);         // Loading state indicator

    // State variables for snackbar notifications
    const [snackbarOpen, setSnackbarOpen] = useState(false);        // Controls snackbar visibility
    const [snackbarMessage, setSnackbarMessage] = useState('');     // Message displayed in snackbar
    const [snackbarSeverity, setSnackbarSeverity] = useState('success'); // Severity level of snackbar

    const navigate = useNavigate(); // Hook for navigation

    // Key used for storing form data in localStorage
    const localStorageKey = calendarId ? `calendarForm_${calendarId}` : 'newCalendarForm';

    /**
     * useEffect: Load saved form data from localStorage when the component mounts or calendarId changes.
     * This ensures that user progress is not lost if they navigate away and return to the form.
     */
    useEffect(() => {
        const savedData = localStorage.getItem(localStorageKey);
        if (savedData) {
            const parsedData = JSON.parse(savedData);
            setCalendarName(parsedData.calendarName || '');
            setDescription(parsedData.description || '');
            setImage(parsedData.image || '');
            setSelectedProviderIds(parsedData.selectedProviderIds || []);
            setDuration(parsedData.duration || 60);
            setFormFields(parsedData.formFields || []);
            setLastUpdated(parsedData.lastUpdated || null);
        }
    }, [calendarId]); // Dependency ensures this runs when calendarId changes

    /**
     * useEffect: Save form data to localStorage whenever relevant state variables change.
     * This provides persistence of user input, preventing data loss on accidental navigation.
     */
    useEffect(() => {
        const formData = {
            calendarName,
            description,
            image,
            selectedProviderIds,
            duration,
            formFields,
            lastUpdated,
        };
        localStorage.setItem(localStorageKey, JSON.stringify(formData)); // Persist form data
    }, [
        calendarName,
        description,
        image,
        selectedProviderIds,
        duration,
        formFields,
        lastUpdated,
    ]); // Dependencies trigger this effect when any of these state variables change

    /**
     * useEffect: Fetch providers and calendar details (if editing) when the component mounts or calendarId changes.
     * This populates the form with existing data when editing and loads available providers.
     */
    useEffect(() => {
        /**
         * Fetches the list of providers from the backend and updates the state.
         * This ensures that users can select from the most recent list of providers.
         */
        const loadProviders = async () => {
            try {
                const data = await providerService.getProviders(); // Fetch providers
                setProviders(data); // Update providers state
            } catch (err) {
                console.error('Error loading providers:', err);
                setError('Failed to load providers'); // Set error message
                setSnackbarMessage('Failed to load providers'); // Set snackbar message
                setSnackbarSeverity('error'); // Set snackbar severity
                setSnackbarOpen(true); // Show snackbar
            }
        };

        /**
         * Fetches calendar details by ID if editing an existing calendar and updates the state accordingly.
         * This pre-fills the form with existing calendar data, allowing users to make modifications.
         */
        const loadCalendar = async () => {
            if (!calendarId) return; // If not editing, skip loading calendar details
            setLoading(true); // Start loading indicator
            try {
                const calendar = await calendarService.getCalendarById(calendarId); // Fetch calendar details

                // Normalize duration to ensure it's within allowed range and in minutes
                const loadedDuration = calendar.default_duration || 60;
                const normalizedDuration = Math.min(Math.max(loadedDuration / 60, 15), 180);

                // Fetch providers linked to the calendar
                const linkedProviders = await calendarService.getProvidersForCalendar(calendarId);

                // Update state with fetched calendar details
                setCalendarName(calendar.name || '');
                setDescription(calendar.description || '');
                setImage(calendar.img || '');
                setDuration(normalizedDuration);
                setLastUpdated(calendar.updated_at ? new Date(calendar.updated_at) : null);
                setSelectedProviderIds(linkedProviders.map((provider) => provider.id));

                // Parse custom form fields from JSON, if available
                let parsedFormFields = [];
                if (calendar.form_fields) {
                    try {
                        const parsed = JSON.parse(calendar.form_fields);
                        if (Array.isArray(parsed.fields)) {
                            parsedFormFields = parsed.fields;
                        } else {
                            console.warn('Parsed form_fields.fields is not an array');
                        }
                    } catch (e) {
                        console.error('Failed to parse calendar.form_fields:', e);
                    }
                }
                setFormFields(parsedFormFields); // Update form fields state

            } catch (err) {
                console.error('Error loading calendar:', err);
                setError('Failed to load calendar'); // Set error message
                setSnackbarMessage('Failed to load calendar'); // Set snackbar message
                setSnackbarSeverity('error'); // Set snackbar severity
                setSnackbarOpen(true); // Show snackbar
            } finally {
                setLoading(false); // End loading indicator
            }
        };

        // Initiate data loading by fetching calendar details and providers
        loadCalendar();
        loadProviders();
    }, [calendarId]); // Dependency ensures this runs when calendarId changes

    /**
     * Handles the saving of calendar data. It validates inputs, sends data to the backend,
     * updates provider associations, and provides user feedback through snackbars.
     */
    const handleSaveCalendar = async () => {
        // Basic validation to ensure required fields are filled
        if (!calendarName) {
            setSnackbarMessage('Please fill in all required fields');
            setSnackbarSeverity('error');
            setSnackbarOpen(true);
            return; // Exit the function if validation fails
        }

        // Prepare calendar data for submission
        const calendarData = {
            name: calendarName,
            description,
            img: image,
            default_duration: duration * 60, // Convert duration back to seconds if needed
            form_fields: JSON.stringify({ fields: formFields }), // Serialize custom form fields
        };

        setLoading(true); // Start loading indicator
        try {
            let currentCalendar;
            let currentCalendarId = calendarId;

            if (calendarId) {
                // Update existing calendar
                currentCalendar = await calendarService.updateCalendar(calendarId, calendarData);
                setSnackbarMessage('Calendar updated successfully'); // Set success message
            } else {
                // Create new calendar
                currentCalendar = await calendarService.createCalendar(calendarData);
                currentCalendarId = currentCalendar.id;
                setSnackbarMessage('Calendar created successfully'); // Set success message
            }

            // Update lastUpdated timestamp with the latest updated_at from the response
            setLastUpdated(currentCalendar.updated_at ? new Date(currentCalendar.updated_at) : null);

            // Fetch current providers linked to the calendar
            const currentProviders = await calendarService.getProvidersForCalendar(currentCalendarId);
            const currentProviderIds = currentProviders.map((provider) => provider.id);

            // Add new provider associations that are selected but not currently linked
            for (const providerId of selectedProviderIds) {
                if (!currentProviderIds.includes(providerId)) {
                    await calendarService.SetProviderForCalendar(currentCalendarId, providerId);
                }
            }

            // Remove provider associations that are currently linked but not selected anymore
            for (const providerId of currentProviderIds) {
                if (!selectedProviderIds.includes(providerId)) {
                    await calendarService.RemoveProviderFromCalendar(currentCalendarId, providerId);
                }
            }

            // Update snackbar to indicate success
            setSnackbarSeverity('success');
            setSnackbarOpen(true);

            // Remove saved form data from localStorage upon successful save
            localStorage.removeItem(localStorageKey);

            // Invoke onClose callback if provided to close the modal/view
            if (onClose) {
                onClose();
            }
        } catch (err) {
            console.error('Error saving calendar:', err);
            setSnackbarMessage('Failed to save calendar'); // Set error message
            setSnackbarSeverity('error'); // Set snackbar severity to error
            setSnackbarOpen(true); // Show snackbar
        } finally {
            setLoading(false); // End loading indicator
        }
    };

    /**
     * Handles the cancellation of the form. It clears any saved form data from localStorage
     * and invokes the onClose callback to close the modal/view.
     */
    const handleCancel = () => {
        localStorage.removeItem(localStorageKey); // Clear saved form data
        if (onClose) {
            onClose(); // Invoke the onClose callback if provided
        }
    };

    /**
     * Handles the closing of the snackbar notification.
     */
    const handleSnackbarClose = () => setSnackbarOpen(false);

    // Return all necessary state variables and handler functions to be used by the component
    return {
        calendarName,           // Name of the calendar
        description,            // Description of the calendar
        image,                  // Image URL for the calendar
        providers,              // List of available providers
        selectedProviderIds,    // Selected providers for the calendar
        duration,               // Default duration for calendar events
        formFields,             // Custom form fields for the calendar
        lastUpdated,            // Timestamp of the last update

        loading,                // Loading state indicator
        snackbarOpen,           // Controls snackbar visibility
        snackbarMessage,        // Message displayed in snackbar
        snackbarSeverity,       // Severity level of snackbar
        error,                  // Error message, if any

        handleSaveCalendar,     // Function to handle saving the calendar data
        handleSnackbarClose,    // Function to handle closing the snackbar
        setCalendarName,        // Function to update the calendar name
        setDescription,         // Function to update the calendar description
        setImage,               // Function to update the calendar image
        setSelectedProviderIds, // Function to update selected provider IDs
        setDuration,            // Function to update the default duration
        setFormFields,          // Function to update custom form fields
        handleCancel,           // Function to handle form cancellation
    };
};

export default useCalendarManagementController;
