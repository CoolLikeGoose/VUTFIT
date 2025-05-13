/**
 * @file CalendarHeader.js
 * @description React component for displaying the header of the calendar view. Includes navigation controls, view toggles, and provider information.
 * @author Vladyslav Yeroma (xyerom00)
 *
 * This component enables users to navigate the calendar, toggle between daily and weekly views,
 * and view detailed provider information, such as total appointment times.
 */

import React from 'react';
import { Button, Box, Typography } from '@mui/material';
import { startOfWeek, endOfWeek, format } from 'date-fns';

/**
 * CalendarHeader Component
 * @param {Object} props - Component properties.
 * @param {string} props.view - Current calendar view ("day" or "week").
 * @param {Function} props.setView - Callback to toggle between "day" and "week" views.
 * @param {Function} props.onNavigate - Callback for navigating between dates (e.g., "PREV", "NEXT", "TODAY").
 * @param {Date} props.selectedDate - The currently selected date.
 * @param {Array} props.selectedProviders - List of selected providers.
 * @param {Array} props.providerColors - Array of colors for providers (used for color-coding appointments).
 * @param {Array} props.appointments - Array of current appointments.
 * @returns {JSX.Element} The rendered component.
 */
export const CalendarHeader = ({ view, setView, onNavigate, selectedDate, selectedProviders, providerColors, appointments }) => {
    // Format the date range based on the current view (daily or weekly)
    const dateRangeText = view === 'day'
        ? format(selectedDate, 'MMM dd yyyy') // Single date format for "day" view
        : `${format(startOfWeek(selectedDate, { weekStartsOn: 0 }), 'MMM dd yyyy')} - ${format(endOfWeek(selectedDate, { weekStartsOn: 0 }), 'MMM dd yyyy')}`;

    // Calculate the total time (in hours and minutes) scheduled for each provider
    const providerTimes = selectedProviders.map(provider => {
        const totalMinutes = appointments
            .filter(app => app.provider_id === provider.id) // Filter appointments for this provider
            .reduce((sum, app) => sum + (app.duration || 0), 0); // Sum up the durations

        const hours = Math.floor(totalMinutes / 60); // Convert minutes to hours
        const minutes = totalMinutes % 60; // Get remaining minutes
        return { id: provider.id, name: provider.name, time: `${hours}h ${minutes}m` }; // Format time as "Xh Ym"
    });

    return (
        <Box display="flex" flexDirection="column" justifyContent="space-between" padding={2}>
            {/* First row: View toggles (Day/Week) and date navigation */}
            <Box display="flex" justifyContent="space-between" alignItems="center">
                {/* Toggle between "Week" and "Day" views */}
                <Box display="flex" gap={1}>
                    <Button
                        variant={view === 'week' ? 'contained' : 'outlined'}
                        onClick={() => setView('week')}
                    >
                        Week
                    </Button>
                    <Button
                        variant={view === 'day' ? 'contained' : 'outlined'}
                        onClick={() => setView('day')}
                    >
                        Day
                    </Button>
                </Box>

                {/* Display the current date range with navigation buttons */}
                <Box display="flex" alignItems="center" gap={2}>
                    <Button onClick={() => onNavigate('PREV')}>{'<'}</Button>
                    <Typography variant="h6">{dateRangeText}</Typography>
                    <Button onClick={() => onNavigate('NEXT')}>{'>'}</Button>
                </Box>
            </Box>

            {/* Second row: Quick week navigation and "Today" button */}
            <Box display="flex" justifyContent="center" alignItems="center" gap={1} mt={2}>
                <Typography variant="body1">Weeks</Typography>
                <Button onClick={() => onNavigate(-4)}>{'-4'}</Button>
                <Button onClick={() => onNavigate(-3)}>{'-3'}</Button>
                <Button onClick={() => onNavigate(-2)}>{'-2'}</Button>
                <Button onClick={() => onNavigate(-1)}>{'-1'}</Button>
                <Button onClick={() => onNavigate('TODAY')}>Today</Button>
                <Button onClick={() => onNavigate(1)}>{'+1'}</Button>
                <Button onClick={() => onNavigate(2)}>{'+2'}</Button>
                <Button onClick={() => onNavigate(3)}>{'+3'}</Button>
                <Button onClick={() => onNavigate(4)}>{'+4'}</Button>
                <Typography variant="body1">Weeks</Typography>
            </Box>

            {/* Third row: Provider details with color indicators */}
            <Box display="flex" justifyContent="center" alignItems="center" mt={2}>
                {selectedProviders.length > 0 ? (
                    providerTimes.map((provider, index) => (
                        <Box key={index} display="flex" alignItems="center" sx={{ mx: 1 }}>
                            {/* Provider's color indicator */}
                            <Box
                                sx={{
                                    width: 12,
                                    height: 12,
                                    backgroundColor: providerColors[provider.id % providerColors.length], // Assign color based on provider ID
                                    borderRadius: '50%',
                                    marginRight: 1
                                }}
                            />
                            <Typography variant="body1">{provider.name}</Typography>
                            <Typography variant="body2" sx={{ ml: 1, color: 'gray' }}>
                                ({provider.time}) {/* Display total time scheduled for the provider */}
                            </Typography>
                        </Box>
                    ))
                ) : (
                    <Typography variant="body1" sx={{ mx: 1 }}>
                        No providers selected
                    </Typography>
                )}
            </Box>
        </Box>
    );
};
