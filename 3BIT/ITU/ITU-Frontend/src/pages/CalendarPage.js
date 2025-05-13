/**
 * @file CalendarPage.js
 * @description The main page component for managing and displaying a calendar interface with appointment scheduling functionality.
 *              It integrates the Sidebar, Header, and Calendar View to provide a complete appointment management experience.
 * @author Vladyslav Yeroma (xyerom00)
 */

import React, { useEffect, useState } from 'react';
import { Box, CssBaseline, Container, IconButton } from '@mui/material';
import { CalendarHeader } from '../components/Calendar/CalendarHeader';
import { SidebarCalendar } from '../components/Calendar/SidebarCalendar';
import { CalendarView } from '../components/Calendar/CalendarView';
import { useAppointmentsController } from '../controllers/useAppointmentsController';
import MenuIcon from '@mui/icons-material/Menu';
import { startOfWeek } from 'date-fns';

const LOCAL_STORAGE_KEY = 'calendarPageState';

/**
 * CalendarPage Component
 * Provides the main interface for calendar management, including filtering by providers, navigating through weeks/days,
 * and viewing appointments. Integrates the sidebar, calendar header, and calendar view.
 */
const CalendarPage = () => {
    // Load initial state from localStorage or set default values
    const savedState = JSON.parse(localStorage.getItem(LOCAL_STORAGE_KEY)) || {};
    const initialView = savedState.view || 'week';
    const initialDate = savedState.selectedDate ? new Date(savedState.selectedDate) : new Date();

    // State for currently selected calendar ID
    const [calendarId, setCalendarId] = useState(null);

    // State for calendar view (week/day)
    const [view, setView] = useState(initialView);

    // State for currently selected date
    const [selectedDate, setSelectedDate] = useState(initialDate);

    // State for currently selected providers
    const [selectedProviders, setSelectedProviders] = useState([]);

    // State for controlling the visibility of the sidebar
    const [sidebarOpen, setSidebarOpen] = useState(false);

    // Define color scheme for providers
    const providerColors = ['#f57c00', '#4A90E2', '#7C4DFF', '#1DE9B6', '#FF1744'];

    // Hook for managing appointments
    const {
        appointments,
        loadAppointments,
        saveAppointment,
        cancelAppointment,
        restoreAppointment,
    } = useAppointmentsController();

    /**
     * Save the current state to `localStorage` whenever `view` or `selectedDate` changes.
     */
    useEffect(() => {
        localStorage.setItem(
            LOCAL_STORAGE_KEY,
            JSON.stringify({
                view,
                selectedDate: selectedDate.toISOString(),
            })
        );
    }, [view, selectedDate]);

    /**
     * Effect to load appointments whenever relevant filters change (selected date, view, providers, calendar ID).
     */
    useEffect(() => {
        if (selectedProviders.length > 0 && calendarId) {
            const start = selectedDate.toISOString();
            const end = new Date(selectedDate.getTime() + (view === 'week' ? 7 : 1) * 24 * 60 * 60 * 1000).toISOString();
            loadAppointments(start, end, selectedProviders.map((p) => p.id), view, calendarId);
        } else {
            // Clear appointments if no providers are selected
            loadAppointments(null, null, [], view, calendarId);
        }
    }, [selectedDate, view, selectedProviders, calendarId, loadAppointments]);

    /**
     * Handles navigation across calendar dates.
     * @param {string | number} direction - Specifies navigation (e.g., 'NEXT', 'PREV', 'TODAY', or numeric offsets).
     */
    const handleNavigate = (direction) => {
        let current;

        if (direction === 'TODAY') {
            current = new Date();
        } else {
            current = new Date(selectedDate);
        }

        if (direction === 'PREV') {
            current.setDate(current.getDate() - (view === 'week' ? 7 : 1));
            if (view === 'week') {
                current = startOfWeek(current, { weekStartsOn: 0 });
            }
        } else if (direction === 'NEXT') {
            current.setDate(current.getDate() + (view === 'week' ? 7 : 1));
            if (view === 'week') {
                current = startOfWeek(current, { weekStartsOn: 0 });
            }
        } else if (typeof direction === 'number') {
            current.setDate(current.getDate() + direction * 7);
        }

        if (!isNaN(current.getTime())) {
            setSelectedDate(current);
        } else {
            console.error('Invalid date detected');
        }
    };

    /**
     * Handles switching between week and day views, with constraints on the number of providers selected for week view.
     * @param {string} newView - The new view to set ('week' or 'day').
     */
    const handleSetView = (newView) => {
        if (newView === 'week' && selectedProviders.length > 3) {
            alert('Please unselect Providers to 3 or fewer to switch to week view.');
        } else {
            setView(newView);
        }
    };

    return (
        <Box display="flex" height="100vh" overflow="hidden">
            {/* Reset CSS Baseline */}
            <CssBaseline />
            {/* Sidebar for Calendar and Provider Selection */}
            <Box
                sx={{
                    width: sidebarOpen
                        ? { xs: '100%', sm: '300px', md: '350px' }
                        : { xs: '0', sm: '300px', md: '350px' },
                    display: { xs: sidebarOpen ? 'block' : 'none', sm: 'block' },
                    backgroundColor: '#f9f9f9',
                    borderRight: '1px solid #e0e0e0',
                    transition: 'width 0.3s ease, display 0s linear 0.3s',
                    overflow: 'hidden',
                    minWidth: sidebarOpen ? { sm: '300px', md: '350px' } : '0px',
                }}
            >
                <SidebarCalendar
                    selectedDate={selectedDate}
                    setSelectedDate={setSelectedDate}
                    selectedProviders={selectedProviders}
                    setSelectedProviders={setSelectedProviders}
                    view={view}
                    calendarId={calendarId}
                    setCalendarId={setCalendarId}
                />
            </Box>

            {/* Main Content Area */}
            <Box flex={1} display="flex" flexDirection="column" height="100vh">
                {/* Button to Toggle Sidebar on Small Screens */}
                <IconButton
                    sx={{ display: { sm: 'none' }, mb: 2 }}
                    onClick={() => setSidebarOpen(!sidebarOpen)}
                >
                    <MenuIcon />
                </IconButton>

                {/* Calendar Header */}
                <Container sx={{ flex: '1 1 auto', display: 'flex', flexDirection: 'column', padding: 0 }}>
                    <CalendarHeader
                        view={view}
                        setView={handleSetView}
                        onNavigate={handleNavigate}
                        selectedDate={selectedDate}
                        selectedProviders={selectedProviders}
                        providerColors={providerColors}
                        appointments={appointments}
                    />
                    {/* Calendar View */}
                    <Box
                        sx={{
                            flex: 1,
                            overflowY: 'auto',
                            boxSizing: 'border-box',
                            maxWidth: '100%',
                            paddingBottom: '15%',
                            maxHeight: 'calc(100vh)',
                        }}
                    >
                        <CalendarView
                            appointments={appointments}
                            view={view}
                            selectedDate={selectedDate}
                            onRangeChange={(start, end) =>
                                loadAppointments(start, end, selectedProviders.map((p) => p.id), view, calendarId)
                            }
                            saveAppointment={saveAppointment}
                            cancelAppointment={cancelAppointment}
                            restoreAppointment={restoreAppointment}
                            providerColors={providerColors}
                        />
                    </Box>
                </Container>
            </Box>
        </Box>
    );
};

export default CalendarPage;
