/**
 * @file ProviderProfile.js
 * @description React component responsible for rendering the interface to display and manage a provider's profile.
 *              It leverages a controller hook to handle the state and logic, passing necessary
 *              data and callbacks to the ProviderProfileView component.
 * Author: Zverev Daniil (xzvere00)
 */

import React from 'react';
import ProviderProfileView from './ProviderProfileView';
import useProviderProfileController from '../../controllers/useProviderProfileController';
import PropTypes from 'prop-types';

/**
 * ProviderProfile Component
 *
 * @param {Object} props - Component properties.
 * @param {string} props.providerId - The ID of the provider to be displayed or edited.
 * @param {Function} props.onClose - Callback function to close the provider profile modal or view.
 *
 * @description
 * This component serves as the integration layer for displaying and managing a provider's profile.
 * It utilizes the useProviderProfileController hook to handle data fetching, state management,
 * and any necessary side-effects. The controller's data and methods are passed down to the
 * ProviderProfileView component, which handles the rendering of the user interface and user interactions.
 */
const ProviderProfile = ({ providerId, onClose }) => {
    // Initialize the controller with the provided providerId and onClose callback
    const controller = useProviderProfileController(providerId, onClose);

    // Render the ProviderProfileView component, spreading all controller properties and methods as props
    return <ProviderProfileView {...controller} />;
};

// Define prop types for the ProviderProfile component to ensure correct usage
ProviderProfile.propTypes = {
    /**
     * The ID of the provider to display or edit. If not provided, the component may handle creating a new provider.
     */
    providerId: PropTypes.string,

    /**
     * Callback function to close the provider profile modal or view without saving changes.
     */
    onClose: PropTypes.func.isRequired,
};

export default ProviderProfile;
