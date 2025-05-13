/**
 * @file CalendarSettings.js
 * @description React component responsible for rendering the interface to manage calendar settings.
 *              It utilizes a controller hook to handle the state and logic, passing necessary
 *              data and callbacks to the CalendarSettingsView component.
 * Author: Zverev Daniil (xzvere00)
 */

import React from 'react';
import CalendarSettingsView from './CalendarSettingsView';
import useCalendarSettingsController from '../../controllers/useCalendarSettingsController';

/**
 * CalendarSettings Component
 *
 * @description
 * This component serves as the integration layer for managing calendar settings. It leverages
 * the useCalendarSettingsController hook to handle data operations and state management.
 * The controller's data and methods are passed down to the CalendarSettingsView component,
 * which manages the user interface and interactions.
 */
const CalendarSettings = () => {
    // Initialize the controller to manage calendar settings logic and state
    const controller = useCalendarSettingsController();

    // Render the view component, passing all controller properties and methods as props
    return <CalendarSettingsView {...controller} />;
};

export default CalendarSettings;
