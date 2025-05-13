/**
 * @file AppointmentOptionsOverlay.js
 * @description React component for displaying a popup with options to edit, move, or cancel an appointment.
 * @author Vladyslav Yeroma (xyerom00)
 *
 * This component provides an interactive overlay for managing appointment options,
 * allowing users to quickly perform actions such as editing, canceling, or closing the popup.
 */

import React from 'react';
import '../../styles/calendar_popup.css';

/**
 * AppointmentOptionsOverlay Component
 * @param {Object} props - Component properties.
 * @param {Object} props.appointment - The appointment details to display.
 * @param {Function} props.onEdit - Callback to trigger editing the appointment.
 * @param {Function} props.onCancel - Callback to cancel the appointment.
 * @param {Function} props.onClose - Callback to close the popup.
 * @param {Object} props.style - Inline styles for dynamically positioning the popup.
 * @returns {JSX.Element} The rendered component.
 */
export const AppointmentOptionsOverlay = ({ appointment, onEdit, onCancel, onClose, style }) => {
    return (
        <div
            className="appointment-options-popup"
            style={style} // Apply dynamic position styling while retaining base class styles
        >
            {/* Display the name of the client associated with the appointment */}
            <p>{appointment.client.name}</p>

            {/* Button to edit the selected appointment */}
            <button className="popup-button" onClick={() => onEdit(appointment)}>Edit</button>

            {/* Button to close the popup without performing any action */}
            <button className="popup-button" onClick={onClose}>Close</button>

            {/* Button to cancel the selected appointment */}
            <button className="popup-button cancel-option-button" onClick={() => onCancel(appointment)}>Cancel</button>


        </div>
    );
};
