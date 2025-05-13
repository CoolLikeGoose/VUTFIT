/**
 * Main entry point for the Appointment Booking System application.
 * Provides navigation links to various interfaces in the system.
 * Author: Vladyslav Yeroma (xyerom00)
 */

import React from 'react';
import { Link } from 'react-router-dom';
import { Box, CssBaseline, Container, Typography, List, ListItem } from '@mui/material';

const App = () => {
    return (
        <Box>
            {/* Reset and normalize CSS styles */}
            <CssBaseline />

            {/* Main application container */}
            <Container>
                {/* Application title */}
                <Typography variant="h4" component="h1" gutterBottom>
                    Welcome to the Appointment Booking System
                </Typography>

                {/* Navigation links to different application interfaces */}
                <List>
                    <ListItem>
                        <Link to="/calendar-test">
                            Admin Appointment Management Interface (xyerom00)
                        </Link>
                    </ListItem>
                    <ListItem>
                        <Link to="/booking-test">
                            Unregistered User Booking Interface (xsmirn02)
                        </Link>
                    </ListItem>
                    <ListItem>
                        <Link to="/calendar-stats">
                            Admin Statistics View Interface (xshapo04)
                        </Link>
                    </ListItem>
                    <ListItem>
                        <Link to="/provider-info">
                            Admin Providers Info (xzvere00)
                        </Link>
                    </ListItem>
                    <ListItem>
                        <Link to="/calendar-info">
                            Admin Calendar Info (xzvere00)
                        </Link>
                    </ListItem>
                </List>
            </Container>
        </Box>
    );
};

export default App;
