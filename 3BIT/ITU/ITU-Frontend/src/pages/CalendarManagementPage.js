// src/pages/CalendarManagementPage.js

import React from 'react';
import { Container } from '@mui/material';
import CalendarManagement from '../components/calendars/CalendarManagement';
import { useNavigate } from 'react-router-dom';

const CalendarManagementPage = () => {
    const navigate = useNavigate();

    const handleClose = () => {
        navigate('/calendar-info');
    };

    return (
        <Container>
            <CalendarManagement onClose={handleClose} />
        </Container>
    );
};

export default CalendarManagementPage;
