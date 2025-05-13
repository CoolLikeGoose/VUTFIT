/**
 * @file CalendarManagement.js
 * @description React component responsible for rendering the interface to create or edit calendars.
 *              It leverages a controller hook to manage the state and logic, passing necessary
 *              data and callbacks to the CalendarManagementView component.
 * Author: Zverev Daniil (xzvere00)
 */

import React from 'react';
import PropTypes from 'prop-types';
import CalendarManagementView from './CalendarManagementView';
import useCalendarManagementController from '../../controllers/useCalendarManagementController';

/**
 * CalendarManagement Component
 *
 * @param {Object} props - Component properties.
 * @param {string} [props.calendarId] - The ID of the calendar to be edited.
 *                                      If not provided, a new calendar will be created.
 * @param {Function} props.onClose - Callback function to close the modal or current view.
 *
 * @description
 * This component integrates the calendar management logic with the presentation layer.
 * It utilizes the useCalendarManagementController hook to handle data operations and state management.
 * The resulting controller data and methods are passed down to the CalendarManagementView component,
 * which handles the user interface and interactions.
 */
const CalendarManagement = ({ calendarId, onClose }) => {
    // Initialize the controller with the provided calendarId and onClose callback
    const controller = useCalendarManagementController(calendarId, onClose);

    // Render the view component, spreading all controller properties and methods as props
    return <CalendarManagementView {...controller} />;
};

// Define prop types for the CalendarManagement component to ensure correct usage
CalendarManagement.propTypes = {
    /**
     * The ID of the calendar to edit. If undefined or null, the component will handle creating a new calendar.
     */
    calendarId: PropTypes.string, // Can be null for creating a new calendar

    /**
     * Callback function to close the calendar management modal or view without saving changes.
     */
    onClose: PropTypes.func.isRequired, // Function to close the modal
};

export default CalendarManagement;
