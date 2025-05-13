/**
 * @file useProviderProfileController.js
 * @description Custom React hook for managing the state and logic of creating or editing a provider's profile.
 *              It handles data fetching, form state management, localStorage persistence, and interactions
 *              with the backend services for provider and availability data.
 * Author: Zverev Daniil (xzvere00)
 */

import { useState, useEffect } from 'react';
import { useNavigate } from 'react-router-dom';
import { providerService } from '../services/providerService';
import { formatISO, set } from 'date-fns';

/**
 * Custom Hook: useProviderProfileController
 *
 * @param {string|null} providerId - The ID of the provider to edit. If null, a new provider is being created.
 * @param {Function|null} onClose - Optional callback function to close the modal or navigate away after saving.
 *
 * @returns {Object} An object containing state variables and handler functions for managing provider profiles.
 *
 * @description
 * This hook manages the state and side-effects related to creating or editing a provider's profile. It initializes
 * form fields, fetches necessary data (such as provider details and availability if editing), handles form submission,
 * and manages localStorage for form persistence. It also provides feedback to the user through snackbars.
 */
const useProviderProfileController = (providerId = null, onClose = null) => {
    const navigate = useNavigate(); // Hook for navigation

    const isEditMode = Boolean(providerId); // Determines if the hook is in edit mode based on the presence of providerId

    // State variables for form inputs
    const [firstName, setFirstName] = useState(''); // Provider's first name
    const [lastName, setLastName] = useState(''); // Provider's last name
    const [email, setEmail] = useState(''); // Provider's email address
    const [phone, setPhone] = useState(''); // Provider's phone number

    // State variables for availability schedule
    const [availability, setAvailability] = useState({
        0: [], // Sunday
        1: [], // Monday
        2: [], // Tuesday
        3: [], // Wednesday
        4: [], // Thursday
        5: [], // Friday
        6: []  // Saturday
    });
    const [daysChecked, setDaysChecked] = useState({
        0: false, // Sunday
        1: false, // Monday
        2: false, // Tuesday
        3: false, // Wednesday
        4: false, // Thursday
        5: false, // Friday
        6: false  // Saturday
    });

    // State variables for handling loading and error states
    const [loading, setLoading] = useState(false); // Indicates if data is being loaded
    const [error, setError] = useState(null); // Stores any error messages

    // State variables for snackbar notifications
    const [snackbarOpen, setSnackbarOpen] = useState(false); // Controls snackbar visibility
    const [snackbarMessage, setSnackbarMessage] = useState(''); // Message to display in snackbar
    const [snackbarSeverity, setSnackbarSeverity] = useState('success'); // Severity level of snackbar

    // Key used for storing form data in localStorage
    const localStorageKey = providerId ? `providerForm_${providerId}` : 'newProviderForm';

    /**
     * useEffect Hook: Loads saved form data from localStorage when the hook initializes or providerId changes.
     */
    useEffect(() => {
        const savedData = localStorage.getItem(localStorageKey); // Retrieve saved data
        if (savedData) {
            const parsedData = JSON.parse(savedData); // Parse JSON data
            setFirstName(parsedData.firstName || ''); // Set first name
            setLastName(parsedData.lastName || ''); // Set last name
            setEmail(parsedData.email || ''); // Set email
            setPhone(parsedData.phone || ''); // Set phone
            setAvailability(parsedData.availability || {
                0: [],
                1: [],
                2: [],
                3: [],
                4: [],
                5: [],
                6: []
            }); // Set availability
            setDaysChecked(parsedData.daysChecked || {
                0: false,
                1: false,
                2: false,
                3: false,
                4: false,
                5: false,
                6: false
            }); // Set daysChecked
        }
    }, [providerId]); // Dependency array ensures this runs when providerId changes

    /**
     * useEffect Hook: Saves form data to localStorage whenever relevant state variables change.
     */
    useEffect(() => {
        const formData = {
            firstName,
            lastName,
            email,
            phone,
            availability,
            daysChecked,
        };
        localStorage.setItem(localStorageKey, JSON.stringify(formData)); // Save data as JSON string
    }, [firstName, lastName, email, phone, availability, daysChecked]); // Dependencies trigger this effect

    /**
     * useEffect Hook: Loads provider data and availability if in edit mode.
     */
    useEffect(() => {
        if (isEditMode) { // Only load data if editing an existing provider
            const loadProvider = async () => {
                setLoading(true); // Start loading
                try {
                    const provider = await providerService.getProviderById(providerId); // Fetch provider details

                    // Split the full name into firstName and lastName
                    const nameParts = provider.name ? provider.name.split(' ') : [];
                    setFirstName(nameParts.slice(0, -1).join(' ') || ''); // Set first name
                    setLastName(nameParts.slice(-1).join(' ') || ''); // Set last name
                    setEmail(provider.contact_info || ''); // Set email
                    setPhone(provider.phone || ''); // Set phone

                    // Fetch provider's availability from the backend
                    const providerAvailability = await providerService.getProviderAvailability(providerId);
                    const transformedAvailability = {
                        0: [],
                        1: [],
                        2: [],
                        3: [],
                        4: [],
                        5: [],
                        6: []
                    };

                    // Transform availability data into the required format
                    providerAvailability.forEach(slot => {
                        const dayNumber = slot.week_day;
                        if (dayNumber >= 0 && dayNumber <= 6) { // Ensure dayNumber is valid
                            transformedAvailability[dayNumber].push({
                                start: slot.start_time, // Start time of the slot
                                end: slot.end_time      // End time of the slot
                            });
                        }
                    });

                    setAvailability(transformedAvailability); // Update availability state

                    // Update daysChecked based on availability
                    setDaysChecked(
                        Object.keys(transformedAvailability).reduce((acc, day) => ({
                            ...acc,
                            [day]: transformedAvailability[day].length > 0, // Set true if there are time slots
                        }), {})
                    );
                } catch (err) {
                    setError(err.message || 'Failed to load provider'); // Set error message
                    setSnackbarMessage(err.message || 'Failed to load provider'); // Set snackbar message
                    setSnackbarSeverity('error'); // Set snackbar severity to error
                    setSnackbarOpen(true); // Open snackbar
                    console.error('Error loading provider:', err); // Log error to console
                } finally {
                    setLoading(false); // End loading
                }
            };
            loadProvider(); // Invoke the function to load provider data
        }
    }, [isEditMode, providerId]); // Dependencies ensure this runs when edit mode or providerId changes

    /**
     * Validates the email format.
     *
     * @param {string} email - The email address to validate.
     * @returns {boolean} True if the email is valid, false otherwise.
     */
    const validateEmail = (email) => /\S+@\S+\.\S+/.test(email);

    /**
     * Converts a time string to UTC format (HH:MM).
     *
     * @param {string} time - The time string in "HH:MM" format.
     * @returns {string} The time string converted to UTC in "HH:MM" format.
     */
    const convertToUTC = (time) => {
        const [hours, minutes] = time.split(':').map(Number); // Split time into hours and minutes
        const date = new Date(); // Current date
        const utcDate = set(date, { hours, minutes, seconds: 0, milliseconds: 0 }); // Set the time
        return formatISO(utcDate, { representation: 'time' }).slice(0,5); // Format to "HH:MM"
    };

    /**
     * Handles saving the provider's profile. It validates input fields, sends data to the backend,
     * manages availability, and provides user feedback through snackbars.
     */
    const handleSaveProvider = async () => {
        // Basic validation to ensure required fields are filled
        if (!firstName.trim() || !lastName.trim() || !email.trim()) {
            setError('Please fill in required fields (First Name, Last Name, Email)'); // Set error message
            setSnackbarMessage('Please fill in required fields (First Name, Last Name, Email)'); // Set snackbar message
            setSnackbarSeverity('error'); // Set snackbar severity to error
            setSnackbarOpen(true); // Open snackbar
            return; // Exit the function
        }

        // Validate email format
        if (!validateEmail(email.trim())) {
            setError('Please enter a valid Email'); // Set error message
            setSnackbarMessage('Please enter a valid Email'); // Set snackbar message
            setSnackbarSeverity('error'); // Set snackbar severity to error
            setSnackbarOpen(true); // Open snackbar
            return; // Exit the function
        }

        // Transform availability data into the required format for the backend
        const transformedAvailability = [];
        for (const day in availability) { // Iterate through each day
            availability[day].forEach(slot => { // Iterate through each time slot of the day
                if (slot.start && slot.end) { // Ensure both start and end times are provided
                    transformedAvailability.push({
                        week_day: Number(day), // Day of the week as a number (0-6)
                        start_time: convertToUTC(slot.start), // Start time in UTC
                        end_time: convertToUTC(slot.end)      // End time in UTC
                    });
                }
            });
        }

        // Prepare provider data for submission
        const providerData = {
            name: `${firstName.trim()} ${lastName.trim()}`, // Combine first and last names
            contact_info: email.trim(), // Email address
            phone: phone.trim(),        // Phone number
        };

        setLoading(true); // Start loading
        try {
            if (isEditMode) { // If editing an existing provider
                await providerService.updateProvider(providerId, providerData); // Update provider details
                await providerService.setAvailability(providerId, transformedAvailability); // Update availability
                console.log('Availability data saved:', transformedAvailability); // Log success
                setSnackbarMessage('Provider successfully updated'); // Set success message
            } else { // If creating a new provider
                const newProvider = await providerService.createProvider(providerData); // Create provider
                if (newProvider.id) { // Ensure the new provider has an ID
                    await providerService.setAvailability(newProvider.id, transformedAvailability); // Set availability
                    console.log('Availability data saved for new provider:', transformedAvailability); // Log success
                } else {
                    throw new Error('New provider ID is missing'); // Throw error if ID is missing
                }
                setSnackbarMessage('Provider successfully created'); // Set success message
            }
            setSnackbarSeverity('success'); // Set snackbar severity to success
            setSnackbarOpen(true); // Open snackbar

            localStorage.removeItem(localStorageKey); // Clear saved form data from localStorage

            if (onClose) { // If an onClose callback is provided
                onClose(); // Invoke the callback to close the modal or navigate away
            }
        } catch (err) { // Handle any errors during the save process
            setError(err.message || 'Failed to save provider'); // Set error message
            setSnackbarMessage(err.message || 'Failed to save provider'); // Set snackbar message
            setSnackbarSeverity('error'); // Set snackbar severity to error
            setSnackbarOpen(true); // Open snackbar
            console.error('Error saving provider:', err); // Log error to console
        } finally {
            setLoading(false); // End loading
        }
    };

    /**
     * Handles cancelling the form. It clears any saved form data from localStorage
     * and invokes the onClose callback or navigates back to the provider info page.
     */
    const handleCancel = () => {
        localStorage.removeItem(localStorageKey); // Clear saved form data
        if (onClose) { // If an onClose callback is provided
            onClose(); // Invoke the callback to close the modal or navigate away
        } else {
            navigate('/provider-info'); // Navigate back to the provider info page
        }
    };

    /**
     * Handles adding a new time slot to a specific day.
     *
     * @param {number} day - The day of the week (0-6) to which the time slot is added.
     */
    const handleAddTimeSlot = (day) => {
        setAvailability((prev) => ({
            ...prev,
            [day]: [...prev[day], { start: '', end: '' }], // Add a new empty time slot
        }));
    };

    /**
     * Handles removing an existing time slot from a specific day.
     *
     * @param {number} day - The day of the week (0-6) from which the time slot is removed.
     * @param {number} index - The index of the time slot to remove.
     */
    const handleRemoveTimeSlot = (day, index) => {
        setAvailability((prev) => ({
            ...prev,
            [day]: prev[day].filter((_, i) => i !== index), // Remove the specified time slot
        }));
    };

    /**
     * Handles changes to a specific time slot's start or end time.
     *
     * @param {number} day - The day of the week (0-6) of the time slot.
     * @param {number} index - The index of the time slot to update.
     * @param {string} field - The field to update ('start' or 'end').
     * @param {string} value - The new time value in "HH:MM" format.
     */
    const handleTimeChange = (day, index, field, value) => {
        const updatedDay = availability[day].map((slot, i) =>
            i === index ? { ...slot, [field]: value } : slot // Update the specified field of the time slot
        );
        setAvailability((prev) => ({
            ...prev,
            [day]: updatedDay, // Update the day's availability with the modified time slot
        }));
    };

    /**
     * Handles checking or unchecking a day's availability.
     *
     * @param {number} day - The day of the week (0-6) being toggled.
     * @param {boolean} checked - The new checked state of the day.
     */
    const handleDayCheck = (day, checked) => {
        setDaysChecked((prev) => ({
            ...prev,
            [day]: checked, // Update the checked state for the day
        }));
        if (!checked) { // If the day is unchecked
            setAvailability((prev) => ({
                ...prev,
                [day]: [], // Clear all time slots for the day
            }));
        }
    };

    /**
     * Handles closing the snackbar notification.
     */
    const handleSnackbarClose = () => {
        setSnackbarOpen(false); // Close the snackbar
    };

    // Return all necessary state variables and handler functions to be used by the component
    return {
        firstName, // Provider's first name
        lastName,  // Provider's last name
        email,     // Provider's email address
        phone,     // Provider's phone number
        availability, // Provider's availability schedule
        daysChecked,   // Indicates which days are selected for availability
        loading,       // Indicates if data is being loaded
        error,         // Error message, if any
        snackbarOpen,  // Controls snackbar visibility
        snackbarMessage, // Message to display in snackbar
        snackbarSeverity, // Severity level of snackbar

        // State setters and handler functions
        setFirstName,       // Function to update firstName
        setLastName,        // Function to update lastName
        setEmail,           // Function to update email
        setPhone,           // Function to update phone
        handleSaveProvider, // Function to handle saving the provider's profile
        handleSnackbarClose, // Function to handle closing the snackbar
        handleAddTimeSlot,    // Function to add a new time slot
        handleRemoveTimeSlot, // Function to remove an existing time slot
        handleTimeChange,     // Function to handle time slot changes
        handleDayCheck,       // Function to handle day availability toggling
        handleCancel,         // Function to handle form cancellation
    };
};

export default useProviderProfileController;
