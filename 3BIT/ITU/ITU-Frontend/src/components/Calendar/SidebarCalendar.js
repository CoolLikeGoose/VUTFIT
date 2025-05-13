/**
 * @file SidebarCalendar.js
 * @description Component for rendering a sidebar with a calendar, provider filters, and calendar selection dropdown.
 *              Enables users to select dates, manage providers, and switch between different calendars.
 * @author Vladyslav Yeroma (xyerom00)
 */

import React, { useState, useEffect } from 'react';
import { DateCalendar } from '@mui/x-date-pickers/DateCalendar';
import { LocalizationProvider } from '@mui/x-date-pickers';
import { AdapterDateFns } from '@mui/x-date-pickers/AdapterDateFnsV3';
import { Box, Checkbox, FormControlLabel, Typography, Select, MenuItem, FormControl, InputLabel } from "@mui/material";
import { isWeekend, isSameDay } from 'date-fns';
import { PickersDay } from '@mui/x-date-pickers';
import { useProvidersController } from '../../controllers/useProvidersController';
import { useCalendarController } from '../../controllers/useCalendarController';

const LOCAL_STORAGE_KEY = 'sidebarCalendarState';

/**
 * CustomDay Component
 * Renders a styled day in the DateCalendar with specific styles for weekends and selected days.
 * @param {Object} props - Props passed to the component.
 * @param {Date} props.day - Current day being rendered.
 * @param {Date} props.selectedDate - Currently selected date.
 * @returns {JSX.Element} Styled day component.
 */
const CustomDay = ({ day, selectedDate, ...props }) => {
    const isWeekendDay = isWeekend(day); // Check if the day is a weekend
    const isSelected = isSameDay(day, selectedDate); // Check if the day is the selected date

    // Apply conditional styling based on the day type
    const dayStyle = {
        backgroundColor: isSelected ? '#1976d2' : isWeekendDay ? '#e0e0e0' : 'transparent',
        color: isSelected ? '#ffffff' : isWeekendDay ? '#757575' : 'default',
    };

    return <PickersDay {...props} day={day} style={dayStyle} />;
};

/**
 * SidebarCalendar Component
 * Renders the sidebar containing a calendar, a provider filter, and a dropdown for calendar selection.
 * @param {Object} props - Component properties.
 * @param {Date} props.selectedDate - The currently selected date.
 * @param {Function} props.setSelectedDate - Function to update the selected date.
 * @param {Array} props.selectedProviders - Array of selected provider IDs.
 * @param {Function} props.setSelectedProviders - Function to update the selected providers.
 * @param {string} props.view - Current calendar view ("week" or "day").
 * @param {Function} props.setCalendarId - Function to update the selected calendar ID.
 * @returns {JSX.Element} Rendered SidebarCalendar component.
 */
export const SidebarCalendar = ({ selectedDate, setSelectedDate, selectedProviders, setSelectedProviders, view, setCalendarId }) => {
    const { calendars } = useCalendarController(); // Fetch all available calendars
    const [selectedCalendar, setSelectedCalendar] = useState(null); // State to track the currently selected calendar
    const { providers } = useProvidersController(selectedCalendar); // Fetch providers for the selected calendar
    const selectionLimit = view === 'week' ? 3 : 5;

    // Flags to track restoration and data readiness
    const [calendarRestored, setCalendarRestored] = useState(false);
    const [providersRestored, setProvidersRestored] = useState(false);

    /**
     * Restore state from `localStorage` for calendars after calendars are loaded.
     */
    useEffect(() => {
        if (!calendarRestored && calendars.length > 0) {
            const savedState = JSON.parse(localStorage.getItem(LOCAL_STORAGE_KEY));
            if (savedState?.calendarId) {
                const validCalendar = calendars.find((cal) => cal.id === savedState.calendarId);
                if (validCalendar) {
                    setSelectedCalendar(validCalendar.id);
                    setCalendarId(validCalendar.id);
                } else {
                    setSelectedCalendar(calendars[0].id);
                    setCalendarId(calendars[0].id);
                }
            } else {
                setSelectedCalendar(calendars[0].id);
                setCalendarId(calendars[0].id);
            }
            setCalendarRestored(true); // Mark calendar as restored
        }
    }, [calendars, setCalendarId, calendarRestored]);

    /**
     * Restore state from `localStorage` for providers after providers are loaded.
     */
    useEffect(() => {
        if (calendarRestored && !providersRestored && providers.length > 0) {
            const savedState = JSON.parse(localStorage.getItem(LOCAL_STORAGE_KEY));
            if (savedState?.selectedProviders?.length) {
                const validProviders = savedState.selectedProviders.filter((savedProvider) =>
                    providers.some((provider) => provider.id === savedProvider.id)
                );
                setSelectedProviders(validProviders);
            }
            setProvidersRestored(true); // Mark providers as restored
        }
    }, [providers, setSelectedProviders, calendarRestored, providersRestored]);

    /**
     * Save the current state to `localStorage` only after both calendar and providers are restored.
     */
    useEffect(() => {
        if (calendarRestored && providersRestored && selectedCalendar !== null) {
            localStorage.setItem(
                LOCAL_STORAGE_KEY,
                JSON.stringify({
                    calendarId: selectedCalendar,
                    selectedProviders,
                })
            );
        }
    }, [selectedCalendar, selectedProviders, calendarRestored, providersRestored]);

    /**
     * Handles calendar selection change.
     * @param {Object} event - Event triggered by dropdown change.
     */
    const handleCalendarChange = (event) => {
        const calendarId = event.target.value;
        setCalendarId(calendarId);
        setSelectedCalendar(calendarId);
        setSelectedProviders([]); // Clear selected providers when switching calendars
    };

    /**
     * Handles provider selection toggle.
     * @param {Object} provider - The provider object being toggled.
     */
    const handleProviderChange = (provider) => {
        const isSelected = selectedProviders.some((p) => p.id === provider.id);
        if (isSelected) {
            setSelectedProviders(selectedProviders.filter((p) => p.id !== provider.id));
        } else if (selectedProviders.length < selectionLimit) {
            setSelectedProviders([...selectedProviders, provider]);
        }
    };

    return (
        <LocalizationProvider dateAdapter={AdapterDateFns}>
            <Box sx={{ padding: 2, borderRight: '1px solid #e0e0e0', minHeight: '100vh', backgroundColor: '#f9f9f9' }}>

                {/* Calendar Dropdown for selecting service calendars */}
                <FormControl fullWidth sx={{ mb: 2 }}>
                    <InputLabel id="calendar-select-label">Calendar</InputLabel>
                    <Select
                        labelId="calendar-select-label"
                        value={selectedCalendar || ''}
                        onChange={handleCalendarChange}
                        label="Calendar"
                        MenuProps={{
                            PaperProps: {
                                style: { maxHeight: 200 },
                            },
                        }}
                    >
                        {calendars.map((calendar) => (
                            <MenuItem key={calendar.id} value={calendar.id}>
                                {calendar.name}
                            </MenuItem>
                        ))}
                    </Select>
                </FormControl>

                {/* Date Picker Component */}
                <DateCalendar
                    value={selectedDate}
                    onChange={setSelectedDate}
                    renderDay={(day, selectedDays, DayProps) => (
                        <CustomDay {...DayProps} day={day} selectedDate={selectedDate} />
                    )}
                />

                {/* Provider Selection */}
                <Typography variant="h6" style={{ marginTop: '20px', marginBottom: '10px' }}>
                    Providers
                </Typography>
                <Box sx={{ display: 'flex', flexDirection: 'column', gap: '8px' }}>
                    {providers.map((provider) => (
                        <FormControlLabel
                            key={provider.id}
                            control={
                                <Checkbox
                                    checked={selectedProviders.some((p) => p.id === provider.id)}
                                    onChange={() => handleProviderChange(provider)}
                                    disabled={
                                        selectedProviders.length >= selectionLimit &&
                                        !selectedProviders.some((p) => p.id === provider.id)
                                    }
                                />
                            }
                            label={provider.name}
                        />
                    ))}
                </Box>
            </Box>
        </LocalizationProvider>
    );
};
