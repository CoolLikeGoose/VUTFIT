/**
 * @file ProviderInfo.js
 * @description React component responsible for rendering the interface to display provider information.
 *              It leverages a controller hook to manage the state and logic, passing necessary
 *              data and callbacks to the ProviderInfoView component.
 * Author: Zverev Daniil (xzvere00)
 */

import React from 'react';
import ProviderInfoView from './ProviderInfoView';
import useProviderInfoController from '../../controllers/useProviderInfoController';

/**
 * ProviderInfo Component
 *
 * @description
 * This component serves as the integration layer for displaying provider information.
 * It utilizes the useProviderInfoController hook to handle data operations and state management.
 * The controller's data and methods are passed down to the ProviderInfoView component,
 * which manages the user interface and interactions.
 */
const ProviderInfo = () => {
    // Initialize the controller to manage provider information logic and state
    const controller = useProviderInfoController();

    // Render the view component, spreading all controller properties and methods as props
    return <ProviderInfoView {...controller} />;
};

export default ProviderInfo;
