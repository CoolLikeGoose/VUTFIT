// src/pages/CalendarInfoPage.js
import React from 'react';
import { Container } from '@mui/material';
import CalendarSettings from '../components/calendars/CalendarSettings';

const CalendarInfoPage = () => {
    return (
        <Container maxWidth="lg" sx={{ mt: 4 }}>
            <CalendarSettings />
        </Container>
    );
};

export default CalendarInfoPage;
