/**
 * Main entry point for rendering the React application.
 * Configures routing for the Appointment Booking System.
 * Author: Vladyslav Yeroma (xyerom00)
 */

import React from 'react';
import { createRoot } from 'react-dom/client'; // Modern ReactDOM API
import { BrowserRouter as Router, Route, Routes } from 'react-router-dom'; // Router for navigation

// Import main app and page components
import App from './App';
import CalendarPage from './pages/CalendarPage';
import ProviderProfilePage from './pages/ProviderProfilePage';
import ProviderInfoPage from './pages/ProviderInfoPage';
import CalendarManagementPage from './pages/CalendarManagementPage';
import CalendarInfoPage from './pages/CalendarInfoPage';
import BookingPage from './pages/BookingPage';
import StatsPage from './pages/StatsPage';

// Select the root DOM element
const container = document.getElementById('root');
const root = createRoot(container);

// Render the application with routing
root.render(
    <Router>
        <Routes>
            {/* Route for Admin Appointment Management Interface */}
            <Route path="/calendar-test" element={<CalendarPage />} />

            {/* Route for Unregistered User Booking Interface */}
            <Route path="/booking-test" element={<BookingPage />} />

            {/* Route for Admin Statistics View */}
            <Route path="/calendar-stats" element={<StatsPage />} />

            {/* Route for Admin Providers Information */}
            <Route path="/provider-info" element={<ProviderInfoPage />} />

            {/* Route for Admin Provider Profile */}
            <Route path="/provider-profile" element={<ProviderProfilePage />} />

            {/* Route for Calendar Management Interface */}
            <Route path="/calendar-management" element={<CalendarManagementPage />} />

            {/* Route for Calendar Information */}
            <Route path="/calendar-info" element={<CalendarInfoPage />} />

            {/* Dynamic route for specific Calendar Management */}
            <Route path="/calendar-management/:calendarId" element={<CalendarManagementPage />} />

            {/* Dynamic route for specific Provider Profile */}
            <Route path="/provider-profile/:providerId" element={<ProviderProfilePage />} />

            {/* Default route for the main App landing page */}
            <Route path="/" element={<App />} />
        </Routes>
    </Router>
);
